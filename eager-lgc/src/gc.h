#ifndef __GC_H__
#define __GC_H__ 1
#include<vector>
#include<deque>
#include<unordered_map>
#include<set>
#include<utility>
#include "DemandStructure.h"

using namespace std;


#define REACHABILITY_BFS
//#define REACHABILITY_DFS
#define LIVENESS_BFS
//#define LIVENESS_DFS
//#define K_LIVENESS
//#defines to be used for meta data generation
//#define TEST_RUN //determines the min size of memory required (max reachability value achieved during program execution) to execute the program with gc-plain
//#define ENABLE_SHARING_STATS //displays the amount of sharing among the cells

/*-------------------------------------------------------------
 * To enable GC statistic recording. Comment #define line
 * to disable stat generation.
 * Author : Amey Karkare
 * */
 #define GC_ENABLE_STATS
#if 0
#define DBG(stmt) stmt
#else
#define DBG(stmt) (void)0
#endif

enum GCStatus {gc_disable, gc_live, gc_freq, gc_plain};

typedef int state_index;
typedef int liv_index;
typedef set<state_index> stateset;
typedef stateset::iterator setiter;
typedef unordered_map<string, state_index>::const_iterator stateMapIter;
typedef unordered_map<string, state_index> stateMap;
typedef unordered_map<std::string, std::string> call_next_data;


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
  Scheme::Demands::LivenessState funcLiveness;
}actRec;

typedef unsigned long clock_tick;
typedef struct cell_cons
{
  char typecell, typecar, typecdr;   //'c' for typecell
  void *car, *cdr, *forward;
  stateset *setofStates;
  int depth;
#ifdef GC_ENABLE_STATS
    /*----------------------------------------------------------------------
     * Following fields are added by Amey Karkare to
     * generate gc related statistics
     */
    /* size is used only if we support vectors */
    clock_tick created;       /* creation time of cell */
    clock_tick first_use;     /* first use time of cell */
    clock_tick last_use;      /* last use time of cell */
    char       is_reachable:1,  /* true if cell is reachable during current gc */
               is_used:1;       /* true if cell is dereferenced for use */
    /*----------------------------------------------------------------------*/
#endif
#ifdef ENABLE_SHARING_STATS
    int visited;
#endif
} cons;




 void* getCar(void* ref, const char fromGC);
 void* getCdr(void* ref, const char fromGC);
 void init_gc_stats();
 void finish_gc_stats();
 void dump_garbage_stats();
 int sizeof_cons();

 void allocate_heap(unsigned long size);
 void swap_buffer();
 void update_free(unsigned int size);
 int check_space(int size);
 void* return_null();
 void* allocate_cons(const char cartype, void* addrcar, const char cdrtype, void* addrcdr);
 void empty_environment(string functionname);
 void make_environment(const char *functionname, std::string returnpoint);
 void set_return_point(int pt);
 int set_reference(var_heap bind, const char *var, void* newref);
 void delete_environment();
 void make_reference_addr(const char *var, void* addr);
 void make_reference_value(const char *var, void* val, const char type);
 char locate_var(const char *var);
 void* lookup_addr(const char *var);
 void* lookup_value(const char *var);
 void printval(void *ref);
 int current_heap();
 int getType(void* node, int field);
 void* copy(void* node);
 int copy_scan_children(void* node);
 actRec& return_stack();


 int lt_scan_free();
 unsigned long diff_scan_free();
 void set_car(void* loc, void* ref);
 void set_cdr(void* loc, void* ref);

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

//GC methods
 void reachability_gc();
 void initialize (string program_name, state_index, GCStatus);
 void initialize2 (GCStatus gc_type);
 void cleanup(state_index);
 void liveness_gc();
 void dump_heap(string label);
#endif
