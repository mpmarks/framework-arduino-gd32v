#ifndef TwoWire_h
#define TwoWire_h


/* MPM Aug 2020 */

#include "Arduino.h"
#include <Stream.h>


#define I2C_RX_BUFFER_SIZE 32

class TwoWire : public Stream {

public:	

	TwoWire() {};

	void begin();

	void begin(uint8_t address);

	void end();

	void setClock(uint32_t freq);

	void beginTransmission(uint8_t address);

	uint8_t endTransmission(bool stopBit);

	uint8_t endTransmission(void);

	uint8_t requestFrom(uint8_t address, size_t len, bool stopBit);

	uint8_t requestFrom(uint8_t address, size_t len);

//void TwoWire::onReceive(void(*)(int)) {};

//void TwoWire::onRequest(void(*)(void)){};

	size_t write(uint8_t);

	int read();

	int peek();

	void flush();

	int available();

private:
	byte rx_buffer[I2C_RX_BUFFER_SIZE];
	int rx_index, rx_len;

	uint8_t read_rx(bool ack); // Used by requestFrom to read into rx_buffer



};



extern TwoWire Wire;


#endif
