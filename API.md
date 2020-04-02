# SimpleLMIC API

## Include Library

```c
#include <SimpleLMIC.h>

SimpleLMIC ttn;
```

### Begin

Initialize the library.

```c
ttn.begin();
```
Returns `1` on success, `0` on failure.

### Loop

This function need to be on the loop to make the library work properly.

```c
ttn.loop();
```

### Sleep

Sleep the library

```c
ttn.sleep();
```

### End

Stop the library

```c
ttn.end();
```

## Configuration

### Pin Mapping

The pin mapping should be set like the [arduino-lmic](https://github.com/mcci-catena/arduino-lmic#pin-mapping)

```c
const lmic_pinmap lmic_pins = {
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = LMIC_UNUSED_PIN,
  .dio = {2, 2, 2}
};
```

### Link

Set LoRaWAN link.

```c
ttn.setLink(true);
```
or
```c
ttn.setLink(false);
```

### Adaptive Data Rate (Adr)

Set LoRaWAN Adaptive Data Rate.

```c
ttn.setAdr(true);
```
or
```c
ttn.setAdr(false);
```

### Clock Error

Set the clock error percentage. Check the [lmic_setclockerror](https://github.com/mcci-catena/arduino-lmic#lmic_setclockerror) for more information.

```c
ttn.setError(error);
```
* `error` - Set the clock error, defaults to `5`

### Event Debug

Show the events on the Serial monitor.

```c
ttn.debug(true);
```
or
```c
ttn.debug(false);
```

### LoRaWAN Channels

Set the Channel of the US region.

```c
ttn.setSubBand(band);
```
* `band` - Set the subband plan, defaults to `2` - (`1` to `8`)

## Over The Air Activation (OTAA)

For OTAA you will use the `join()`.

```c
ttn.setDevEui(devEui);
ttn.setAppEui(appEui);
ttn.setAppKey(appKey);
ttn.join();
```
```c
ttn.setDevEui(devEui);
ttn.join(appEui, appKey);
```
```c
ttn.join(devEui, appEui, appKey);
```
 * `devEui` - LoRaWAN Device EUI.
 * `appEui` - LoRaWAN Application EUI.
 * `appKey` - LoRaWAN App Key.

## Activation By Personalization (ABP)

For ABP you will use the `personalize()`.

```c
ttn.personalize(devAddr, nwkSKey, appSKey);
```
 * `devAddr` - LoRaWAN Device Address.
 * `nwkSKey` - LoRaWAN Network Session Key.
 * `appSKey` - LoRaWAN App Session Key.

#### Register callback

### onMessage

Register a callback function for when a payload is received.

```c
ttn.onMessage(onMessage);

void onMessage(uint8_t *payload, size_t size, uint8_t port) {
 // ...
}
```

 * `onMessage` - function to call when a valid payload is received.
    * `payload` - array with the payload data.
    * `size` - length of the payload data.
    * `port` - port of the payload.

## Sending data

### Writing

Write data to the payload buffer.

```c
ttn.write(byte);

ttn.write(buffer, length);
```
* `byte` - single byte to write to packet

or

* `buffer` - data to write to packet
* `length` - size of data to write

Returns the number of bytes written.

**Note:** Other Arduino `Print` API's can also be used to write data into the packet

### Send

```c
ttn.send(port, confirm);
```
* `port` - port number of the payload, defaults to `1`
* `confirm` - confirm payload, defaults to `false`

### Busy

Check if the LoRaWAN is busy, like waiting the rx1 and rx2, ask request, etc.

```c
void loop() {
    ttn.loop();
    if (!ttn.isBusy())
    {
        ttn.print("hello");
        ttn.send();
    }
}
```

### Link

Check if the LoRaWAN has link.

```c
void loop() {
    ttn.loop();
    if (ttn.isLink())
    {

    }
}
```