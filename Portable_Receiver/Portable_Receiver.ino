#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        23 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4 // Popular NeoPixel ring size

#define buttonPin 17

int buttonValue = 0;

int bright;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels



#include "heltec.h"


#include "BLEDevice.h"

//BLE

/* Specify the Service UUID of Server */
static BLEUUID serviceUUID("0000ffe4-0000-1000-8000-00805f9b34fb");
/* Specify the Characteristic UUID of Server */
static BLEUUID    charUUID("0000ffe5-0000-1000-8000-00805f9b34fb");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

bool deviceConnected = false;
bool oldDeviceConnected = false;

BLEClient*  pClient  = BLEDevice::createClient();

String rssiBLE = "";
int rssiLED;
String bluet = "N/A";

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                            uint8_t* pData, size_t length, bool isNotify)
{
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient* pclient)
  {
    deviceConnected = true;
  }

  void onDisconnect(BLEClient* pclient)
  {
    deviceConnected = false;
    connected = false;
  }
};

/* Start connection to the BLE Server */
bool connectToServer()
{
 
  pClient->setClientCallbacks(new MyClientCallback());

    /* Connect to the remote BLE Server */
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Heltec.display->drawString(45, 0, "bonding");
     Heltec.display->display();


    /* Obtain a reference to the service we are after in the remote BLE server */
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    pClient->disconnect();
    return false;
  }



  /* Obtain a reference to the characteristic in the service of the remote BLE server */
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  /* Read the value of the characteristic */
  /* Initial value is 'Hello, World!' */
  if(pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
  }

  if(pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  }

    connected = true;
    return true;
}
/* Scan for BLE servers and find the first one that advertises the service we are looking for. */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
 /* Called for each advertising BLE server. */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {

    /* We have found a device, let us now see if it contains the service we are looking for. */
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    }
  }
};


void onButton(){
  Serial.println("button pressed");
  if(connected){
    // String newValue = "beep"; //String für Buzzer am CubeCell Board
    String newValue = "AT+PIO41"; //String für Buzzer direkt am BLE Modul
    Serial.println(newValue);

    
    /* Set the characteristic's value to be the array of bytes that is actually a string */
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    /* You can see this value updated in the Server's Characteristic */
        delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        newValue = "AT+PIO40";
        pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());    
  }else {
          Heltec.display->drawString(45, 0, "search");
          Heltec.display->display();
          BLEDevice::init("ESP32-BLE-Client");

          esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); 
          esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
          esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);  



            /* Retrieve a Scanner and set the callback we want to use to be informed when we
              have detected a new device.  Specify that we want active scanning and start the
              scan to run for 5 seconds. */
            BLEScan* pBLEScan = BLEDevice::getScan();
            pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
            pBLEScan->setInterval(1349);
            pBLEScan->setWindow(449);
            pBLEScan->setActiveScan(true);
            pBLEScan->start(10, false);
            }
   

}

//battery
#include <Arduino.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>
#define MAXBATT                 4200    // The default Lipo is 4200mv when the battery is fully charged.
#define LIGHT_SLEEP_VOLTAGE     3750    // Point where start light sleep
#define MINBATT                 3200    // The default Lipo is 3200mv when the battery is empty...this WILL be low on the 3.3v rail specs!!!

#define VOLTAGE_DIVIDER         3.20    // Lora has 220k/100k voltage divider so need to reverse that reduction via (220k+100k)/100k on vbat GPIO37 or ADC1_1 (early revs were GPIO13 or ADC2_4 but do NOT use with WiFi.begin())
#define DEFAULT_VREF            1100    // Default VREF use if no e-fuse calibration
#define VBATT_SAMPLE            500     // Battery sample rate in ms
#define VBATT_SMOOTH            50      // Number of averages in sample
#define ADC_READ_STABILIZE      5       // in ms (delay from GPIO control and ADC connections times)
#define LO_BATT_SLEEP_TIME      10*60*1000*1000     // How long when low batt to stay in sleep (us)
#define HELTEC_V2_1             1       // Set this to switch between GPIO13(V2.0) and GPIO37(V2.1) for VBatt ADC.
#define VBATT_GPIO              21      // Heltec GPIO to toggle VBatt read connection ... WARNING!!! This also connects VEXT to VCC=3.3v so be careful what is on header.  Also, take care NOT to have ADC read connection in OPEN DRAIN when GPIO goes HIGH
#define __DEBUG                 0       // DEBUG Serial output

uint16_t Sample();
void drawBattery(uint16_t, bool = false);

esp_adc_cal_characteristics_t *adc_chars;

