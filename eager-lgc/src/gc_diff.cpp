#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
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
#include "gc.h"
using namespace std;

int numcopied = 0; //TODO:Delete after debugging
int numLiveness = 7; //TODO:Delete after debugging


extern GCStatus gc_status;

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
int gccount = 1;



//Use only push_front and pop_front to insert and remove elements from actRecStack
//Use iterator to iterate over the elements of actRecStack during garbage collection
deque<actRec> actRecStack = deque<actRec>(0);
deque<actRec>::iterator actRecIter;
vector<var_heap>::iterator heapRefIter;
vector<var_stack>::iterator nonRefIter;
stateMap statemap;
state_index **state_transition_table; //Dynamically allocating a 2-D array might be error prone. See if vectors can be used instead.
//call_next_data call_next; //Should we use a map?
std::unordered_map<std::string, std::string> call_next;

//ofstream gcout;


void reachable_dfs();
void clear_liveness_data();
void print_heap();
void print_buffer_data();
void print_cell_data();
int is_valid_address(void*);
int lt_scan_free();
int lt_scan_freept();
void* getscan();
void update_scan();
//long diff_scan_free();
state_index lookup_dfanode(string nodename);
void* followpaths(void* loc, state_index index);
state_index get_target_dfastate(state_index i1, state_index i2);
void set_car(void* loc,  void* ref);
void set_cdr(void* loc,  void* ref);
void copy_children(void* cellptr, stateset* st);

#ifdef GC_ENABLE_STATS
/* declarations of GC statistics related functions */
static clock_tick gc_clock();
static void tick();
void init_gc_stats();
static void clear_rch_flag();
void dump_garbage_stats();
void finish_gc_stats();
static void update_last_use(cons *cell);
/* variables to traverse the cons cells for the statistics */
static cons* array_stats;
static unsigned long last_pos;
static clock_tick current_cons_tick;

#else /* stubs */

static clock_tick gc_clock() {return 0;}
void init_gc_stats() {}
static void clear_rch_flag() {}
void dump_garbage_stats() {}
void finish_gc_stats() {}
static void update_last_use(cons *cell) {}
#endif




int mmc=0;
long gmaxheapreachability = 0;
#ifdef TEST_RUN

void set_max_reachability();
#endif


/*-----Memory manager-------*/

//!!!caution remove memorysize from this function
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
    // cout<<freept << " " << boundary_live<<endl;
    boundary_dead=(void*)((char*)(buffer_live)+memorysize);
    mmc=mmc+memorysize;
    newheapcells=0;oldheapcells=0;
    copycells=0;garbagecells=0;heapslot=0;

#ifdef GC_ENABLE_STATS
    FILE *f=fopen("./output/garbage-dump.txt","w");
    fclose(f);
    array_stats = static_cast<cons*>(buffer_live);
#endif
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
#ifdef GC_ENABLE_STATS
    /* point to the old buffer to collect statistics */
    array_stats = static_cast<cons*>(buffer_live);
    last_pos = (static_cast<cons*>(freept) - static_cast<cons*>(buffer_live));
    clear_rch_flag();
#endif
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


void* return_null()
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
    return conscell;
}

void* allocate_cons(const char cartype, void* addrcar, const char cdrtype, void* addrcdr)
{
	// cout<<"allocatd"<<endl;
#ifdef GC_ENABLE_STATS
    /* cons clock tick */
    tick();
#endif

    if (check_space(sizeof(cons))==0)
    {
      fprintf(stderr,"No Sufficient Memory - cons\n");
      throw bad_alloc();
      return NULL;
    }
    // cout<<"freept" << freept<<endl;
    cons *conscell=(cons *)freept;
    update_free(sizeof(cons));

    newheapcells=newheapcells+1;

    conscell->typecar=cartype;
    conscell->typecdr=cdrtype;
    conscell->forward=NULL;
    conscell->car=addrcar;
    conscell->cdr=addrcdr;
    conscell->setofStates = new stateset();
    // cout<<conscell<<endl;

 #ifdef GC_ENABLE_STATS
    conscell->created = gc_clock();
    conscell->is_reachable = 0;
    conscell->is_used = 0;
#endif
#ifdef TEST_RUN
    set_max_reachability();
#endif
    // cout<<conscell<<endl;
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
    newenv.funcLiveness = Scheme::Demands::ALL;
    actRecStack.push_front(newenv);

    mmc=mmc+(sizeof(actRec));
}

