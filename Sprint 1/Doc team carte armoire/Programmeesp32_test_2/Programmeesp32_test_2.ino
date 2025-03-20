#include <HardwareSerial.h>
#include <AlmostRandom.h>

#define RX_PIN 16
#define TX_PIN 17

AlmostRandom ar;
HardwareSerial LoRaSerial(1);  // UART1 de l'ESP32

void setup() {
  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Port Série pour Lora

  // Connexion au réseau LoRaWAN
  sendCommand("AT+CJOIN=1,0,10,8");
  delay(10000);
  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    Serial.print(c);
  }
}

void loop() {
  int vitesse_vent = ar.getRandomUInt();
  int vitesse_vent_int = (int)(vitesse_vent * 100);
  char payload[6];
  sprintf(payload, "%04X", vitesse_vent_int);


  Serial.print("Envoi des données: ");
  Serial.println(payload);
  String command = "AT+DTRX=1,2,10," + String(payload);
  sendCommand(command.c_str());
  delay(30000);
}

void sendCommand(const char *cmd) {
  LoRaSerial.println(cmd);
  delay(1000);
  while (LoRaSerial.available()) {
    Serial.write(LoRaSerial.read());
  }
}