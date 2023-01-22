#include <Arduino.h>
#include "GPS_Air530Z.h"
#include "LoRaWan_APP.h"



//if GPS module is Air530Z, use this
Air530ZClass GPS;

typedef enum {
  STATUS_LPM = 0,
  STATUS_UPDATE_GPS
} dev_status;

int indexnew;
char str_latitude[27];
char str_longitude[27];
int counter = 0;

//fisrt fix timeout
#define GPS_INIT_TIMEOUT 300000  // 300sec/5min

//sleep time until next GPS update
#define GPS_SLEEPTIME 300000  // 300sec/5min


//when gps waked, if in GPS_UPDATE_TIMEOUT, gps not fixed then into low power mode
#define GPS_UPDATE_TIMEOUT 30000  // 30sec/0,5min

//once fixed, GPS_CONTINUE_TIME later into low power mode
#define GPS_CONTINUE_TIME 5000  // 5sec/

static TimerEvent_t autoGPS;
dev_status mcu_status;
uint32_t starttime;

int32_t fracPart(double val, int n) {
  return (int32_t)((val - (int32_t)(val)) * pow(10, n));
}


/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB red means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY 868000000  // Hz

#define TX_OUTPUT_POWER 14  // dBm

#define LORA_BANDWIDTH 0         // [0: 125 kHz, \
                                 //  1: 250 kHz, \
                                 //  2: 500 kHz, \
                                 //  3: Reserved \
                                 
#define LORA_SPREADING_FACTOR 7  // [SF7..SF12]
#define LORA_CODINGRATE 1        // [1: 4/5, \
                                 //  2: 4/6, \
                                 //  3: 4/7, \
                                 //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8   // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0    // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false


#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 40  // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

double txNumber;

int16_t rssi, rxSize;
unsigned long lastMillis;

uint16_t voltage;


void onAutoGPS() {
  TimerStop(&autoGPS);
  mcu_status = STATUS_UPDATE_GPS;
}

void gpsUpdate(uint32_t timeout, uint32_t continuetime) {
  //VextOFF();// oled power on;
  delay(10);

  GPS.begin();
  starttime = millis();
  while ((millis() - starttime) < timeout) {
    while (GPS.available() > 0) {
      GPS.encode(GPS.read());
    }

    // gps fixed in a second
    if (GPS.location.age() < 1000) {
      break;
    }
  }

  //if gps fixed update gps and print gps info every 1 second
  if (GPS.location.age() < 1000) {
    starttime = millis();
    uint32_t printinfo = 0;
    while ((millis() - starttime) < continuetime) {
      while (GPS.available() > 0) {
        GPS.encode(GPS.read());
      }

      if ((millis() - starttime) > printinfo) {
        printinfo += 1000;
        int index;
        char str_la[27];
        index = sprintf(str_la, "%d.%d", (int)GPS.location.lat(), fracPart(GPS.location.lat(), 6));
        str_la[index] = 0;
    

        char str_lo[27];
        index = sprintf(str_lo, "%d.%d", (int)GPS.location.lng(), fracPart(GPS.location.lng(), 6));
        str_lo[index] = 0;


        char* cmp_la = strstr(str_la, str_latitude); 
        char* cmp_lo = strstr(str_lo, str_longitude); 

        if(cmp_la && cmp_lo){
          counter ++;

          if (counter >= 6) {          
            sprintf(txpacket + strlen(txpacket), " %s", "R");  //add to the end of package  
            Radio.Send((uint8_t *)txpacket, strlen(txpacket));  //send the package out   
            delay (500);
            CySoftwareReset();   
          }    
        }



        sprintf(txpacket, "%s %s", str_la, str_lo);  //start a package
      }
    }
  } else  //if gps no fixed waited for 2s in to lowpowermode
  {
    
    int index;
    char str_la[27];
    index = sprintf(str_la, "%d.%d", (int)GPS.location.lat(), fracPart(GPS.location.lat(), 6));
    str_la[index] = 0;

    char str_lo[27];
    index = sprintf(str_lo, "%d.%d", (int)GPS.location.lng(), fracPart(GPS.location.lng(), 6));
    str_lo[index] = 0;

    char* cmp_la = strstr(str_la, str_latitude);      
    char* cmp_lo = strstr(str_lo, str_longitude); 

     if(cmp_la && cmp_lo){       
      voltage = getBatteryVoltage();
      sprintf(txpacket + strlen(txpacket), " %d%s", voltage, "V N");  //add to the end of package  
          Radio.Send((uint8_t *)txpacket, strlen(txpacket));  //send the package out 
          delay(5000);
          CySoftwareReset();  
     }



    sprintf(txpacket, "%s %s", str_la, str_lo);  //start a package


    delay(2000);
  }

  
    indexnew = sprintf(str_latitude, "%d.%d", (int)GPS.location.lat(), fracPart(GPS.location.lat(), 6));
    str_latitude[indexnew] = 0;
    


  GPS.end();
  voltage = getBatteryVoltage();

  sprintf(txpacket + strlen(txpacket), " %d%s", voltage, "V");  //add to the end of package

  Radio.Send((uint8_t *)txpacket, strlen(txpacket));  //send the package out
  delay(1000);
  Radio.Sleep();

  delay(2000);

  TimerSetValue(&autoGPS, GPS_SLEEPTIME);
  TimerStart(&autoGPS);
}




void setup() {


  txNumber = 0;
  rssi = 0;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);


  TimerInit(&autoGPS, onAutoGPS);

  gpsUpdate(GPS_INIT_TIMEOUT, GPS_CONTINUE_TIME);
  mcu_status = STATUS_LPM;

}






void loop() {
  

  switch (mcu_status) {
    case STATUS_LPM:
      lowPowerHandler();
      break;
    case STATUS_UPDATE_GPS:
      gpsUpdate(GPS_UPDATE_TIMEOUT, GPS_CONTINUE_TIME);
      mcu_status = STATUS_LPM;
      break;
    default:
      mcu_status = STATUS_LPM;
      break;
  }
}

/**
  * @brief  Double To String
  * @param  str: Array or pointer for storing strings
  * @param  double_num: Number to be converted
  * @param  len: Fractional length to keep
  * @retval None
  */
