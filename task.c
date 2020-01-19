#include "task.h"
#include "ucontext.h"
#include "stdlib.h"
#include "stdio.h"
#ifdef _LP64
#define STACK_SIZE 2097152+16384
#else
#define STACK_SIZE 16384
#endif

int act_count = 0;

status_type activate_task(task_type id)
{
    time_count++;
    // default type
    status_type result = STATUS_OK;

    // activate itself is not permittd
    if(running_task.id == id)
        return STATUS_ERROR;

    for(int i=0; i<ready_count; i++)
    {
        if(ready_queue[i].id == id)
            return STATUS_ERROR;
    }

    char stack[STACK_SIZE];
    getcontext(&act_ctx[act_count]);

    act_ctx[act_count].uc_stack.ss_sp = stack;
    act_ctx[act_count].uc_stack.ss_size = sizeof(stack);
    act_ctx[act_count].uc_stack.ss_flags = 0;
    act_ctx[act_count].uc_link = &schedule;

    makecontext(&act_ctx[act_count],task_const[id].entry,0);

    // basic set for new task
    ready_queue[ready_count].id = task_const[id].id;
    ready_queue[ready_count].entry = task_const[id].entry;
    ready_queue[ready_count].static_p = task_const[id].static_priority;
    ready_queue[ready_count].current_p = task_const[id].static_priority;
    ready_queue[ready_count].state = READY;
    ready_queue[ready_count].time = time_count;
    ready_queue[ready_count].task = &act_ctx[act_count];
    for(int j=0; j<RESOURCES_COUNT; j++)
        ready_queue[ready_count].res[j]=0;

    act_count++;
    ready_count++;

    //get the RUNNING task
    getcontext(running_task.task);

    task_type running_before = running_task.id;
    preempt();

    // if after comparing , different
    if(running_before!=running_task.id)
        setcontext(running_task.task);

    return result;
}

status_type terminate_task(void)
{
    // default set
    status_type result = STATUS_OK;
    // if hold any task and not release
    for(int i=0; i<RESOURCES_COUNT; i++)
    {
        if(running_task.res[i]==1)
            return STATUS_ERROR;
    }

    // for which task can be terminated
    running_task.state = SUSPENDED;
    //find the highest task from ready queue
    scheduler();

    return result;
}

// if  RUNNING task -> SUSPEND , choose the highest priority task in READY be RUNNING
void scheduler()
{
    int max=0;
    for(int i=0; i<ready_count; i++)
    {
        // find the highest priority in ready queue
        // if same , find which task arrive early
        if( (ready_queue[i].current_p>ready_queue[max].current_p) ||
                (ready_queue[i].current_p==ready_queue[max].current_p &&
                 ready_queue[i].time <= ready_queue[max].time             ) )
        {
            max=i;
        }
    }

    // after finding the max , set running
    running_task.id = ready_queue[max].id;
    running_task.task = ready_queue[max].task;
    running_task.entry = ready_queue[max].entry;
    running_task.static_p = ready_queue[max].static_p;
    running_task.current_p = ready_queue[max].current_p;
    running_task.time = ready_queue[max].time;
    running_task.state = RUNNING;
    for(int j=0; j<RESOURCES_COUNT; j++)
        running_task.res[j] = ready_queue[max].res[j];

    // shift left the queue after the max task
    for(int k=max; k<ready_count; k++)
    {
        ready_queue[k].id = ready_queue[k+1].id;
        ready_queue[k].task = ready_queue[k+1].task;
        ready_queue[k].entry = ready_queue[k+1].entry;
        ready_queue[k].static_p = ready_queue[k+1].static_p;
        ready_queue[k].current_p = ready_queue[k+1].current_p;
        ready_queue[k].time = ready_queue[k+1].time;
        ready_queue[k].state = READY;
        for(int j=0; j<RESOURCES_COUNT; j++)
            ready_queue[k].res[j] = ready_queue[k+1].res[j];
    }
    ready_count--;
    setcontext(running_task.task);
    return;
}

void preempt()
{
    // SUSPENDED
    if(running_task.state==SUSPENDED)
    {
        scheduler();
        return;
    }
    else
    {
        int max_task=0;
        //find the highest priority task
        for(int i=0; i<ready_count; i++)
        {
            if ( (ready_queue[i].current_p>ready_queue[max_task].current_p) ||
                    ((ready_queue[i].current_p==ready_queue[max_task].current_p)&&
                     (ready_queue[i].time<ready_queue[max_task].time)))
            {
                max_task=i;
            }
        }

        // if running task is highest , back to caller
        if( (running_task.current_p>ready_queue[max_task].current_p) ||
                ((running_task.current_p==ready_queue[max_task].current_p)&&
                 (running_task.time<=ready_queue[max_task].time)))
        {
            return;
        }

        // some tasks' priority in READY > RUNNING's , swap them
        else
        {
            // temp = ready max / ready shift left / ready later =  run / run = temp

            temp.id = ready_queue[max_task].id;
            temp.task = ready_queue[max_task].task;
            temp.entry = ready_queue[max_task].entry;
            temp.static_p = ready_queue[max_task].static_p;
            temp.current_p = ready_queue[max_task].current_p;
            temp.time = ready_queue[max_task].time;
            temp.state = RUNNING;
            for(int j=0; j<RESOURCES_COUNT; j++)
                temp.res[j] = ready_queue[max_task].res[j];

            ready_count--;

            // shift left
            for(int k=max_task; k<ready_count; k++)
            {
                ready_queue[k].id = ready_queue[k+1].id;
                ready_queue[k].task = ready_queue[k+1].task;
                ready_queue[k].entry = ready_queue[k+1].entry;
                ready_queue[k].static_p = ready_queue[k+1].static_p;
                ready_queue[k].current_p = ready_queue[k+1].current_p;
                ready_queue[k].time = ready_queue[k+1].time;
                ready_queue[k].state = READY;
                for(int j=0; j<RESOURCES_COUNT; j++)
                    ready_queue[k].res[j]=ready_queue[k+1].res[j];
            }

            ready_queue[ready_count].id = running_task.id;
            ready_queue[ready_count].task = running_task.task;
            ready_queue[ready_count].entry = running_task.entry;
            ready_queue[ready_count].static_p = running_task.static_p;
            ready_queue[ready_count].current_p = running_task.current_p;
            ready_queue[ready_count].time = running_task.time;
            ready_queue[ready_count].state = READY;
            for(int j=0; j<RESOURCES_COUNT; j++)
                ready_queue[ready_count].res[j] = running_task.res[j];

            ready_count++;

            running_task.id = temp.id;
            running_task.task = temp.task;
            running_task.entry = temp.entry;
            running_task.static_p = temp.static_p;
            running_task.current_p = temp.current_p;
            running_task.time = temp.time;
            running_task.state = READY;
            for(int j=0; j<RESOURCES_COUNT; j++)
                running_task.res[j] = temp.res[j];

            return;
        }
    }
}