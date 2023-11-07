
#include "time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include "semphr.h"

void vTask1(void *pvParameters);
void vTask2(void* pvParameters);
void vTask3(void* pvParameters);

xSemaphoreHandle xSemaphore;

char display[100];

int main_(void)
{

	xSemaphore = xSemaphoreCreateMutex();

	xTaskCreate(vTask1, "Task 1", 1000,(void*) 1, 1, NULL);
	xTaskCreate(vTask2, "Task 2", 1000,(void*) 2, 1, NULL);
	xTaskCreate(vTask3, "Task 3", 1000,(void*) 3, 1, NULL);

	vTaskStartScheduler();

	for (;; );
	return 0;
}

void vTask1(void* pvParameters)
{

	const int idTask = (int)pvParameters;

	for (;; )
	{
		if (xSemaphoreTake(xSemaphore, (TickType_t)10)) {

			time_t segundos = time(&segundos);
			struct tm* datetimeNow = localtime(&segundos);

			int dia = datetimeNow->tm_mday;
			int mes = datetimeNow->tm_mon + 1;
			int ano = datetimeNow->tm_year + 1900;

			sprintf(display, "Task %d -> Data: %d / %d / %d.\n", idTask, dia, mes, ano);

			vPrintString(display);

			xSemaphoreGive(xSemaphore);
		}
		else {

			char espera[100];
			sprintf("Task %d - Esperando display ser liberado...", idTask);

			vPrintString(espera);

		}

		vTaskDelay(1000);

	}

	vTaskDelete(NULL);
}

void vTask2(void* pvParameters)
{

	const int idTask = (int)pvParameters;

	for (;; )
	{
		if (xSemaphoreTake(xSemaphore, (TickType_t)10)) {
		
			time_t segundos = time(&segundos);
			struct tm* datetimeNow = localtime(&segundos);

			int hora = datetimeNow->tm_hour;
			int min = datetimeNow->tm_min;
			int seg = datetimeNow->tm_sec;

			sprintf(display, "Task %d -> Horario: %d:%d:%d.\n", idTask, hora, min, seg);

			vPrintString(display);

			xSemaphoreGive(xSemaphore);
		}
		else {
			
			char espera[100];
			sprintf("Task %d - Esperando display ser liberado...", idTask);

			vPrintString(espera);

		}

		vTaskDelay(1000);

	}

	vTaskDelete(NULL);
}

void vTask3(void* pvParameters)
{

	const double maxTemp = 40.0;
	const int idTask = (int)pvParameters;

	for (;; )
	{
		if (xSemaphoreTake(xSemaphore, (TickType_t)10)) {
				
			float temp = ((float)rand() / (float)(RAND_MAX)) * maxTemp;

			sprintf(display, "Task %d -> Curitiba  %.2f °C\n", idTask, temp);

			vPrintString(display);

			xSemaphoreGive(xSemaphore);

		}
		else {

			char espera[100];
			sprintf("Task %d - Esperando display ser liberado...", idTask);

			vPrintString(espera);

		}

		vTaskDelay(1000);

	}

	vTaskDelete(NULL);
}