void make_environment(const char *functionname, std::string returnpoint)
{
	actRec newenv;
    actRecStack.front().return_point = returnpoint;
    newenv.return_point=-1;
    newenv.funcname=functionname;
    newenv.funcLiveness = progLiveness2["L/" + returnpoint][""][actRecStack.front().funcLiveness];
   	// if (newenv.funcLiveness != Scheme::Demands::ALL && newenv.funcLiveness != Scheme::Demands::EPSILON){
   	// 	cout<<"#######" << " " <<newenv.funcLiveness<<endl;
   	// }
    //cout<<"before pushing actRecStack.front="<<&actRecStack.front()<<endl;
    actRecStack.push_front(newenv);
    //cout<<"after pushing actRecStack.front="<<&actRecStack.front()<<endl;
    //cout << "Added function " << functionname << " to stack" <<endl;
    //cout << "Current stack size is " << actRecStack.size() << endl;
    mmc=mmc+(sizeof(actRec));
}

void delete_environment()
{
	//cout<<"before popping actRecStack.front="<<&actRecStack.front()<<endl;
	actRecStack.pop_front();
	//cout<<"after popping actRecStack.front="<<&actRecStack.front()<<endl;
    mmc=mmc-(sizeof(actRec));
    actRecStack.front().return_point=-1; //reset
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
	//cout << "Creating reference value for " << var << " with type " << type << endl;
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
    //cout << "looking for variable " << var << endl;

	//assert(&actRecStack != 0);
	//printf("actRecStack pointer=");

	//cout << &actRecStack<<endl;
	//cout << "Size of activation stack " << actRecStack.size() << endl;

	vector<var_heap> &vh = actRecStack.front().heapRefs;
	//cout << "Function on top of heap is " << actRecStack.front().funcname << endl;
	vector<var_heap>::iterator it = vh.begin();
	while(it != vh.end())
	{
		if(strcmp(it->varname.c_str(),var)==0)
			return 'a';

		++it;
	}

	vector<var_stack> &vs=actRecStack.front().nonRefs;
	vector<var_stack>::iterator its = vs.begin();
	while(its != vs.end())
	{
		if (strcmp(its->varname.c_str(),var)==0)
			return its->type;

		++its;
	}

	// cout << "Element "<< var <<" not found." << endl;
	return 'n'; /*Element not found.*/
}


