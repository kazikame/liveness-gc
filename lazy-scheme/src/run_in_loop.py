import subprocess
import sys,os
import os.path
import time
import datetime
from collections import namedtuple

prog_size = {'nqueens':10000, 'nperm':40000, 'fibheap':500, 'treejoin':1800000, 'sudoku':4075, 'lcss':550, 'lambda':2790}

gc_info = namedtuple("gc_info", "heap_total heap_left heap_used gc_invocations gc_time exec_time")
gc_stat = namedtuple("gc_stat", "prog_name reachability liveness")
gc_stat_list = []


path = os.getcwd()
collector_path = os.path.join(path, "Simulator ")

def run_program(filename, prog_dir_path, gc_type):
    inputfile =  prog_dir_path + '/' + filename + '.scm'
    cmd = collector_path + " "  + inputfile + " " +  str(prog_size[filename]) + "   " + gc_type + " > " + prog_dir_path +'/results/' + filename + '-' + gc_type
    subprocess.call(cmd,shell=True)

def collect_gc_results(filename, prog_dir_path, gc_type):
    grep_cmd =  "egrep -o --line-buffered "
    result_file_name =  prog_dir_path + '/results/' + filename + '-' + gc_type
    print result_file_name
    int_grep = "| egrep -o \"[0-9]*\" | tr -d \'\\n\'"
    float_grep = "| egrep -o \"[0-9]*(\.[0-9]*)?\" | tr -d \'\\n\'"
    heap_total_cmd = grep_cmd + " \"Heap total=[0-9]* \"  " + result_file_name  + int_grep
    heap_left_cmd = grep_cmd + " \"Heap left=[0-9]*\"  " +  result_file_name  + int_grep
    heap_used_cmd = grep_cmd + " \"Heap used=[0-9]*\"   " +  result_file_name  + int_grep
    gc_invocations_cmd = grep_cmd + " \"GC Invocations=[0-9]*\"   " +  result_file_name  + int_grep
    gc_time_cmd = grep_cmd + " \"GC Time=[0-9]*(\.[0-9]*)?\"   " +  result_file_name  + float_grep
    prog_execution_time_cmd = grep_cmd + " \"Program Execution Time=[0-9]*(\.[0-9]*)?\"   " +  result_file_name  + float_grep
    heap_total = os.popen(heap_total_cmd).read()
    heap_left = os.popen(heap_left_cmd).read()
    heap_used = os.popen(heap_used_cmd).read()
    gc_invocations = os.popen(gc_invocations_cmd).read()
    gc_time = os.popen(gc_time_cmd).read()
    prog_execution_time = os.popen(prog_execution_time_cmd).read()
    return gc_info(int(heap_total), int(heap_left), int(heap_used), int(gc_invocations), float(gc_time), float(prog_execution_time))
    #return gc_info(0, 0, 0, int(gc_invocations), float(gc_time), float(prog_execution_time))


def main():
    numTimes = 1
    i = 0
    folder = '../benchmarks/programs'
    prog_folder_path = os.path.join(path, folder)
    
    for prog_dir in os.listdir(prog_folder_path):
        prog_dir_path = os.path.join(prog_folder_path, prog_dir)
        if (prog_dir in prog_size.keys()):
            i = i + 1
            subprocess.call('mkdir -p ' + prog_dir_path + '/results', shell=True)
            gc_plain_info = gc_info(0,0,0,0,0,0)
            gc_live_info = gc_info(0,0,0,0,0,0)

            for _ in xrange(numTimes):
                run_program(prog_dir, prog_dir_path, "gc-plain")
                temp_gc_plain_info = collect_gc_results(prog_dir, prog_dir_path, "gc-plain")
                gc_plain_info = gc_plain_info._replace(gc_invocations = gc_plain_info. gc_invocations + temp_gc_plain_info.gc_invocations)
                gc_plain_info = gc_plain_info._replace(gc_time= gc_plain_info.gc_time + temp_gc_plain_info.gc_time)
            for _ in xrange(numTimes):
                run_program(prog_dir, prog_dir_path, "gc-live")
                temp_gc_live_info = collect_gc_results(prog_dir, prog_dir_path, "gc-live")
                gc_live_info = gc_live_info._replace(gc_invocations=gc_live_info.gc_invocations + temp_gc_live_info.gc_invocations)
                gc_live_info = gc_live_info._replace(gc_time=gc_live_info.gc_time + temp_gc_live_info.gc_time)
            
            #TODO Fill in other details of gc_info
            gc_stat_prog = gc_stat(prog_dir, gc_plain_info, gc_live_info)
            gc_stat_list.append(gc_stat_prog)
            

    print "Generating report for  ", i," programs."
    d = str(datetime.date.today().isoformat())
    subprocess.call('mkdir -p ' + d, shell=True)
    reportpath = os.path.join(path, d, 'final_report_' + str(time.time())  + '.tex')
    with open(reportpath, 'w') as f:
        f.write('\\documentclass{article}\n')
        f.write('\\usepackage[english]{babel}\n')
        f.write('\\begin{document}\n')
        f.write('\\begin{center}\n')
        f.write('\\begin{tabular}\n')
        f.write('{| l | l | l | l | l |}\n')
        f.write('\\hline\n')
        f.write('Prog Name & Num RGC & Num LGC & Avg. RGC Time & Avg. LGC Time \\\\ \\hline\n')
        for g in gc_stat_list:
            f.write('\\verb@' + g.prog_name + '@'  + '&' + str(g.reachability.gc_invocations/float(numTimes)) + '&' + str(g.liveness.gc_invocations/float(numTimes)) + '&' + str(g.reachability.gc_time/float(numTimes)) + '&' + str(g.liveness.gc_time/float(numTimes)) + '\\\\ \\hline\n') 
        f.write('\\end{tabular}\n')
        f.write('\\end{center}\n')
        f.write('\\end{document}\n')


if __name__ == '__main__':
    main()