// Poll the proper ADC for VBatt on Heltec Lora 32 with GPIO21 toggled
uint16_t ReadVBatt() {
  uint16_t reading = 666;

  digitalWrite(VBATT_GPIO, LOW);              // ESP32 Lora v2.1 reads on GPIO37 when GPIO21 is low
  delay(ADC_READ_STABILIZE);                  // let GPIO stabilize
  #if (defined(HELTEC_V2_1))
  pinMode(ADC1_CHANNEL_1, OPEN_DRAIN);        // ADC GPIO37
  reading = adc1_get_raw(ADC1_CHANNEL_1);
  pinMode(ADC1_CHANNEL_1, INPUT);             // Disconnect ADC before GPIO goes back high so we protect ADC from direct connect to VBATT (i.e. no divider)
  #else
  pinMode(ADC2_CHANNEL_4, OPEN_DRAIN);        // ADC GPIO13
  adc2_get_raw(ADC2_CHANNEL_4,ADC_WIDTH_BIT_12,&reading);
  pinMode(ADC2_CHANNEL_4, INPUT);             // Disconnect ADC before GPIO goes back high so we protect ADC from direct connect to VBATT (i.e. no divider
  #endif

  uint16_t voltage = esp_adc_cal_raw_to_voltage(reading, adc_chars);  
  voltage*=VOLTAGE_DIVIDER;

  return voltage;
}

//  Use a buffer to average/sample ADC
uint16_t Sample() {
  static uint8_t i = 0;
  static uint16_t samp[VBATT_SMOOTH];
  static int32_t t = 0;
  static bool f = true;
  if(f){ for(uint8_t c=0;c<VBATT_SMOOTH;c++){ samp[c]=0; } f=false; }   // Initialize the sample array first time
  t -= samp[i];   // doing a rolling recording, so remove the old rolled around value out of total and get ready to put new one in.
  if (t<0) {t = 0;}

  // ADC read
  uint16_t voltage = ReadVBatt();

  samp[i]=voltage;
  #if defined(__DEBUG) && __DEBUG > 0

  #endif
  t += samp[i];

  if(++i >= VBATT_SMOOTH) {i=0;}
  uint16_t s = round(((float)t / (float)VBATT_SMOOTH));
  #if defined(__DEBUG) && __DEBUG > 0

  #endif

  return s;
}


void drawBattery(uint16_t voltage, bool sleep) {

  uint16_t v = voltage;
  if (v < MINBATT) {v = MINBATT;}
  if (v > MAXBATT) {v = MAXBATT;}
  double pct = map(v,MINBATT,MAXBATT,0,100);
  uint8_t bars = round(pct / 10.0);

  uint16_t rnd = round(voltage/10.0);
  Serial.println(rnd);

  if (rnd > 413 || rnd < 380){
    Heltec.display->drawString(1,14,String(round(voltage/10.0)/100.0)+"V");

  }

  #if defined(__DEBUG) && __DEBUG > 0
  static uint8_t c = 0;
  if ((c++ % 10) == 0) {
    c = 1;
 
  }
  #endif
}



//LoRa
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
String rssiLoRa = " ";
String snr = " ";
String packSize = "--";
String packet ;
String str ;
String sublat ;
// int sublatsplit ;
String sublon ;
// int sublonsplit ;
String position;
String battery;

String fence;
int indexLON ;
int indexV ;


void DisplayData(){
  Heltec.display->clear();

  //battery
  uint16_t voltage = Sample();
  drawBattery(voltage, voltage < LIGHT_SLEEP_VOLTAGE);

  //BLE
  Heltec.display->drawString(0, 0, bluet);  
  Heltec.display->drawString(25, 0, rssiBLE); 

  //LoRa
  Heltec.display->drawString(0, 30, "LoRa:");
  Heltec.display->drawString(10, 45, position); 
  Heltec.display->drawStringMaxWidth(35 , 30 , 128, fence);
 
  Heltec.display->drawStringMaxWidth(85 , 0 , 128, battery);   
  Heltec.display->drawString(100, 20, snr); 
  Heltec.display->drawString(100, 30, rssiLoRa);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";

  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read();  }
  rssiLoRa = String(LoRa.packetRssi(), DEC) ;
  snr = (LoRa.packetSnr(), DEC) ;
  

  
  str = packet;
  indexLON = str.indexOf(' ');
  indexV = str.indexOf('V');
  sublat = str.substring(0, indexLON);
  sublon = str.substring(indexLON + 1, indexV - 5);
  battery = str.substring(indexV - 4, indexV + 1);
  position = sublat + " " + sublon;
  
  //setting zones
  if (sublat.toFloat() > 48.741537 && sublat.toFloat() < 48.741653 && sublon.toFloat() > 10.788777 && sublon.toFloat() < 10.789115) {
    fence = "at home";
  }else if(sublat.toFloat() > 48.739608 && sublat.toFloat() < 48.740880 && sublon.toFloat() > 10.790100 && sublon.toFloat() < 10.790737){
    fence = "forest";
  }else {
    fence = "unknown";
  }
  
}