void* lookup_addr(const char *var)
{
 try
 {
   vector<var_heap> &vh=actRecStack.front().heapRefs;
  vector<var_heap>::iterator it = vh.begin();
  //cout <<"Looking for address " << var << endl;
  while(it != vh.end())
    {
      if (strcmp(it->varname.c_str(),var)==0)
	{
    	  //cout << "Returning addr " << it->ref << endl;
    	  return it->ref;
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
  //cout <<"Looking for value " << var << endl;
  while(it != vs.end())
    {
      if (strcmp(it->varname.c_str(),var)==0)
	{
      //cout << "Returning value " << it->value << endl;
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

void printval(void *ref)
{

	if (ref == NULL)
	{
		cout << "()" << endl;
		return;
	}
	if (!is_valid_address(ref)) // Check added for liveness based GC as this might not be a valid pointer to chase.
	{                       // In case of reachability all addresses will be valid.
		cout << "Invalid address "<< ref << endl;
		exit(-1);
	}

	cons *cref;
	cref=(cons *)ref;
#ifdef GC_ENABLE_STATS
	update_last_use(cref);
#endif

	printf("(");
	if(cref->typecar=='i' || cref->typecar=='b')
		printf("%d", (int)(intptr_t)cref->car);
	else if(cref->typecar=='s')
		cout << ((string*)(cref->car))->c_str();
	else
		printval(cref->car);
	printf(".");

	if(cref->typecdr=='i' || cref->typecdr=='b')
		printf("%d", (int)(intptr_t)cref->cdr);
	else if(cref->typecdr=='s')
		cout << ((string*)(cref->cdr))->c_str();
	else
		printval(cref->cdr);
	printf(")");
}

int current_heap()
{
   return ((char*)boundary_live - (char*)freept)/sizeof(cons);
}

////garbage collector support functions for python module
int sizeof_cons()
{
    return sizeof(cons);
}

void* getCar(void* ref, const char fromGC)
{
   if (ref == NULL)
	   cout << "Error dereferencing a NULL pointer for car" << endl;
	cons *conscell=static_cast<cons*>(ref);
#ifdef GC_ENABLE_STATS
    if (!fromGC)
        update_last_use(conscell);
#endif

    void* car=conscell->car;
    return car;
}



void* getCdr(void* ref, const char fromGC)
{
	if (ref == NULL)
		   cout << "Error dereferencing a NULL pointer for cdr" << endl;
    cons *conscell=static_cast<cons*>(ref);
#ifdef GC_ENABLE_STATS
    if (!fromGC)
        update_last_use(conscell);
#endif

    void* cdr=conscell->cdr;
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

//getType function: -1 for NULL, 1 for cons, 2 for int, 3 for string, 4 for bool
int getType(void* node, int field)
{
    if (node==NULL)  return -1;
    cons * conscell = static_cast<cons*>(node);

    if (field==1)
    {
        if(conscell->typecar=='i' ) return 2;
        else if(conscell->typecar=='s' ) return 3;
        else if (conscell->typecar=='b' ) return 4;
        else if(conscell->car==NULL) return -1;
        else return 1;
    }
    else if (field==2)
    {
        if(conscell->typecdr=='i' ) return 2;
        if(conscell->typecdr=='s' ) return 3;
        else if (conscell->typecdr=='b' ) return 4;
        else if(conscell->cdr==NULL) return -1;
        else return 1;
    }
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
//garbage collector functions
void* copy(void* node)
{
	void *addr;
	if(node==NULL) return NULL;
	if(!(node>=buffer_dead && node < boundary_dead)) return node; //TODO : This seems to be a hack fix it correctly
	else
	{
		heapslot=heapslot+1;
		cons *conscell=static_cast<cons*>(node);
#ifdef GC_ENABLE_STATS
		conscell->is_reachable = 1;
#endif
		if((conscell->forward >= buffer_live) &&
				(boundary_live > conscell->forward))
			return (conscell->forward);

		addr = dup_cons(conscell);
		conscell->forward=addr;
		copycells=copycells+1;
		++numcopied;
		 // if (!(conscell >= buffer_dead && conscell < boundary_dead))
		 // {
		 // 	//gcout << "buffer_dead = " << buffer_dead<<endl;
		 // 	//gcout << "boundary_dead = " << boundary_dead<<endl;
		 // 	//gcout << "buffer_live = " << buffer_live << endl;
		 // 	//gcout << "boundary_live = " << boundary_live<<endl;
		 // 	//gcout << "Invalid address for conscell " << conscell<<endl;
		 // }
		 //gcout << "Copied cell #"<<(static_cast<cons*>(conscell)-static_cast<cons*>(buffer_dead)<<endl;
		return addr;
	}

	return return_null();
}

int copy_scan_children(void* node)
{
	void *addr=NULL;
	cons *conscell=static_cast<cons*>(node);
	if (conscell->typecar=='a')
	{
		addr=copy(conscell->car);
		conscell->car=addr;
	}
	if (conscell->typecdr=='a')
	{
		addr=copy(conscell->cdr);
		conscell->cdr=addr;
	}

	return (sizeof(cons));
}

#ifdef REACHABILITY_DFS
void* followpaths_reachability(void* loc)
{
  void* loccopy = copy(loc);

  if (getType(loccopy,1) == 1)
  {
	  void* newloc = getCar(loc, 1);
	  void* addr = followpaths_reachability(newloc);
	  set_car(loccopy, addr);
  }


  if (getType(loccopy,2) == 1)
  {
	  void* newloc = getCdr(loc, 1);
	  void* addr = followpaths_reachability(newloc);
	  set_cdr(loccopy, addr);
  }

  return loccopy;
}


void reachability_gc()
{
	swap_buffer();
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref)
			{
				void *addr   = followpaths_reachability(vhit->ref);
				vhit->ref = addr;
			}
		}
	}
	return;
}
#endif

#ifdef TEST_RUN
long find_reachability(void* c)
{
	cons* cell = static_cast<cons*>(c);
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
		(static_cast<cons*>(i))->visited = 0;
	}
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref && (static_cast<cons*>(vhit->ref))->visited == 0)
			{
				(static_cast<cons*>(vhit->ref))->visited = 1;
				++maxheapreachability;
				if ((static_cast<cons*>(vhit->ref))->typecar == 'a')
					maxheapreachability += find_reachability((static_cast<cons*>(vhit->ref))->car);
				if ((static_cast<cons*>(vhit->ref))->typecdr == 'a')
					maxheapreachability += find_reachability((static_cast<cons*>(vhit->ref))->cdr);

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

	swap_buffer();
	numcopied = 0;
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			void *reference=vhit->ref;
			void *addr=copy(reference);
			vhit->ref = addr;
		}
	}

	//chasing reachable cells from roots
	while(lt_scan_freept()==1)
	{
		copy_scan_children(getscan());
		update_scan();
	}

	cout << "Num of cells copied during garbage collection " << numcopied << endl;
#ifdef TEST_RUN
	cout << "Max heap reachability till now "<< gmaxheapreachability << endl;
#endif
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
		numvisits.push_back(make_pair(j, (static_cast<cons*>(i))->visited));

    sort(numvisits.begin(), numvisits.end(), comp);

    for (vector<pair<int,int> >::iterator it = numvisits.begin(); it != numvisits.end(); ++it)
		cout << "Cell " << it->first << " is visited " << it->second << " times" << endl;
}
#endif

#ifdef LIVENESS_DFS
void liveness_gc()
{
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  (static_cast<cons*>(i))->visited = 0;
#endif
  dump_heap("before");
  swap_buffer();

  for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
    {
	  for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
      {
    	  string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;
    	  stateMapIter got = statemap.find(nodeName);

    	  if (got != statemap.end())
    	  {

//    		  if ((static_cast<cons*>(vhit->ref))->forward == NULL)//If pointer already not copied then clear its contents
//    			  c->s->clear();

    		  void *addr   = followpaths(vhit->ref, got->second);
    		  cons* c = static_cast<cons*>(addr);
    		  //c->s->insert(got->second);
    		  vhit->ref = addr;


#ifdef ENABLE_SHARING_STATS
    		  if (vhit->ref)
    			  ++((static_cast<cons*>(vhit->ref))->visited);
#endif
    	  }
      }
    }
  dump_heap("after");
  ++gccount;
#ifdef ENABLE_SHARING_STATS
  print_sharing_stats();
#endif
  return;
}

void* followpaths(void* loc, state_index index)
{
  void* loccopy = copy(loc);
  state_index a0 = state_transition_table[index][0]; //get_target_dfastate(index, 0);


#ifdef ENABLE_SHARING_STATS
  if (loccopy)
	  ++((static_cast<cons*>(loccopy))->visited);
#endif
  if (a0 > 0)
  {
	  // If using type information we can ensure that the type is always address type, this check can be eliminated
	  // set the liveness for operators operating on unboxed values to NULL so that it will never come here.
	  if (getType(loccopy,1) == 1)
	  {
		  void* newloc = getCar(loc, 1);
		  void* addr = followpaths(newloc, a0);
		  set_car(loccopy, addr);
	  }
  }

  state_index a1 = state_transition_table[index][1];//get_target_dfastate(index, 1);
  if (a1 > 0)
    {
  	  // If using type information we can ensure that the type is always address type, this check can be eliminated
  	  // set the liveness for operators operating on unboxed values to NULL so that it will never come here.
  	  if (getType(loccopy,2) == 1)
  	  {
  		  void* newloc = getCdr(loc, 1);
  		  void* addr = followpaths(newloc, a1);
  		  set_cdr(loccopy, addr);
  	  }
    }

  return loccopy;
}
#endif

int copy_given_scan_children(void* node, int docar, int docdr)
{
	void *addr=NULL;
	cons *conscell= static_cast<cons*>(copy(node));
	if (conscell->typecar=='a' and docar)
	{
		addr=copy(conscell->car);
		conscell->car=addr;
	}
	if (conscell->typecdr=='a' and docdr)
	{
		addr=copy(conscell->cdr);
		conscell->cdr=addr;
	}
	return (sizeof(cons));
}

typedef void* (*gcFunc)(void* );

void print_cons(cons* a)
{
	cout<<a<<'\t'<<a->typecell<<'\t'<<a->typecar<<'\t'<<a->typecdr<<'\t'<<a->car<<'\t'<<a->cdr<<'\t'<<a->forward<<endl;
}
void print_heap(){
	cons* myscan;
	map<void*, set<string> > varmap;
	
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		string nodeLabel = "L/" + stackit->return_point;
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			varmap[vhit->ref].insert(nodeLabel+" "+vhit->varname);
		}
	}

	cout<<"Varname\t\t\tRef\ttcell\ttcar\ttcdr\tcar\tcdr\tforward\n";
	for (myscan = static_cast<cons*>(buffer_live); myscan != freept; myscan++){
		if (varmap.count(myscan))
		{
			for (auto i: varmap[myscan])
				cout<<i<<',';
		}
		else
		{
			cout<<"no var";
		}
		cout<<"\t\t\t";
		print_cons(myscan);
	}

	cout<<"\n\n\n";
}

