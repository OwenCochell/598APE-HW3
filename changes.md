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
