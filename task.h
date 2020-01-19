#ifndef TASK_H
#define TASK_H

#include "typedefine.h"
#include "config.h"
#include <ucontext.h>

status_type activate_task(task_type id);
status_type terminate_task(void);

void preempt(void);
void scheduler(void);

int time_count;
int ready_count;

ucontext_t schedule;
ucontext_t auto_ctx[AUTO_START_TASKS_COUNT];
ucontext_t act_ctx[TASKS_COUNT];

typedef struct
{
    ucontext_t *task;
    int id;
    int time;
    int static_p;
    int current_p;
    int res[RESOURCES_COUNT];
    entry_point_type entry;
    task_state_type state;
} node;

node temp;
node running_task;
node suspend_task;
node ready_queue[TASKS_COUNT];

#endif /* TASK_H */