#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <utility>
#include <algorithm>
#include <stack>
#include "gc.h"
//#include "dumptographviz.h"
using namespace std;


#define DUMP_HEAP_AS_GRAPHVIZ
#undef DUMP_HEAP_AS_GRAPHVIZ


cons* memory_loc = NULL;

int numcopied = 0; //TODO:Delete after debugging


int NullBuffer::overflow(int c) { return c; }
NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);


extern GCStatus gc_status;
extern double gctime;
extern string outdir;

//globals
void *freept;
void *scan;
void *buffer_live;
void *buffer_dead;
void *boundary_live;
void *boundary_dead;
unsigned long memorysize;
unsigned long newheapcells,oldheapcells;
unsigned long copycells,heapslot,garbagecells;
actRec *stack_start, *stack_top;
GCStatus gc_status = gc_plain;
extern int gccount;

//ofstream gout("gc_addr.txt", ios::app);

//Use only push_front and pop_front to insert and remove elements from actRecStack
//Use iterator to iterate over the elements of actRecStack during garbage collection
deque<actRec> actRecStack = deque<actRec>(0);
deque<actRec>::iterator actRecIter;
vector<var_heap>::iterator heapRefIter;
vector<var_stack>::iterator nonRefIter;
stateMap statemap;
state_index **state_transition_table; //Dynamically allocating a 2-D array might be error prone. See if vectors can be used instead.
std::unordered_map<std::string, std::string> call_next;
stack<cons*> update_heap_refs;
stack<cons*> print_stack;
unsigned int num_of_allocations = 0;
map<cons*, int> heap_map;
map<int, string> root_var_map;



#ifdef __DEBUG__GC
ofstream gcout("gc_messages.txt", ios::out);
#else
ostream &gcout = null_stream;
#endif


void clear_liveness_data();
void print_buffer_data();
void print_cell_data();
int is_valid_address(void*);
int lt_scan_free();
int lt_scan_freept();
void* getscan();
void update_scan();
state_index lookup_dfanode(string nodename);
state_index get_target_dfastate(state_index i1, state_index i2);
void set_car(void* loc,  void* ref);
void set_cdr(void* loc,  void* ref);


#ifdef GC_ENABLE_STATS
/* variables to traverse the cons cells for the statistics */
cons* array_stats = NULL;
unsigned long last_pos = 0;
clock_tick current_cons_tick = 0;
#else
/* stubs */
void init_gc_stats() {}
void finish_gc_stats() {}
#endif




int mmc=0;
long gmaxheapreachability = 0;
#ifdef TEST_RUN

void set_max_reachability();
#endif




//Functions for printing heap annotated with liveness
void traverseHeap();
void dump_heap_as_graphviz();
void gen_graphviz_code(Scheme::AST::cons* conscell, ostream& out);
std::string getNodeLabel(Scheme::AST::cons* conscell, size_t idx);
void depthfirstpaths(Scheme::AST::cons* loc, Scheme::AST::state_index index);




#define FIND_REACHABLE
#undef FIND_REACHABLE
#ifdef FIND_REACHABLE
void reach_dfs(cons* loc, vector<cons*>& v)
{
	if (NULL == loc)
		return;

	if (find(v.begin(), v.end(), loc) == v.end())
		v.push_back(loc);
	else
		return;

	if (loc->typecell == consExprClosure)
	{

		reach_dfs(loc->val.cell.car, v);

		reach_dfs(loc->val.cell.cdr, v);
	}
	else
	{
		switch(loc->typecell)
		{
		case constIntExprClosure:
		case constBoolExprClosure:
		case constStringExprClosure:
		case nilExprClosure:
			break;
		case unaryprimopExprClosure:
		case binaryprimopExprClosure:
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			reach_dfs(loc->val.closure.arg1, v);

			reach_dfs(loc->val.closure.arg2, v);
		}

		break;
		default : cout << "Should not have come to this point"<<endl;
		cout << "Processing " << loc << " with type " << loc->typecell << endl;
		break;
		}
	}

	return;
}

size_t find_num_reachable_cells()
{
	vector<cons*> reachable_cells;
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref)
			{
				reach_dfs(static_cast<cons*>(vhit->ref), reachable_cells);
			}
		}
	}

	stack<cons*> temp;
	while(!print_stack.empty())
	{
		cons* heap_ref = print_stack.top();
		print_stack.pop();
		temp.push(heap_ref);
		reach_dfs(heap_ref, reachable_cells);
	}
	while(!temp.empty())
	{
		print_stack.push(temp.top());
		temp.pop();
	}

	return reachable_cells.size();
}
#endif

#ifdef _OPT_TIME
void print_gc_move(cons* from, cons* to, ostream& out)
{
	if (from != NULL && to != NULL)
	{
		long int to_index = 0;
		long int from_index = 0;
		if (is_valid_address(from))
			from_index = (from - (cons*)buffer_live);
		else if (from)
			from_index = (from - (cons*)buffer_dead);

		if (to)
			to_index = (to - (cons*)buffer_live);

		DBG(out << from_index << "-->" << to_index << "(" << to << ")" << endl);
		DBG(out << "Type of cell " << from->typecell << endl);
	}
	return;
}

cons* deep_copy(cons* node, int gc_type, ostream& out)
{
	//assert(!is_valid_address(node));

	if (gc_type == 0)
		return followpaths_reachability(node);
	else
	{
		DBG(out << "Processing node " << node << endl);
		if (node != NULL)
		{
			if (node->typecell != consExprClosure && node->forward != NULL)
				return static_cast<cons*>(node->forward);
			else if (node->typecell == consExprClosure)
			{

				if (node->forward == NULL || node->copied_using_rgc == false)
				{
					cons* new_loc = copy(node, out);
					//node->copied_using_rgc = true;
					new_loc->val.cell.car = deep_copy(node->val.cell.car, 1, out);
					new_loc->val.cell.cdr = deep_copy(node->val.cell.cdr, 1, out);

					GC_STAT_UPDATE_LAST_GC(new_loc);
					return new_loc;
				}
				else
				{
					return static_cast<cons*>(node->forward);
				}
			}
			else
			{
				cons* new_loc = copy(node, out);
//				assert(new_loc != node);
				if ((node->typecell == unaryprimopExprClosure) ||
				    (node->typecell == binaryprimopExprClosure) ||
				    (node->typecell == funcApplicationExprClosure) ||
				    (node->typecell == funcArgClosure))
				{
					//node->copied_using_rgc = true;
					new_loc->val.closure.arg1 = deep_copy(node->val.closure.arg1, 1, out);
					new_loc->val.closure.arg2 = deep_copy(node->val.closure.arg2, 1, out);
					GC_STAT_UPDATE_LAST_GC(new_loc);
				}
				return new_loc;
			}
		}

	}
	return NULL;
}
cons* copy(cons* node, ostream& out)
{
	void *addr;
	if(node==NULL) return NULL;
	if(!(node >= buffer_dead && node < boundary_dead))
	{
//		out << "Returning the same pointer as node is already in live buffer "<< node <<endl;
		return node;
	}
	else
	{
		heapslot=heapslot+1;
		cons *conscell=(cons*)node;
        GC_STAT_MARK_REACHABLE(conscell);
		if((conscell->forward >= buffer_live) &&
				(boundary_live > conscell->forward))
		{
			return (static_cast<cons*>(conscell->forward));
		}


		addr = dup_cons(conscell);
		conscell->forward=addr;
		copycells=copycells+1;
		++numcopied;
		return static_cast<cons*>(addr);
	}

	return return_null();
}
cons* followpaths_reachability(cons* loc, ostream& out)
{

	if (NULL == loc)
		return loc;

	if (loc->forward != NULL)
		return static_cast<cons*>(loc->forward);

	cons* loccopy = copy(loc);

	if (loccopy->typecell == consExprClosure)
	{
		cons *oldcar = loccopy->val.cell.car;
		cons *addr=followpaths_reachability(loccopy->val.cell.car, out);
		loccopy->val.cell.car=addr;


		cons* oldcdr = loccopy->val.cell.cdr;
		addr=followpaths_reachability(loccopy->val.cell.cdr, out);
		loccopy->val.cell.cdr=addr;

	}
	else
	{
		switch(loccopy->typecell)
		{
		case constIntExprClosure:
		case constBoolExprClosure:
		case constStringExprClosure:
		case nilExprClosure:
			break;
		case unaryprimopExprClosure:
		case binaryprimopExprClosure:
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			cons* oldarg1 = loccopy->val.closure.arg1;
			cons *addr=followpaths_reachability(loccopy->val.closure.arg1, out);
			loccopy->val.closure.arg1=addr;

			cons* oldarg2 = loccopy->val.closure.arg2;
			addr=followpaths_reachability(loccopy->val.closure.arg2, out);
			loccopy->val.closure.arg2=addr;

		}

		break;
		default : cout << "Should not have come to this point"<<endl;
		cout << "Processing " << loccopy << " with type " << loccopy->typecell << endl;
		break;
		}
	}

	return loccopy;
}
#else
//Doing full liveness for closures

void print_gc_move(cons* from, cons* to)
{
	if (from != NULL && to != NULL)
	{
		long int to_index = 0;
		long int from_index = 0;
		if (is_valid_address(from))
			from_index = (from - (cons*)buffer_live);
		else if (from)
			from_index = (from - (cons*)buffer_dead);

		if (to)
			to_index = (to - (cons*)buffer_live);

		cout << from_index << "-->" << to_index << "(" << to << ")" << endl;
		cout << "Type of cell " << from->typecell << endl;
	}
	return;
}


cons* deep_copy(cons* node, int gc_type, ostream& out)
{
	if (gc_type == 0)
		return followpaths_reachability(node);
	else
	{
		stateMapIter got = statemap.find("L/-1/c");
		//This should always return D/all
		assert(got != statemap.end());
		cerr << "Doing LGC with state D/all for node " << node << " state = " << got->second << endl;
		return followpaths(node, got->second);
	}
	return NULL;
}


cons* copy(cons* node)
{
	void *addr;
	if(node == NULL)
		return NULL;
	if(!(node >= buffer_dead && node < boundary_dead))
	{
//		cout << "Returning the same pointer as node is already in live buffer "<< node <<endl;
		return node;
	}
	else
	{
		heapslot=heapslot+1;
		cons *conscell=(cons*)node;
        GC_STAT_MARK_REACHABLE(conscell);
		if((conscell->forward >= buffer_live) &&
				(boundary_live > conscell->forward))
		{
			return (static_cast<cons*>(conscell->forward));
		}

		addr = dup_cons(conscell);
		conscell->forward=addr;
		copycells=copycells+1;
		++numcopied;
//		print_gc_move(conscell, (cons*)addr);
		return static_cast<cons*>(addr);
	}

	return NULL;
}
cons* followpaths_reachability(cons* loc)
{

	if (NULL == loc)
		return loc;

	if (loc->forward != NULL)
		return static_cast<cons*>(loc->forward);

	cons* loccopy = copy(loc);

	if (loccopy->typecell == consExprClosure)
	{
		cons *oldcar = loccopy->val.cell.car;
		cons *addr=followpaths_reachability(loccopy->val.cell.car);
		loccopy->val.cell.car=addr;


		cons* oldcdr = loccopy->val.cell.cdr;
		addr=followpaths_reachability(loccopy->val.cell.cdr);
		loccopy->val.cell.cdr=addr;

	}
	else
	{
		switch(loccopy->typecell)
		{
		case constIntExprClosure:
		case constBoolExprClosure:
		case constStringExprClosure:
		case nilExprClosure:
			break;
		case unaryprimopExprClosure:
		case binaryprimopExprClosure:
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			cons* oldarg1 = loccopy->val.closure.arg1;
			cons *addr=followpaths_reachability(loccopy->val.closure.arg1);
			loccopy->val.closure.arg1=addr;
			//print_gc_move(oldarg1, addr);


			cons* oldarg2 = loccopy->val.closure.arg2;
			addr=followpaths_reachability(loccopy->val.closure.arg2);
			loccopy->val.closure.arg2=addr;
			//print_gc_move(oldarg2, addr);
		}

		break;
		default : cout << "Should not have come to this point"<<endl;
		cout << "Processing " << loccopy << " with type " << loccopy->typecell << endl;
		break;
		}
	}

	return loccopy;
}
#endif


