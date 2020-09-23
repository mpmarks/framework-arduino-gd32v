// Bridge to CDC ACM functionality
// MPM Aug 2020


#include "CDC.h"

#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include "usbd_core.h"
#include <stdlib.h>
#include <string.h>
//#include "gd32vf103_it.h"

extern uint8_t packet_receive;
extern uint32_t  receive_length;

#define USB_BUFFER_SIZE 1024

typedef struct{
    uint8_t buffer[USB_BUFFER_SIZE];
    uint32_t max_size;
    uint32_t index;
}USB_serial_buffer_t;

USB_serial_buffer_t usb_buffer = { .buffer = "", .max_size = USB_BUFFER_SIZE, .index = 0};
uint8_t rcvstr[CDC_ACM_DATA_PACKET_SIZE] = {0};

/* function declarations */
/* this function handles USB wakeup interrupt handler */
extern void USBFS_WKUP_IRQHandler(void);
/* this function handles USBFS IRQ Handler */
extern void USBFS_IRQHandler(void);

extern  uint8_t packet_sent, packet_receive;

// This is a dummy to force linker to include USB IRQ handlers
void MPM_TEST(void) {
  USBFS_IRQHandler();
}

usb_core_driver USB_OTG_dev =
{
    .dev = {
        .desc = {
            .dev_desc       = (uint8_t *)&device_descriptor,
            .config_desc    = (uint8_t *)&configuration_descriptor,
            .strings        = usbd_strings,
        }
    }
};


void CDC_Init() {

// Initialize USB
    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);
    usb_rcu_config();
    usb_timer_init();
    usb_intr_config();
    usbd_init (&USB_OTG_dev, USB_CORE_ENUM_FS, &usbd_cdc_cb);
    // wait until USB device is enumerated successfully
    while (USBD_CONFIGURED != USB_OTG_dev.dev.cur_status) {};
}


void CDC_deInit() {

}

#define TIMEOUT_SEND 1000000

// Crude: Sends data to host
size_t CDC_write(uint8_t* buff, size_t len) {
//  while (packet_sent == 0){}; //wait for last write to finish
//int64_t temp = get_timer_value();
//  while((volatile uint8_t) packet_sent == 0 && temp + TIMEOUT_SEND > get_timer_value()) {};

  packet_sent = 0;
  usbd_ep_send(&USB_OTG_dev, CDC_ACM_DATA_IN_EP, (uint8_t*) buff, len);
  // wait for send to complete
//  while( (volatile uint8_t) packet_sent == 0) {};

// Wait for send to complete

 while (packet_sent == 0 ) {
  //    usbd_ep_send(&USB_OTG_dev, CDC_ACM_DATA_IN_EP, NULL, 0); // send null packet
      usb_mdelay(10);
    }

  return len;
}

// Read from EP into long buffer and return with data written to buffer and len
size_t CDC_recv(uint8_t* data) {
  size_t len  = 0;
  usb_buffer.index = 0;

    while (packet_receive) {
    if (receive_length > 0) {  // copy from EP into buffer
      memcpy(&usb_buffer.buffer[usb_buffer.index], rcvstr, receive_length);
      usb_buffer.index += receive_length;
      len += receive_length;
    }

    packet_receive = 0;
    receive_length = 0;
    usbd_ep_recev(&USB_OTG_dev, CDC_ACM_DATA_OUT_EP, rcvstr, CDC_ACM_DATA_PACKET_SIZE);
    usb_mdelay(1);
  }
//  usb_buffer.buffer[usb_buffer.index++] = '\0';
  if (len>0) memcpy(data, usb_buffer.buffer, usb_buffer.index);

  return len;
}
