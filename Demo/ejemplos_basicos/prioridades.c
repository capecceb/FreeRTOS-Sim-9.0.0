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

void myTask1 (void *p){

	for(;;){
		printf("Ejecutando tarea 1\n");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void myTask2 (void *p){

	for(;;){
		printf("Ejecutando tarea 2\n");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void myTask3 (void *p){

	for(;;){
		printf("Ejecutando tarea 3\n");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void myTask4 (void *p){

	for(;;){
		printf("Ejecutando tarea 4\n");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int main(void){
	xTaskCreate(myTask1, "task1", 200, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(myTask2, "task2", 200, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(myTask3, "task3", 200, NULL, tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(myTask4, "task4", 200, NULL, configMAX_PRIORITIES-1, NULL);

	vTaskStartScheduler();

	for(;;);
}
