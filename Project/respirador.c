#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "in_out.h"

SemaphoreHandle_t xSemaforoNotificacion;
TaskHandle_t myTask2Handle = NULL;
/*--------------------------------------------------------------------------------*/
/*
 * Prototypes for the standard FreeRTOS callback/hook functions implemented
 * within this file.
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);

void vEnviarInfo(void *p);
void vNotificarAlarma(void *p);
void vManejarRespiracion(void *p);

void vApplicationMallocFailedHook(void) {
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
	vAssertCalled( __LINE__, __FILE__);
}

void vAssertCalled(unsigned long ulLine, const char *const pcFileName) {
	taskENTER_CRITICAL();
	{
		printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
		fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}

void vApplicationIdleHook(void) {
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	 task.  It is essential that code added to this hook function never attempts
	 to block in any way (for example, call xQueueReceive() with a block time
	 specified, or call vTaskDelay()).  If the application makes use of the
	 vTaskDelete() API function (as this demo application does) then it is also
	 important that vApplicationIdleHook() is permitted to return to its calling
	 function, because it is the responsibility of the idle task to clean up
	 memory allocated by the kernel to any task that has since been deleted. */

	//metodoALlamarEnIdle();
}
/*--------------------------------------------------------------------------------*/

void vEnviarInfo(void *p) {
	char buffer[500];
	for (;;) {
		sprintf(buffer, "niv pres %d | prox t %d | curr t %d",
				nivel_limite_presion,
				tick_inicio_respiracion + pdMS_TO_TICKS(tiempo_respiracion),
				xTaskGetTickCount());
		xQueueSend(p, (void* ) buffer, (TickType_t ) 0);
		vTaskDelay(500);
	}
	vTaskDelete( NULL);
}

void vNotificarAlarma(void *p) {
	// ... COMPLETAR
	TickType_t real_inicio_respiracion = xTaskGetTickCount();
	TickType_t respiracion;
	for (;;) {
		xSemaphoreTake(xSemaforoNotificacion, 2000);
		respiracion = xTaskGetTickCount() - real_inicio_respiracion;
		printf("Respiracion %d\n", respiracion);
		if (respiracion > 1200) {
			xTaskNotify(myTask2Handle, (1 << 1), eSetBits);
		}
		if (nivel_limite_presion == 2) {
			xTaskNotify(myTask2Handle, (1 << 2), eSetBits);
		}
		real_inicio_respiracion = xTaskGetTickCount();
		vTaskDelay(1);
	}
	vTaskDelete( NULL);
}

void vManejarRespiracion(void *p) {
	// ... COMPLETAR
	for (;;) {
		xSemaphoreTake(xSemaforoRespiracion, 2000);
		if (nivel_limite_presion == 0) {
			tiempo_respiracion = 1000;
		} else if (nivel_limite_presion == 1) {
			tiempo_respiracion = 700;
		} else {
			tiempo_respiracion = 500;
		}
		printf("Corrijo %d %d\n", nivel_limite_presion, tiempo_respiracion);
		xSemaphoreGive(xSemaforoNotificacion);
		vTaskDelay(1);
	}
	vTaskDelete( NULL);
}

int main(void) {
// ... COMPLETAR
	char texto_display[500];
	infoQueue = xQueueCreate(5, sizeof(texto_display));

	xTaskCreate(vRealizarRespiracion, "realizarRespiracion", 200, NULL,
	tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(vImprimirInfo, "imprimirInfo", 200, NULL, tskIDLE_PRIORITY + 2,
	NULL);
	xTaskCreate(vImprimirAlarma, "imprimirAlarma", 200, NULL,
	tskIDLE_PRIORITY + 3, &myTask2Handle);

	xTaskCreate(vEnviarInfo, "enviarInfo", 200, infoQueue, tskIDLE_PRIORITY + 4,
	NULL);
	xTaskCreate(vManejarRespiracion, "manejarRespiracion", 200, infoQueue,
	tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(vNotificarAlarma, "notificarAlarma", 200, infoQueue,
	tskIDLE_PRIORITY + 2, NULL);

	// Valores iniciales

	sprintf(texto_display, "INICIANDO SISTEMA");
	xSemaforoRespiracion = xSemaphoreCreateCounting(200, 0);
	xSemaforoNotificacion = xSemaphoreCreateCounting(200, 0);
	tiempo_respiracion = 1000;

	xQueueSend(infoQueue, (void* ) texto_display, (TickType_t ) 0);
	vTaskStartScheduler();

	for (;;)
		;
}
