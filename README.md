# Cache simulator

In this project, Implementation cache simulator.

There are two version.

**Direct-mapped**

**n-way set associative & fully-associative**

###

You can also run this on Linux.

Before running, You have to match data form.

**Data Scheme**
```sh
#I/O type | Tran. ID | Sector (now memory) Address | I/O size (now data)
Read 0 34762328 16
Read 0 33585744 8 
Read 0 33585848 8
Write 1 18491656 128
...
```

Here is a list of CL arguments
```
[Usage] ./cache_sim cache_mode cache_size block_size trace_file_name LRU_mode
```

##
**128KM direct-mapped cache with 8-byte blocks**

```
./cache_sim 0 128 8 sample.txt 0
```
##

**256KM 4-way set associative cache with 4-byte blocks, LRU on**

```
./cache_sim 4 256 4 sample.txt 1
```

##
**64KM fully-associative cache with 16-byte blocks**


```
./cache_sim -1 64 16 sample.txt 1
```
