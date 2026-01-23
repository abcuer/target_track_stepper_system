#include "usart.h"
#include "board.h"
#include "led.h"
#include "motor.h"

volatile RxState_t rx_state = WAIT_HEADER1;
volatile uint8_t rx_buf[MAX_FRAME_LEN];
volatile uint8_t rx_index = 0;
volatile uint8_t data_len = 0;

void UsartDeviceInit(void)
{
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

static void UartCmd(uint8_t cmd, uint8_t *data, uint8_t len)
{
    switch(cmd)
    {
        case 0x01:  // 启动指令示例
            // 执行启动动作
			LED_Blue_ON();
			StepXSetAngle(data[0]);
			StepYSetAngle(data[1]);
            LED_Blue_OFF();
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
                    UartCmd(rx_buf[0], (uint8_t *)&rx_buf[1], data_len);
                }
                rx_state = WAIT_HEADER1; 
                break;

            default:
                rx_state = WAIT_HEADER1;
                break;
        }
    }
}

