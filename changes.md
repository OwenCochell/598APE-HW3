# Optimization Changes

This file will contain notes and benchmarks for any optimizations made.
Any changes made will be documented here, along with performance information.
Any changes should have their effects quantified with the `bench.sh` script.

All changes are documented in this file in order, as well as their commit IDs,
so one can jump back to a previous state and compare performance.

## Control

Control data, prior to any optimizations.
We were only able to measure case one, the other cases were too slow and killed.

ID: `4b70684aff1c9434ed40f32856272562eccd25bb`

```
 Performance counter stats for './main.exe 100 2.269 100000':

     9,285,941,030      task-clock:u                     #    0.995 CPUs utilized             
                 0      context-switches:u               #    0.000 /sec                      
                 0      cpu-migrations:u                 #    0.000 /sec                      
             2,151      page-faults:u                    #  231.640 /sec                      
    24,231,162,134      cpu_atom/instructions/u          #    1.92  insn per cycle              (1.89%)
    44,840,214,030      cpu_core/instructions/u          #    1.75  insn per cycle              (98.11%)
     <not counted>      cpu_lowpower/instructions/u                                             (0.00%)
    12,640,094,118      cpu_atom/cycles/u                #    1.361 GHz                         (1.89%)
    25,549,986,034      cpu_core/cycles/u                #    2.751 GHz                         (98.11%)
     <not counted>      cpu_lowpower/cycles/u                                                   (0.00%)
     1,662,886,935      cpu_atom/branches/u              #  179.076 M/sec                       (1.89%)
     2,063,583,792      cpu_core/branches/u              #  222.227 M/sec                       (98.11%)
     <not counted>      cpu_lowpower/branches/u                                                 (0.00%)
        17,613,658      cpu_atom/branch-misses/u         #    1.06% of all branches             (1.89%)
        20,557,254      cpu_core/branch-misses/u         #    1.00% of all branches             (98.11%)
     <not counted>      cpu_lowpower/branch-misses/u                                            (0.00%)
 #     60.9 %  tma_backend_bound        (1.89%)
 #     70.2 %  tma_backend_bound      
 #      0.8 %  tma_bad_speculation    
 #      1.2 %  tma_frontend_bound     
 #     27.8 %  tma_retiring             (98.11%)
 #      0.0 %  tma_retiring             (1.89%)
 #      4.4 %  tma_bad_speculation    
 #      0.0 %  tma_frontend_bound       (1.89%)

       9.337062243 seconds time elapsed

       9.230506000 seconds user
       0.028909000 seconds sys
```

## Change to C++

ID: `5f233420ebdaf0a578b2a86ec5179dbb9cea9801`

We change to C++. This does not invovle any major performance changes,
it just allows us to utilize modern components.

Performance is the same as previous entry.

## Energy Caching

ID: `03163ac0e00d87008c7816d737a20022aaa03ae6`

We cache the energy value for each entry,
meaning we no longer need to recompute the energy values after each loop.

This leads to some good performance improvements.

```
 Performance counter stats for './main.exe 100 2.269 100000':

        24,656,204      task-clock:u                     #    0.675 CPUs utilized             
                 0      context-switches:u               #    0.000 /sec                      
                 0      cpu-migrations:u                 #    0.000 /sec                      
             2,325      page-faults:u                    #   94.297 K/sec                     
        76,613,172      instructions:u                   #    1.46  insn per cycle            
                                                  #    0.07  stalled cycles per insn   
        52,651,194      cycles:u                         #    2.135 GHz                       
         5,360,474      stalled-cycles-frontend:u        #   10.18% frontend cycles idle      
        13,298,360      branches:u                       #  539.351 M/sec                     
           380,803      branch-misses:u                  #    2.86% of all branches           

       0.036539513 seconds time elapsed

       0.010583000 seconds user
       0.015649000 seconds sys

 Performance counter stats for './main.exe 1024 2.269 100000000':

     9,038,164,438      task-clock:u                     #    1.003 CPUs utilized             
                 0      context-switches:u               #    0.000 /sec                      
                 0      cpu-migrations:u                 #    0.000 /sec                      
             5,422      page-faults:u                    #  599.901 /sec                      
    18,633,226,818      instructions:u                   #    0.43  insn per cycle            
                                                  #    0.02  stalled cycles per insn   
    43,028,729,009      cycles:u                         #    4.761 GHz                       
       372,499,480      stalled-cycles-frontend:u        #    0.87% frontend cycles idle      
     1,880,276,128      branches:u                       #  208.037 M/sec                     
        80,541,143      branch-misses:u                  #    4.28% of all branches           

       9.011819855 seconds time elapsed

       8.961687000 seconds user
       0.035463000 seconds sys

 Performance counter stats for './main.exe 1024 2.269 100000000000':

   106,568,215,609      task-clock:u                     #    1.000 CPUs utilized             
                 0      context-switches:u               #    0.000 /sec                      
                 0      cpu-migrations:u                 #    0.000 /sec                      
             5,437      page-faults:u                    #   51.019 /sec                      
   203,396,958,306      instructions:u                   #    0.40  insn per cycle            
                                                  #    0.02  stalled cycles per insn   
   509,317,015,493      cycles:u                         #    4.779 GHz                       
     4,160,766,106      stalled-cycles-frontend:u        #    0.82% frontend cycles idle      
    18,077,042,893      branches:u                       #  169.629 M/sec                     
       916,719,133      branch-misses:u                  #    5.07% of all branches           

     106.617528547 seconds time elapsed

     106.008894000 seconds user
       0.064508000 seconds sys
```