void clear_live_buffer(ostream& out)
{
	cons* start_ptr = static_cast<cons*>(buffer_live);
	cons* end_ptr = static_cast<cons*>(freept);

	while(start_ptr < freept)
	{
		switch(start_ptr->typecell)
		{
		case consExprClosure:
			if (!is_valid_address(start_ptr->val.cell.car))
			{
				out << "Setting car part to null for cell with index " << (start_ptr - getbufferlive()) << endl;
				start_ptr->val.cell.car = NULL;
			}
			if (!is_valid_address(start_ptr->val.cell.cdr))
				start_ptr->val.cell.cdr = NULL;
			break;
		case unaryprimopExprClosure:
			if (!is_valid_address(start_ptr->val.closure.arg1))
				start_ptr->val.closure.arg1 = NULL;
			break;
		case binaryprimopExprClosure:
			if (!is_valid_address(start_ptr->val.closure.arg1))
				start_ptr->val.closure.arg1 = NULL;
			if (!is_valid_address(start_ptr->val.closure.arg1))
				start_ptr->val.closure.arg1 = NULL;
			break;
		case funcArgClosure:
		case funcApplicationExprClosure:
			if (!is_valid_address(start_ptr->val.closure.arg1))
				start_ptr->val.closure.arg1 = NULL;
			if (!is_valid_address(start_ptr->val.closure.arg1))
				start_ptr->val.closure.arg1 = NULL;
			break;
		default : break;
		}
		++start_ptr;
	}
}

void print_activation_record_stack(ostream& out)
{
	for (auto st_elem : actRecStack)
	{
		out << st_elem.funcname << " at return point " << st_elem.return_point << endl;
	}
}

/*-----Memory manager-------*/

//!!!caution remove memory size from this function
// allocates heap in terms of NUMBER OF CONS CELLS
void allocate_heap(unsigned long size)
{
    unsigned long memorysize=size*sizeof(cons);
    if ((buffer_live=malloc(memorysize)) == 0) {
        fprintf(stderr, "Too big memory demanded. Reduce it.\n");
        exit(-1);
    }

    freept=buffer_live;
    scan=buffer_live;
    buffer_dead=(void*)((char*)(buffer_live)+(memorysize/2));
    boundary_live=buffer_dead;
    boundary_dead=(void*)((char*)(buffer_live)+memorysize);
    mmc=mmc+memorysize;
    newheapcells=0;oldheapcells=0;
    copycells=0;garbagecells=0;heapslot=0;

    FILE *f=fopen("./output/garbage-dump.txt","w");
    fclose(f);
    GC_STAT_SAVE_LIVE_HALF();
}

void calculate_garbage()
{
  garbagecells=garbagecells+(oldheapcells-copycells);
}

void detail_gc()
{
	FILE *f=fopen("./output/garbage-dump.txt","a");
	unsigned long curcells=oldheapcells;
	if (oldheapcells==0) curcells=newheapcells;     //for gc-disable
	garbagecells=curcells-copycells;
	fprintf(f,"%lu %lu %lu %lu %lu %lu %lu mmc=%d\n", curcells, curcells*sizeof(cons), copycells,
			copycells*sizeof(cons), garbagecells, garbagecells*sizeof(cons), heapslot, mmc);
	fclose(f);
}


void swap_buffer()
{
    void *temp;
    GC_STAT_INIT_PARAMS();
    
    //swap buffers
    temp=buffer_dead;
    buffer_dead=buffer_live;
    buffer_live=temp;
    //swap buffer boundaries
    temp=boundary_dead;
    boundary_dead=boundary_live;
    boundary_live=temp;
    //set free and scan pointers
    freept=buffer_live;
    scan=buffer_live;

    oldheapcells=newheapcells;
    newheapcells=0;copycells=0;
    heapslot=0;
}

void update_free(unsigned int size)
{
    freept=(void *) ((char*)freept + size);
}

int check_space(int size)
{
	if (((char*)boundary_live - (char*)freept) >= size)
    	return 1;
    else
    {
    	cout << "Boundary Live = "<<boundary_live << endl;
    	cout << "Freept = " << freept << endl;
    	return 0;
    }
}


cons* return_null()
{
  return NULL;
}

void* dup_cons(cons* cell)
{
    if (check_space(sizeof(cons))==0)
    {
      fprintf(stderr,"Not Sufficient Memory - cons\n");
      return NULL;
    }
    cons *conscell=(cons *)freept;
    update_free(sizeof(cons));
    newheapcells=newheapcells+1;
    memcpy(conscell, cell, sizeof(cons));
    conscell->forward = NULL;
    conscell->copied_using_rgc = false;
    return conscell;
}

cons* allocate_cons()
{
    GC_STAT_CLOCK_TICK();
    if (check_space(sizeof(cons))==0)
    {
    	fprintf(stderr,"No Sufficient Memory - cons\n");
    	throw bad_alloc();
    	return NULL;

    }
    cons *conscell=(cons *)freept;
    update_free(sizeof(cons));
    newheapcells=newheapcells+1;
    conscell->forward=NULL;
    conscell->setofStates = new stateset();
    conscell->isLive = false;
    conscell->forward = NULL;
    conscell->copied_using_rgc = false;
    ++num_of_allocations;
    GC_STAT_MARK_CREATED(conscell);
#ifdef TEST_RUN
    set_max_reachability();
#endif
    return conscell;
}


//////////////environment (activation record stack)


void empty_environment(string functionname)
{
    actRec newenv;
    newenv.return_point=-1;
    newenv.funcname = functionname;
    newenv.heapRefs = vector<var_heap>(0);
    newenv.nonRefs  = vector<var_stack>(0);

    actRecStack.push_front(newenv);

    mmc=mmc+(sizeof(actRec));
}

void make_environment(const char *functionname, std::string returnpoint)
{
	actRec newenv;
	//TODO: In lazy programs this can be 0. Check if there is another way.
	//if (actRecStack.size() > 0)
	//	actRecStack.front().return_point = returnpoint;

	newenv.return_point = "-1";
    newenv.funcname=functionname;
    actRecStack.push_front(newenv);
    mmc=mmc+(sizeof(actRec));
}

void delete_environment()
{

	//cout << "Removing function " << actRecStack.front().funcname << " with return point " << actRecStack.front().return_point << endl;
	actRecStack.pop_front();
    mmc=mmc-(sizeof(actRec));

    //TODO: In lazy programs this can be 0. Check if there is another way.
    //In the case of lazy evaluation the point of liveness checking need not be the current point
    //if (actRecStack.size() > 0)
    //	actRecStack.front().return_point="-1"; //reset
}

void set_return_point(int pt)
{
    actRecStack.front().return_point = pt;

}

//rename to return_top_of_stack
actRec& return_stack()
{
    actRecIter = actRecStack.begin();
    return actRecStack.front();
}

actRec& get_next_activation_record()
{
	++actRecIter;
	return *actRecIter;
}

int is_end_of_activation_record_stack()
{
	if (actRecIter == actRecStack.end())
		return 1;
	else
		return 0;
}

int is_empty_heap_ref_vector()
{
	if ((*actRecIter).heapRefs.empty())
		return 1;
	else
		return 0;
}

var_heap& get_heap_ref_vector()
{
	heapRefIter = (*actRecIter).heapRefs.begin();
	return *heapRefIter;
}

var_heap& get_next_heap_ref_vector()
{
	++heapRefIter;
	return *heapRefIter;
}

int is_end_of_heap_ref_vector()
{
	if (heapRefIter == (*actRecIter).heapRefs.end())
		return 1;
	else
		return 0;
}

int is_empty_non_ref_vector()
{
	if ((*actRecIter).nonRefs.empty())
		return 1;
	else
		return 0;
}

var_stack& get_non_ref_vector()
{
	nonRefIter = (*actRecIter).nonRefs.begin();
	return *nonRefIter;
}

var_stack& get_next_non_ref_vector()
{
	++nonRefIter;
	return *nonRefIter;
}

int is_end_of_non_ref_vector()
{
	if (nonRefIter == (*actRecIter).nonRefs.end())
		return 1;
	else
		return 0;
}

//!!!!!caution update it to check for already existing variable in same environment
void make_reference_addr(const char *var, void* addr)
{
	int found=0;
	vector<var_heap>& vh=actRecStack.front().heapRefs;
	vector<var_heap>::iterator it = vh.begin();
	while(it != vh.end()) {

		if (strcmp(it->varname.c_str(),var)==0)
		{
			it->ref=addr;
			found=1;
			break;
		}
		++it;
	}
	if(found==0){
		var_heap newref;
		newref.varname=var;
		newref.ref=addr;

		actRecStack.front().heapRefs.push_back(newref);
		mmc=mmc+(sizeof(var_heap));
	}

}

void make_reference_value(const char *var, void* val, const char type)
{
	int found=0;

	vector<var_stack>& vs=actRecStack.front().nonRefs;
	if (!vs.empty())
	{
		vector<var_stack>::iterator it = vs.begin();
		while(it != vs.end())
		{
			if (strcmp(it->varname.c_str(),var)==0)
			{
				it->value = val;
				it->type = type;
				found=1;
				break;
			}
			++it;
		}
	}

	if(found==0)
	{
		var_stack newref;
		newref.varname=var;
		newref.value=val;
		newref.type=type;
		actRecStack.front().nonRefs.push_back(newref);
		mmc=mmc+(sizeof(var_stack));
	}
}

char locate_var(const char *var)
{
   
	vector<var_heap> &vh = actRecStack.front().heapRefs;
	vector<var_heap>::iterator it = vh.begin();
	while(it != vh.end())
	{
		if(strcmp(it->varname.c_str(),var)==0)
			return 'a';

		++it;
	}
	cout << "Element "<< var <<" not found." << endl;
	return 'n'; /*Element not found.*/
}


cons* lookup_addr(const char *var)
{
	try
	{
		vector<var_heap> &vh=actRecStack.front().heapRefs;
		vector<var_heap>::iterator it = vh.begin();

		while(it != vh.end())
		{
			if (strcmp(it->varname.c_str(),var)==0)
			{
				return static_cast<cons*>(it->ref);
			}
			++it;
		}
	}
	catch(exception &e)
	{
		cout << "Exception while looking up address: " << e.what() << endl;
	}
	return NULL;
}

