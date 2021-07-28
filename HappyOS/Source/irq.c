#include "HappyOS.h"


#define portNVIC_INT_CTRL_REG				( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t *) 0xe000ed20 ) )

#define portMIN_INTERRUPT_PRIORITY	( 255UL )
#define portNVIC_PENDSV_PRI			( portMIN_INTERRUPT_PRIORITY << 16UL )
#define portNVIC_PENDSVSET			0x10000000


void hTaskStart( void )
{
	__set_PSP(0);
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;//ICSR 0xE000_ED04 
	/* Set a PendSV to request a context switch. */
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET;

	/* Barriers are normally not required but do ensure the code is completely
	within the specified behaviour for the architecture. */
	__DSB();
	__ISB();
}

void hTaskYield( void )
{
	if(current_task_ptr == task_table[0])
	{
		next_task_ptr = task_table[1];
	}
	else
	{
		next_task_ptr = task_table[0];
	}
	
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET;
}

__asm void PendSV_Handler(void)
{
	IMPORT current_task_ptr
	IMPORT next_task_ptr
	
	MRS R0,PSP
	CBZ R0,PendSV_Handler_no_save_lable
	STMDB R0!,{R4-R11}
	LDR R1, =current_task_ptr	
	LDR R1,[R1]
	STR R0,[R1]
	
PendSV_Handler_no_save_lable
	LDR R0, =current_task_ptr	
	LDR R1, = next_task_ptr
	LDR R1,[R1]
	STR R1,[R0]
	
	LDR R0,[R1]
	LDMIA R0!,{R4-R11}
	
	MSR PSP,R0
		
	ORR LR,#0x04
	BX LR
	
}