void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  //battery
  // Characterize ADC at particular atten
  #if (defined(HELTEC_V2_1))
  adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_6);
  #else
  // Use this for older V2.0 with VBatt reading wired to GPIO13
  adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
  adc2_config_channel_atten(ADC2_CHANNEL_4,ADC_ATTEN_DB_6);
  #endif

  #if defined(__DEBUG) && __DEBUG > 0
  Serial.printf("ADC Calibration: ");
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.printf("eFuse Vref\n");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
      Serial.printf("Two Point\n");
  } else {
      Serial.printf("Default[%dmV]\n",DEFAULT_VREF);
  }
  #else
  if (val_type);    // Suppress warning
  #endif

 #if defined(__DEBUG) && __DEBUG >= 1
 
  #else
  if (val_type);    // Suppress warning
  #endif

  // Prime the Sample register
  for (uint8_t i = 0;i < VBATT_SMOOTH;i++) {
    Sample();
  }

  pinMode(VBATT_GPIO,OUTPUT);
  digitalWrite(VBATT_GPIO, LOW);              // ESP32 Lora v2.1 reads on GPIO37 when GPIO21 is low
  delay(ADC_READ_STABILIZE);                  // let GPIO stabilize




  //LoRa

  //LoRa.setSpreadingFactor(7);
  LoRa.setSpreadingFactor(9);
  // LoRa.setSignalBandwidth(250E3);
  LoRa.setSignalBandwidth(125E3);
  // LoRa.setSignalBandwidth(41.7E3);
  LoRa.setCodingRate4(1);
  LoRa.setSyncWord(0x12);   //0x34
  LoRa.setPreambleLength(8);
  
  //display
  
  Heltec.display->init(); 
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Empfänger");
  Heltec.display->display();
  delay(500);
  Heltec.display->clear();
  Heltec.display->display();

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(50);
  pixels.clear();
  pixels.show();
  Serial.println("Starting Arduino BLE Client application...");
  pinMode(buttonPin, INPUT);
}


void loop()
{

int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);   

  DisplayData();
 rssiBLE = pClient->getRssi();
 rssiLED = pClient->getRssi();
  buttonValue =  digitalRead(buttonPin);
Serial.println(buttonValue);

   if (!deviceConnected) {
    bluet = "N/A";

    pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(125, 0, 255));
      
      pixels.show();
   }else{
    bluet = "BT";

    if (rssiLED < -140){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      
      pixels.show();
    }else if (rssiLED < -110){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
     
      pixels.show();
    }else if (rssiLED < -105){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      
      pixels.show();
    }else if (rssiLED < -100){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
 
      pixels.show();
    }else if (rssiLED < -90){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(255, 0, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      
      pixels.show();
    }else if (rssiLED < -70){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(0, 255, 0));
      pixels.setPixelColor(2, pixels.Color(255, 0, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      
      pixels.show();
    }else if (rssiLED < -60){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(0, 255, 0));
      pixels.setPixelColor(2, pixels.Color(0, 255, 0));
      pixels.setPixelColor(3, pixels.Color(255, 0, 0));
      
      pixels.show();   
    }else if (rssiLED < -50){
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.setPixelColor(1, pixels.Color(0, 255, 0));
      pixels.setPixelColor(2, pixels.Color(0, 255, 0));
      pixels.setPixelColor(3, pixels.Color(0, 255, 0));
      
      pixels.show();
    }else {
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 255, 180));
      pixels.setPixelColor(3, pixels.Color(0, 255, 180));
      
      pixels.show();
    }
   }
  
  if (buttonValue == 1){
    onButton();    
  }

  /* If the flag "doConnect" is true, then we have scanned for and found the desired
     BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
     connected we set the connected flag to be true. */
  if (doConnect == true)
  {
    if (connectToServer())
    {
      Serial.println("We are now connected to the BLE Server.");
    } 
    else
    {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

if (!deviceConnected && oldDeviceConnected) { 
        oldDeviceConnected = deviceConnected;

    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }  


  /* If we are connected to a peer BLE Server, update the characteristic each time we are reached
     with the current time since boot */
  
  
  
  delay(500); /* Delay a second between loops */
}