void* lookup_value(const char *var)
{
  vector<var_stack> &vs=actRecStack.front().nonRefs;
  vector<var_stack>::iterator it = vs.begin();

  while(it != vs.end())
  {
	  if (strcmp(it->varname.c_str(),var)==0)
	  {
		  return it->value;
	  }
	  ++it;
  }
  cout << "Value for " << var << "  not found" << endl;
  return (void*)-1;
}

int set_reference(var_heap , const char *var, void* newref)
{
  vector<var_heap>& vh = actRecIter->heapRefs;
  vector<var_heap>::iterator it = vh.begin();
  while(it != vh.end())
    {
      if (strcmp(it->varname.c_str(), var) == 0)
	{
	  it->ref = newref;
	  return 1;
	}
      ++it;
    }
  return 0;
}

int remove_reference(const char *var, var_heap)
{
	vector<var_heap>& vh = actRecIter->heapRefs;
	vector<var_heap>::iterator it = vh.begin();
	while(it != vh.end())
	{
		if (strcmp(it->varname.c_str(), var) == 0)
		{
			it->ref = NULL;
			return 1;
		}
		++it;
	}
	return 0;
}


//utility

void printval(void *ref, ostream& out)
{

	if (ref == NULL || ((cons*)ref)->typecell == nilExprClosure)
	{
		out << "()" << endl;
		return;
	}
	if (!is_valid_address(ref)) // Check added for liveness based GC as this might not be a valid pointer to chase.
	{                       	// In case of reachability all addresses will be valid.
//		out << "Invalid address "<< ref << endl;
//		out << "Index of address is " << ((cons*)ref - (cons*)buffer_dead) << endl;
		exit(-1);
	}

	cons *cref;
	cref=(cons *)ref;
	GC_STAT_UPDATE_LAST_USE(cref);

	if(!cref->inWHNF)
	{
		update_heap_refs.push(cref);
		print_stack.push(cref);
		cons *temp = cref->val.closure.expr->evaluate();

		//assert(is_valid_address(temp));
		//cref = update_heap_refs.top();
		cref = print_stack.top();
		//assert(is_valid_address(cref));

		cref->typecell = temp->typecell;
		cref->inWHNF = temp->inWHNF;
		cref->val = temp->val;
		update_heap_refs.pop();
		print_stack.pop();
	}

	if (cref->typecell == nilExprClosure)
	{
		out << "()" << endl;
		return;
	}

	if (cref->typecell == consExprClosure)
	{
		//Save the cdr pointer on stack. This might be updated if a GC happens during the printing of the car field.

		print_stack.push(cref->val.cell.cdr);
		cref->val.cell.can_delete_car = false;
		print_stack.push(cref);

		out<<"(";
		printval(cref->val.cell.car, out);

		cref = print_stack.top();
		print_stack.pop();

		cons* cdr = print_stack.top();

		cref->val.cell.can_delete_car = true;
		out<<".";
		printval(cdr, out);
		print_stack.pop();
		out<<")";
	}
	else if(cref->typecell == constIntExprClosure)
	{
		out << cref->val.intVal;
		out << endl;
	}
	else if(cref->typecell == constBoolExprClosure)
		out << cref->val.boolval;
	else if(cref->typecell == constStringExprClosure)
			out << *(cref->val.stringVal);
	else
		out << "Unknown type at " << cref << endl;

}

unsigned int current_heap()
{
   return ((char*)boundary_live - (char*)freept)/sizeof(cons);
}

////garbage collector support functions for python module
int sizeof_cons()
{
    return sizeof(cons);
}

cons* getCar(void* ref, const char fromGC)
{
   if (ref == NULL)
	   cout << "Error dereferencing a NULL pointer for car" << endl;
	cons *conscell=(cons*)ref;
#ifdef GC_ENABLE_STATS
    if (!fromGC)
        update_last_use(conscell);
#endif
    if (!conscell->inWHNF)
    	conscell->val.closure.expr->evaluate();
    assert(conscell->typecell == consExprClosure);
    cons* car=conscell->val.cell.car;
    return car;
}



cons* getCdr(void* ref, const char fromGC)
{
	if (ref == NULL)
		   cout << "Error dereferencing a NULL pointer for cdr" << endl;
    cons *conscell=(cons*)ref;
#ifdef GC_ENABLE_STATS
    if (!fromGC)
        update_last_use(conscell);
#endif
    if(!conscell->inWHNF)
    	conscell->val.closure.expr->evaluate();
    assert(conscell->typecell == consExprClosure);
    cons* cdr=conscell->val.cell.cdr;
    return cdr;
}


int isEqualCons (cons* v1,cons* v2, const char fromGC)
{
#ifdef GC_ENABLE_STATS
    if (!fromGC)
    {
        update_last_use(v1);
        update_last_use(v2);
    }

#endif

    if (v1==v2) return 1;
    else return 0;
}


//DO WE STILL NEED THESE FUNCTIONS?????
//getType function: -1 for NULL, 1 for cons, 2 for int, 3 for string, 4 for bool
int getType(void* node, int field)
{
    if (node==NULL)  return -1;
    cons * conscell=(cons*)node;

//    if (field==1)
//    {
//        if(conscell->typecar=='i' ) return 2;
//        else if(conscell->typecar=='s' ) return 3;
//        else if (conscell->typecar=='b' ) return 4;
//        else if(conscell->car==NULL) return -1;
//        else return 1;
//    }
//    else if (field==2){
//        if(conscell->typecdr=='i' ) return 2;
//        if(conscell->typecdr=='s' ) return 3;
//        else if (conscell->typecdr=='b' ) return 4;
//        else if(conscell->cdr==NULL) return -1;
//        else return 1;
//    }
    return 0;
}


void start_GC_dump()
{
	//gcout.open("GC"+to_string(gccount), ios_base::out);
	//gcout << "boundary_live = " << boundary_live << endl;
	//gcout << "freeptr = " << freept << endl;
	//gcout << "buffer_live= " << buffer_live << endl;
}
void end_GC_dump()
{
	 //gcout.close();
	++gccount;
}
//gc_type : 0 --> gc-plain
//          1 --> gc-live
void update_heap_ref_stack(ostream& out, int gc_type)
{
	stack<cons*> temp;

	while(!print_stack.empty())
	{
		cons* heap_ref = print_stack.top();
		if (gc_type == 0)
		{
			followpaths_reachability(heap_ref);
			heap_ref = static_cast<cons*>(heap_ref->forward);
		}
		else if (heap_ref)
		{
			if (heap_ref->forward == NULL)
			{
				cons* new_ref;
				if (gc_type == 1 && (heap_ref->typecell == consExprClosure))
				{

					if (heap_ref->val.cell.can_delete_car == false)
					{
						//If it is liveness based GC and we are copying a cons cell then check for can_delete_car flag
						//whether to copy the cdr part or not

						new_ref = copy(heap_ref);
						GC_STAT_UPDATE_LAST_GC(new_ref);
						//Should we count this during garbage collection?

						cons* cdr_ref = deep_copy(heap_ref->val.cell.cdr, gc_type, out);
						new_ref->val.cell.cdr = static_cast<cons*>(cdr_ref);
					}
				}
				else
				{
					new_ref = deep_copy(heap_ref, gc_type, out);
				}
			}
			heap_ref = static_cast<cons*>(heap_ref->forward);
		}
		temp.push(heap_ref);
		print_stack.pop();
	}
	while(!temp.empty())
	{
		print_stack.push(temp.top());
		temp.pop();
	}
	int i = 0;
	while(!update_heap_refs.empty())
	{
		cons* heap_ref = update_heap_refs.top();
		if (!is_valid_address(heap_ref) && heap_ref->forward == NULL)
		{
			cout << "ERROR !!!! " << heap_ref << endl;
			cout << "In heap ref stack copying " << (heap_ref - (cons*)buffer_dead) << " with index " << i << endl;
			assert(false);
		}

		heap_ref = static_cast<cons*>(heap_ref->forward);
		temp.push(heap_ref);
		update_heap_refs.pop();
		++i;
	}
	while(!temp.empty())
	{
		update_heap_refs.push(temp.top());
		temp.pop();
	}

}

//garbage collector functions



int copy_scan_children_lgc(cons* node)
{
	cons *addr=NULL;
	cons *conscell = node;

	if (conscell->typecell == consExprClosure)
	{
		cons* oldcar = conscell->val.cell.car;
		//addr=copy_scan_children_lgc(conscell->val.cell.car);
		conscell->val.cell.car=addr;


		cons* oldcdr = conscell->val.cell.cdr;
		//addr=copy_scan_children_lgc(conscell->val.cell.cdr);
		conscell->val.cell.cdr=addr;

	}
	else
	{
		switch(conscell->typecell)
		{
		case constIntExprClosure:
		case constBoolExprClosure:
		case constStringExprClosure:
		case nilExprClosure:
			break;
		case unaryprimopExprClosure:
		case binaryprimopExprClosure:
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			cons* oldarg1 = conscell->val.closure.arg1;
			addr=copy(conscell->val.closure.arg1);
			conscell->val.closure.arg1=addr;

			cons* oldarg2 = conscell->val.closure.arg2;
			addr=copy(conscell->val.closure.arg2);
			conscell->val.closure.arg2=addr;
		}

		break;
		default : cout << "Should not have come to this point"<<endl;
				  cout << "Processing " << conscell << " with type " << conscell->typecell << endl;
		break;
		}
	}

	return (sizeof(cons));
}

inline bool is_closure_type(cons* node)
{
	return (node->typecell == unaryprimopExprClosure) ||
			(node->typecell == binaryprimopExprClosure) ||
			(node->typecell == funcApplicationExprClosure) ||
			(node->typecell == funcArgClosure);

}


int copy_scan_children(cons* node)
{
	cons *addr=NULL;
	cons *conscell = node;

	if (conscell->typecell == consExprClosure)
	{
		cons* oldcar = conscell->val.cell.car;
		addr=copy(conscell->val.cell.car);
		conscell->val.cell.car=addr;


		cons* oldcdr = conscell->val.cell.cdr;
		addr=copy(conscell->val.cell.cdr);
		conscell->val.cell.cdr=addr;

	}
	else
	{
		switch(conscell->typecell)
		{
		case constIntExprClosure:
		case constBoolExprClosure:
		case constStringExprClosure:
		case nilExprClosure:
			break;
		case unaryprimopExprClosure:
		case binaryprimopExprClosure:
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			cons* oldarg1 = conscell->val.closure.arg1;
			addr=copy(conscell->val.closure.arg1);
			conscell->val.closure.arg1=addr;
			DBG(print_gc_move(oldarg1, addr));


			cons* oldarg2 = conscell->val.closure.arg2;
			addr=copy(conscell->val.closure.arg2);
			conscell->val.closure.arg2=addr;
			DBG(print_gc_move(oldarg2, addr));
		}

		break;
		default : cout << "Should not have come to this point"<<endl;
				  cout << "Processing " << conscell << " with type " << conscell->typecell << endl;
		break;
		}
	}

	return (sizeof(cons));
}

#ifdef REACHABILITY_DFS



