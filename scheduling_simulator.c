#include "resource.h"
#include "task.h"
#include "config.h"
#include "task_set.h"
#include "ucontext.h"
#include "typedefine.h"
#include "stdlib.h"
#include "stdio.h"
#ifdef _LP64
#define STACK_SIZE 2097152+16384
#else
#define STACK_SIZE 16384
#endif

int main()
{
    time_count=0;
    ready_count = AUTO_START_TASKS_COUNT;
    for(int i=0; i<AUTO_START_TASKS_COUNT; i++)
    {
        char stack[STACK_SIZE];

        getcontext(&auto_ctx[i]);

        auto_ctx[i].uc_stack.ss_sp = stack;
        auto_ctx[i].uc_stack.ss_size = sizeof(stack);
        auto_ctx[i].uc_stack.ss_flags = 0;
        auto_ctx[i].uc_link = &schedule;

        ready_queue[i].id = task_const[auto_start_tasks_list[i]].id;
        ready_queue[i].entry = task_const[auto_start_tasks_list[i]].entry;
        ready_queue[i].task = &auto_ctx[i];
        ready_queue[i].static_p = task_const[auto_start_tasks_list[i]].static_priority;
        ready_queue[i].current_p = task_const[auto_start_tasks_list[i]].static_priority;
        ready_queue[i].state = READY;  // set READY
        ready_queue[i].time = 0; // initial time
        for(int j=0; j<RESOURCES_COUNT; j++)
            ready_queue[i].res[j] = 0;

        makecontext(&auto_ctx[i],ready_queue[i].entry,0);
    }

    preempt();

    swapcontext(&schedule,running_task.task);
    // printf("swap error\n");

    return 0;
}