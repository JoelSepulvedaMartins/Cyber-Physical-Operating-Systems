#include "contiki.h"
#include <stdio.h>
#include <random.h>
//#include "sys/mutex.h"

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

//declarando o mutex 
//static struct mutex alert_mutex;

PROCESS_THREAD(bpm_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);

    int bpmTemp = random_rand() % (maxBpm - minBpm) + minBpm;

    
    sprintf(msg, "BPM: %d\n", bpmTemp);
    printf("%s", msg);


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
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}





PROCESS_THREAD(saturation_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);
 
    

    int saturationTemp = random_rand() % (maxSaturation - minSaturation) + minSaturation;

    
    sprintf(msg, "Saturation: %d\n", saturationTemp);
    printf("%s", msg);

    if (saturationTemp < 90)
    {
      
      sprintf(alertTemp, "Saturacao Baixa: %d Percent ", saturationTemp);
      process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);


    }
    

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}

PROCESS_THREAD(temperature_monitor, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertTemp[80];
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);

   

    int temperatureTemp = random_rand() % (maxTemperature - minTemperature) + minTemperature;

     
    sprintf(msg, "Temperature: %d\n", temperatureTemp);
    printf("%s", msg);

  if (temperatureTemp < 35)
      {
        
        sprintf(alertTemp, "Hipotermia: %d Celsius", temperatureTemp);
        process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);


      }else if (temperatureTemp > 37){

        sprintf(alertTemp, "Febre: %d Celsius", temperatureTemp);
        process_post(&ALERT, PROCESS_EVENT_MSG, alertTemp);

      }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }
  
  PROCESS_END();
}

PROCESS_THREAD(ALERT, ev, data)
{
  PROCESS_BEGIN();

  
 // mutex_init(&alert_mutex);

  while (1)
  {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_MSG)
    {
    // mutex_lock(&alert_mutex); // Bloqueia o mutex

      if (!alert_in_use)
      {
       
        alert_in_use = 1;
        printf("!!!!!!!!!Alert Problema (Niveis Anormais) !!!!!!!!! =-=-=-=- %s -=-=-=-=\n", (char *)data);
        alert_in_use = 0;
        
      }

      //mutex_unlock(&alert_mutex);  // Libera o mutex
    }
  }

  PROCESS_END();
}
