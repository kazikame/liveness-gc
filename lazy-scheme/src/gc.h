#ifndef __GC_H__
#define __GC_H__ 1
#include<vector>
#include<deque>
#include<unordered_map>
#include<set>
#include<utility>
#include "SchemeAST.h"
using namespace std;
using namespace Scheme::AST;


#define _OPT_TIME
//#undef _OPT_TIME

#ifdef _OPT_TIME
#undef _OPT_FULL_LGC
#else
#define _OPT_FULL_LGC
#endif


#ifndef __DEBUG__GC
#define __DEBUG__GC
//#undef __DEBUG__GC
#endif

//#define REACHABILITY_BFS
#define REACHABILITY_DFS
//#define LIVENESS_BFS
#define LIVENESS_DFS
//#define K_LIVENESS
//#defines to be used for meta data generation
//#define TEST_RUN //determines the min size of memory required (max reachability value achieved during program execution) to execute the program with gc-plain
//#define ENABLE_SHARING_STATS //displays the amount of sharing among the cells

/*-------------------------------------------------------------
 * To enable GC statistic recording. Comment #define line
 * to disable stat generation.
 * Author : Amey Karkare
 * */
//#define GC_ENABLE_STATS
#if 0
#define DBG(stmt) stmt
#else
#define DBG(stmt) (void)0
#endif

enum GCStatus {gc_disable, gc_live, gc_freq, gc_plain};


typedef stateset::iterator setiter;
typedef unordered_map<string, state_index>::const_iterator stateMapIter;
typedef unordered_map<string, state_index> stateMap;


extern std::ostream null_stream;



typedef struct Var_Heap
{
    string varname;
    void* ref;
}var_heap;

typedef struct Var_Stack
{
    string varname;
    void* value;
    char type;
}var_stack;

/* A better name for the environment is Activation Record */
/* Maybe it is better to combine reference and non-reference
   values through a union type*/
typedef struct activationRecord
{
	string return_point;
	string funcname;
	vector<var_heap> heapRefs;
	vector<var_stack> nonRefs;

}actRec;


 cons* getCar(void* ref, const char fromGC);
 cons* getCdr(void* ref, const char fromGC);
 void init_gc_stats();
 void finish_gc_stats();
 void dump_garbage_stats();
 int sizeof_cons();
 unsigned int current_heap();
 int getType(void* node, int field);
 void allocate_heap(unsigned long size);
 void swap_buffer();
 void update_free(unsigned int size);
 int check_space(int size);
 cons* return_null();
 cons* allocate_cons();
 void empty_environment(string functionname);
 void make_environment(const char *functionname, std::string returnpoint);
 void set_return_point(int pt);
 int set_reference(var_heap bind, const char *var, void* newref);
 void delete_environment();
 void make_reference_addr(const char *var, void* addr);
 void make_reference_value(const char *var, void* val, const char type);
 char locate_var(const char *var);
 cons* lookup_addr(const char *var);
 void* lookup_value(const char *var);
 void printval(void *ref,ostream& out = cout);
#ifdef _OPT_TIME
 cons* copy(cons* node, ostream& out = null_stream);
 cons* deep_copy(cons* node, int gc_type = 0, ostream& out = null_stream);
 cons* followpaths_reachability(cons* loc, ostream& out = null_stream);
 cons* followpaths(cons* loc, state_index index, ostream& out = null_stream);
 void print_gc_move(cons* from, cons* to, ostream& out = null_stream);
 void clear_live_buffer(ostream& out = null_stream);
#else
 cons* copy(cons* node);
 cons* deep_copy(cons* node, int gc_type = 0);
 cons* followpaths_reachability(cons* loc);
 cons* followpaths(cons* loc, state_index index);
 void print_gc_move(cons* from, cons* to);
#endif

 int copy_scan_children(cons* node);
 actRec& return_stack();
 int lt_scan_free();
 unsigned long diff_scan_free();
 void set_car(void* loc, void* ref);
 void set_cdr(void* loc, void* ref);
 void* dup_cons(cons* cell);

 void detail_gc();
 int is_null_stack(actRec st);
 int is_null_binding(var_heap *bind);
 int void_to_int(void *val);
 string* void_to_char(void *val);
 void* int_to_void(int val);
 void* char_to_void(std::string* str);
 cons* void_to_cons(void* val);
 long void_to_long(void* val);
 void* long_to_void(long val);


 void* getscan();
 void* getfree();
 cons* getbufferlive();
 int lt_scan_freept();
 void calculate_garbage();

//Iterator methods to access activation record stack, heap references and non-reference variables.
 actRec& get_next_activation_record();
 int is_end_of_activation_record_stack();
 var_heap& get_heap_ref_vector();
 var_heap& get_next_heap_ref_vector();
 int is_end_of_heap_ref_vector();
 var_stack& get_non_ref_vector();
 var_stack& get_next_non_ref_vector();
 int is_end_of_non_ref_vector();
 int is_empty_heap_ref_vector();
 int is_empty_non_ref_vector();
 void print_activation_record_stack(ostream& out = null_stream);

//GC methods
 void reachability_gc();
 void initialize (string program_name, state_index, GCStatus);
 void cleanup(state_index);
 void liveness_gc();
 void dump_heap(string label);
 void print_accessible_heap(const string);
 int is_valid_address(void* addr);
 void create_heap_bft(ostream&);
 string print_cell_type(cell_type t);
 void update_heap_ref_stack(ostream& out = cerr, int gc_type = 0);
#endif
