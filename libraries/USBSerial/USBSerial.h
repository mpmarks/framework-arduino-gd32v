// USB Serial header
#if defined (USBCON) && defined (USBD_USE_CDC)

#include "Stream.h"



class USBSerial : public Stream {
public:
  void begin(void);
  void begin(uint32_t);
  void begin(uint32_t, uint8_t);
  void end(void);

  virtual int available(void);
//  virtual int availableForWrite(void);
  virtual int peek(void);
  virtual int read(void);
  virtual size_t readBytes(char *buffer, size_t length);  // read chars from stream into buffer
  virtual size_t readBytesUntil(char terminator, char *buffer, size_t length);  // as readBytes with terminator character
  virtual void flush(void);
  virtual size_t write(uint8_t);
  virtual size_t write(uint8_t *buffer, size_t size);
  virtual size_t write(const uint8_t *buffer, size_t size);
  using Print::write; // pull in write(str) from Print
  operator bool(void);


  uint32_t baud();
  uint8_t stopbits();
  uint8_t paritytype();
  uint8_t numbits();
  bool dtr();
  bool rts();

  enum {
    ONE_STOP_BIT = 0,
    ONE_AND_HALF_STOP_BIT = 1,
    TWO_STOP_BITS = 2,
  };

  enum {
    NO_PARITY = 0,
    ODD_PARITY = 1,
    EVEN_PARITY = 2,
    MARK_PARITY = 3,
    SPACE_PARITY = 4,
  };

};

extern USBSerial SerialUSB;

#endif
