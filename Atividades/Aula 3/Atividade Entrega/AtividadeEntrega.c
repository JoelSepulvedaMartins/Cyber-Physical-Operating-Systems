/*
Joel Sepulveda Martins Turma da Noite BC
TURMA U
*/
#include <stdlib.h>
#include <time.h>
/*Free Rtos*/
#include "FreeRTOS.h"
#include "basic_io.h"
#include "task.h"
void TaskPrin(void *pvParameters);
void BatReg(void *pvParameters);
void OxiSat(void *pvParameters);
void MoniTemp(void *pvParameters);
void BatReg(void *pvParameters) {
char outp[45];
char *msg = (char *)pvParameters;
// Defina a faixa desejada.
int batimento;
int minBatimento = 50;
int maxBatimento = 140;
for (;;) {
int batimento = minBatimento + rand() % (maxBatimento - minBatimento + 1);
sprintf(outp, "Batimento Do Paciente: %d\n", batimento);
vPrintString(outp);
vPrintString("\n");
vPrintString("\n");
vPrintString("\n");
if (batimento < 50) {
vPrintString("!!!Low heartbeat!!!");
vPrintString("\n");
}
if (batimento > 90) {
vPrintString("!!!High Heartbeat!!!");
vPrintString("\n");
}
vTaskDelay(500);
}
vTaskDelete(NULL);
}
void TaskPrin(void *pvParameters) {
char *mchar = (char *)pvParameters;
for (;;) {
vPrintString(mchar);
vTaskDelay(500);
}
vTaskDelete(NULL);
}
/*fUNCIONANDO ATE AQ*/
void OxiSat(void *pvParameters) {
char *mchar = (char *)pvParameters;
int OX;
char outp[45];
for (;;) {
OX = ((int)rand() * (80 - 100)) / (int)RAND_MAX + 100;
sprintf(outp, "%d", OX);
vPrintString("Starting Monitoring...");
vPrintString("\n");
vPrintString("Saturation: ");
vPrintString(outp);
vPrintString(" %");
vPrintString("...\n");
if (OX < 90) {
vPrintString("!!!!Low saturation!!!!");
vPrintString("...\n");
}
vPrintString("\n");
vTaskDelay(500);
vPrintString("\n");
}
vTaskDelete(NULL);
}
/*Consegui implementar a func de oxi*/
/*Monitorar*/
void MoniTemp(void *pvParameters) {
char *mchar = (char *)pvParameters;
double temp;
char outp[45];
for (;;) {
temp = ((double)rand() * (34.0 - 41.0)) / (double)RAND_MAX + 41.0;
sprintf(outp, "%.2f", temp);
vPrintString("temperature: \n");
vPrintString("...");
vPrintString(outp);
vPrintString(" - Degrees Celsius.");
vPrintString("\n");
vPrintString("...\n");
if (35.00 > temp) {
vPrintString("\n");
vPrintString("!!!!Low Temperature!! Hypothermia detected!!!!");
vPrintString("\n");
}
if (37.50 < temp) {
vPrintString("\n");
vPrintString("!!! High Temperature Fever detected !!!\n");
vPrintString("\n");
}
vPrintString("\n");
vTaskDelay(500);
}
vTaskDelete(NULL);
}
/* Monitoramento de Temperatura implementado, verificar se da erro nos doubles,
* e otimizar o calculo de gerar numero aleatorio*/
/*Funcao main principal provisoria*/
int main_(void) {
xTaskCreate(BatReg, "Gerar batimento", 1000, NULL, 1, NULL);
xTaskCreate(OxiSat, "Oxygen Saturation", 1000, NULL, 1, NULL);
xTaskCreate(MoniTemp, "Temperature Monitoring", 1000, NULL, 1, NULL);
vTaskStartScheduler();
for (;;)
;
return 0;
}