#ifdef LIVENESS_BFS
void liveness_gc()
{
	// print_heap();
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  (static_cast<cons*>(i))->visited = 0;
#endif
	numcopied = 0;
	// cout << "Heap before GC"<<endl;
	//dump_heap("before");
	//start_GC_dump();
	swap_buffer();

	vector<vector<vector<var_heap>::iterator> > heapRefsBuckets;
	heapRefsBuckets.resize(numLiveness);
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		string nodeLabel = "L/" + stackit->return_point;
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			const auto& curstate = progLiveness2[nodeLabel][vhit->varname][stackit->funcLiveness];
					// cout<<"putting in heap bucket "<<vhit->varname << " " << vhit->ref<<endl;
					//todo : assume for now that got->second returns the liv_index
			heapRefsBuckets[curstate.latticeNo].push_back(vhit);
			// cout<<"Node label: "<<nodeLabel<<' ';
			// cout<<"Var name: "<<vhit->varname<<' '<<stackit->funcLiveness<<endl;

			// auto curstate = progLiveness2["L/" + stackit->return_point][vhit->varname][Scheme::Demands::ALL];
			// cout<<"curstate"<<curstate<<endl;
			// cout<<"I am here with "<<progLiveness2["L/" + stackit->return_point][vhit->varname][ALL]<<endl;	
			//			else
			//			cout << "Node " << nodeName << " not found " << endl;
		}
	}
	for (int ind = numLiveness-1; ind >= 0; ind--) 
	{
			// cout<<"reaching here"<<endl;
			// cout<<"liveness : " << elem.first << " " << ind <<endl;
			// if (vhit->ref && (static_cast<cons*>(vhit->ref))->forward == NULL) //Check to see if a new cell was allocated
			// 	(static_cast<cons*>(vhit->ref))->setofStates->clear();
			// cout<<"calling with " << vhit->ref << " and " << ind<<endl;
			// vhit->ref = static_cast<cons*>(followpathsBFS(vhit->ref, ind));
				// cout<<"NULL Before\n";
				void* addr;
				switch(ind)
				{
					case 1:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							addr = copy(vhit->ref);
							while(lt_scan_freept()==1)
								update_scan();
							vhit->ref = addr;
						}
					}
						break;
					case 2:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							addr = copy(vhit->ref);
							copy_given_scan_children(addr, 1 , 0);
							while(lt_scan_freept()==1)
								update_scan();
							vhit->ref = addr;
						}
					}
						break;

					case 3:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							addr = copy(vhit->ref);
							copy_given_scan_children(addr, 0, 1);
							if(lt_scan_freept()==1)
								update_scan();
							if(lt_scan_freept()==1)
								update_scan();
							assert(lt_scan_freept() != 1);
							vhit->ref = addr;
						}
					}
						break;
					

					case 4:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							cons* cellptrtemp = static_cast<cons*>(vhit->ref);
							if (cellptrtemp->typecar == 'a')
							{
								addr = copy(cellptrtemp->car);
								while(lt_scan_freept()==1)
								{
									// cout<<"doing loop"<<endl;
									copy_given_scan_children(getscan(), 1, 1);
									update_scan();
								}
								cellptrtemp->car = addr;
							}

							heapRefsBuckets[1].push_back(vhit);
						}
					}
						break;

					case 5:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							cons* cellptrtemp = static_cast<cons*>(vhit->ref);
							if (cellptrtemp->typecdr == 'a')
							{
								addr = copy(cellptrtemp->cdr);
								while(lt_scan_freept()==1)
								{
									// cout<<"doing loop"<<endl;
									copy_given_scan_children(getscan(), 1, 1);
									update_scan();
								}
								cellptrtemp->cdr = addr;
							}

							heapRefsBuckets[1].push_back(vhit);
						}
					}
						break;

					case 6:
					for (auto vhit : heapRefsBuckets[ind]) 
					{
						if (vhit->ref) 
						{
							addr = copy(vhit->ref);
							while(lt_scan_freept()==1)
							{
								// cout<<"doing loop"<<endl;
								copy_given_scan_children(getscan(), 1, 1);
								update_scan();
							}
							vhit->ref = addr;
						}
					}
					break;

					default: break;
				}
			// if (!(vhit->ref)) cout<<"NULL After\n";
	}
	// cout<<"Nodename sorted on liveness values:\n";
	// gcout << "Completed processing root set" << endl;
	//chasing reachable cells from roots
