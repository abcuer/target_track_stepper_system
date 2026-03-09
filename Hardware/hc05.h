#ifndef _hc05_h
#define _hc05_h

#define HEADER1 0xAA
#define HEADER2 0x55
#define TAIL1   0x0D
#define TAIL2   0x0A
#define MAX_FRAME_LEN 100

/*  AA 55 01 00 01 0D 0A  */
typedef enum {
    WAIT_HEADER1,
    WAIT_HEADER2,
    WAIT_CMD, // ÈÎÎñºÅ
    WAIT_LEN,
    WAIT_DATA,
    WAIT_CHECKSUM,
    WAIT_TAIL1,
    WAIT_TAIL2
} RxState_t;

void HC05_Init(void);
void process_cmd(uint8_t task, uint8_t *data, uint8_t len);


#endif