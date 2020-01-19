#include "resource.h"
#include "task.h"
#include "ucontext.h"
#ifdef _LP64
#define SIZE 2097152+16384
#else
#define SIZE 16384
#endif

status_type get_resource(resource_type id)
{
    // default
    status_type result = STATUS_OK;

    if(running_task.res[id]==1)
        return STATUS_ERROR;

    for(int i=0; i<ready_count; i++)
    {
        if(ready_queue[i].res[id]==1)
            return STATUS_ERROR;
    }

    // give resource for the task
    running_task.res[id]=1;
    // priority ceiling
    if(running_task.current_p<resources_priority[id])
        running_task.current_p = resources_priority[id];

    return result;
}

status_type release_resource(resource_type id)
{
    // default set
    status_type result = STATUS_OK;

    // if the task don't hold the resource, it can't release anything
    if(running_task.res[id]==0)
        return STATUS_ERROR;

    // if the task has resource
    else
    {
        running_task.res[id]=0;
        // solution for priority inversion
        task_priority_type max = running_task.static_p;
        for(int i=0; i<RESOURCES_COUNT; i++)
        {
            if(running_task.res[i]==1 && max<resources_priority[i])
                max=resources_priority[i];
        }

        running_task.current_p = max;

        getcontext(running_task.task);

        task_type running_before = running_task.id;

        preempt();

        if(running_before!=running_task.id)
            setcontext(running_task.task);

        return result;
    }
}