/*
Alunos:
Joel Sepulveda

*/
#include "time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include "semphr.h"
void Arfagem(void *pvParameters);
void Rolagem(void *pvParameters);
void Guinada(void *pvParameters);
void Menu();
//Variaveis do motor
int rotorSpeed[4] = { 100, 100, 100, 100 };
int rotorDirection[4] = { 0, 0, 0, 0 };
int countAux = 0;
int count = 0;
int direction = -1;
int deletTaskA = 0;
int deletTaskR = 0;
int deletTaskG = 0;
byte drone_ligado;
struct {
double motor1; // dianteiro esquerdo
double motor2; // dianteiro direito
double motor3; // traseiro direito
double motor4; // traseiro esquerdo
} drone;
int main_(void)
{
// Crie as tarefas
xTaskCreate(Arfagem, "Arfagem", 1000, NULL, 1, NULL);
xTaskCreate(Rolagem, "Rolagem", 1000, NULL, 1, NULL);
xTaskCreate(Guinada, "Guinada", 1000, NULL, 1, NULL);
xTaskCreate(Menu, "Menu", 1000, NULL, 1, NULL);
// Inicie o scheduler
vTaskStartScheduler();
for (;; );
return 0;
}
// Tarefa de Arfagem
void Arfagem(void *pvParameters)
{
/*
b) arfagem (pitch):
- mover para frente: aumentar a velocidade dos motores 2 e 3, diminuir a velocidade
dos motores 0 e 1,
- mover para tr s: diminuir a velocidade dos motores 2 e 3, aumentar a velocidade�
dos motores 0 e 1,
*/
int countTemp = 0;
for (;; )
{
// manobra de arfagem
//int direction = *((int*)pvParameters); // 1 para frente, -1 para tr s�
while (rotorDirection[2] != 0 && rotorDirection[3] != 0 && countTemp
<=2) {
printf("Arfagem:");
// Ajusta as velocidades dos motores
rotorSpeed[2] += 25 * direction;
rotorSpeed[3] += 25 * direction;
rotorSpeed[0] += 25 * direction * -1;
rotorSpeed[1] += 25 * direction * -1;
// Sa da no console�
printf("\n");
printf("Manobra de Arfagem: %s\n", direction > 0 ? "Para
Frente" : "Para Tras");
printf("Velocidades dos motores:\n %d %d\n %d %d\n",
rotorSpeed[0], rotorSpeed[1], rotorSpeed[2], rotorSpeed[3]);
printf("\n");
if (countTemp == 2) {//zera variaveis de controle
countTemp = 0;
countAux = 1;
rotorSpeed[2] = 100;
rotorSpeed[3] = 100;
rotorSpeed[0] = 100;
rotorSpeed[1] = 100;
rotorDirection[2] = 0;
rotorDirection[3] = 0;
}
countTemp += 1;
}
vTaskDelay(40); // Aguarde 40ms
}
}
// Tarefa de Rolagem
void Rolagem(void *pvParameters)
{
/*
c) rolagem (roll):
- virar direita: aumentar a velocidade dos motores 0 e 3, diminuir a velocidade�
dos motores 1 e 2,
- virar esquerda: diminuir a velocidade dos motores 0 e 3, aumentar a velocidade�
dos motores 1 e 2.
*/
int countTemp = 0;
for (;; )
{
if (deletTaskR == 1) { vTaskDelete(NULL); }
while (rotorDirection[0] != 0 && rotorDirection[3] != 0 && countTemp <=
2) {
// Simula a manobra de rolagem
printf("Rolagem:");
// Ajusta as velocidades dos motores
rotorSpeed[0] += 25 * direction;
rotorSpeed[3] += 25 * direction;
rotorSpeed[1] += 25 * direction * -1;
rotorSpeed[2] += 25 * direction * -1;
// Sa da no console�
printf("\n");
printf("Manobra de Rolagem: %s\n", direction > 0 ? "Direita" :
"Esquerda");
printf("Velocidades dos motores:\n %d %d\n %d %d\n",
rotorSpeed[0], rotorSpeed[1], rotorSpeed[2], rotorSpeed[3]);
printf("\n");
if (countTemp == 2) {//zera variaveis de controle
countTemp = 0;
countAux = 1;
rotorSpeed[0] = 100;
rotorSpeed[3] = 100;
rotorSpeed[1] = 100;
rotorSpeed[2] = 100;
rotorDirection[0] = 0;
rotorDirection[3] = 0;
}
countTemp += 1;
}
vTaskDelay(20); // Aguarde 20ms
}
}
//Tarefa de Guinada
void Guinada(void *pvParameters)
{
/*
a) guinada (yaw):
- sentido hor rio: aumentar a velocidade dos motores 0 e 2, diminuir a velocidade�
dos motores 1 e 3,
- sentido anti-hor rio: diminuir a velocidade dos motores 0 e 2, aumentar a�
velocidade dos motores 1 e 3,
*/
int countTemp = 0;
for (;; )
{
if (deletTaskG == 1) { vTaskDelete(NULL);}
//int direction = *((int*)pvParameters); // 1 para direita, -1 para
esquerda
while (rotorDirection[0] != 0 && rotorDirection[2] != 0 && countTemp <=
2) {
// Simula a manobra de rolagem
printf("Guinada:");
// Ajusta as velocidades dos motores
rotorSpeed[0] += 25 * direction;
rotorSpeed[2] += 25 * direction;
rotorSpeed[1] += 25 * direction * -1;
rotorSpeed[3] += 25 * direction * -1;
// Sa da no console�
printf("\n");
printf("Manobra de Guinada: %s\n", direction > 0 ? "Horario" :
"Anti-Horario");
printf("Velocidades dos motores:\n %d %d\n %d %d\n",
rotorSpeed[0], rotorSpeed[1], rotorSpeed[2], rotorSpeed[3]);
printf("\n");
if (countTemp == 2) {//zera variaveis de controle
countTemp = 0;
countAux = 1;
rotorSpeed[0] = 100;
rotorSpeed[2] = 100;
rotorSpeed[1] = 100;
rotorSpeed[3] = 100;
rotorDirection[0] = 0;
rotorDirection[3] = 0;
}
countTemp += 1;
}
vTaskDelay(10); // Aguarde 10ms
}
}
void MostrarMenu()
{
printf("\n");
printf("\n");
printf("\n");
printf("Controle do Quadric ptero\n");�
printf("1. Arfagem para frente\n");
printf("2. Arfagem para tr s\n");�
printf("3. Rolagem Sentido Direita\n");
printf("4. Rolagem Sentido Esquerda\n");
printf("5. Guinada Sentido Horario\n");
printf("6. Guinada Sentido AntHorario\n");
printf("Velocidades dos motores: %d %d %d %d\n", rotorSpeed[0],
rotorSpeed[1], rotorSpeed[2], rotorSpeed[3]);
printf("8. Sair\n");
printf("\n");
printf("\n");
}
void Menu(void *pvParameters)
{
drone_ligado = 1;
int escolha = 0;
while (drone_ligado) {
MostrarMenu();
printf("Escolha uma opcao: ");
scanf("%d", &escolha);
countAux = 0;
switch (escolha) {
case 1:
direction = 1;
rotorDirection[2] = 1;
rotorDirection[3] = 1;
printf("VRotor Direction: %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 2:
direction = -1;
rotorDirection[2] = 1;
rotorDirection[3] = 1;
printf("VRotor Direction: %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 3:
direction = 1;
rotorDirection[0] = 1;
rotorDirection[3] = 1;
printf("Rotor Direction : %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 4:
direction = -1;
rotorDirection[0] = 1;
rotorDirection[3] = 1;
printf("Rotor Direction : %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 5:
direction = 1;
rotorDirection[0] = 1;
rotorDirection[2] = 1;
printf("Rotor Direction : %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 6:
direction = -1;
rotorDirection[0] = 1;
rotorDirection[2] = 1;
printf("Rotor Direction : %d %d %d %d\n", rotorDirection[0],
rotorDirection[1], rotorDirection[2], rotorDirection[3]);
printf("\n");
while (countAux == 0) { vTaskDelay(100); }
break;
case 8:
printf("Desligando o Drone.\n");
drone_ligado = 0;
break;
default:
printf("Opcaoo invalida. Tente novamente.\n");
break;
}
}
vTaskDelete(xTaskGetHandle("Arfagem"));//Encerra Arfage
vTaskDelete(xTaskGetHandle("Rolagem"));//Encerra Rolagem
vTaskDelete(xTaskGetHandle("Guinada"));//Encerra Guinada
vTaskDelete(NULL); // Encerra esta tarefa
}