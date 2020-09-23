// USB Serial code
#if defined (USBCON) && (USBD_USE_CDC)

#include "USBSerial.h"
#include "CDC.h"

#include "RingBuffer.h"

extern "C" uint8_t packet_receive;

USBSerial SerialUSB;

#define RX_BUFFER_SIZE 1024

RingBufferN<RX_BUFFER_SIZE> rx_buffer;


void USBSerial::begin(void)
{
  CDC_Init();
  rx_buffer.clear();
}

void USBSerial::begin(uint32_t /* baud_count */)
{
  // uart config is ignored in USB-CDC
  begin();
}

void USBSerial::begin(uint32_t /* baud_count */, uint8_t /* config */)
{
  // uart config is ignored in USB-CDC
  begin();
}

void USBSerial::end()
{
  CDC_deInit();
}

#if 0
int USBSerial::availableForWrite()
{
  // Just transmit queue size, available for write
  return static_cast<int>(CDC_TransmitQueue_WriteSize(&TransmitQueue));
}
#endif

size_t USBSerial::write(uint8_t ch)
{
  // Just write single-byte buffer.
  return write(&ch, 1);
}

size_t USBSerial::write(const uint8_t *buffer, size_t size) {
  return write((uint8_t*) buffer, size);
}

size_t USBSerial::write(uint8_t *buffer, size_t size)
{
  size_t rest = size;
  
  if (packet_receive) available(); // check for read before write
 
  return CDC_write(buffer, size);

  return size - rest;
}

int USBSerial::available(void)
{
  uint8_t tbuf[64];
  size_t rlen;
  rlen = CDC_recv(tbuf); // read any new data
  if (rlen) {
    for (size_t i=0; i< rlen; i++) rx_buffer.store_char(tbuf[i]); // write to ring
  };
  return rx_buffer.available();
}

int USBSerial::read(void)
{
  return rx_buffer.read_char();

}

size_t USBSerial::readBytes(char *buffer, size_t length)
{
  uint16_t nread = 0;
  while ((this->available() > 0) && (nread < length)) {
    *buffer++ = rx_buffer.read_char();
    nread++;
  }

  return nread;
}

size_t USBSerial::readBytesUntil(char terminator, char *buffer, size_t length)
{
  uint16_t nread = 0;
  char c;
  while ((this->available() > 0) && (nread < length)) {
    c = rx_buffer.read_char();
    if (c == terminator) break;
    *buffer++ = c;
    nread++;
  }

  return nread;

}

int USBSerial::peek(void)
{
    return rx_buffer.peek();
}

void USBSerial::flush(void)
{
  // Wait for TransmitQueue read size becomes zero
  // TS: safe, because it not be stopped while receive 0
//  while (CDC_TransmitQueue_ReadSize(&TransmitQueue) > 0) {}
}

uint32_t USBSerial::baud()
{
  return 115200;
}

uint8_t USBSerial::stopbits()
{
  return ONE_STOP_BIT;
}

uint8_t USBSerial::paritytype()
{
  return NO_PARITY;
}

uint8_t USBSerial::numbits()
{
  return 8;
}

bool USBSerial::dtr(void)
{
  return false;
}

bool USBSerial::rts(void)
{
  return false;
}

USBSerial::operator bool()
{
  bool result = false;
#if 0
  if (lineState == 1) {
    result = true;
  }
#endif
//  delay(10);
  return result;
}
#endif
