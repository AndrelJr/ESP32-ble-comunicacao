# Comunicação BLE entre dois ESP32

Este projeto demonstra como dois ESP32 podem se comunicar entre si usando Bluetooth Low Energy (BLE). Um atua como periférico (server) e outro como central (client).

## 🛠 Requisitos
- Placa ESP32 (duas unidades)
- IDE Arduino configurada para ESP32
- Biblioteca BLE (`ESP32 BLE Arduino`)

## 📂 Estrutura
- `central/`: código para o dispositivo central (client)
- `periferico/`: código para o periférico (server)

## 🚀 Como usar
1. Abra o código em `periferico/periferico.ino` e envie para um ESP32
2. Abra o código em `central/central.ino` e envie para o outro ESP32
3. Veja a comunicação via monitor serial

## 📄 Licença
Este projeto está licenciado sob a Licença MIT.