void reachability_gc()
{
	clock_t pstart = clock();
	++gccount;
#ifdef __DEBUG__GC
	ofstream pre("GC" + to_string(gccount) + ".txt", ios_base::out);
	pre << "Doing reachability based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	ofstream pregc("PreGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(pregc);
	pregc.close();
	print_activation_record_stack(pre);
#else
	ostream &pre = null_stream;
#endif
	swap_buffer();
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref)
			{
				DBG(pre << "Processing root variable " << vhit->ref << " and index "<< ((cons*)vhit->ref - (cons*)buffer_dead)<<endl);
				cons *addr   = followpaths_reachability(static_cast<cons*>(vhit->ref));
				//print_gc_move((cons*)vhit->ref, addr, pre);
				DBG(pre << "Processed variable " << vhit->varname << " copied it to " << ((cons*)addr - (cons*)buffer_live) << endl);
				vhit->ref = addr;

			}
		}
	}
	update_heap_ref_stack(pre, 0);
#ifdef __DEBUG__GC
	DBG(pre << "Number of cells copied " << ((cons*)freept - (cons*)buffer_live) << " = " << copycells << endl);
	ofstream postgc("PostGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(postgc);
	postgc.close();
#endif
	clock_t pend = clock();
	double gc_time = ((double(pend - pstart)/CLOCKS_PER_SEC));
//	cerr << "GC Time for " << gccount << " = " << gc_time << endl;
	gctime += gc_time;
	return;
}
#endif

#ifdef TEST_RUN
long find_reachability(void* c)
{
	cons* cell = (cons*)c;
	long count = 0;
	if (cell && cell->visited == 0)
	{
		cell->visited = 1;
		++count;
		if(cell->typecar == 'a')
			count += find_reachability(cell->car);
		if(cell->typecdr == 'a')
			count += find_reachability(cell->cdr);
	}
	return count;
}

void set_max_reachability()
{
	long maxheapreachability = 0;
	for (void* i = buffer_live; i < freept ; i += sizeof(cons))
	{
		((cons*)i)->visited = 0;
	}
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref && ((cons*)vhit->ref)->visited == 0)
			{
				((cons*)vhit->ref)->visited = 1;
				++maxheapreachability;
				if (((cons*)vhit->ref)->typecar == 'a')
					maxheapreachability += find_reachability(((cons*)vhit->ref)->car);
				if (((cons*)vhit->ref)->typecdr == 'a')
					maxheapreachability += find_reachability(((cons*)vhit->ref)->cdr);

			}

		}
	}
	if (maxheapreachability > gmaxheapreachability)
		gmaxheapreachability = maxheapreachability;
	return;
}
#endif


#ifdef REACHABILITY_BFS
void reachability_gc()
{

	++gccount;
	//dump_heap_as_graphviz();

#ifdef __DEBUG__GC
	gcout << "Doing reachability based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	gcout << " At program point " << return_stack().funcname <<  endl;
	ofstream pre("PreGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(pre);
	pre.close();
	gcout << "Starting reachability based GC #"<< gccount << " after " <<  num_of_allocations << " allocations."<<endl;
#endif

	swap_buffer();
	numcopied = 0;
	int index = 0;

	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			cons *reference=static_cast<cons*>(vhit->ref);
			cons *addr=copy((cons*)reference);
			vhit->ref = addr;
#ifdef __DEBUG__GC
			gcout << "Processed variable " << vhit->varname << " copied it to " << ((cons*)addr - (cons*)buffer_live) << endl;
#endif
		}
	}
#ifdef __DEBUG__GC
	gcout << "Finished processing root variables"<<endl;
#endif
	//chasing reachable cells from roots
	while(lt_scan_freept()==1)
	{
#ifdef __DEBUG__GC
		gcout << "Processing " << ((cons*)getscan() - (cons*)buffer_live) << endl;
#endif
		copy_scan_children((cons*)getscan());
		update_scan();
	}
#ifdef __DEBUG__GC
	gcout << "Num of cells copied during garbage collection " << numcopied << endl;
#endif
#ifdef TEST_RUN
	cout << "Max heap reachability till now "<< gmaxheapreachability << endl;
#endif
	update_heap_ref_stack(null_stream, 0);

	return;
}
#endif


#ifdef ENABLE_SHARING_STATS
bool comp (pair<int, int> a, pair<int,int> b)
{
	return a.second > b.second;
}


void print_sharing_stats()
{
	int j = 0;
	cout << "Printing sharing stats"<<endl;
	vector< pair<int,int> > numvisits;
	for (void* i = buffer_live; i < freept ; i += sizeof(cons), ++j)
		numvisits.push_back(make_pair(j, ((cons*)i)->visited));

    sort(numvisits.begin(), numvisits.end(), comp);

    for (vector<pair<int,int> >::iterator it = numvisits.begin(); it != numvisits.end(); ++it)
		cout << "Cell " << it->first << " is visited " << it->second << " times" << endl;
}
#endif

#ifdef LIVENESS_DFS
#ifdef _OPT_TIME
void liveness_gc()
{
	clock_t pstart = clock();
	 ++gccount;
//	 cerr << "Starting LGC#"<<gccount<<endl;
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  ((cons*)i)->visited = 0;
#endif




#ifdef __DEBUG__GC
	  ofstream pre("GC" + to_string(gccount) + ".txt", ios_base::out);
	  pre << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	  ofstream pregc("PreGC" + to_string(gccount) + ".txt", ios_base::out);
	  create_heap_bft(pregc);
	  pregc.close();
	  print_activation_record_stack(pre);
#else
	  ostream &pre = null_stream;
#endif
	  DBG(pre << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl);
#ifdef FIND_REACHABLE
	  numcopied = 0;
	  size_t num_reachable_cells = find_num_reachable_cells();
#endif

	  swap_buffer();

  for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
    {
//	  cerr << "Processing function " << stackit->funcname << endl;
	  for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
      {
    	  DBG(pre << "Doing gc at return point " << stackit->return_point << endl);
		  string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;
    	  stateMapIter got = statemap.find(nodeName);

    	  if (got != statemap.end())
    	  {

    		  DBG(pre << "Processing var " << vhit->varname << endl);
    		  DBG(pre << "Index is " << ((cons*)vhit->ref - (cons*)buffer_dead) << endl);
    		  cons *addr   = followpaths(static_cast<cons*>(vhit->ref), got->second, pre);
    		  vhit->ref = addr;


#ifdef ENABLE_SHARING_STATS
    		  if (vhit->ref)
    			  ++(((cons*)vhit->ref)->visited);
#endif
    	  }
    	  else
    	  {
    		  //The root variable is not live, set it to NULL as it might later become part of a closure and cause problems
    		  //during GC.
    		  vhit->ref = NULL;
    	  }
      }
    }
  DBG(pre << "Completed liveness GC" << endl);
  update_heap_ref_stack(pre, 1);
  DBG(pre << "Number of cells copied " << ((cons*)freept - (cons*)buffer_live) << " = " << copycells << endl);
  clear_live_buffer(pre);
#ifdef __DEBUG__GC
	pre.close();
	ofstream postgc("PostGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(postgc);
	postgc.close();
#endif
#ifdef ENABLE_SHARING_STATS
  print_sharing_stats();
#endif
  clock_t pend = clock();
  double gc_time = ((double(pend - pstart)/CLOCKS_PER_SEC));
//  cerr << "GC Time for " << gccount << " = " << gc_time << endl;
  gctime += gc_time;
#ifdef FIND_REACHABLE
  cout << "Num of reachable cells = " << num_reachable_cells << endl;
  cout << "Num of live cells = " << numcopied << endl;
#endif
  return;
}

cons* followpaths(cons* loc, state_index index, ostream& out)
{

	if (NULL == loc)
		return loc;

	cons* loccopy = copy(loc, out);

    DBG(out << "Copying cell from " << loc << " with type " << loc->typecell << endl);

#ifdef ENABLE_SHARING_STATS
  if (loccopy)
	  ++(((cons*)loccopy)->visited);
#endif
  //Can we add a check here too to find out if a cons cell has been copied using reachability?
  //That might improve the efficiency
  if (loc->typecell == consExprClosure && loc->copied_using_rgc == false)
  {
	  state_index a0 = state_transition_table[index][0]; //get_target_dfastate(index, 0);
	  if (a0 > 0)
	  {
		  cons* newloc = getCar(loc, 1);
		  cons* addr = followpaths(newloc, a0);
		  set_car(loccopy, addr);
	  }
	  state_index a1 = state_transition_table[index][1];//get_target_dfastate(index, 1);
	  if (a1 > 0)
	  {
		  cons* newloc = getCdr(loc, 1);
		  cons* addr = followpaths(newloc, a1);
		  set_cdr(loccopy, addr);
	  }

  }
  else if (loc->typecell == funcApplicationExprClosure ||
		  loc->typecell == unaryprimopExprClosure ||
		  loc->typecell == binaryprimopExprClosure ||
		  loc->typecell == funcArgClosure)
  {
	  DBG(out << "Copying cell with index " << (loc - getbufferlive()) << endl);
	  if (loc == loccopy)
	  {
		  DBG(out << "returning already copied pointer"<<endl);
		  return static_cast<cons*>(loc->forward);
	  }
	  else
	  {

		  DBG(out << "arg1 --> " << loc->val.closure.arg1 << " with index = " <<(loc->val.closure.arg1 - (cons*)buffer_dead) << endl);
		  cons* oldarg1 = loc->val.closure.arg1;
		  cons* addr = deep_copy(loc->val.closure.arg1, 1, out);
		  loccopy->val.closure.arg1 = addr;
		  DBG(out << "Copied arg1 from " << oldarg1 << " to " << addr <<endl);

		  DBG(out << "arg2 --> " << loc->val.closure.arg2 << " with index = " <<(loc->val.closure.arg2 - (cons*)buffer_dead) << endl);
		  cons* oldarg2 = loc->val.closure.arg2;
		  addr = deep_copy(loc->val.closure.arg2, 1, out);
		  loccopy->val.closure.arg2 = addr;
		  DBG(out << "Copied arg2 from " << oldarg2 << " to " << addr << endl);

		  loc->copied_using_rgc = true;
		  GC_STAT_UPDATE_LAST_GC(loccopy);
	  }

  }

  return loccopy;
}
#else
//Liveness GC with liveness for closures
void liveness_gc()
{
	clock_t pstart = clock();
	 ++gccount;
	 cerr << "Starting LGC#"<<gccount<<endl;
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  ((cons*)i)->visited = 0;
#endif




#ifdef __DEBUG__GC
	  ofstream pre("GC" + to_string(gccount) + ".txt", ios_base::out);
	  pre << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	  ofstream pregc("PreGC" + to_string(gccount) + ".txt", ios_base::out);
	  create_heap_bft(pregc);
	  pregc.close();
	  print_activation_record_stack(pre);
#else
	  ostream &pre = null_stream;
#endif
	  pre << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	  swap_buffer();

  for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
    {
	  cerr << "Processing function " << stackit->funcname << endl;
	  for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
      {
    	  cerr << "Doing gc at return point " << stackit->return_point << endl;
		  string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;
    	  stateMapIter got = statemap.find(nodeName);
    	  cerr << "Looking up " << nodeName << endl;
    	  if (got != statemap.end())
    	  {
    		  cons *addr   = followpaths(static_cast<cons*>(vhit->ref), got->second);
    		  vhit->ref = addr;

#ifdef ENABLE_SHARING_STATS
    		  if (vhit->ref)
    			  ++(((cons*)vhit->ref)->visited);
#endif
    	  }
      }
    }
  cerr << "Completed liveness GC" << endl;
  update_heap_ref_stack(pre, 1);
  clear_live_buffer(pre);
  cerr << "Copied " << numcopied << " cells" << endl;
  numcopied = 0;
#ifdef __DEBUG__GC
	pre.close();
	ofstream postgc("PostGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(postgc);
	postgc.close();
#endif
#ifdef ENABLE_SHARING_STATS
  print_sharing_stats();
#endif
  clock_t pend = clock();
  double gc_time = ((double(pend - pstart)/CLOCKS_PER_SEC));
 // cout << "GC Time for " << gccount << " = " << gc_time << endl;
  gctime += gc_time;
  //cerr << "Number of cells copied = " << numcopied << endl;
  return;
}

cons* followpaths(cons* loc, state_index index)
{
  cons* loccopy = copy(loc);
#ifdef ENABLE_SHARING_STATS
  if (loccopy)
	  ++(((cons*)loccopy)->visited);
#endif

  if (NULL == loc)
	  return loc;

  switch(loc->typecell)
  {
  case consExprClosure :
  {
	  state_index a0 = state_transition_table[index][0]; //get_target_dfastate(index, 0);
	  if (a0 > 0)
	  {
		  cons* newloc = getCar(loc, 1);
		  cons* addr = followpaths(newloc, a0);
		  set_car(loccopy, addr);
	  }


	  state_index a1 = state_transition_table[index][1];//get_target_dfastate(index, 1);
	  if (a1 > 0)
	  {
		  cons* newloc = getCdr(loc, 1);
		  cons* addr = followpaths(newloc, a1);
		  set_cdr(loccopy, addr);
	  }
  }

  break;
  case nilExprClosure:
  case constIntExprClosure:
  case constBoolExprClosure:
  case constStringExprClosure: return loccopy;
  break;
  case unaryprimopExprClosure :
  {
	  string liveness_string = "L/" + *(loccopy->val.closure.prog_pt) + "/" + *(loccopy->val.closure.arg1_name);

	  auto liveness_state = statemap.find(liveness_string);
	  if (liveness_state != statemap.end())
	  {
	    //cerr << "Doing GC corresponding to " << liveness_string << endl;
	  	  auto new_arg1 = followpaths(loccopy->val.closure.arg1, liveness_state->second);
	  	  loccopy->val.closure.arg1 = new_arg1;
	  }
  }
  break;
  case binaryprimopExprClosure:
  {
	  string liveness_string = "L/" + *(loccopy->val.closure.prog_pt) + "/" + *(loccopy->val.closure.arg1_name);

	  auto liveness_state = statemap.find(liveness_string);
	  if (liveness_state != statemap.end())
	  {
	    // cerr << "Doing GC corresponding to " << liveness_string << endl;
		  auto new_arg1 = followpaths(loccopy->val.closure.arg1, liveness_state->second);
		  loccopy->val.closure.arg1 = new_arg1;
	  }
	  liveness_string = "L/" + *(loccopy->val.closure.prog_pt) + "/" + *(loccopy->val.closure.arg2_name);

	  liveness_state = statemap.find(liveness_string);
	  if (liveness_state != statemap.end())
	  {
	    //cerr << "Doing GC corresponding to " << liveness_string << endl;
		  auto new_arg2 = followpaths(loccopy->val.closure.arg2, liveness_state->second);
		  loccopy->val.closure.arg2 = new_arg2;
	  }
  }
  break;
  case funcApplicationExprClosure:
  case funcArgClosure:
  {
      //What happens to arg1? Shouldn't it get updated during GC?
	  string liveness_string = "L/" + *(loccopy->val.closure.prog_pt) + "/" + *(loccopy->val.closure.arg2_name);

	  auto liveness_state = statemap.find(liveness_string);
	  if (liveness_state != statemap.end())
	  {
	    //cerr << "Doing GC corresponding to " << liveness_string << endl;
		  auto new_arg2 = followpaths(loccopy->val.closure.arg2, liveness_state->second);
		  loccopy->val.closure.arg2 = new_arg2;
	  }

	  if (loccopy->val.closure.arg1 != NULL)
	  {
		  //More arguments exist and have to be copied
		  auto liveness_state = statemap.find("L/-1/c");
		  //Using dummy state to always make it live
		  auto new_arg1 = followpaths(loccopy->val.closure.arg1, liveness_state->second);
		  loccopy->val.closure.arg1 = new_arg1;
	  }
  }
  break;
  default : cerr << "Should have never come here " << loc << endl;
  }

  return loccopy;
}
#endif


#endif


#if 1

//Traverse the heap in depth first manner and at each node check for liveness using the liveness automata 
//and annotate it with liveness info.

void depthfirstpaths(cons* loc, state_index index)
{
	
	if (NULL == loc)
		return;
	
	loc->isLive = true;

	switch(loc->typecell)
	{
	case constIntExprClosure:
	case constBoolExprClosure:
	case constStringExprClosure:
	case nilExprClosure:
	{
		break;
	}
	case unaryprimopExprClosure:
	case binaryprimopExprClosure:
	case funcApplicationExprClosure:
	case funcArgClosure:
	{
		cons* oldarg1 = loc->val.closure.arg1;
		depthfirstpaths(oldarg1, 0);

		cons* oldarg2 = loc->val.closure.arg2;
		depthfirstpaths(oldarg2, 0);

	}
	break;
	case consExprClosure:
	{
		state_index a0;
		if (index > 0)
			a0 = state_transition_table[index][0]; //get_target_dfastate(index, 0);
		else 
			a0 = 0;


		cons* newloc = (cons*)getCar(loc, 1);
		if (a0 >= 0) //Either if this is live due to automaton or if this is part of a closure
			depthfirstpaths(newloc, a0);


		state_index a1;
		if (index > 0)
			a1 = state_transition_table[index][1];//get_target_dfastate(index, 1);
		else
			a1 = 0;


		newloc = (cons*)getCdr(loc, 1);
		if (a1 >= 0)
			depthfirstpaths(newloc, a1);
	}
	break;
	default : break;
	}
  return;
}

void traverseHeap()
{

	DOUT(" in GC#" << gccount);
	
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;
			stateMapIter got = statemap.find(nodeName);

			if (got != statemap.end())
			{
				DOUT(nodeName << " is live.");
				depthfirstpaths((cons*)vhit->ref, got->second);
			}

		}
	}
	
	return;
	
}


