/****************************
 *     author: Chienheng
 *      data :  2020/11/20 
*****************************/

#ifndef __USART_PROPERTY__H__
#define __USART_PROPERTY__H__
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   /*文件控制定义*/
#include <termios.h> /*PPSIX 终端控制定义*/
#include <errno.h>   /*错误号定义*/
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define UART_DATA_LEN   1
#define MAX_BUFFER      1024
#define RD_MODE (O_RDONLY | O_NOCTTY)
#define WR_MODE (O_WRONLY | O_NOCTTY)
#define RW_MODE (O_RDWR | O_NOCTTY)

enum parityEnum
{
        EVEN,
        ODD,
        PA_NONE
};

enum hw_control 
{
        HARDWARE,
        SOFTWARE,
        HS_NONE
};

typedef struct Usart
{
        char *dev;
        int fd;
        u_int16_t rw_flag;
        struct termios options;
        struct termios old_options;
        ssize_t total_recv;
        ssize_t total_send;
} usart_t;



__always_inline void flag_set(usart_t *usart, u_int16_t rw_flag)
{
        usart->rw_flag = rw_flag;
}

__always_inline void set_char_size (usart_t *usart)
{
        usart->options.c_cflag |= CLOCAL | CREAD;
}

__always_inline void databit_set(usart_t *usart, u_int8_t databit)
{
        usart->options.c_cflag &= ~CSIZE;
        usart->options.c_cflag |= databit;
        
}

__always_inline void hareware_control (usart_t *usart, u_int8_t hs_ctl)
{
        if (hs_ctl == HARDWARE)
                usart->options.c_cflag |= CRTSCTS;
        else if (hs_ctl == SOFTWARE)
                usart->options.c_cflag |= IXON|IXOFF|IXANY;
        else if (hs_ctl == HS_NONE)
                usart->options.c_cflag &= ~CRTSCTS;
}       

__always_inline void parity_set(usart_t *usart, u_int8_t parity)
{
        switch (parity)
        {
        case ODD:
                usart->options.c_cflag |= PARENB;
                usart->options.c_cflag |= PARODD;
                usart->options.c_iflag |= (INPCK | ISTRIP);
                break;
        
        case EVEN:
                usart->options.c_cflag |= (INPCK | ISTRIP);
                usart->options.c_cflag |= PARENB;
                usart->options.c_cflag &= ~PARODD;
                break;

        case PA_NONE:
                usart->options.c_cflag &= ~PARENB;
                usart->options.c_cflag &= ~CRTSCTS;
                break;
        }

}

__always_inline void baudrate_set(usart_t *usart, speed_t budrate)
{
        cfsetispeed (&usart->options, budrate);
        cfsetospeed (&usart->options, budrate);
}

__always_inline void stop_bit_set(usart_t *usart, u_int8_t stop_bit)
{
        if (stop_bit == 1)
                usart->options.c_cflag &= ~CSTOPB;
        else if (stop_bit == 2)
                usart->options.c_cflag |= CSTOPB;
}

__always_inline void vmin_vtime_set (usart_t *usart, u_int8_t vmin, u_int8_t vtime)
{
        usart->options.c_cc[VMIN] = 0;
        usart->options.c_cc[VTIME] = 0;
}

__always_inline size_t get_send_bytes(usart_t * usart) { return usart->total_send; }

__always_inline size_t get_recv_bytes(usart_t * usart) { return usart->total_recv; }



#endif //!__USART_PROPERTY__H__