#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "in_out.h"

QueueHandle_t infoQueue;
SemaphoreHandle_t xSemaforoRespiracion;
ushort tiempo_respiracion;
TickType_t tick_inicio_respiracion;
ushort nivel_limite_presion;
#define CHANCE_EXCEDER_TIEMPO 10 // Porcentaje: puede cambiarse para testear
#define CHANCE_LIMITE_PRESION 35 // Porcentaje: puede cambiarse para testear

static void hardwAlarma(char *input){
	printf("\033[1;31m");
	printf("ALARMA!> ");
	printf("%s\n",input);
	printf("\033[0m");
}

void vImprimirInfo (void *p){
	char texto_imprimir[80];

	for(;;){
		if(infoQueue != 0){
			if (xQueueReceive(infoQueue, (void*) texto_imprimir, (TickType_t) 0)){
				printf("\033[1;36m");
				printf("DISPLAY> ");
				printf("%s\n",texto_imprimir);
				printf("\033[0m");
			}
		}
	}
}

void vImprimirAlarma (void *p){
	uint32_t ulNotifiedValue;
	for(;;){
		if(xTaskNotifyWait(0xFFFFFFFF, 0, &ulNotifiedValue, portMAX_DELAY) == pdTRUE){
			if(ulNotifiedValue & (1<<0)){
				continue;
			}
			else if(ulNotifiedValue & (1<<1)){
				hardwAlarma("TIEMPO RESP. EXCEDIDO");
			}
			else if(ulNotifiedValue & (1<<2)){
				hardwAlarma("PRESION NIVEL 2");
			}
		}
	}
}

void vRealizarRespiracion (void *p){
	tick_inicio_respiracion = xTaskGetTickCount();
	nivel_limite_presion = 0;
	srand(time(NULL));
	for(;;){
	    if( xTaskGetTickCount() >= tick_inicio_respiracion + pdMS_TO_TICKS(tiempo_respiracion) )
	    {

	    	double chance = (double)rand() / RAND_MAX;
		    if (chance <(double)CHANCE_EXCEDER_TIEMPO/100){
		    	tick_inicio_respiracion += pdMS_TO_TICKS(tiempo_respiracion); // Chances de que se exceda del tiempo pedido
		    }else{
		    	chance = (double)rand() / RAND_MAX;
				if (chance <(double)CHANCE_LIMITE_PRESION/100){
					nivel_limite_presion = rand() % 3; // Chances de cambiar el nivel de presion actual
				}
		        xSemaphoreGive(xSemaforoRespiracion);

		        tick_inicio_respiracion = xTaskGetTickCount();
		    }
	    }
	    vTaskDelay(1);
	}
}