//	while(lt_scan_freept()==1)//
//	{
//		//gcout << "Processing location " << (static_cast<cons*>(scan) - static_cast<cons*>(buffer_live))<<endl;
//		copy_children(scan, (static_cast<cons*>(scan))->setofStates);
//		update_scan();
//	}
	//cout << "Heap after GC"<<endl;
	// dump_heap("after");
	//++gccount;
	// cout<<"gc is returning here"<<endl;
	//end_GC_dump();

	cout << "Num of cells copied " << numcopied << '\n';
	// print_heap();
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
		++((static_cast<cons*>(cellptr))->visited);
#endif

	for(setiter i = st->begin(); i != st->end(); ++i)
	{
		state_index t = state_transition_table[*i][0]; // get_target_dfastate(*i, 0);
		if (t > 0)
			s0.insert(t);
		t = state_transition_table[*i][1]; //get_target_dfastate(*i, 1);
		if (t > 0)
			s1.insert(t);
	}
	 //gcout << "processing " << cellptr << endl;
	//process car field
	if(!s0.empty())
	{
		if (getType(cellptr, 1) == 1)//Do this only if car is address type
		{
			void* fpt = freept;
			void* newaddr = copy((static_cast<cons*>(cellptr))->car);
			 //gcout << "Car part of " << cellptr << " points to " << (static_cast<cons*>(cellptr))->car << endl;
			if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
			{
				(static_cast<cons*>(newaddr))->setofStates->clear();
				(static_cast<cons*>(newaddr))->setofStates->insert(s0.begin(), s0.end());
			}
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
			{
				(static_cast<cons*>(newaddr))->setofStates->insert(s0.begin(), s0.end());
			}
			else if (newaddr) //forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				stateset *s_child = (static_cast<cons*>(newaddr))->setofStates;
				for(setiter i = s0.begin(); i != s0.end(); ++i)
					if (s_child->find(*i) == s_child->end())
						s_new.insert(*i);
				if (!s_new.empty())
				{
					copy_children(newaddr, &s_new);
					(static_cast<cons*>(newaddr))->setofStates->insert(s_new.begin(), s_new.end());
				}
			}
			(static_cast<cons*>(cellptr))->car = newaddr;
		}
	}

	//process cdr field
	if(!s1.empty())
	{
		if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
		{
			void* fpt = freept;
			//gcout << "Cdr part of " << cellptr << " points to " << (static_cast<cons*>(cellptr))->cdr << endl;
			void* newaddr = copy((static_cast<cons*>(cellptr))->cdr);
			//gcout << "newaddr = "<<newaddr<<" && scan ptr ="<<scan<<" && freept = "<<fpt<<endl;
			if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
			{
				(static_cast<cons*>(newaddr))->setofStates->clear();
				(static_cast<cons*>(newaddr))->setofStates->insert(s1.begin(), s1.end());
			}
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
			{
				//gcout << "newaddr > scan" << endl;
				(static_cast<cons*>(newaddr))->setofStates->insert(s1.begin(), s1.end());
			}
			else if(newaddr)//forwarded pointer which has already been processed in the live buffer
			{
				//gcout << "newaddr < scan" << endl;
				stateset s_new;
				stateset *s_child = (static_cast<cons*>(newaddr))->setofStates;
				for(setiter i = s1.begin(); i != s1.end(); ++i)
					if (s_child->find(*i) == s_child->end())
						s_new.insert(*i);
				if (!s_new.empty())
				{
					copy_children(newaddr, &s_new);
					(static_cast<cons*>(newaddr))->setofStates->insert(s_new.begin(), s_new.end());
				}
			}
			(static_cast<cons*>(cellptr))->cdr = newaddr;

		}
	}
 return;
}
#endif