std::string getNodeLabel(cons* conscell, size_t idx)
{
	char buf[1024];
	sprintf(buf, "\"%p(%lu)\"", conscell,idx);
	return buf;
}

void gen_graphviz_code(cons* conscell, ostream& out)
{

	if (NULL == conscell)
		return;

	switch(conscell->typecell)
	{
	case constIntExprClosure:
	case constBoolExprClosure:
	case constStringExprClosure:
	case nilExprClosure:
	{
		if (is_valid_address(conscell))
		{
			size_t idx = conscell - (cons*)buffer_live;
			out << idx  << " [label=" <<  getNodeLabel(conscell, idx) <<",shape=box, color=";
			if (conscell->isLive)
				out << "green]"<<endl;
			else
				out << "red]"<<endl;
		}
		break;
	}
	case unaryprimopExprClosure:
	case binaryprimopExprClosure:
	case funcApplicationExprClosure:
	case funcArgClosure:
	{
		if (is_valid_address(conscell))
		{
			cons* oldarg1 = conscell->val.closure.arg1;
			size_t idx = conscell - (cons*)buffer_live;
			out << idx  << " [label=" <<  getNodeLabel(conscell, idx) <<",shape=box, color=";
			if (conscell->isLive)
				out << "green]"<<endl;
			else
				out << "red]"<<endl;

			//Add graphviz code to generate an edge between loc and oldarg1
			if (oldarg1 && is_valid_address(oldarg1))
			{
				gen_graphviz_code(oldarg1, out);
				size_t dest1 = oldarg1 - (cons*)buffer_live;
				out << idx << " -> " << dest1 << "[color=blue];" <<endl;
			}
			cons* oldarg2 = conscell->val.closure.arg2;
			if (oldarg2 && is_valid_address(oldarg2))
			{
				gen_graphviz_code(oldarg2, out);
				//Add graphviz code to generate an edge between loc and oldarg1
				size_t dest2 = oldarg2 - (cons*)buffer_live;
				out << idx << " -> " << dest2 << "[color=yellow];" << endl;
			}
		}
	}
	break;
	case consExprClosure:
	{
		if (is_valid_address(conscell))
		{

			//Add graphviz code to generate a node with number(index)
			//Add color info
			size_t idx = conscell - (cons*)buffer_live;
			out << idx  << " [label=" <<  getNodeLabel(conscell, idx) <<",shape=box, color=";
			if (conscell->isLive)
				out << "green]"<<endl;
			else
				out << "red]"<<endl;

			cons* carloc = (cons*)getCar(conscell, 1);
			if (carloc && is_valid_address(carloc))
			{
				gen_graphviz_code(carloc, out);
				//Add edge between loc and carloc
				size_t dest1 = carloc - (cons*)buffer_live;
				out << idx << " -> " << dest1 << "[color=blue];" << endl;
			}
			cons* cdrloc = (cons*)getCdr(conscell, 1);
			if (cdrloc && is_valid_address(cdrloc))
			{
				gen_graphviz_code(cdrloc, out);
				//Add edge between loc and cdrloc
				size_t dest2 = cdrloc - (cons*)buffer_live;
				out << idx << " -> " << dest2 << "[color=yellow];" << endl;
			}
		}
	}
	break;
	default : break;
	}
return;
}

void dump_heap_as_graphviz()
{
	DOUT("In dump_heap_as_graphviz");


	for (auto i = (cons*)buffer_live; i < (cons*)boundary_live; ++i)
		i->isLive = false;
	
	traverseHeap();
	
	ofstream outfile("heap_for_GC" + to_string(gccount) + ".gv", ios::out);
	outfile << "digraph " << "heap_for_GC" << to_string(gccount) << "{"<<endl; 
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref)
			{
				//Add code to generate graphviz code and add it to the stream
				gen_graphviz_code((cons*)vhit->ref, outfile);
			}
			
				
		}
	}
	outfile << "}";
	outfile.close();
	return;

}

#endif

