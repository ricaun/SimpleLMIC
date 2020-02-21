#include <SimpleLMIC.h>

const char *devEui = "1111111111111111";
const char *appEui = "1111111111111111";
const char *appKey = "11111111111111111111111111111111";

const lmic_pinmap lmic_pins = {
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = LMIC_UNUSED_PIN,
  .dio = {2, 2, 2}
};

SimpleLMIC ttn;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  ttn.begin();
  ttn.onMessage(message);
  ttn.join(devEui, appEui, appKey);
}

void loop() {
  ttn.loop();
  if (!ttn.isBusy())
  {
    Serial.println("Not Busy!");
    delay(5000);
    ttn.print("Hello World");
    ttn.send();
  }
}

void message(uint8_t *payload, size_t size, uint8_t port)
{
  Serial.println("Received " + String(size) + " bytes on port " + String(port));
  switch (port) {
    case 1:
      break;
  }
}
