#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>

using namespace std;

#define MAXLINE 256
#define MAXCELL 50000000
#define MAXGC 100000

static const char *FS="|";
static long c_total  = 0;
static long c_dragged = 0;
static long t_dragged = 0;
static long last_gc_time = 0;
static long this_gc_time;

static vector<long> gc_reachable;
static vector<long> gc_used;
static vector<long> gc_rgcied;
static vector<long> gc_times;
static int  gc_count = 0;

void parse(const char* line);
int main(int argc, char* argv[]) 
{
    int len;
    size_t n = MAXLINE, ncell = 0;
    char *line = new char[MAXLINE];
    const char *filename = (argc == 1) ? "GC_STATS.txt" : argv[1];
    FILE* statsfile = fopen(filename, "r");
    
    while ((len = getline(&line, &n, statsfile)) >0) {
        ncell++;
        parse(line);
    }
    fclose(statsfile);
    /* Dump the data */
    printf("# Total Cells               : %ld\n", c_total);
    printf("# Dragged Cells             : %ld\n", c_dragged);
    if (c_dragged > 0)
    {
        printf("# Total Drag Time           : %ld\n", t_dragged);
        printf("# Average Drag Time         : %lf\n", t_dragged*1.0/c_dragged);
    }
    
    FILE* file1 = fopen("rch.out", "w");
    FILE* file2 = fopen("use.out", "w");
    FILE* file3 = fopen("gbg.out", "w");
    FILE* file4 = fopen("rgc.out", "w");
    unsigned int the_time;    
    for(the_time = 0; the_time <= this_gc_time;  the_time++) {
        long rch = gc_reachable[the_time];
        long use = gc_used[the_time];
        long rgc = gc_rgcied[the_time];
        fprintf(file2, "%u %ld\n", the_time, use);
        if ((the_time > 0) && (rch == 0)) /* ignore */
            continue;
        
        fprintf(file1, "%u %ld\n", the_time, rch);
        fprintf(file3, "%u %ld\n", the_time, rch-use);
        // if (rgc > 0) : approx use is rgc + use, else it is use.
        fprintf(file4, "%u %ld\n", the_time, use + (rgc > 0 ? rgc : 0));
    }
    return 0;
}

long get2ndVal(const char* line) 
{
    long val;
    static char str[MAXLINE];
    strcpy(str,line);
    char* token;
    token = strtok(str, FS);
    token = strtok(NULL, FS);
    sscanf(token, "%ld", &val);
    return val;
}

void getValues(const char *line,
               long *this_gc_time,
               long *address,
               long *create_time,
               long *first_use,
               long *last_use,
               long *last_copied) 
{
    static char str[MAXLINE];
    strcpy(str,line);
    char* token;
    token = strtok(str, FS);
    sscanf(token, "%ld", this_gc_time);
    token = strtok(NULL, FS);
    sscanf(token, "0x%ld", address);
    token = strtok(NULL, FS);
    sscanf(token, "%ld", create_time);
    token = strtok(NULL, FS);
    sscanf(token, "%ld", first_use);
    token = strtok(NULL, FS);
    sscanf(token, "%ld", last_use);
    token = strtok(NULL, FS);
    sscanf(token, "%ld", last_copied);
    return;
}

void push_in_vector(vector<long>& vec, ulong idx) 
{
    if (vec.size() > idx)
        return;
    for (uint i = vec.size(); i <= idx; i++)
        vec.push_back(0);
    return;
}

        

void parse(const char* line) 
{
    if (strncmp(line, "T_gc", 4) == 0) {
        /* Ignore header T_gc	| Address	| Tnew	| T_Fu	| T_Lu */ 
        return;
    }
        
    if (strncmp(line, "//--- GC", 8) == 0) {
        /* Ignore GC time stamp, but store last gc_time */
        last_gc_time = this_gc_time;
        this_gc_time = get2ndVal(line);
        gc_times.push_back(this_gc_time);
        gc_count++;
        return;
    }
    
    if (strncmp(line, "//--- Finish", 12) == 0) {
        /* End of Run */
        last_gc_time = this_gc_time;
        this_gc_time = get2ndVal(line);
        gc_times.push_back(this_gc_time);
        gc_count++;
        
        FILE* endfile = fopen("endtime", "w");
        fprintf(endfile, "#END_OF_BM %ld\n", this_gc_time);
        fclose(endfile);
        return;
    }

    /* Finally, the real data about cells */
    /* fields: gctime | Address | Create | 1stUse | LastUse | LastCP */
    long first_use, last_use, time_gc, last_copied;
    long create_time, address;
    getValues(line, &time_gc, &address, &create_time, &first_use,
              &last_use, &last_copied);

    //assert(last_use < MAXCELL);
    if (time_gc != this_gc_time) {
        fprintf(stderr, "Error_1 in GC statistics\n\t");
        fprintf(stderr, "%ld != %ld\n", time_gc, this_gc_time);
        exit(0);
    }
    
    c_total++;
    if ((c_total %  10000) == 0)
        fprintf(stderr, ".");
    if ((c_total % 600000) == 0)
        fprintf(stderr, "\n");
    
    /* this cell could potentially be collected at previous GC.
       hence treated as dragged.*/
    if(create_time < last_gc_time && last_use < last_gc_time) {
        c_dragged++;
        t_dragged += (time_gc - last_use);
    }
    assert(gc_count < MAXGC);
 
    int idx;
    long the_time;
    for (idx = 0; idx < gc_count; idx++) {
        the_time = gc_times[idx];
        //assert(the_time < MAXCELL);
        if (time_gc > this_gc_time) {
            fprintf(stderr, "Error_2 in GC statistics\n\t");
            fprintf(stderr, "%ld > %ld\n", time_gc, this_gc_time);
            assert(the_time <= this_gc_time);
        }
        
        if (the_time < create_time)
            continue;
        push_in_vector(gc_reachable, the_time+1);
        push_in_vector(gc_rgcied, the_time+1);
        gc_reachable[the_time]++;
        /* this cell copied using RGC inside a closure while doing LGC */
        if (last_copied != -1) {
            if (last_use < last_copied) {
                gc_rgcied[the_time]++;
            }
        }
        long next_gc_time = (idx + 1 == gc_count) ? -1 : gc_times[idx+1];
        if ((the_time != this_gc_time) && (the_time + 1 != next_gc_time)) {
            gc_reachable[the_time+1]++;
        
            /* this cell copied using RGC inside a closure while doing LGC */
            if (last_copied != -1) {
                if (last_use < last_copied) {
                    gc_rgcied[the_time+1]++;
                }
            }
        }
    }
    
    if (last_use == -1)
        return;
    push_in_vector(gc_used, last_use);
    for(the_time=create_time; the_time <= last_use; the_time++) {
        gc_used[the_time]++;
    }
    return;
}
