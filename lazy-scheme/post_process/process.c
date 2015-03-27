#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXLINE 256
#define MAXCELL 50000000

static const char *FS="|";
static long c_total  = 0;
static long c_dragged = 0;
static long t_dragged = 0;
static long last_gc_time = 0;
static long this_gc_time;

static long gc_reachable[MAXCELL];
static long gc_used[MAXCELL];

void parse(const char* line);
int main(int argc, char* argv[]) 
{
    int len;
    size_t n = MAXLINE;
    char *line = malloc(sizeof(char)*MAXLINE);
    const char *filename = (argc == 1) ? "GC_STATS.txt" : argv[1];
    FILE* statsfile = fopen(filename, "r");
    
    while ((len = getline(&line, &n, statsfile)) >0) {
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
    unsigned int the_time;    
    for(the_time = 0; the_time <= this_gc_time;  the_time++) {
        long rch = gc_reachable[the_time];
        long use = gc_used[the_time];
        
        fprintf(file1, "%u %ld\n", the_time, rch);
        fprintf(file2, "%u %ld\n", the_time, use);
        fprintf(file3, "%u %ld\n", the_time, rch-use);
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
               long *last_use) 
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
        return;
    }
    
    if (strncmp(line, "//--- Finish", 12) == 0) {
        /* End of Run */
        last_gc_time = this_gc_time;
        this_gc_time = get2ndVal(line);
        FILE* endfile = fopen("endtime", "w");
        fprintf(endfile, "#END_OF_BM %ld\n", this_gc_time);
        fclose(endfile);
        return;
    }

    /* Finally, the real data about cells */
    /* fields: gctime | Address | Create | 1stUse | LastUse */
    long first_use, last_use, time_gc;
    long create_time, address;
    getValues(line, &time_gc, &address, &create_time, &first_use, &last_use);
    assert(time_gc == this_gc_time);
    
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
    
    
    long the_time;
    for(the_time=create_time; the_time <= this_gc_time; the_time++) {
        gc_reachable[the_time]++;
    }
    for(the_time=create_time; the_time <= last_use; the_time++) {
        gc_used[the_time]++;
    }
    return;
}

