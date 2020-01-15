# README #

Liveness based GC with restricted lattice of demands.

## Pre-req ##

1. boost
2. bison
3. flex

Tested on Ubuntu 16.04(xenial) /18.04(bionic)

## Build ##

```bash
cd eager-lgc/src
make -j<no-of-cores> <task>
```

The valid \<task\> are:

1. `Simulator` - Default lattice
2. `Simulator_simple` - Simple lattice
3. `Simulator_diff` - Comprehensive lattice

For more details, check the report

## Run ##

Make sure the maximum allowed stack size is high enough for benchmarks to run.

```bash
$ ulimit -s
unlimited
```

```bash
./<task> <filename.scm> <heap-size> <type>
```

Types:

1. `gc-live`
2. `gc-plain`
