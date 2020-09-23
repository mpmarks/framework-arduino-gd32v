/* Longan Nano I2C
   This is specific to the Longan Nano and uses the default pins PB6 () and PB7 () for I2C
   MPM Aug 2020
*/
#include <Wire.h>
#include <Stream.h>




void TwoWire::begin() {

}

void TwoWire::begin(uint8_t adr) {
	  // gpio config -  set pins PB6 and PB7 to I2C alternate
  rcu_periph_clock_enable(RCU_GPIOB);
  gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7); // configure I2C0 pins (default)
  // I2C device setup
  rcu_periph_clock_enable(RCU_I2C0); // start I2C clock
  i2c_clock_config(I2C0, 100000, I2C_DTCY_2); // set I2C clock freq to 100KHz
  i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, adr << 1);
  i2c_enable(I2C0); // enable it
  i2c_ack_config(I2C0,I2C_ACK_ENABLE);   /* enable acknowledge */

}

void TwoWire::end() {

}

void TwoWire::setClock(uint32_t freq) {

}
  
void TwoWire::beginTransmission(uint8_t adr) {

  while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {}; // wait for bus idle

  i2c_start_on_bus(I2C0);  // start bit
  while( ! i2c_flag_get(I2C0, I2C_FLAG_SBSEND) ) {};

  i2c_master_addressing(I2C0, (adr << 1), I2C_TRANSMITTER);

  while( ! i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) ) {};

  i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
  /* wait until the transmit data buffer is empty */
  while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE)) {};

};

uint8_t TwoWire::endTransmission(bool stopBit) {
	return endTransmission(); // FIXIT
};

uint8_t TwoWire::endTransmission(void) {
 //  if (i2c_flag_get(I2C0, I2C_FLAG_AERR)) return 2;
   i2c_stop_on_bus(I2C0);
  /* wait until stop condition is generated */
  while(I2C_CTL0(I2C0)& I2C_CTL0_STOP) {};
  i2c_ack_config(I2C0,I2C_ACK_ENABLE);
  i2c_ackpos_config(I2C0,I2C_ACKPOS_CURRENT);
  return 0;
};

uint8_t TwoWire::requestFrom(uint8_t address, size_t len, bool stopBit) {
	return requestFrom(address, len);
};

uint8_t TwoWire::requestFrom(uint8_t adr, size_t len) {

	rx_index = rx_len = 0; // clear rx buffer

  	i2c_start_on_bus(I2C0);
  	while( ! i2c_flag_get(I2C0, I2C_FLAG_SBSEND) ) {};

  	i2c_master_addressing(I2C0, (adr << 1), I2C_RECEIVER);

  	i2c_ack_config(I2C0, (len > 1)? I2C_ACK_ENABLE : I2C_ACK_DISABLE);
  
  	while( ! i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) ) {};
  
  	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

  	// Now read the bytes into the rx_buffer TODO check for buffer overrun
  	for (uint8_t i=0;  i< (uint8_t) len; i++) {

  		rx_buffer[i] = read_rx(i == (len-1)); // Nack last data read
  		rx_len++;
  	}

  	return 0;
};

//void TwoWire::onReceive(void(*)(int)) {};

//void TwoWire::onRequest(void(*)(void)){};

size_t TwoWire::write(uint8_t data) {
	 i2c_data_transmit(I2C0, data);
  while( ! i2c_flag_get(I2C0, I2C_FLAG_TBE) ) {}; // wait for send to finish
	return 0; 
};

uint8_t TwoWire::read_rx(bool ack) {
 uint8_t retval;

  while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
  retval = i2c_data_receive(I2C0);
  i2c_ack_config(I2C0, (ack)? I2C_ACK_DISABLE : I2C_ACK_ENABLE);

  return retval;
	
};

int TwoWire::read() {
 uint8_t retval = 0;
 if (rx_index < rx_len) {
 	retval = rx_buffer[rx_index++];
 }

 if (rx_index >= rx_len) {
 	rx_len = rx_index = 0; // its empty now
 };

  return retval;
	
};

int TwoWire::peek() {
	return rx_buffer[rx_index]; // TODO checks
};

void TwoWire::flush() {

};

int TwoWire::available() {
	return rx_len - rx_index; // TODO checks
};

TwoWire Wire = TwoWire();
