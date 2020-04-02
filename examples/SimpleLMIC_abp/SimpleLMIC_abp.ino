#include <SimpleLMIC.h>

const char *devAddr = "11111111";
const char *nwkSKey = "11111111111111111111111111111111";
const char *appSKey = "11111111111111111111111111111111";

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
  ttn.setSubBand(2);
  ttn.onMessage(message);
  ttn.personalize(devAddr, nwkSKey, appSKey);
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
