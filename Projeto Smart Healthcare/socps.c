#include "contiki.h"
#include <stdio.h>
#include <random.h>
//#include "sys/mutex.h"

// Definindo os processos principais
PROCESS(bpm_monitor, "BPM");
PROCESS(saturation_monitor, "Saturation");
PROCESS(temperature_monitor, "Temperature");
PROCESS(ALERT, "Alert");
AUTOSTART_PROCESSES(&bpm_monitor, &saturation_monitor, &temperature_monitor, &ALERT);

// Definindo as variáveis globais para BPM
int maxBpm = 140;
int minBpm = 20;

// Definindo as variáveis globais para saturação
int maxSaturation = 100;
int minSaturation = 80;

// Definindo as variáveis globais para temperatura
int maxTemperature = 41;
int minTemperature = 34;

// Sinalizador para controle de recursos compartilhados
static int alert_in_use = 0;
//static struct mutex alert_mutex;

// Processo de Monitoramento de BPM
PROCESS_THREAD(bpm_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3); // Configura um temporizador de 3 segundos

    // Gera um valor aleatório simulando leitura de BPM
    int bpmTemp = random_rand() % (maxBpm - minBpm) + minBpm;

    // Formata a mensagem e imprime no console
    sprintf(msg, "BPM: %d\n", bpmTemp);
    printf("%s", msg);

    // Verifica se o BPM está fora dos limites normais e envia alerta
    if (bpmTemp > 90)
    {
      sprintf(alertTemp, "Taquicardia: %d Bpm", bpmTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);
    }
    else if (bpmTemp < 50)
    {
      sprintf(alertTemp, "Bradicardia: %d Bpm", bpmTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);
    }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); // Aguarda até que o temporizador expire
  }

  PROCESS_END();
}

// Processo de Monitoramento de Saturação
PROCESS_THREAD(saturation_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3); // Configura um temporizador de 3 segundos

    // Gera um valor aleatório simulando leitura de saturação
    int saturationTemp = random_rand() % (maxSaturation - minSaturation) + minSaturation;

    // Formata a mensagem e imprime no console
    sprintf(msg, "Saturation: %d\n", saturationTemp);
    printf("%s", msg);

    // Verifica se a saturação está abaixo do normal e envia alerta
    if (saturationTemp < 90)
    {
      sprintf(alertTemp, "Saturacao Baixa: %d Percent ", saturationTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);
    }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); // Aguarda até que o temporizador expire
  }

  PROCESS_END();
}

// Processo de Monitoramento de Temperatura
PROCESS_THREAD(temperature_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3); // Configura um temporizador de 3 segundos

    // Gera um valor aleatório simulando leitura de temperatura
    int temperatureTemp = random_rand() % (maxTemperature - minTemperature) + minTemperature;

    // Formata a mensagem e imprime no console
    sprintf(msg, "Temperature: %d\n", temperatureTemp);
    printf("%s", msg);

    // Verifica se a temperatura está fora dos limites normais e envia alerta
    if (temperatureTemp < 35)
    {
      sprintf(alertTemp, "Hipotermia: %d Celsius", temperatureTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);
    }
    else if (temperatureTemp > 37)
    {
      sprintf(alertTemp, "Febre: %d Celsius", temperatureTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);
    }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); // Aguarda até que o temporizador expire
  }

  PROCESS_END();
}

// Processo de Alerta
PROCESS_THREAD(ALERT, ev, data)
{
  PROCESS_BEGIN();

  while (1)
  {
    PROCESS_WAIT_EVENT(); // Aguarda por eventos

    if (ev == PROCESS_EVENT_MSG) // Se o evento for uma mensagem
    {
          // mutex_lock(&alert_mutex); // Bloqueia o mutex

      if (!alert_in_use)
      {
        alert_in_use = 1; // Sinaliza que o recurso está em uso
        printf("!!!!!!!!!Alert Problema (Niveis Anormais) !!!!!!!!! =-=-=-=- %s -=-=-=-=\n", (char *)data);
        alert_in_use = 0; // Libera o recurso
      }
            //mutex_unlock(&alert_mutex);  // Libera o mutex

    }
  }

  PROCESS_END();
}