void clear_liveness_data()
{
	for(void* i = buffer_live; i < freept; i = i + sizeof(cons))
	{
		(static_cast<cons*>(i))->setofStates->clear();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For debugging
void print_accessible_heap()
{
	cout <<"----------------------------------------------------------------------------"<<endl;
	for (deque<actRec>::iterator stackit = actRecStack.begin();stackit != actRecStack.end(); ++stackit)
	{
		cout << "In function " << stackit->funcname << endl;
		for(vector<var_heap>::iterator vhit = stackit->heapRefs.begin(); vhit != stackit->heapRefs.end(); ++vhit)
		{
			if (vhit->ref)
			{
				cout << "Printing " << vhit->varname << " = ";
				printval(vhit->ref);
				cout << endl;
			}
		}
	}
	cout <<"----------------------------------------------------------------------------"<<endl;
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
	for(void* i = buffer_live; i < freept; i = i + sizeof(cons))
	{
		cout << "Size of set " << (static_cast<cons*>(i))->setofStates->size()<<endl;
	}
}

void dump_heap(string label)
{
	cons* i = NULL;
	string filename = "./" + label + "GC" + to_string(gccount);
	ofstream out(filename);
	for(i = static_cast<cons*>(buffer_live); i < static_cast<cons*>(boundary_live); ++i)
	{
		cons* conscell = i;
		void* carval = conscell->car;
		void* cdrval = conscell->cdr;
		void* forward = conscell->forward;
		char cartype = conscell->typecar;
		char cdrtype = conscell->typecdr;

		if (cartype == 'a' && is_valid_address(carval))
			carval = static_cast<cons*>(carval) - static_cast<cons*>(buffer_live);
		if (cdrtype == 'a' && is_valid_address(cdrval))
			cdrval = static_cast<cons*>(cdrval) - static_cast<cons*>(buffer_live);
		if (forward != NULL)
			forward = static_cast<cons*>(forward) - static_cast<cons*>(buffer_live);

		out << "("<< cartype << ", " ;
		if(carval==0)
			out << "(nil)";
		else
			out << carval;
		out <<", "<< cdrtype<<", ";
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
  return ((addr >= buffer_live) && (addr < boundary_live));
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
 	cons *conscell=static_cast<cons*>(loc);
	conscell->car=ref;
}


void set_cdr(void* loc,  void* ref)
{
 	cons *conscell=static_cast<cons*>(loc);
	conscell->cdr=ref;
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
	cons* cell = static_cast<cons*>(val);
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
      //cout << "Processing line " << line << endl;
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

		string filepath = "../benchmarks/programs/" + program_name + "/fsmdump-" + program_name;
		//cout << "Reading " << (filepath + "-state-map") << endl;
		read_state_map_from_file(filepath + "-state-map");
		//cout << "Reading " << (filepath + "-state-transition-table") << endl;
		read_state_transition_table_from_file(filepath + "-state-transition-table");
	}
	// cout << "Done initializing" << endl;

	return;
}

void initialize2 (GCStatus gc_type)
{
	gc_status = gc_type;
	// if (gc_type == gc_live)
	// {
	// 	state_transition_table = new state_index*[numkeys];
	// 	for (state_index i = 0; i < numkeys; ++i)
	// 	{
	// 		state_transition_table[i] = new state_index[2];
	// 		state_transition_table[i][0]=0;
	// 		state_transition_table[i][1]=0;
	// 	}

	// 	string filepath = "../benchmarks/programs/" + program_name + "/fsmdump-" + program_name;
	// 	//cout << "Reading " << (filepath + "-state-map") << endl;
	// 	read_state_map_from_file(filepath + "-state-map");
	// 	//cout << "Reading " << (filepath + "-state-transition-table") << endl;
	// 	read_state_transition_table_from_file(filepath + "-state-transition-table");
	// }
	// cout << "Done initializing" << endl;

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
static clock_tick gc_clock()
{
    return current_cons_tick;
}

static void tick()
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

static void clear_rch_flag()
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
                /*GC  | Cell  |Create| First | Last */
                /*Time| Addrs |Time  | Use   | Use  */
                "%lu\t| %p  \t| %lu\t| %lu\t | %lu\t\n",
                TIME_ms(gc_clock()), gs,
                TIME_ms(gs->created), TIME_ms(gs->first_use),
                TIME_ms(gs->last_use));
    }
    else
    { /* first and last use are -1 */
        fprintf(gc_stats_outfile,
                /*GC  | Cell  |Create| First | Last */
                /*Time| Addrs |Time  | Use   | Use  */
                "%lu\t| %p  \t| %lu\t| -1\t  | -1\t \n",
                TIME_ms(gc_clock()), gs, TIME_ms(gs->created));
    }
}

