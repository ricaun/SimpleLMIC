# SimpleLMIC - Alpha Version

Simple library for using the Arduino LMIC library with The Things Network and LoRaWAN&reg; networks.

## Overview

The **`SimpleLMIC`** library provides a structured way of using the [`arduino-lmic` library][0] to send sensor data over The Things Network or a similar LoRaWAN-based data network. Inspireded by [`arduino-lorawan` library][1]

## Library Dependency

The library uses the [`MCCI LoRaWAN LMIC library`][0], to configurate the [`MCCI LoRaWAN LMIC library`][0] and select the LoRa chip and the LoRaWAN plan. [`MCCI LoRaWAN LMIC Configuration`][2].

## API

See [API.md](API.md).

## Installation

* Install the library by [Using the Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3)
* **OR** by [Importing the .zip library](https://www.arduino.cc/en/Guide/Libraries#toc4) using the [releases](https://github.com/ricaun/SimpleLMIC/releases) ZIP files.

## Examples

The library comes with [examples](examples). After installing the library you need to restart the Arduino IDE before they can be found under **File > Examples > SimpleLMIC**.

## License

This library is [licensed](LICENSE) under the [MIT Licence](https://en.wikipedia.org/wiki/MIT_License).

---

Do you like this library? Please [star this project on GitHub](https://github.com/ricaun/SimpleLMIC/stargazers)!

[0]: https://github.com/mcci-catena/arduino-lmic
[1]: https://github.com/mcci-catena/arduino-lorawan
[2]: https://github.com/mcci-catena/arduino-lmic#configuration