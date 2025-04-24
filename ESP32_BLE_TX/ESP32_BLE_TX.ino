//Codigo base para o Emissor (cliente)
//15/04/2025

/*Este projeto demonstra como dois ESP32 podem se comunicar entre si usando Bluetooth Low Energy (BLE). 
Um atua como periférico (server) e outro como central (client).
O cliente faz uma leitura do botão caso verdadeiro, o server acende o LED
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <BLEAdvertisedDevice.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define BUTTON_PIN 23 //Pino onde liga o botão.

BLEAdvertisedDevice* myDevice;
BLEClient*  pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;
bool doConnect = false;
bool connected = false;
bool lastState = LOW;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Encontrado: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT); //Define o pino como entrada

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  Serial.println("Escaneando BLE...");
}

void loop() {
  //Verifica os dispositivos disponiveis.
  RotCom();//Rotina de conexão

  //Apos conectado executa a logica pertinente ao sistema de comunicação entre eles.
  //Se esta ligado 1, senão envia 0.
  if (connected) {
    bool currentState = digitalRead(BUTTON_PIN);
    if (currentState != lastState) {
      const char* msg = currentState ? "1" : "0";
      pRemoteCharacteristic->writeValue(msg);
      Serial.printf("Botão %s - enviado %s\n", currentState ? "HIGH" : "LOW", msg);
      lastState = currentState;
      delay(100); // debounce simples
    }
  }
}


//Rotina de conexão
void RotCom(){
  if (doConnect) {
    pClient = BLEDevice::createClient();

    if (!pClient->connect(myDevice)) {
      Serial.println("Falha na conexão com o servidor BLE.");
      return;
    }

    Serial.println("Conectado ao servidor BLE.");

    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
      Serial.println("Serviço BLE não encontrado.");
      pClient->disconnect();
      return;
    }

    pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.println("Característica BLE não encontrada.");
      pClient->disconnect();
      return;
    }

    connected = true;
    doConnect = false;
  }
 }