#ifdef LIVENESS_BFS
void liveness_gc()
{
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  ((cons*)i)->visited = 0;
#endif
	numcopied = 0;
	++gccount;

	//cout << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;

#ifdef __DEBUG__GC
	ofstream pre("GC" + to_string(gccount) + ".txt", ios_base::out);
	pre << "Doing liveness based GC #" << gccount << " after " << num_of_allocations << " allocations"<<endl;
	ofstream pregc("PreGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(pregc);
	pregc.close();
	print_activation_record_stack(pre);
#else
	ostream &pre = null_stream;
#endif

	swap_buffer();

	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
//		pre << "Processing activation record for " << stackit->funcname << endl;
//		pre << "Return point considered " << stackit->return_point << endl;
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
		
			string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;
			stateMapIter got = statemap.find(nodeName);
//			pre << "Doing GC at " << stackit->funcname << " & prog_pt " << stackit->return_point << endl;
//			pre << "Processing variable " << nodeName << endl;
			if (got != statemap.end())
			{
			
				pre << nodeName << " is live" << endl;
				if (vhit->ref && ((cons*)vhit->ref)->forward == NULL) //Check to see if a new cell was allocated
					((cons*)vhit->ref)->setofStates->clear();

				//TODO : This might not be a simple copy
				cons* cons_cell = (cons*)vhit->ref;
				cons* addr = NULL;
				
				assert(cons_cell);
				
				if (cons_cell->typecell == consExprClosure)
				{
					pre << "Copying only one cell as part of cons for " << (cons_cell - (cons*)buffer_dead) << endl;
					addr = copy(cons_cell, pre);
					print_gc_move(cons_cell, addr, pre);
				}
				else
				{
					pre << "Doing a deep copy for " << (cons_cell - (cons*)buffer_dead) << endl;
					addr = deep_copy(cons_cell, pre); //Copy using Breadth First Traversal
					pre << "Copied cell to " << ((cons*)addr - (cons*)buffer_live) << endl;
					print_gc_move(cons_cell, addr, pre);
				}
				
				if (addr)
				{
					cons* c = (cons*) addr;
					c->setofStates->insert(got->second);
				}
				vhit->ref = addr;
//				pre << "Processed variable " << vhit->varname << " copied it to " << ((cons*)addr - (cons*)buffer_live) << endl;
			}
//			else
//			{
//				pre << "Variable is not live " << vhit->varname << endl;
//			}
		}
	}

	pre << "Completed processing root set"<<endl;

	//chasing reachable cells from roots
	while(lt_scan_freept()==1)//
	{
		pre << "Processing node " << ((cons*)scan - (cons*)buffer_live) << endl;
		copy_children(scan, ((cons*)scan)->setofStates, pre);
		update_scan();
	}
	update_heap_ref_stack(pre, 1);
//	pre << "Copied " << numcopied << " cells to the live buffer." << endl;

#ifdef __DEBUG__GC
	pre.close();
	ofstream postgc("PostGC" + to_string(gccount) + ".txt", ios_base::out);
	create_heap_bft(postgc);
	postgc.close();
#endif
	
	
#ifdef ENABLE_SHARING_STATS
  print_sharing_stats();
#endif
	return;
}

void copy_children(void* cellptr, stateset* st, ostream& out)
{
	stateset s0;
	stateset s1;
//	out << "Type of node is " << ((cons*)cellptr)->typecell <<
//			" for " << ((cons*)cellptr - (cons*)buffer_live) << endl;
#ifdef ENABLE_SHARING_STATS
	if (cellptr)
		++(((cons*)(cellptr))->visited);
#endif

	if (((cons*)cellptr)->typecell != consExprClosure)
	{
#ifdef __DEBUG__GC
		cons* cptr = static_cast<cons*>(cellptr);
		if (cptr->typecell == constIntExprClosure ||
				cptr->typecell == constBoolExprClosure ||
				cptr->typecell == constStringExprClosure ||
				cptr->typecell == nilExprClosure)
		{
			return;
		}

		if (is_valid_address(cptr))
			return;
//		out << "Returning as cell is not cons " << ((cons*)cellptr - (cons*)buffer_live) << endl;
		if (!(cptr->val.closure.arg1 == NULL || cptr->val.closure.arg1->forward != NULL))
		{
			cerr << "Closure was not properly copied for " << cellptr << " with index " <<
					(cptr - getbufferlive()) << endl;
			cerr << "Address that was not copied " << cptr->val.closure.arg1 << endl;
			cerr << "Type of closure is " << cptr->typecell << endl;
		}
		assert(cptr->val.closure.arg1 == NULL || cptr->val.closure.arg1->forward != NULL);
		if (!(cptr->val.closure.arg2 == NULL || cptr->val.closure.arg2->forward != NULL))
		{
			cerr << "Closure was not properly copied for " << cellptr << " with index " <<
					(cptr - getbufferlive()) << endl;
			cerr << "Type of closure is " << cptr->typecell << endl;
		}
		assert(cptr->val.closure.arg2 == NULL || cptr->val.closure.arg2->forward != NULL);
		//Add an assert to ensure that if its not a cons cell, the closure has already been copied fully.
#endif
		return;
	}


	//assert(!st->empty());
	for(setiter i = st->begin(); i != st->end(); ++i)
	{
//		out << "Checking liveness with state " << *i << endl;
		state_index t = state_transition_table[*i][0];
		if (t > 0)
			s0.insert(t);
		t = state_transition_table[*i][1];
		if (t > 0)
			s1.insert(t);
	}
	
	
	assert(((cons*)cellptr)->typecell == consExprClosure);
	out << "Processing cons cell " << ((cons*)cellptr - (cons*)buffer_live) << endl;
	//process car field
	if(!s0.empty())
	{
		out << "Copying the car part " << endl;
		//if (getType(cellptr, 1) == 1)//Do this only if car is address type
		{

			void* fpt = freept;
			void* newaddr;
			cons* car_ptr = ((cons*)cellptr)->val.cell.car;
			assert(car_ptr);
			if (car_ptr->typecell == consExprClosure)
				newaddr = copy(car_ptr, out);
			else
				newaddr = deep_copy(car_ptr, out);
			
			if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
			{
				((cons*)newaddr)->setofStates->clear();
				((cons*)newaddr)->setofStates->insert(s0.begin(), s0.end());
			}
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
			{
				((cons*)newaddr)->setofStates->insert(s0.begin(), s0.end());
			}
			else if (newaddr) //forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				stateset *s_child = ((cons*)newaddr)->setofStates;
				for(setiter i = s0.begin(); i != s0.end(); ++i)
					if (s_child->find(*i) == s_child->end())
						s_new.insert(*i);
				if (!s_new.empty())
				{
					copy_children(newaddr, &s_new, out);
					((cons*)newaddr)->setofStates->insert(s_new.begin(), s_new.end());
				}
			}
			((cons*)cellptr)->val.cell.car = static_cast<cons*>(newaddr);
		}
	}
	else
		out << "car part is not live for " << ((cons*)cellptr - (cons*)buffer_live) << endl;

	//process cdr field
	if(!s1.empty())
	{
		out << "Copying the cdr part " << endl;
		//if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
		{
			//assert(getType(cellptr, 2) == 1);
			void* fpt = freept;
						
			void* newaddr;
			cons* cdr_ptr = ((cons*)cellptr)->val.cell.cdr;
			
			if (cdr_ptr->typecell == consExprClosure)
				newaddr = copy(cdr_ptr, out);
			else
				newaddr = deep_copy(cdr_ptr, out);
			
			if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
			{
				((cons*)newaddr)->setofStates->clear();
				((cons*)newaddr)->setofStates->insert(s1.begin(), s1.end());
			}
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
			{
				((cons*)newaddr)->setofStates->insert(s1.begin(), s1.end());
			}
			else if(newaddr)//forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				stateset *s_child = ((cons*)newaddr)->setofStates;
				for(setiter i = s1.begin(); i != s1.end(); ++i)
					if (s_child->find(*i) == s_child->end())
						s_new.insert(*i);
				if (!s_new.empty())
				{
					copy_children(newaddr, &s_new, out);
					((cons*)newaddr)->setofStates->insert(s_new.begin(), s_new.end());
				}
			}
			((cons*)cellptr)->val.cell.cdr = static_cast<cons*>(newaddr);

		}
	}
	else
		out << "cdr part is not live for " << ((cons*)cellptr - (cons*)buffer_live) << endl;

 return;
}
#endif


void clear_liveness_data()
{
	for(cons* cur = (cons*)buffer_live; cur < (cons*)freept; cur++)
	{
		cur->setofStates->clear();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For debugging
void print_accessible_heap(const string filename)
{
	ofstream out(filename);
	int index = 0;
	out <<"----------------------------------------------------------------------------"<<endl;
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			cons* conscell = static_cast<cons*>(vhit->ref);
			out << vhit->varname << " points to " << conscell << " and has type " << conscell->typecell << endl;
		}
	}
	out <<"----------------------------------------------------------------------------"<<endl;
}


struct heap_data
{
	int index;
	enum cell_type ct;
	cons* arg1;
	cons* arg2;
	union
	{
		int intval;
		bool boolval;
		std::string* stringval;
	}val;
};

string print_cell_type(cons* cell)
{
	if (!cell)
		return "";

	auto t = cell->typecell;

//	cout << "Printing cell " << cell << " with index " << (cell - getbufferlive()) << endl;
	switch(t)
	{
	case constIntExprClosure : return "Integer";
	case constBoolExprClosure: return "Boolean";
	case constStringExprClosure: return "String";
	case nilExprClosure: return "()";
	case consExprClosure: return "cons cell";
	case unaryprimopExprClosure : return "UnaryPrimOp(" + to_string(cell->val.closure.expr->type) + ")";
	case binaryprimopExprClosure: return "BinaryPrimOp(" + to_string(cell->val.closure.expr->type) + ")";
	case funcApplicationExprClosure:
	case funcArgClosure: return "FuncApp";
	default : cout << "Trying to print cell type " << t << endl ; return " error ";
	}
}

void add_elements_to_vector(vector<cons*> &v, stack<cons*> st)
{
	vector<cons*> temp;
	while(!st.empty())
	{
		temp.push_back(st.top());
		st.pop();
	}
	v.insert(v.end(), temp.begin(), temp.end());
	std::reverse(temp.begin(), temp.end());
	for(auto ele : temp)
		st.push(ele);

}

int num_root_vars = 0;
void print_heap_cell_list(vector<cons*> heap_cell_list, map<cons*, int> heap_map, ostream& out)
{

	int index = 0;
	//Add elements from the other stacks also
	out << "Number of root variables = " << heap_cell_list.size() << endl;
	out << "Number of references to be updated on stack = " << update_heap_refs.size() << endl;
	out << "Number of references on print stack = " << print_stack.size() << endl;
	//TODO: Check if this is needed. This might not be needed at all, put asserts.
	add_elements_to_vector(heap_cell_list, update_heap_refs);
	//These have already been added to the map
	//add_elements_to_vector(heap_cell_list, print_stack);

	out << "Starting GC " << endl;
	for(auto elem : heap_cell_list)
	{
		//assert(elem);
		if(!elem)
			continue;

		if (index < num_root_vars)
		{
			out << (elem - (cons*)buffer_live) << "(" << elem << ")" << "("<< root_var_map[index]<< ")\t" << print_cell_type(elem)<<"\t\t";
			++index;
		}
		else
		{
			out << (elem - (cons*)buffer_live) << "(" << elem << ")" << "\t" << print_cell_type(elem)<<"\t\t";
			index++;
		}

//		if (!elem)
//			continue;

		switch(elem->typecell)
		{
		case constIntExprClosure:out << "X\tX\t"<< elem->val.intVal<< endl; break;
		case constBoolExprClosure:out << "X\tX\t"<< elem->val.boolval<<endl; break;
		case constStringExprClosure:out << "X\tX\t"<< *(elem->val.stringVal)<<endl; break;
		case nilExprClosure: out << "X\tX\t()"<< endl; break;
		case unaryprimopExprClosure: out << (elem->val.closure.arg1 - (cons*)buffer_live)<<"\tX\t"<<endl;break;
		case binaryprimopExprClosure:out << (elem->val.closure.arg1 - (cons*)buffer_live)<<"\t"
				                         << (elem->val.closure.arg2-(cons*)buffer_live) << endl;break;
		case consExprClosure: out << (elem->val.cell.car-(cons*)buffer_live)<<"\t"
                				  << (elem->val.cell.cdr-(cons*)buffer_live) << endl;break;
		case funcApplicationExprClosure:
		case funcArgClosure:
		{
			if (elem->val.closure.arg1 == NULL)
			{
				FuncExprNode* f = (FuncExprNode*)elem->val.closure.expr;
				out << f->getFunction() << "\t";
			}
			else
				out << (elem->val.closure.arg1 - (cons*)buffer_live) << "\t";
			if (elem->val.closure.arg2 != NULL)
			{
				out << (elem->val.closure.arg2 - (cons*)buffer_live) << "\t";
			}
		}
		out  << endl;
		break;
		default : out << "Unknown type. Should not have come here " << elem << " with index(heap_list) " << (index - 1) << endl;
				break;
		}

	}

//	ofstream pre_gc_addr_list("pre_gc_addr.txt", ios::out);
//	for(auto ele: heap_cell_list)
//	{
//		pre_gc_addr_list << ele << " with type "<< print_cell_type(ele) << endl;
//	}
//	pre_gc_addr_list.close();

	//out << "Completed writing GC " << gccount << endl;
	out << "Completed writing GC " <<  endl;

}

