//Codigo base para o sistema Receptor (server)
//15/04/2025

/*Este projeto demonstra como dois ESP32 podem se comunicar entre si usando Bluetooth Low Energy (BLE). 
Um atua como periférico (server) e outro como central (client).
O cliente faz uma leitura do botão caso verdadeiro, o server acende o LED
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED_PIN 2

BLECharacteristic *pCharacteristic;


class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    auto rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.print("Recebido: ");
      Serial.println(rxValue.c_str());

      // Controle do LED
      if (rxValue == "1") {
        //Aqui caso verdadeiro faz algo...
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ligado");
        //Aqui caso falso faz algo...
      } else if (rxValue == "0") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED desligado");
      }

      const char* resposta = "Recebido com sucesso!";
      pCharacteristic->setValue(resposta);
      pCharacteristic->notify();
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  BLEDevice::init("ESP32-SERVIDOR");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("Servidor BLE pronto e esperando conexões...");
}

void loop() {
  delay(100);
}