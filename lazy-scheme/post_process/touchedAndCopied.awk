BEGIN {
    copied=0
    touched=0
    coll=0
    numgc=0
    batch=1 # for generating table
}
{
    numgc++;
    copied += $3
    touched += $7
    coll += $5
    lastcopied = $3
    lasttouched = $7
    lastcoll = $5
}
END{
    numgc--
    copied -= lastcopied
    touched -= lasttouched
    coll -= lastcoll

    if (batch == 0) {
        print "Number of GC: ", numgc
        print "Copied Cells (Avg) : ", copied, copied/numgc
        print "Touched Cells (Avg) : ", touched, touched/numgc
        print "Collected Cells (Avg) : ", coll, coll/numgc
    }
    else {
        # print only avg of collected and touched and num
        print coll/numgc, touched/numgc, numgc
    }
}