void dump_garbage_stats()
{
	// return;
	static int num_gcs;

    DBG(printf("dump_garbage_stats ==>\n"));
    cout<<"ere2"<<endl;
    fprintf(gc_stats_outfile, "//--- GC #%d | %lu \n",
            ++num_gcs, TIME_ms(gc_clock())); 
    cout<<"ere1"<<endl;
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
    DBG(printf("finish_gc_stats ==>"));
    unsigned long i;
    array_stats = static_cast<cons*>(buffer_live);
    last_pos = (static_cast<cons*>(freept) - static_cast<cons*>(buffer_live));

    /* dump the statistics for all cells */
    fprintf(gc_stats_outfile, "//--- Finish | %lu\n", TIME_ms(gc_clock()));
    for (i = 0; i < last_pos; i++)
        dump_cell_stats(&array_stats[i]);

    fclose(gc_stats_outfile);
}

static void update_last_use(cons *cell)
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



#endif

///////////////////////////////////////////////////k-Liveness GC //////////////////////////////////////////////////////////////////////
#ifdef K_LIVENESS
int k_depth = 10; //TODO This should be taken as a parameter and initialized globally
void liveness_gc()
{
#ifdef ENABLE_SHARING_STATS
	  for (void* i = buffer_dead; i < boundary_dead ; i += sizeof(cons))
			  (static_cast<cons*>(i))->visited = 0;
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
			string nodeName = "L/" + stackit->return_point + "/" + vhit->varname;

			stateMapIter got = statemap.find(nodeName);

			if (got != statemap.end() || k_depth == 0) //Don't check for any liveness
			{

				if (vhit->ref && (static_cast<cons*>(vhit->ref))->forward == NULL) //Check to see if a new cell was allocated
				{
					(static_cast<cons*>(vhit->ref))->setofStates->clear();
					(static_cast<cons*>(vhit->ref))->depth = 0;
				}

				void* addr = copy(vhit->ref);
				if (addr)
				{
					cons* c = static_cast<cons*>(addr);
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
		//gcout << "Processing location " << (static_cast<cons*>(scan) - static_cast<cons*>(buffer_live))<<endl;
		copy_children(scan, (static_cast<cons*>(scan)->setofStates));
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
		++((static_cast<cons*>((cellptr)))->visited);
#endif
	int curr_depth = (static_cast<cons*>(cellptr))->depth;
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
				void* newaddr = copy((static_cast<cons*>(cellptr))->car);

				if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
				{
					(static_cast<cons*>(newaddr))->setofStates->clear();
					(static_cast<cons*>(newaddr))->setofStates->insert(s0.begin(), s0.end());
					(static_cast<cons*>(newaddr))->depth = curr_depth + 1;
				}
				else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				{
					(static_cast<cons*>(newaddr))->setofStates->insert(s0.begin(), s0.end());
					int temp_depth = (static_cast<cons*>(newaddr))->depth < (curr_depth + 1) ? (curr_depth + 1) : (static_cast<cons*>(newaddr))->depth;
					(static_cast<cons*>(newaddr))->depth = temp_depth;
				}
				else if (newaddr) //forwarded pointer which has already been processed in the live buffer
				{
					stateset s_new;
					stateset *s_child = (static_cast<cons*>(newaddr))->setofStates;
					for(setiter i = s0.begin(); i != s0.end(); ++i)
						if (s_child->find(*i) == s_child->end())
							s_new.insert(*i);
					int temp_depth = (static_cast<cons*>(newaddr))->depth < (curr_depth + 1) ? (curr_depth + 1) : (static_cast<cons*>(newaddr))->depth;
					(static_cast<cons*>(newaddr))->depth = temp_depth;
					if (!s_new.empty())
					{
						copy_children(newaddr, &s_new);
						(static_cast<cons*>(newaddr))->setofStates->insert(s_new.begin(), s_new.end());
					}
				}
				(static_cast<cons*>(cellptr))->car = newaddr;
			}
		}

		//process cdr field
		if(!s1.empty())
		{
			if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
			{
				void* fpt = freept;
				void* newaddr = copy((static_cast<cons*>(cellptr))->cdr);
				if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
				{
					(static_cast<cons*>(newaddr))->setofStates->clear();
					(static_cast<cons*>(newaddr))->setofStates->insert(s1.begin(), s1.end());
					(static_cast<cons*>(newaddr))->depth = curr_depth + 1;
				}
				else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				{
					(static_cast<cons*>(newaddr))->setofStates->insert(s1.begin(), s1.end());
					int temp_depth = (static_cast<cons*>(newaddr))->depth < (curr_depth + 1) ? (curr_depth + 1) : (static_cast<cons*>(newaddr))->depth;
					(static_cast<cons*>(newaddr))->depth = temp_depth;
				}
				else if(newaddr)//forwarded pointer which has already been processed in the live buffer
				{
					stateset s_new;
					stateset *s_child = (static_cast<cons*>(newaddr))->setofStates;
					for(setiter i = s1.begin(); i != s1.end(); ++i)
						if (s_child->find(*i) == s_child->end())
							s_new.insert(*i);
					int temp_depth = (static_cast<cons*>(newaddr))->depth < (curr_depth + 1) ? (curr_depth + 1) : (static_cast<cons*>(newaddr))->depth;
					(static_cast<cons*>(newaddr))->depth = temp_depth;
					if (!s_new.empty())
					{
						copy_children(newaddr, &s_new);
						(static_cast<cons*>(newaddr))->setofStates->insert(s_new.begin(), s_new.end());
					}
				}
				(static_cast<cons*>(cellptr))->cdr = newaddr;

			}
		}
	}
	else //Start reachability based GC from this node
	{
		//cout << "Entering reachability GC " << endl;
		if (getType(cellptr, 1) == 1)//Do this only if car is address type
		{
			void* fpt = freept;
			void* newaddr = copy((static_cast<cons*>(cellptr))->car);
			if(newaddr && newaddr == fpt) //A cell was copied from the dead buffer to live buffer
				(static_cast<cons*>(newaddr))->depth = curr_depth; //curr_depth is k+1 so we don't need to increase it anymore
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				(static_cast<cons*>(newaddr))->depth = curr_depth;
			else if (newaddr && (static_cast<cons*>(newaddr))->depth < k_depth) //forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				(static_cast<cons*>(newaddr))->depth = curr_depth;
				copy_children(newaddr, &s_new);
			}
			(static_cast<cons*>(cellptr))->car = newaddr;
		}
		if (getType(cellptr, 2) == 1)//Do this only if cdr is address type
		{
			void* fpt = freept;
			void* newaddr = copy((static_cast<cons*>(cellptr))->cdr);
			if(newaddr && newaddr == fpt)//A cell was copied from the dead buffer to live buffer
				(static_cast<cons*>(newaddr))->depth = curr_depth;
			else if (newaddr > scan) //forwarded pointer but not yet processed in the live buffer
				(static_cast<cons*>(newaddr))->depth = curr_depth;
			else if(newaddr && (static_cast<cons*>(newaddr))->depth < k_depth)//forwarded pointer which has already been processed in the live buffer
			{
				stateset s_new;
				(static_cast<cons*>(newaddr))->depth = curr_depth;
				copy_children(newaddr, &s_new);
			}
			(static_cast<cons*>(cellptr))->cdr = newaddr;
		}
	}
 return;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

