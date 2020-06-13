#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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
TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
QueueHandle_t myQueue;


void myTask1 (void *p){
	char myTxBuff[30];

	myQueue = xQueueCreate(5,sizeof(myTxBuff));

	sprintf(myTxBuff, "message 1");
	xQueueSend(myQueue, (void*) myTxBuff, (TickType_t) 0);

	sprintf(myTxBuff, "message 2");
	xQueueSend(myQueue, (void*) myTxBuff, (TickType_t) 0);

	sprintf(myTxBuff, "message 3");
	xQueueSend(myQueue, (void*) myTxBuff, (TickType_t) 0);


	printf("mensajes en cola: %d\r\n", uxQueueMessagesWaiting(myQueue));
	printf("espacios libres: %d\r\n", uxQueueSpacesAvailable(myQueue));
	for(;;){


	}
}

void myTask2 (void *p){
	char myRxBuff[30];
	for(;;){

		if(myQueue != 0){
			if (xQueueReceive(myQueue, (void*) myRxBuff, (TickType_t) 5)){
				printf("recibido: %s\r\n", myRxBuff);
			}

		}
	}
}

int main(void){
	xTaskCreate(myTask1, "task1", 200,(void *) 0, tskIDLE_PRIORITY, &myTask1Handle);
	xTaskCreate(myTask2, "task2", 200,(void *) 0, tskIDLE_PRIORITY, &myTask2Handle);

	vTaskStartScheduler();

	for(;;);
}
