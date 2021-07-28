#include <stdio.h>
#include <stdint.h>
#include "HappyOS.h"

void delay(int count);
void delay(int count)
{
	while(--count > 0);	
}

/***********************************task Init************************************************/
task_stack_t *current_task_ptr;
task_stack_t *next_task_ptr;
task_stack_t *task_table[2];


void hTask_Init(task_stack_t *stack,void (*entery)(void *param),void *param,hTask_data_type *task_env)
{
	/*硬件自动保存*/
	*(--task_env) = (hTask_data_type )1<<24;//xPSR T必须为1
	*(--task_env) = (hTask_data_type )entery;//R15 PC 
	*(--task_env) = (hTask_data_type )0x14;//R14 LR
	*(--task_env) = (hTask_data_type )0x12;//R12
	*(--task_env) = (hTask_data_type )0x03;//R3
	*(--task_env) = (hTask_data_type )0x02;//R2
	*(--task_env) = (hTask_data_type )0x01;//R1
	*(--task_env) = (hTask_data_type )param;//R0
	
	/*手动保存 R4 - R11*/
	*(--task_env) = (hTask_data_type )0x11;//R11
	*(--task_env) = (hTask_data_type )0x10;//R10
	*(--task_env) = (hTask_data_type )0x09;//R9
	*(--task_env) = (hTask_data_type )0x08;//R8
	*(--task_env) = (hTask_data_type )0x07;//R7
	*(--task_env) = (hTask_data_type )0x06;//R6
	*(--task_env) = (hTask_data_type )0x05;//R5
	*(--task_env) = (hTask_data_type )0x04;//R4

	stack->stack = task_env;
}

/***********************************task 1************************************************/
task_stack_t hTask1;
hTask_data_type hTask1_Env[1024];
uint8_t task1_flag = 0;
void task1Entery (void *param)
{
	for(;;)
	{
		task1_flag = 0;
		delay(1000);
		task1_flag = 1;
		delay(1000);
		hTaskYield();
	}
}
/***********************************task 2************************************************/
task_stack_t hTask2;
hTask_data_type hTask2_Env[1024];
uint8_t task2_flag = 0;
void task2Entery (void *param)
{
	for(;;)
	{
		task2_flag = 0;
		delay(1000);
		task2_flag = 1;
		delay(1000);
		hTaskYield();
	}
}

/***********************************main************************************************/
int main(void)
{
	hTask_Init(&hTask1,task1Entery,(hTask_data_type *)0x11111111,&hTask1_Env[1023]);
	hTask_Init(&hTask2,task2Entery,(hTask_data_type *)0x22222222,&hTask2_Env[1023]);
	
	task_table[0] = &hTask1;
	task_table[1] = &hTask2;
	
	next_task_ptr = task_table[0];
	
	hTaskStart();
	
	return 0;
}
