BEGIN {
    max_time = -1; # end time of benchmark
    # Plot will be from 0 to 100%, at 5% interval
    delta=5;
}
# benchmark endtime
/END_OF_BM/ { max_time = $2+0; next;}
# otherwise
{
    x = $1+0;
    y = $2+0;
    
    percent = (x * 100) / max_time;
    slot = int(percent/delta);
    table[slot] += y;
}
END {
    for (i = 0; i < 100/delta; i++)
    {
        table[i] += 0; #in case it is uninitialised
        print (i*delta)"-"(i+1)*delta, table[i];
    }
}
