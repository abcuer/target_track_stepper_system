#include "headfile.h"

void uart0_Init(void)
{
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

void uart0_send_char(char ch)
{
    while (DL_UART_isBusy(UART_0_INST) == true);
    DL_UART_Main_transmitData(UART_0_INST, ch);
}

void uart0_send_string(char *str)
{

    while (*str != 0 && str != 0)
    {
        uart0_send_char(*str++);
    }
}

void uart0_SendByte(uint8_t data)
{
    while (DL_UART_isBusy(UART_0_INST) == true);
    DL_UART_Main_transmitData(UART_0_INST, data);
}

void uart0_SendCmd(uint8_t *cmd, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        uart0_SendByte(cmd[i]);
    }
}

#define HEADER1 0xAA
#define HEADER2 0x55
#define TAIL1   0x0D
#define TAIL2   0x0A
#define MAX_FRAME_LEN 256

/*  AA 55 01 00 01 0D 0A  */
typedef enum {
    WAIT_HEADER1,
    WAIT_HEADER2,
    WAIT_CMD, // 任务号
    WAIT_LEN,
    WAIT_DATA,
    WAIT_CHECKSUM,
    WAIT_TAIL1,
    WAIT_TAIL2
} RxState_t;

volatile RxState_t rx_state = WAIT_HEADER1;
volatile uint8_t rx_buf[MAX_FRAME_LEN];
volatile uint8_t rx_index = 0;
volatile uint8_t data_len = 0;

void process_cmd(uint8_t cmd, uint8_t *data, uint8_t len)
{
    switch(cmd)
    {
        case 0x01:  // 启动指令示例
            // 执行启动动作
			LED_Blue_ON();
            break;

        case 0x02:  // 停止指令示例
            LED_Blue_OFF();
            break;

        case 0x03:  // 设置速度，data[0]为速度值
            break;

        // 其他命令处理...

        default:
            // 未知命令
            break;
    }
}

void UART_0_INST_IRQHandler(void)
{
    if(DL_UART_getPendingInterrupt(UART_0_INST) == DL_UART_IIDX_RX)
    {
        uint8_t byte = DL_UART_Main_receiveData(UART_0_INST);

        switch(rx_state)
        {
            case WAIT_HEADER1:
                if(byte == HEADER1) rx_state = WAIT_HEADER2;
                break;

            case WAIT_HEADER2:
                if(byte == HEADER2) rx_state = WAIT_CMD;
                else rx_state = WAIT_HEADER1;
                break;

            case WAIT_CMD:
                rx_buf[0] = byte; // cmd
                rx_state = WAIT_LEN;
                break;

            case WAIT_LEN:
                data_len = byte;
                if(data_len > MAX_FRAME_LEN - 6)
                    rx_state = WAIT_HEADER1; // 长度异常重置
                else
                {
                    rx_index = 0;
                    rx_state = (data_len > 0) ? WAIT_DATA : WAIT_CHECKSUM;
                }
                break;

            case WAIT_DATA:
                rx_buf[1 + rx_index++] = byte;
                if(rx_index >= data_len) rx_state = WAIT_CHECKSUM;
                break;

            case WAIT_CHECKSUM:
            {
                uint8_t checksum = rx_buf[0] ^ data_len;
                for(uint8_t i=0; i<data_len; i++)
                    checksum ^= rx_buf[1 + i];

                if(checksum == byte)
                    rx_state = WAIT_TAIL1;
                else
                    rx_state = WAIT_HEADER1; // 校验失败丢弃
                break;
            }

            case WAIT_TAIL1:
                if(byte == TAIL1) rx_state = WAIT_TAIL2;
                else rx_state = WAIT_HEADER1;
                break;

            case WAIT_TAIL2:
                if(byte == TAIL2)
                {
                    // 完整帧接收成功，调用命令处理
                    process_cmd(rx_buf[0], (uint8_t *)&rx_buf[1], data_len);
                }
                rx_state = WAIT_HEADER1; // 准备下一帧
                break;

            default:
                rx_state = WAIT_HEADER1;
                break;
        }
    }
}

