#include "uart.h"
#include "ti_msp_dl_config.h"
#include "led.h"
#include "motor.h"
#include "soundlight.h"

// 使用静态全局变量保存协议解析状态，避免中断退出后数据丢失
static RxState_t g_rx_state = WAIT_HEADER1;
static uint8_t  g_rx_buf[MAX_FRAME_LEN];
static uint8_t  g_rx_index = 0;
static uint8_t  g_data_len = 0;
// 判断步进电机是否追踪到对象
volatile uint8_t g_is_tracking = 0;

void UartDeviceInit(void)
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

/**
 * @brief UART 协议指令执行函数
 * @param cmd 指令字节
 * @param data 数据负载指针
 * @param len 数据负载长度
 */
static void UART_ExecuteCommand(uint8_t cmd, uint8_t *data, uint8_t len)
{
    switch(cmd)
    {
		case 0x01:  // 电机同步控制指令
			if (len >= 2) 
			{
				// 坐标映射成角度
				float stepX_angle = (data[0] * 360.0f / 255.0f) - 180.0f;
				float stepY_angle = (data[1] * 360.0f / 255.0f) - 180.0f;
				StepMotor_SetAngle(&StepX, stepX_angle);
				StepMotor_SetAngle(&StepY, stepY_angle);
				SoundLightRun();
				// 标记：开始向新目标移动，等待到达
				g_is_tracking = 1;
			}
			break;
            
			default:
				break;
    }
}

/**
 * @brief UART 数据流解析状态机
 * @param byte 接收到的单个字节
 */
static void UART_ParseData(uint8_t byte)
{
    switch(g_rx_state)
    {
        case WAIT_HEADER1:
            if(byte == HEADER1) g_rx_state = WAIT_HEADER2;
            break;

        case WAIT_HEADER2:
            g_rx_state = (byte == HEADER2) ? WAIT_CMD : WAIT_HEADER1;
            break;

        case WAIT_CMD:
            g_rx_buf[0] = byte; // 暂存 Command
            g_rx_state = WAIT_LEN;
            break;

        case WAIT_LEN:
            // 增加缓冲区溢出保护
            if(byte > (MAX_FRAME_LEN - 6)) {
                g_rx_state = WAIT_HEADER1; 
            } else {
                g_data_len = byte;
                g_rx_index = 0;
                g_rx_state = (g_data_len > 0) ? WAIT_DATA : WAIT_CHECKSUM;
            }
            break;

        case WAIT_DATA:
            g_rx_buf[1 + g_rx_index++] = byte;
            if(g_rx_index >= g_data_len) g_rx_state = WAIT_CHECKSUM;
            break;

        case WAIT_CHECKSUM:
        {
            // 校验和计算：CMD ^ LEN ^ DATA...
            uint8_t checksum = g_rx_buf[0] ^ g_data_len;
            for(uint8_t i = 0; i < g_data_len; i++) {
                checksum ^= g_rx_buf[1 + i];
            }

            if(checksum == byte) g_rx_state = WAIT_TAIL1;
            else                 g_rx_state = WAIT_HEADER1;
            break;
        }

        case WAIT_TAIL1:
            g_rx_state = (byte == TAIL1) ? WAIT_TAIL2 : WAIT_HEADER1;
            break;

        case WAIT_TAIL2:
            if(byte == TAIL2) {
                // 解析完成，调用指令执行函数
                UART_ExecuteCommand(g_rx_buf[0], &g_rx_buf[1], g_data_len);
            }
            g_rx_state = WAIT_HEADER1; 
            break;

        default:
            g_rx_state = WAIT_HEADER1;
            break;
    }
}

void UART_0_INST_IRQHandler(void)
{
    // 检查是否为接收中断
    if(DL_UART_getPendingInterrupt(UART_0_INST) == DL_UART_IIDX_RX)
    {
        UART_ParseData(DL_UART_Main_receiveData(UART_0_INST));
    }
}
