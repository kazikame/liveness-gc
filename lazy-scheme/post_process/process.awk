# COMMENTS
#  26/Sep/2013: Stats for SAS 2014 Paper
#
BEGIN {
    FS="|";
    c_total  = 0;
    c_dragged = 0;
    delta = 100; # assume < 1 sec for shrt life.
    skipall = 0;
    last_gc_time = 0;
    gc_time = 0;
    endtime = 0;
}
{
    if (skipall) next; # skip on error condition
}
/Address/ {
    next;
}
/GC/ {
    next;
}
/Finish/ {
    this_gc_time=$2 + 0;
    endtime = this_gc_time;
    
    # populate the endtime
    endfile="endtime";
    print "#END_OF_BM ", this_gc_time > endfile;
    next;
}
#-- the data about cells
{
# fields: gctime | Address | Create | 1stUse | LastUse
    this_gc_time= $1 + 0;
    create_time = $3 + 0;
    first_use   = $4 + 0;
    last_use    = $5 + 0;
    obj_count   = 1; # cons cells only

    c_total += obj_count;
    # this cell could potentially be collected at previous GC.
    # hence treated as dragged.
    if(create_time < last_gc_time && last_use < last_gc_time)
        c_dragged += obj_count;
  
    for(the_time=create_time; the_time <= this_gc_time; the_time++) {
        gc_reachable[the_time] += obj_count;
    }
    for(the_time=create_time; the_time <= last_use; the_time++) {
        gc_used[the_time] += obj_count;
    }
}
# -- finally dump the data
END {
    print "# Total Cells               : ", c_total;
    print "# Dragged Cells             : ", c_dragged;
    print "# Average Drag Time         : ", c_dragged*1.0/endtime;
    
    file1 = "rch.out";
    file2 = "use.out";
    file3 = "gbg.out";
    
    for(the_time in gc_reachable) {
        rch = gc_reachable[the_time] + 0;
        use = gc_used[the_time] + 0;
        
        print the_time, rch >> file1;
        print the_time, use >> file2;
        print the_time, rch-use >> file3;        
    }
}
