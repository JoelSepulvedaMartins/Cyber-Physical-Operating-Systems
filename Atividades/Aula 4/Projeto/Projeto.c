
//Joel Sepulveda
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "basic_io.h"
#include <time.h>
#include <stdlib.h>
// Definindo o prot tipo das tarefas�
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask4(void* pvParameters);
char msg[35] = { '\0' };
const double maxTemp = 30.0;
struct {
int dia;
int mes;
int ano;
int hora;
int minutos;
int segundos;
double temperatura;
} display;
// Criando a vari vel para o sem foro� �
xSemaphoreHandle xSemaphore = NULL;
int main_(void)
{
// Fun o para criar um sem foro bin rio�� � �
xSemaphore = xSemaphoreCreateMutex();
// Criando duas tarefas
xTaskCreate(vTask1, "Atualiza a Data ", 1000, (void *)1, 1, NULL);
xTaskCreate(vTask2, "Atualiza a hora", 1000, (void*)2, 1, NULL);
//xTaskCreate(vTask3, "Atualzia a temperatura", 1000, (void*)3, 1,
NULL);
xTaskCreate(vTask4, "Atualzia a temperatura", 1000, (void*)3, 1, NULL);
// Inicializa o escalonador
vTaskStartScheduler();
for (;; );
return 0;
}
void vTask1(void* pvParameters)
{
// Imprime a Data atual obtida no display
for (;; )
{
if (xSemaphore != NULL) {
if (xSemaphoreTake(xSemaphore, (portTickType)10) == pdTRUE) {
time_t generic_time;
time(&generic_time);
struct tm* tmp = localtime(&generic_time);
display.dia = tmp->tm_mday;
display.mes = tmp->tm_mon;
display.ano = tmp->tm_year;
const int idTask = (int)pvParameters;
char msg[] = { '\0' };
sprintf(msg, "Task %ld - Data Atual: %d/%d/%d\n", idTask,
display.dia, display.mes, display.ano);
vPrintString(msg);
xSemaphoreGive(xSemaphore);
}
}
else
{
printf("O sem foro n o foi criado\n");� �
}
vTaskDelay(portTICK_RATE_MS * 100);
}
vTaskDelete(NULL);
}
void vTask4(void* pvParameters)
{
// Imprime a Data atual obtida no display
for (;; )
{
if (xSemaphore != NULL) {
if (xSemaphoreTake(xSemaphore, (portTickType)10) == pdTRUE) {
display.temperatura = ((double)rand() / (double)(RAND_MAX))
* maxTemp;
const long idTask = (int)pvParameters;
sprintf(msg, "Task %d -> Curitiba %.2f C\n", idTask,
display.temperatura);
vPrintString(msg);
xSemaphoreGive(xSemaphore);
}
}
else
{
printf("O sem foro n o foi criado\n");� �
}
vTaskDelay(portTICK_RATE_MS * 100);
}
vTaskDelete(NULL);
}
void vTask2(void* pvParameters)
{
// Imprime a hora atual no display
for (;; )
{
if (xSemaphore != NULL) {
if (xSemaphoreTake(xSemaphore, (portTickType)10) == pdTRUE) {
time_t generic_time;
time(&generic_time);
char msg[] = { '\0' };
struct tm* tmp = localtime(&generic_time);
display.hora = tmp->tm_hour;
display.minutos = tmp->tm_min;
display.segundos = tmp->tm_sec;
const int idTask = (int)pvParameters;
sprintf(msg, "Task %ld - Hora atual: %d:%d:%d\n", idTask,
display.hora, display.minutos, display.segundos);
vPrintString(msg);
xSemaphoreGive(xSemaphore);
}
}
else
{
// imprime a mensagem caso o sem foro n o tenha sido criado� �
printf("O sem foro n o foi criado\n");� �
}
// Efetua um delay na tarefa a cada 100 ticks
vTaskDelay(portTICK_RATE_MS * 100);
}
// A fun o vTaskDelete() permite liberar explicitamente a tarefa��
vTaskDelete(NULL);
}