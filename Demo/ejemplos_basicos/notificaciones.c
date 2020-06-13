#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

/*
 * Prototypes for the standard FreeRTOS callback/hook functions implemented
 * within this file.
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	vAssertCalled( __LINE__, __FILE__ );
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */

		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		//vFullDemoIdleFunction();
}
/*-----------------------------------------------------------*/
TaskHandle_t myTask2Handle = NULL;


void myTask1 (void *p){
	for(;;){
		/*
		eNoAction,					Notify the task without updating its notify value.
		eSetBits,					Set bits in the task's notification value.
		eIncrement,					Increment the task's notification value.
		eSetValueWithOverwrite,		Set the task's notification value to a specific value even if the previous value has not yet been read by the task.
		eSetValueWithoutOverwrite	Set the task's notification value if the previous value has been read by the task.
		*/

		// https://en.wikipedia.org/wiki/Bitwise_operations_in_C#Shift_operators
		xTaskNotify(myTask2Handle, (1<<0), eSetBits); // Seteo valor de notificacion 1 (primer bit)
		vTaskDelay(pdMS_TO_TICKS(200));

		xTaskNotify(myTask2Handle, (1<<1), eSetBits); //2 (segundo bit)
		vTaskDelay(pdMS_TO_TICKS(200));

		xTaskNotify(myTask2Handle, (1<<2), eSetBits); //4 (tercer bit)
		vTaskDelay(pdMS_TO_TICKS(200));

	}
}

void myTask2 (void *p){
	uint32_t ulNotifiedValue;
	for(;;){
		if(xTaskNotifyWait(0xFFFFFFF, 0, &ulNotifiedValue, portMAX_DELAY) == pdTRUE){
			if(ulNotifiedValue & (1<<0)){
				printf("red\r\n");
			}
			else if(ulNotifiedValue & (1<<1)){
				printf("green\r\n");
			}
			else if(ulNotifiedValue & (1<<2)){
				printf("blue\r\n");
			}
		}
	}
}

int main(void){
	xTaskCreate(myTask1, "task1", 200,(void *) 0, tskIDLE_PRIORITY, NULL);
	xTaskCreate(myTask2, "task2", 200,(void *) 0, tskIDLE_PRIORITY, &myTask2Handle);

	vTaskStartScheduler();

	for(;;);
}
