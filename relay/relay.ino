#include <Homie.h>

#define PIN_RELAY D2
#define LED_WIFI D0
#define LED_MQTT D1

HomieNode lightNode("heater", "switch");

bool lightOnHandler(String value) {
  if (value == "true") {
    digitalWrite(PIN_RELAY, HIGH);
    Homie.setNodeProperty(lightNode, "on", "true"); // Update the state of the light
    Serial.println("Heater is on");
  } else if (value == "false") {
    digitalWrite(PIN_RELAY, LOW);
    Homie.setNodeProperty(lightNode, "on", "false");
    Serial.println("Heater is off");
  } else {
    return false;
  }

  return true;
}


void setup() {
  Homie.enableLogging(true);
  Homie.setLedPin(D0, HIGH);
  Homie.enableBuiltInLedIndicator(true);
  Homie.setBrand("ymbihq");
  Homie.setFirmware("esp8266-plug-relay", "1.0.0");
  
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  
  lightNode.subscribe("on", lightOnHandler);
  Homie.registerNode(lightNode);
  Homie.setup();
}

void loop() {
  Homie.loop();
}