void create_heap_bft(ostream& out)
{
	vector<cons*> heap_cell_list;

	heap_map.clear();
	root_var_map.clear();
	unsigned long base = (unsigned long)buffer_live;
	int index = 0;

	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			cons* conscell = static_cast<cons*>(vhit->ref);
			if (heap_map.find(conscell) == heap_map.end() && is_valid_address(conscell))
			{
				
				heap_map[conscell] = index;
				heap_cell_list.push_back(conscell);
				root_var_map[index] = vhit->varname;
				++index;
			}

		}
	}
	num_root_vars = index;
	//Add any cells in the print_stack also to this list
	vector<cons*> print_vector;
	add_elements_to_vector(print_vector, print_stack);
	for (auto ele : print_vector)
	{
		if (heap_map.find(ele) == heap_map.end())
		{
			assert(is_valid_address(ele));
			heap_map[ele] = index;
			ele->isLive = true;
			heap_cell_list.push_back(ele);
			++index;
		}
	}



	unsigned int curr_index = 0;
	size_t dummy = heap_cell_list.size();
	while (curr_index < heap_cell_list.size())
	{

		cons* curr_cell = heap_cell_list[curr_index];
		if (curr_cell)
		{

			switch(curr_cell->typecell)
			{
			case consExprClosure: {
				cons* car_part = curr_cell->val.cell.car;

				if (heap_map.find(car_part) == heap_map.end())
				{
					heap_map[car_part] = index;
					++index;
					heap_cell_list.push_back(car_part);
				}
				cons* cdr_part = curr_cell->val.cell.cdr;
				if (heap_map.find(cdr_part) == heap_map.end())
				{
					heap_map[cdr_part] = index;
					++index;
					heap_cell_list.push_back(cdr_part);
				}
			}
			break;
			case unaryprimopExprClosure: {
				cons* arg1 = curr_cell->val.closure.arg1;
				if (arg1 && (heap_map.find(arg1) == heap_map.end()))
				{
					heap_map[arg1] = index;
					++index;
					heap_cell_list.push_back(arg1);
				}
			}
			break;
			case binaryprimopExprClosure:
			case funcApplicationExprClosure:
			case funcArgClosure: {
				cons* arg1 = curr_cell->val.closure.arg1;
				if (arg1 && (heap_map.find(arg1) == heap_map.end()))
				{
					heap_map[arg1] = index;
					++index;
					heap_cell_list.push_back(arg1);
				}
				cons* arg2 = curr_cell->val.closure.arg2;
				if (arg2 && (heap_map.find(arg2) == heap_map.end()))
				{
					heap_map[arg2] = index;
					++index;
					heap_cell_list.push_back(arg2);
				}
			}
			break;
			default : break;

			}
		}
		++curr_index;
	}
	print_heap_cell_list(heap_cell_list, heap_map, out);
}

void print_buffer_data()
{
  cout << "Buffer Live "<< buffer_live << endl;
  cout << "Boundary Live "<< boundary_live << endl;
  cout << "Scan ptr " << scan << endl;
  cout << "Free ptr "<< freept << endl;
  cout << "Buffer Dead "<< buffer_dead << endl;
  cout << "Boundary Dead "<< boundary_dead << endl;
  cout << "Number of cells copied " << ((char*)freept - (char*)buffer_live)/sizeof(cons) << endl;
}
void print_cell_data()
{
	for(cons* cur = (cons*)buffer_live; cur < (cons*)freept; cur++)
	{
		cout << "Size of set " << cur->setofStates->size()<<endl;
	}
}

void dump_heap(string label)
{
	cons* i = NULL;
	string filename = "./" + label + "GC" + to_string(gccount);
	ofstream out(filename);
	for(i = (cons*)buffer_live; i < (cons*)boundary_live; ++i)
	{
		cons* conscell = i;
		cons* carval = (cons*)conscell->val.cell.car;
		cons* cdrval = (cons*)conscell->val.cell.cdr;
		cons* forward = (cons*)conscell->forward;
        // TBD: Remove Warning from next few lines
		if (is_valid_address(carval))
			carval = (cons*)(carval - (cons*)buffer_live);
		if (is_valid_address(cdrval))
			cdrval = (cons*)(cdrval - (cons*)buffer_live);
		if (forward != NULL)
			forward = (cons*)(forward - (cons*)buffer_live);

		if(carval==0)
			out << "(nil)";
		else
			out << carval;

		if(cdrval==0)
			out << "(nil)";
		else
			out<<cdrval;
		out <<", "<<forward<<")"<<endl;
	}
	out.close();
}

