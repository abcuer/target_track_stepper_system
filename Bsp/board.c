/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-26     LCKFB     first version
 */
#include "board.h"
#include "headfile.h"

//#define RE_0_BUFF_LEN_MAX 128

//volatile uint8_t recv0_buff[RE_0_BUFF_LEN_MAX] = {0};

void board_init(void)
{
    SYSCFG_DL_init();
}

void delay_us(unsigned long __us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;
	
    ticks = __us * (32000000 / 1000000);


    told = SysTick->VAL;

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            if (tcnt >= ticks)
                break;
        }
    }
}

void delay_ms(unsigned long ms)
{
    delay_us(ms * 1000);
}

void delay_1us(unsigned long __us) { delay_us(__us); }
void delay_1ms(unsigned long ms) { delay_ms(ms); }

#if !defined(__MICROLIB)
// ��ʹ��΢��Ļ�����Ҫ��������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
// �����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
    int handle;
};
#endif

FILE __stdout;

// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
#endif

// printf�����ض���
int fputc(int ch, FILE *stream)
{
    // ������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
    while (DL_UART_isBusy(UART_2_INST) == true)
        ;

    DL_UART_Main_transmitData(UART_2_INST, ch);

    return ch;
}




