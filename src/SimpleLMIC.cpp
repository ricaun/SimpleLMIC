// ---------------------------------------------------- //
// SimpleLMIC.cpp
// ---------------------------------------------------- //

#include <Arduino.h>
#include "SimpleLMIC.h"

#include <SPI.h>
#include "lmic.h"
#include "hal/hal.h"
#include "arduino_lmic_hal_boards.h"

SimpleLMICClass *pSimpleLMIC = NULL;

SimpleLMICClass::SimpleLMICClass()
{
  pSimpleLMIC = this;
  setTimeout(0);
}

/***********************************************************************************/

bool SimpleLMICClass::begin()
{
  os_init();
  LMIC_reset();
  return true;
}

bool SimpleLMICClass::begin(const void *pPinMap)
{
  if (!os_init_ex(pPinMap))
    return false;

  LMIC_reset();
  return true;
}

void SimpleLMICClass::join()
{
  LMIC_startJoining();
}

void SimpleLMICClass::join(const char *_aeui, const char *_akey)
{
  setAppKey(_akey);
  setAppEui(_aeui);
  join();
}

void SimpleLMICClass::join(const char *_deui, const char *_aeui, const char *_akey)
{
  setDevEui(_deui);
  setAppKey(_akey);
  setAppEui(_aeui);
  join();
}

/***********************************************************************************/

void SimpleLMICClass::setLink(bool link)
{
  LMIC_setLinkCheckMode(link);
}

void SimpleLMICClass::setAdr(bool adr)
{
  LMIC_setAdrMode(adr);
}

void SimpleLMICClass::setSubBand(uint8_t band)
{
  LMIC_selectSubBand(band - 1);
}

void SimpleLMICClass::setError(uint8_t error)
{
  LMIC_setClockError(MAX_CLOCK_ERROR * error / 100);
}

/***********************************************************************************/

bool SimpleLMICClass::isBusy()
{
  return (LMIC.opmode & OP_TXDATA) || (LMIC.opmode & OP_TXRXPEND) || (LMIC.opmode & OP_POLL) || (LMIC.opmode & OP_JOINING);
}

bool SimpleLMICClass::isLink()
{
  return !(LMIC.opmode & OP_LINKDEAD);
}

void SimpleLMICClass::loop()
{
  os_runloop_once();
}

void SimpleLMICClass::sleep()
{
  os_radio(RADIO_RST);
  SPI.end();
}

void SimpleLMICClass::end()
{
  LMIC_shutdown();
  SPI.end();
}

/***********************************************************************************/

size_t SimpleLMICClass::write(uint8_t c)
{
  if (LMIC.pendTxLen == SIZEOFEXPR(LMIC.pendTxData))
    return 0;
  LMIC.pendTxData[LMIC.pendTxLen++] = c;
  return 1;
};

size_t SimpleLMICClass::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  for (size_t i = 0; i < size; i++)
  {
    n += write(*(buffer + i));
  }
  return n;
}

int SimpleLMICClass::available()
{
  return LMIC.dataLen - position;
}

int SimpleLMICClass::read()
{
  return position < LMIC.dataLen ? (LMIC.frame + LMIC.dataBeg)[position++] : -1;
}

int SimpleLMICClass::peek()
{
  return position < LMIC.dataLen ? (LMIC.frame + LMIC.dataBeg)[position] : -1;
}

void SimpleLMICClass::flush()
{
}

void SimpleLMICClass::send(uint8_t port, bool confirm)
{
  LMIC.pendTxConf = confirm;
  LMIC.pendTxPort = port;
  LMIC_setTxData();
}

int16_t SimpleLMICClass::read16()
{
  if (available() < 2)
    return 0;
  return (read() << 8 | read());
}

int32_t SimpleLMICClass::read24()
{
  if (available() < 3)
    return 0;
  return (read() << 24 | read() << 16 | read() << 8) >> 8;
}

int32_t SimpleLMICClass::read32()
{
  if (available() < 4)
    return 0;
  return (read() << 24 | read() << 16 | read() << 8 | read());
}

uint8_t *SimpleLMICClass::buffer()
{
  return ((uint8_t *)(LMIC.frame + LMIC.dataBeg + position));
}

void SimpleLMICClass::clear()
{
  LMIC.pendTxLen = 0;
  position = 0;
}

unsigned int SimpleLMICClass::readInt()
{
  if (available() < 2)
    return 0;
  unsigned int i = 0;
  i += read() << 8;
  i += read();
  return i;
}

