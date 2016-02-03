#!/usr/bin/python
import subprocess
import sys,os
import os.path
import time
import datetime
from collections import namedtuple

#prog_size = {'nqueens':10101, 'nperm':27428, 'fibheap':37043, 'treejoin':1616533, 'sudoku':4066, 'lcss':22243, 'lambda':20466, 'gc_bench':204850, 'knightstour':677800, 'huffman':100070, 'paraffins':5185, 'deriv':11124}
prog_size = {'nqueens':10101, 'nperm':27428, 'fibheap':37043, 'treejoin':1616533, 'sudoku':4066, 'lcss':22243, 'lambda':20466, 'gc_bench':204850, 'huffman':100070, 'paraffins':5185, 'deriv':11124}
gc_info = namedtuple("gc_info", "heap_total heap_left heap_used gc_invocations gc_time dfa_time num_states exec_time")
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
    float_grep = "| egrep -o \"(([0-9][0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)?\" | tr -d \'\\n\'"
    heap_total_cmd = grep_cmd + " \"Heap total=[0-9]* \"  " + result_file_name  + int_grep
    heap_left_cmd = grep_cmd + " \"Heap left=[0-9]*\"  " +  result_file_name  + int_grep
    heap_used_cmd = grep_cmd + " \"Heap used=[0-9]*\"   " +  result_file_name  + int_grep
    gc_invocations_cmd = grep_cmd + " \"GC Invocations=[0-9]*\"   " +  result_file_name  + int_grep
    gc_time_cmd = grep_cmd + " \"GC Time=(([0-9][0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)?\"   " +  result_file_name  + float_grep
    dfa_time_cmd = grep_cmd + " \"DFA Gen Time=(([0-9][0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)?\"   " +  result_file_name  + float_grep
    dfa_states_cmd = grep_cmd + " \"Num of DFA states=(([0-9][0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)?\"   " +  result_file_name  + int_grep
    prog_execution_time_cmd = grep_cmd + " \"Program Execution Time=[0-9]*(\.[0-9]*)?\"   " +  result_file_name  + float_grep
    heap_total = os.popen(heap_total_cmd).read()
    heap_left = os.popen(heap_left_cmd).read()
    heap_used = os.popen(heap_used_cmd).read()
    gc_invocations = os.popen(gc_invocations_cmd).read()
    dfa_time = os.popen(dfa_time_cmd).read()
    num_states = os.popen(dfa_states_cmd).read()
    gc_time = os.popen(gc_time_cmd).read()
    prog_execution_time = os.popen(prog_execution_time_cmd).read()
    return gc_info(int(heap_total), int(heap_left), int(heap_used), int(gc_invocations), float(gc_time), float(dfa_time), int(num_states), float(prog_execution_time))
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
            gc_plain_info = gc_info(0,0,0,0,0,0,0,0)
            gc_live_info = gc_info(0,0,0,0,0,0,0,0)

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
        f.write('{| l | r | r | r | r | r |}\n')
        f.write('\\hline\n')
        f.write('Prog Name & Num RGC & Num LGC & Avg. RGC Time & Avg. LGC Time & LGC/RGC \\\\ \\hline\n')
        for g in gc_stat_list:
            f.write('\\verb@' + g.prog_name + '@'  + 
                    '&' + str(g.reachability.gc_invocations/float(numTimes)) +
                    '&' + str(g.liveness.gc_invocations/float(numTimes)) +
                    '&' + str(g.reachability.gc_time/float(numTimes)) +
                    '&' + str(g.liveness.gc_time/float(numTimes)) +
                    '&' + str(g.liveness.gc_time/g.reachability.gc_time) +
                    '\\\\ \\hline\n') 
        f.write('\\end{tabular}\n')
        f.write('\\end{center}\n')
        f.write('\\end{document}\n')
if __name__ == '__main__':
    main()
