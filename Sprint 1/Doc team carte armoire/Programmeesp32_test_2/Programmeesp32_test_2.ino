#include <HardwareSerial.h>
#include <AlmostRandom.h>

#define RX_PIN 16
#define TX_PIN 17

AlmostRandom ar;
HardwareSerial LoRaSerial(1);

void setup() {
  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  sendCommand("AT+CJOIN=1,0,10,8");
  delay(10000);
  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();
    Serial.print(c);
  }
}

void loop() {
  int id_capteur1 = 1;
  int id_capteur2 = 2;
  int id_cpateur3 = 3;
  int vitesse_vent = 122;
  int direction = 2;
  char payload_vitesse[6];
  char payload_direction[6];
  sprintf(payload_vitesse, "%02X", vitesse_vent);
  sprintf(payload_direction, "%02X", direction);


  if(id_capteur1 == 1){
  Serial.print("Envoi de la vitesse en hexa: ");
  Serial.println(payload_vitesse);
  Serial.print("Envoi de la vitesse en hexa: ");
  Serial.println(payload_direction);
  String command = "AT+DTRX=1,2,10,0101" + String(payload_vitesse) + "0201" + String(payload_direction);
  sendCommand(command.c_str());
  delay(30000);

  }

}

void sendCommand(const char *cmd) {
  LoRaSerial.println(cmd);
  delay(1000);
  while (LoRaSerial.available()) {
    Serial.write(LoRaSerial.read());
  }
}