unsigned long SimpleLMICClass::readLong()
{
  if (available() < 4)
    return 0;
  unsigned long i = 0;
  i += read() << 24;
  i += read() << 16;
  i += read() << 8;
  i += read();
  return i;
}

/***********************************************************************************/

// ---------------------------------------------------- //
// callback
// ---------------------------------------------------- //

void SimpleLMICClass::onMessage(void (*cb)(uint8_t *payload, size_t size, uint8_t port))
{
  messageCallback = cb;
}

void SimpleLMICClass::message(uint8_t *payload, size_t size, uint8_t port)
{
  if (messageCallback)
  {
    messageCallback(LMIC.frame + LMIC.dataBeg, LMIC.dataLen, port);
  }
}

// ---------------------------------------------------- //
// join keys
// ---------------------------------------------------- //

void SimpleLMICClass::setAppKey(uint8_t *_akey)
{
  memcpy(AppKey, _akey, 16);
}

void SimpleLMICClass::setAppKey(const char *_akey)
{
  LORA_HEX_TO_BYTE(AppKey, _akey, 16);
}

void SimpleLMICClass::setAppEui(uint8_t *_aeui)
{
  memcpy(AppEui, _aeui, 8);
}

void SimpleLMICClass::setAppEui(const char *_aeui)
{
  LORA_HEX_TO_BYTE(AppEui, _aeui, 8);
}

void SimpleLMICClass::setDevEui(uint8_t *_deui)
{
  memcpy(DevEui, _deui, 8);
}

void SimpleLMICClass::setDevEui(const char *_deui)
{
  LORA_HEX_TO_BYTE(DevEui, _deui, 8);
}

void SimpleLMICClass::personalize(const char *_devAddr, const char *_nwkSKey, const char *_appSKey)
{
  uint8_t devAddr[4];
  uint8_t nwkSKey[16];
  uint8_t appSKey[16];
  LORA_HEX_TO_BYTE((char *)devAddr, _devAddr, 4);
  LORA_HEX_TO_BYTE((char *)nwkSKey, _nwkSKey, 16);
  LORA_HEX_TO_BYTE((char *)appSKey, _appSKey, 16);
  LMIC_setSession(0x13, LORA_DEVADDR(devAddr), nwkSKey, appSKey);
}

// ---------------------------------------------------- //
// LMIC
// ---------------------------------------------------- //

void os_getArtEui(u1_t *buf)
{
  for (int i = 0; i < 8; i++)
    buf[i] = pSimpleLMIC->AppEui[7 - i];
}

void os_getDevEui(u1_t *buf)
{
  for (int i = 0; i < 8; i++)
    buf[i] = pSimpleLMIC->DevEui[7 - i];
}

void os_getDevKey(u1_t *buf)
{
  memcpy(buf, pSimpleLMIC->AppKey, 16);
}

void onEvent(ev_t ev)
{
  if (pSimpleLMIC->debug)
  {
    Serial.print(os_getTime());
    Serial.print(": ");
    Serial.print(LMIC.freq);
    Serial.print(" | ");

    const char *const evNames[] = {LMIC_EVENT_NAME_TABLE__INIT};

    if (ev < sizeof(evNames) / sizeof(evNames[0]))
    {
      Serial.print(evNames[ev]);
    }
    else
    {
      Serial.print(F("Unknown event: "));
      Serial.print((unsigned)ev);
    }
    Serial.println();
  }

  switch (ev)
  {
  case EV_JOINING:
    break;
  case EV_JOINED:
    //LMIC_getSessionKeys(&pSimpleLMIC->NetID, &pSimpleLMIC->DevAddr, pSimpleLMIC->NwkSKey, pSimpleLMIC->AppSKey);
    break;
  case EV_JOIN_FAILED:
    break;
  case EV_REJOIN_FAILED:
    break;
  case EV_TXCOMPLETE:
    pSimpleLMIC->clear();
    if (LMIC.dataLen != 0 || LMIC.dataBeg != 0)
    {
      uint8_t port = 0;
      if (LMIC.txrxFlags & TXRX_PORT)
      {
        port = LMIC.frame[LMIC.dataBeg - 1];
      }
      pSimpleLMIC->message(LMIC.frame + LMIC.dataBeg, LMIC.dataLen, port);
    }
    break;
  case EV_LOST_TSYNC:
    break;
  case EV_RESET:
    break;
  case EV_RXCOMPLETE:
    break;
  case EV_LINK_DEAD:
    break;
  case EV_LINK_ALIVE:
    break;
  case EV_TXSTART:
    break;
  }
}
