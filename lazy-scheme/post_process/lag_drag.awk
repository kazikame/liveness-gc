# COMMENTS
#  4/May/2006:  void cells are counted as dragged cells also.
#
BEGIN {
    FS="|";
    delta = 1; # interval to plot
    high_factor = 100000;
    
    cumulative_lag = cumulative_lag_high = 0;
    cumulative_drag =cumulative_drag_high = 0;
    cumulative_void_drag = cumulative_void_drag_high = 0;
    cumulative_stay =cumulative_stay_high = 0;
    last_gc_time = 0;
    gc_time = 0;
}
{
    if (skipall) next; # skip on error condition
}
/Address/ {
    next;
}
/GC|Finish/ {
    this_gc_time=$2+0;
    if (this_gc_time != gc_time) 
    {
        if (this_gc_time < 0)
        {
            print "\nERROR - Too big benchmark - Time wrap.";
            print "        Reduce its runtime.\n";
            skipall = 1;
        }
        else 
        {
            last_gc_time=gc_time;
            gc_time = this_gc_time;
        }
    }
            
    next;
}
#-- the data about cells
{
# fields: gctime | Address | Create | 1stUse | LastUse | OnThefly? | Size |
    create_time = $3 + 0;
    first_use   = $4 + 0;
    last_use    = $5 + 0;
    obj_count   = $7 + 0;
    
    #-- Total in-memory Time 
    stay_time = (gc_time - create_time);
    tot_stay_time = obj_count*stay_time;
    # update cumulative_stay
    add_low = tot_stay_time % high_factor;
    add_high = int(tot_stay_time / high_factor);

    tmp = cumulative_stay + add_low;
    cumulative_stay_high += (add_high + int(tmp/high_factor));
    cumulative_stay = tmp % high_factor;

    if (first_use == -1) 
    {
        # Void Cell Drag Time
        void_drag_time = stay_time;
        void_drag_interval = int(void_drag_time / delta);
        void_drag_table[void_drag_interval] += obj_count;

        # update cumulative_void_drag
        tmp = cumulative_void_drag + add_low;
        cumulative_void_drag_high += (add_high + int(tmp/high_factor));
        cumulative_void_drag = tmp % high_factor;

        # Add to drag table too
        if(create_time < last_gc_time)
        {
            drag_table[void_drag_interval] += obj_count;
            # update cumulative_drag
            tmp = cumulative_drag + add_low;
            cumulative_drag_high += (add_high + int(tmp/high_factor));
            cumulative_drag = tmp % high_factor;
        }

    }
    else 
    {
        #-- Lag Time 
        lag_time = (first_use - create_time);
	tot_lag_time = obj*count*lag_time;
        lag_interval = int(lag_time / delta);
        lag_table[lag_interval] += obj_count;
        # update cumulative_lag
        add_low = tot_lag_time % high_factor;
        add_high = int(tot_lag_time / high_factor);

        tmp = cumulative_lag + add_low;
        cumulative_lag_high += (add_high + int(tmp/high_factor));
        cumulative_lag = tmp % high_factor;

        #-- Drag Time
        if(last_use < last_gc_time)
        {
            drag_time = (gc_time - last_use);
            tot_drag_time = obj_count*drag_time;
            drag_interval = int(drag_time / delta);
            drag_table[drag_interval] += obj_count;
            # update cumulative_drag
            add_low = tot_drag_time % high_factor;
            add_high = int(tot_drag_time / high_factor);

            tmp = cumulative_drag + add_low;
            cumulative_drag_high += (add_high + int(tmp/high_factor));
            cumulative_drag = tmp % high_factor;
        }
    }
}
# -- finally dump the data
END {
    file1 = "drag.out";
    print "#drag data" > file1;
    for(t in drag_table) {
        print t*delta, drag_table[t] >> file1;
    }
    printf "# Total In-Memory Time      : %u%05u\n",
        cumulative_stay_high, cumulative_stay;
    
    printf "# Total Drag Time           : %u%05u\n",
        cumulative_drag_high, cumulative_drag;
    
    file2 = "lag.out";
    print "#lag data" > file2;
    for(t in lag_table) {
        print t*delta, lag_table[t] >> file2;
    }
    printf "# Total Lag Time            : %u%05u\n",
        cumulative_lag_high, cumulative_lag;

    file3 = "void.out";
    print "#void data" > file3;
    for(t in void_drag_table) {
        print t*delta, void_drag_table[t] >> file3;
    }
    printf "# Total Void Cell Drag Time : %u%05u\n",
        cumulative_void_drag_high, cumulative_void_drag;
}
