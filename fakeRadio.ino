#include <sstream>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define ledPin 2
int ledState = LOW;
unsigned long previousMillis = 0; 
int interval = 1000;
int scanTime = 3; //In seconds
int auxCount = 0;
bool foundBeacon = false;
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf(advertisedDevice.toString().c_str());
    }
};

TaskHandle_t Task1;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  Serial.printf("Start BLE scan for %d seconds...\n", scanTime);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  Serial.println("setup end");

  xTaskCreatePinnedToCore(
        Blink, /* Function to implement the task */
        "BlinkTask", /* Name of the task */
        1000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        1,  /* Priority of the task */
        &Task1,  /* Task handle. */
        1); /* Core where the task should run */
        
  Serial.println("---- ESCANEANDO ELEMENTOS RADIOATIVOS ----");
}  

void Blink( void * parameter) {
  for(;;) {
  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      if (foundBeacon == true){
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
      }
  }
}
}

void loop() {
     
  Serial.println("*realizando nova leitura*");
  
  std::stringstream ss; //nessa posição para zerar a cada loop
  int rssi;
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  int count = foundDevices.getCount();
  //Serial.print("Beacons Encontrados: ");
  //Serial.println(count);
  auxCount = 0;
  for (int i = 0; i < count; i++)
  {  
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    std::stringstream aux; //nessa posição para zerar sempre que receber um novo address
    //int rssi;
    aux << d.getAddress().toString();
    //rssi << d.getRSSI();
    //Serial.println(aux.str().c_str());
    if(aux.str() == "e6:fa:2a:42:de:17"){ //aqui coloca os MAC dos beacons que quer filtrar
     ss << d.getAddress().toString() << "," << d.getRSSI() << "\n";
     rssi = d.getRSSI();
     foundBeacon = true;
     Serial.print(ss.str().c_str());
     auxCount++;
    } 
  }

  if (auxCount == 0){
    foundBeacon = false;
    Serial.println("-=-=Nenhum Elemento Encontrado=-=-\n");
  }
  //Serial.println("\n");
  if (foundBeacon == true){
//    Serial.print(ss.str().c_str());
    if(rssi <= -70){
    Serial.println("O elemênto radioativo está a mais de 10m de distância\n");
    interval = 800;
  } else if ( -70 < rssi && rssi <= -50){
    Serial.println("O elemênto radioativo está de 5m a 10m de distância\n");
    interval = 400;
  } else if (rssi > -49){
    Serial.println("O elemênto radioaivo está a menos de 5m de distância\n");
    interval = 100;
  }
  } else {
    digitalWrite(ledPin, LOW);
  }
}
