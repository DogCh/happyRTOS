#ifndef _HAPPY_OS_H
#define _HAPPY_OS_H

#include <stdio.h>
#include <stdint.h>
#include <ARMCM3.h>

typedef uint32_t hTask_data_type;

typedef struct _task_stack_t {

		hTask_data_type *stack;
	
}task_stack_t;


extern task_stack_t *current_task_ptr;
extern task_stack_t *next_task_ptr;
extern task_stack_t *task_table[2];

void hTaskStart(void);

void hTaskYield(void);
void systemTick_Init(uint32_t ms);
#endif
