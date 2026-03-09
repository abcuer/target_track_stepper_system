#include "headfile.h"

void HC05_Init(void)
{
	NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
	NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
}

volatile RxState_t rx_state = WAIT_HEADER1;
uint8_t rx_buf[MAX_FRAME_LEN];
volatile uint8_t rx_index = 0;
uint8_t data_len = 0;

float bytes_to_float(uint8_t *bytes)
{
    float f;
    memcpy(&f, bytes, sizeof(float)); // 安全地拷贝
    return f;
}

void process_cmd(uint8_t cmd, uint8_t *data, uint8_t len)
{
    float angle1 = bytes_to_float(&data[0]);  // 从 data[0~3] 解出 float
    float angle2 = bytes_to_float(&data[4]);  // 从 data[4~7] 解出 float
    switch(cmd)
    {
        case 0x01:
            LED_Blue_ON();
		/*  1：绝对位置 
			0：相对位置
		*/
            Emm_V5_Move_To_Angle(1, angle1, 100, 10, data[8]);  
            Emm_V5_Move_To_Angle(2, angle2, 100, 10, data[8]);  
            break;

        case 0x02:
            LED_Blue_OFF();
            break;

        case 0x03:
            // 预留命令
            break;

        default:
            // 未知命令
            break;
    }
}

void UART_1_INST_IRQHandler(void)
{
    if(DL_UART_getPendingInterrupt(UART_1_INST) == DL_UART_IIDX_RX)
    {
        uint8_t byte = DL_UART_Main_receiveData(UART_1_INST);

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
					//rev_flag = 1;
                }
                rx_state = WAIT_HEADER1; // 准备下一帧
                break;

            default:
                rx_state = WAIT_HEADER1;
                break;
        }
    }
}
