#include <Homie.h>
/*
 * Magnetic Contact Switch (MCS)
*/
#define PIN_MCS D3
#define LED_WIFI D0
#define LED_MQTT D1

HomieNode mcsNode("mcs", "switch");
/*
void setOpen() {
  Serial.println( digitalRead( PIN_MCS));

}
*/
void setup() {
  Homie.enableLogging(true);
  //Homie.setLedPin(D0, HIGH);
  Homie.enableBuiltInLedIndicator(true);
  Homie.setBrand("ymbihq");
  Homie.setFirmware("esp8266-magnetic-contact-switch", "1.0.0");
  
  //pinMode(PIN_MCS, INPUT);
  //attachInterrupt( digitalPinToInterrupt( PIN_MCS), setOpen, CHANGE);
  //attachInterrupt( 0, setOpen, CHANGE);
  
  Homie.registerNode(mcsNode);
  //Homie.setNodeProperty(mcsNode, "unit", "c", true);
  Homie.setup();
}

void loop() {
  Homie.loop();
}