int is_valid_address(void* addr)
{

	bool is_valid = ((addr >= buffer_live) && (addr < boundary_live));
	if (!is_valid)
	{
		;//cout << "Invalid address " << addr  << " at index " << ((cons*)addr - (cons*)buffer_live) << endl;
		//assert(is_valid);

	}
	return is_valid;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////garbage collection function for python module garbagecollector
int isSame (void* loc1, void *loc2)
{
  if (loc1 == loc2)  return 1;
  else    return 0;
}


int isCar(void *loc)
{
    int type=getType(loc,1);
    return type;
}

int isCdr(void *loc)
{
    int type=getType(loc,2);
    return type;
}

int lt_scan_free()
{
  if ((char*)scan <= (char*)boundary_live)  return 1;
    else    return 0;
}


unsigned long diff_scan_free()
{
  return ((char*)freept - (char*)scan)/sizeof(cons);
}

void set_car(void* loc,  void* ref)
{
 	cons *conscell=(cons*)loc;
	conscell->val.cell.car=static_cast<cons*>(ref);
}


void set_cdr(void* loc,  void* ref)
{
 	cons *conscell=(cons*)loc;
	conscell->val.cell.cdr=static_cast<cons*>(ref);
}
int is_null_stack(actRec )
{
    if (actRecStack.empty())  return 1;
    else 	return 0;
}

int is_null_binding(var_heap *bind)
{
  if (bind==NULL)  return 1;
  else 	return 0;
}


void* int_to_void(int val)
{
  void* newval= (void*)((long)val); // Added a double cast to remove warnings of casting int to pointer and vice-versa.
  return newval;
}

void* char_to_void(std::string* str)
{
  void* newval=str;
  return newval;
}

int void_to_int(void *val)
{
  int newval=(int)((long)val); // Added a double cast to remove warnings of casting int to pointer and vice-versa.
  return newval;
}

string* void_to_char(void *val)
{
  string* newval= (string*)val;
  return newval;
}

cons* void_to_cons(void* val)
{
	cons* cell = (cons*)val;
	return cell;
}
long void_to_long(void* val)
{
  long ret = (long)val;
    return ret;
}
void* long_to_void(long val)
{
  void* newval = (void*) val;
  return newval;
}

void update_scan()
{
  scan=(void*)((char*)scan+sizeof(cons));
}

void* getscan()
{
  return scan;
}

void* getfree()
{
  return freept;
}

cons* getbufferlive()
{
	return static_cast<cons*>(buffer_live);
}

int lt_scan_freept()
{
  if ((char*)scan < (char*)freept)  return 1;
  else    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions for handling liveness based GC

void read_state_map_from_file(string filepath)
{

  ifstream state_map_file;
  string line;

  state_map_file.open(filepath.c_str());
  while(getline(state_map_file, line))
    {
      string key;
      state_index value;
      state_index start = 0, end = 0;
      end = line.find(':', start);
      key = line.substr(start, end);
      start = end + 1;
      end = 0;
      end = line.find(';', start);
      value = stol(line.substr(start, end));
      statemap[key] = value;
    }
  state_map_file.close();
  return;
}

void read_state_transition_table_from_file(string filepath)
{

  ifstream state_transition_file;
  string line;

  state_transition_file.open(filepath.c_str());
  while(getline(state_transition_file, line))
    {
      state_index index1, index2, value;
      state_index start = 0, end = 0;
      end = line.find(':', start);
      index1 = stol(line.substr(start, end));
      start = end + 1;
      end = 0;
      end = line.find(':', start);
      index2 = stol(line.substr(start, end));
      start = end + 1;
      end = 0;
      end = line.find(";", start);
      value = stol(line.substr(start, end));
      state_transition_table[index1][index2] = value;
      //cout << "Translated to "<<index1<<" "<<index2<<" "<<value<<endl;
    }
  state_transition_file.close();
  return;
}

void initialize (string program_name, state_index numkeys, GCStatus gc_type)
{
	gc_status = gc_type;
	if (gc_type == gc_live)
	{
		state_transition_table = new state_index*[numkeys];
		for (state_index i = 0; i < numkeys; ++i)
		{
			state_transition_table[i] = new state_index[2];
			state_transition_table[i][0]=0;
			state_transition_table[i][1]=0;
		}

		string filepath = outdir + program_name + "/fsmdump-" + program_name;
		//cout << "Reading " << (filepath + "-state-map") << endl;
		read_state_map_from_file(filepath + "-state-map");
		//cout << "Reading " << (filepath + "-state-transition-table") << endl;
		read_state_transition_table_from_file(filepath + "-state-transition-table");
	}
	//cout << "Done initializing" << endl;
#ifdef DUMP_HEAP_AS_GRAPHVIZ
	if (gc_type != gc_live)
	{
		state_transition_table = new state_index*[numkeys];
		for (state_index i = 0; i < numkeys; ++i)
		{
			state_transition_table[i] = new state_index[2];
			state_transition_table[i][0]=0;
			state_transition_table[i][1]=0;
		}

		string filepath = outdir + program_name + "/fsmdump-" + program_name;
		//cout << "Reading " << (filepath + "-state-map") << endl;
		read_state_map_from_file(filepath + "-state-map");
		//cout << "Reading " << (filepath + "-state-transition-table") << endl;
		read_state_transition_table_from_file(filepath + "-state-transition-table");
		cout << "Done initializing" << endl;
	}
#endif


	return;
}

void cleanup(state_index numkeys)
{
	if (gc_status == gc_live)
	{
		for (int i = 0; i < numkeys; ++i)
		{
			delete [] state_transition_table[i];
		}
		delete [] state_transition_table;
	}
   //free(buffer_live);
}

state_index lookup_dfanode(string nodename)
{
  return statemap[nodename];
}

state_index get_target_dfastate(state_index i1, state_index i2)
{
  return state_transition_table[i1][i2];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef GC_ENABLE_STATS
/* Garbage collection statistics */
/* author : Amey Karkare */
clock_tick gc_clock()
{
    return current_cons_tick;
}

void tick()
{
    ++current_cons_tick;
}

static const char* gc_stats_filename = "./output/GC_STATS.txt";
static FILE* gc_stats_outfile;
void init_gc_stats()
{
    DBG(printf("init_gc_stats ==>\n"));

    gc_stats_outfile = fopen(gc_stats_filename, "w");
    fprintf(gc_stats_outfile, "T_gc\t| Address\t| Tnew\t| T_Fu\t| T_Lu \n");

    DBG(printf("<== init_gc_stats\n"));
}

void clear_rch_flag()
{
    DBG(printf("clear_rch_flag ==>\n"));
    unsigned long i;
    /* clear reachable flag for all cells before GC starts */
    for (i = 0; i < last_pos; i++)
        array_stats[i].is_reachable = 0;
    DBG(printf("<== clear_rch_flag\n"));
}

#define TIME_ms(tm) (tm) // time in cons ticks.
static void dump_cell_stats(cons* gs)
{
    if (gs->is_used)
    {
        fprintf(gc_stats_outfile,
                /*GC  | Cell  |Create| First | Last | Last*/
                /*Time| Addrs |Time  | Use   | Use  | GC  */
                "%lu\t| %p  \t| %lu\t| %lu\t | %lu\t| %ld \n",
                TIME_ms(gc_clock()), gs,
                TIME_ms(gs->created), TIME_ms(gs->first_use),
                TIME_ms(gs->last_use), TIME_ms(gs->last_gc) );
    }
    else
    { /* first and last use are -1 */
        fprintf(gc_stats_outfile,
                /*GC  | Cell  |Create| First | Last | Last*/
                /*Time| Addrs |Time  | Use   | Use  | GC  */
                "%lu\t| %p  \t| %lu\t| -1\t  | -1\t | %ld \n",
                TIME_ms(gc_clock()), gs, TIME_ms(gs->created),
				TIME_ms(gs->last_gc) );
    }
}

void dump_garbage_stats()
{
    static int num_gcs;

    DBG(printf("dump_garbage_stats ==>\n"));
    fprintf(gc_stats_outfile, "//--- GC #%d | %lu \n",
            ++num_gcs, TIME_ms(gc_clock()));
    assert(num_gcs == gccount);
    unsigned long i = 0;
    for (i = 0; i < last_pos; ++i)
    {
        if (!array_stats[i].is_reachable)
        {
            /* dump the statistics for unreachable cells */
            dump_cell_stats(&array_stats[i]);
        }
    }
    DBG(printf("<== dump_garbage_stats\n"));
}

void finish_gc_stats()
{
    DBG(printf("finish_gc_stats ==>\n"));
    unsigned long i;
    array_stats = (cons*)buffer_live;
    last_pos = ((cons*)freept - (cons*)buffer_live);

    /* dump the statistics for all cells */
    fprintf(gc_stats_outfile, "//--- Finish | %lu\n", TIME_ms(gc_clock()));
    for (i = 0; i < last_pos; i++)
        dump_cell_stats(&array_stats[i]);

    fclose(gc_stats_outfile);
    DBG(printf("<== finish_gc_stats\n"));
}

void update_last_use(cons *cell)
{
    DBG(printf("updating last use(%p) ==>\n", cell));
    if (cell == NULL)
	//cout << "No need to count for a NULL list" << endl;
	return;
    assert(cell); // only valid cells allowed

    if (!cell->is_used)
    {
        cell->is_used = 1;
        cell->first_use = cell->last_use = gc_clock();
    }
    else
    {
        cell->last_use = gc_clock();
    }

    DBG(printf("<== updating last use(%p)\n", cell));
}


void update_last_gc(cons *cell)
{
    DBG(printf("updating last gc(%p) ==>\n", cell));
    if (cell == NULL)
	//cout << "No need to count for a NULL list" << endl;
	return;
    assert(cell); // only valid cells allowed


   	cell->last_gc = gc_clock();



    DBG(printf("<== updating last gc(%p)\n", cell));
}


#endif

///////////////////////////////////////////////////k-Liveness GC //////////////////////////////////////////////////////////////////////
#ifdef K_LIVENESS
int k_depth = 10; //TODO This should be taken as a parameter and initialized globally
void liveness_gc()
{
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  ((cons*)i)->visited = 0;
#endif
	numcopied = 0;
	//cout << "Heap before GC"<<endl;
	//dump_heap("before");
	//cout << "Executing k liveness GC " << endl;
	swap_buffer();

	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			cout << "Doing gc at return point " << stackit->return_point << endl;
			string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;

			stateMapIter got = statemap.find(nodeName);

			if (got != statemap.end() || k_depth == 0) //Don't check for any liveness
			{

				if (vhit->ref && ((cons*)vhit->ref)->forward == NULL) //Check to see if a new cell was allocated
				{
					((cons*)vhit->ref)->setofStates->clear();
					((cons*)vhit->ref)->depth = 0;
				}

				void* addr = copy(vhit->ref);
				if (addr)
				{
					cons* c = (cons*) addr;
					//gcout << "car part " << c->car << " & cdr part " << c->cdr << " of "<< addr << " with name "<< nodeName<< endl;
					if (k_depth != 0) //root variable might not be live
						c->setofStates->insert(got->second);
					c->depth = 1;
				}
				vhit->ref = addr;
			}
//			else
//				cout << "Node " << nodeName << " not found " << endl;
		}
	}
	//cout << "Completed processing root set" << endl;
	//chasing reachable cells from roots
	while(lt_scan_freept()==1)//
	{
		//gcout << "Processing location " << ((cons*)scan - (cons*)buffer_live)<<endl;
		copy_children(scan, ((cons*)scan)->setofStates);
		update_scan();
	}
	//cout << "Heap after GC"<<endl;
	//dump_heap("after");
	//++gccount;
	end_GC_dump();
	cout << "Number of cells copied " << numcopied << endl;
#ifdef ENABLE_SHARING_STATS
  print_sharing_stats();
#endif
	return;
}

void copy_children(void* cellptr, stateset* st)
{
	stateset s0;
	stateset s1;
#ifdef ENABLE_SHARING_STATS
	if (cellptr)
		++(((cons*)(cellptr))->visited);
#endif
	int curr_depth = ((cons*)cellptr)->depth;
	if (curr_depth < k_depth)
	{
		//cout << "Executing liveness GC " << curr_depth << " " << k_depth <<endl;
		for(setiter i = st->begin(); i != st->end(); ++i)
		{
			state_index t = state_transition_table[*i][0]; // get_target_dfastate(*i, 0);
			if (t > 0)
				s0.insert(t);
			t = state_transition_table[*i][1]; //get_target_dfastate(*i, 1);
			if (t > 0)
				s1.insert(t);
		}

		//process car field
		if(!s0.empty())
		{
			if (getType(cellptr, 1) == 1)//Do this only if car is address type
			{
				void* fpt = freept;
				void* newaddr = copy(((cons*)cellptr)->car);

				if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
				{
					((cons*)newaddr)->setofStates->clear();
					((cons*)newaddr)->setofStates->insert(s0.begin(), s0.end());
					((cons*)newaddr)->depth = curr_depth + 1;
				}
				else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				{
					((cons*)newaddr)->setofStates->insert(s0.begin(), s0.end());
					int temp_depth = ((cons*)newaddr)->depth < (curr_depth + 1) ? (curr_depth + 1) : ((cons*)newaddr)->depth;
					((cons*)newaddr)->depth = temp_depth;
				}
				else if (newaddr) //forwarded pointer which has already been processed in the live buffer
				{
					stateset s_new;
					stateset *s_child = ((cons*)newaddr)->setofStates;
					for(setiter i = s0.begin(); i != s0.end(); ++i)
						if (s_child->find(*i) == s_child->end())
							s_new.insert(*i);
					int temp_depth = ((cons*)newaddr)->depth < (curr_depth + 1) ? (curr_depth + 1) : ((cons*)newaddr)->depth;
					((cons*)newaddr)->depth = temp_depth;
					if (!s_new.empty())
					{
						copy_children(newaddr, &s_new);
						((cons*)newaddr)->setofStates->insert(s_new.begin(), s_new.end());
					}
				}
				((cons*)cellptr)->car = newaddr;
			}
		}

		//process cdr field
		if(!s1.empty())
		{
			if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
			{
				void* fpt = freept;
				void* newaddr = copy(((cons*)cellptr)->cdr);
				if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
				{
					((cons*)newaddr)->setofStates->clear();
					((cons*)newaddr)->setofStates->insert(s1.begin(), s1.end());
					((cons*)newaddr)->depth = curr_depth + 1;
				}
				else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				{
					((cons*)newaddr)->setofStates->insert(s1.begin(), s1.end());
					int temp_depth = ((cons*)newaddr)->depth < (curr_depth + 1) ? (curr_depth + 1) : ((cons*)newaddr)->depth;
					((cons*)newaddr)->depth = temp_depth;
				}
				else if(newaddr)//forwarded pointer which has already been processed in the live buffer
				{
					stateset s_new;
					stateset *s_child = ((cons*)newaddr)->setofStates;
					for(setiter i = s1.begin(); i != s1.end(); ++i)
						if (s_child->find(*i) == s_child->end())
							s_new.insert(*i);
					int temp_depth = ((cons*)newaddr)->depth < (curr_depth + 1) ? (curr_depth + 1) : ((cons*)newaddr)->depth;
					((cons*)newaddr)->depth = temp_depth;
					if (!s_new.empty())
					{
						copy_children(newaddr, &s_new);
						((cons*)newaddr)->setofStates->insert(s_new.begin(), s_new.end());
					}
				}
				((cons*)cellptr)->cdr = newaddr;

			}
		}
	}
	else //Start reachability based GC from this node
	{
		//cout << "Entering reachability GC " << endl;
		if (getType(cellptr, 1) == 1)//Do this only if car is address type
		{
			void* fpt = freept;
			void* newaddr = copy(((cons*)cellptr)->car);
			if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
				((cons*)newaddr)->depth = curr_depth; //curr_depth is k+1 so we don't need to increase it anymore
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				((cons*)newaddr)->depth = curr_depth;
			else if (newaddr && ((cons*)newaddr)->depth < k_depth) //forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				((cons*)newaddr)->depth = curr_depth;
				copy_children(newaddr, &s_new);
			}
			((cons*)cellptr)->car = newaddr;
		}
		if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
		{
			void* fpt = freept;
			void* newaddr = copy(((cons*)cellptr)->cdr);
			if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
				((cons*)newaddr)->depth = curr_depth;
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				((cons*)newaddr)->depth = curr_depth;
			else if(newaddr && ((cons*)newaddr)->depth < k_depth)//forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				((cons*)newaddr)->depth = curr_depth;
				copy_children(newaddr, &s_new);
			}
			((cons*)cellptr)->cdr = newaddr;
		}
	}
 return;
}
#endif




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

