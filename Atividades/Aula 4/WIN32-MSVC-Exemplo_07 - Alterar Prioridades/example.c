
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"

#define DELAY_LOOP_COUNT		( 0xffffff )
xTaskHandle taskHandle1;
xTaskHandle taskHandle2;

void vTask(void *pvParameters);

void vTask(void *pvParameters)
{
	char* msg = (char*)pvParameters;
	volatile unsigned long i;
	volatile int cont = 0;
	for (;; )
	{
		printf("msg %s %d\n",msg,cont);

		cont = cont + 1;

		// Alterando a prioridade quando a condição for válida
		if (cont == 100) {

			vTaskPrioritySet(taskHandle1, 2);
			vTaskPrioritySet(taskHandle2, 1);
		}

		for (i = 0; i < DELAY_LOOP_COUNT; i++) {

		
		}
	}

	vTaskDelete(NULL);
}

int main_(void)
{
	static const char* msg1 = "Tarefa - Instancia 1";
	static const char* msg2 = "Tarefa - Instancia 2";

	xTaskCreate(vTask, "Task 1", 1000, (void*) msg1, 1, &taskHandle1);
	xTaskCreate(vTask, "Task 2", 1000, (void*) msg2, 2, &taskHandle2);

	vTaskStartScheduler();

	for (;; );
	return 0;
}
