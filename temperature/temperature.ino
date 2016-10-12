#include <Homie.h>

// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];

const int TEMPERATURE_INTERVAL = 30;

unsigned long lastTemperatureSent = 0;

HomieNode temperatureNode("temperature", "temperature");

void setupHandler() {
  Homie.setNodeProperty(temperatureNode, "unit", "c", true);
}

void loopHandler() {
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
    uint8_t i;
    float average;
   
    // take N samples in a row, with a slight delay
    for (i=0; i< NUMSAMPLES; i++) {
     samples[i] = analogRead(THERMISTORPIN);
     delay(10);
    }
   
    // average all the samples out
    average = 0;
    for (i=0; i< NUMSAMPLES; i++) {
       average += samples[i];
    }
    average /= NUMSAMPLES;
   
    Serial.print("Average analog reading "); 
    Serial.println(average);
   
    // convert the value to resistance
    average = 1023 / average - 1;
    average = SERIESRESISTOR / average;
    Serial.print("Thermistor resistance "); 
    Serial.println(average);
   
    float steinhart;
    steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C
    
    Serial.print("Temperature: ");
    Serial.print(steinhart);
    Serial.println(" °C");
    if (Homie.setNodeProperty(temperatureNode, "degrees", String(steinhart), true)) {
      lastTemperatureSent = millis();
    } else {
      Serial.println("Temperature sending failed");
    }
  }
}

void setup() {
  Homie.enableLogging(true);
  Homie.setLedPin(16, HIGH);
  Homie.setBrand("ymbihq");
  Homie.setFirmware("esp8266-plug-temp", "1.0.0");
  Homie.registerNode(temperatureNode);
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}

