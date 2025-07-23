#ifndef _uart0_h
#define _uart0_h

#include "ti_msp_dl_config.h"
void uart0_Init(void);
void uart0_send_char(char ch);
void uart0_send_string(char *str);
void uart0_SendByte(uint8_t data);
void uart0_SendCmd(uint8_t *cmd, uint8_t len);

void process_cmd(uint8_t task, uint8_t *data, uint8_t len);


#endif