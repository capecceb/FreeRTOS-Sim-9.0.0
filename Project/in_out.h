#ifndef IN_OUT_H
#define IN_OUT_H

extern QueueHandle_t infoQueue;
extern TickType_t tick_inicio_respiracion;
extern SemaphoreHandle_t xSemaforoRespiracion;
extern ushort tiempo_respiracion;
extern ushort nivel_limite_presion;
void vImprimirInfo (void *p);
void vImprimirAlarma (void *p);
void vRealizarRespiracion (void *p);
#endif /* IN_OUT_H */
