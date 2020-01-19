# HW3-scheduling-simulation

## Objectives
- Simulate task scheduling
- Understanding priority ceiling protocol
- Understanding how to implement context switch

## Pre-commit

$ astyle --style=bsd --indent=spaces=4</br>

## Commands

$ make test1</br>

$./scheduling_simulator</br>

$ make test2</br>

$./scheduling_simulator</br>

## Output 

#### test1
```
In T1
T1 try to get RESOURCE_1
T1 got RESOURCE_1
T1 try to activate T2
T1 return from activate_task(T2)
T1 try to activate T3
T1 return from activate_task(T3)
T1 try to activate T4
T1 return from activate_task(T4)
T1 try to activate T5
In T5
T5 terminated
T1 return from activate_task(T5)
T1 try to release RESOURCE_1
In T4
T4 try to get RESOURCE_1
T4 got RESOURCE_1
T4 try to release RESOURCE_1
T4 return from release_resource(RESOURCE_1)
T4 terminated
In T3
T3 terminated
In T2
T2 terminated
T1 return from release_resource(RESOURCE_1)
T1 terminated
In idle_task
```

#### test2
```
In T2
T2 try to activate T2
T2 failed to activate T2
T2 try to activate T4
T2 return from activate_task(T4)
T2 try to activate T3
T2 return from activate_task(T3)
T2 terminated
In T4
T4 try to get RESOURCE_1
T4 got RESOURCE_1
T4 try to get RESOURCE_2
T4 got RESOURCE_2
T4 try to activate T5
T4 return from activate_task(T5)
T4 try to release RESOURCE_2
In T5
T5 try to get RESOURCE_2
T5 got RESOURCE_2
T5 try to get RESOURCE_2
T5 failed to get RESOURCE_2
T5 try to terminate
T5 failed to terminate
T5 try to release RESOURCE_2
T5 return from release_resource(RESOURCE_2)
T5 try to release RESOURCE_2
T5 failed to release RESOURCE_2
T5 terminated
T4 return from release_resource(RESOURCE_2)
T4 try to release RESOURCE_1
T4 return from release_resource(RESOURCE_1)
T4 terminated
In T3
T3 try to activate T1
T3 failed to activate T1
T3 terminated
In T1
T1 try to get RESOURCE_1
T1 got RESOURCE_1
T1 try to activate T3
T1 return from activate_task(T3)
T1 try to activate T5
In T5
T5 try to get RESOURCE_2
T5 got RESOURCE_2
T5 try to get RESOURCE_2
T5 failed to get RESOURCE_2
T5 try to terminate
T5 failed to terminate
T5 try to release RESOURCE_2
T5 return from release_resource(RESOURCE_2)
T5 try to release RESOURCE_2
T5 failed to release RESOURCE_2
T5 terminated
T1 return from activate_task(T5)
T1 try to release RESOURCE_1
In T3
T3 try to activate T1
T3 failed to activate T1
T3 terminated
T1 return from release_resource(RESOURCE_1)
T1 terminated
In idle_task
```