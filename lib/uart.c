/****************************
 *     author: Chienheng
 *      data :  2020/11/20 
*****************************/
#include "uart.h"

void open_uart (usart_t * uart, const char *dev, int flag)
{
        uart->fd = open (dev, flag);

        if (uart->fd == -1)
        {
                perror ("open uart error");
                exit (EXIT_FAILURE);
        }
}

void set_usart 
(
        usart_t * usart, u_int8_t databit, u_int8_t hs_ctl,
        u_int8_t parity, speed_t budrate, u_int8_t stop_bit, 
        u_int8_t vmin, u_int8_t vtime)

{
        if (tcgetattr (usart->fd, &usart->old_options) == -1)
        {
                perror ("get usart attr failed");
                exit (EXIT_FAILURE);
        }
        
        set_char_size (usart);
        databit_set (usart, databit);
        hareware_control (usart, hs_ctl);
        parity_set (usart, parity);
        baudrate_set (usart, budrate);
        stop_bit_set (usart, stop_bit);
        vmin_vtime_set (usart, vmin, vtime);
        usart->options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        usart->options.c_oflag &= ~OPOST;
        usart->options.c_iflag &= ~(IXON | IXOFF | IXANY);
        usart->options.c_iflag &= ~(ICRNL|IGNCR);
        tcflush (usart->fd, TCIFLUSH);
        
        if ( tcsetattr (usart->fd, TCSANOW, &usart->options) == -1)
        {
                perror ("usart set new attr failed");
                exit (EXIT_FAILURE);
        }
        
        printf ("usart set done.\n");
}

void close_usart (usart_t * usart)
{
        close (usart->fd);
}

void read_serial_char (usart_t *com, char * buf, char end)
{
        ssize_t recv = 0;

        while (1)
        {
                recv += read (com->fd, buf + recv, UART_DATA_LEN);
                if (recv == -1)
                        perror ("read");
                else if ( buf[recv - 1] == end )
                        break;
                else if (recv > MAX_BUFFER)
                        break;
        }
        
}

void read_serial_hex (usart_t * com, u_int8_t * buf, u_int8_t end)
{
        ssize_t recv = 0;

        while (1)
        {
                recv += read (com->fd, buf + recv, UART_DATA_LEN);
                if (recv == -1)
                {
                        perror ("read");
                        break;
                }
                else if ( buf[recv - 1] == end )
                        break;
                else if (recv > MAX_BUFFER)
                        break;
        }
}

void write_serial_char (usart_t *com, const char * buf, u_int32_t len)
{
        ssize_t send_size = 0;

        while (1)
        {
                send_size += write (com->fd, buf + send_size, UART_DATA_LEN);

                if (send_size == -1)
                        perror ("write");
                else if (send_size == len)
                        break;
        }
}

void write_serial_hex (usart_t *com, const u_int8_t * buf, u_int32_t len)
{
        ssize_t send_size = 0;

        while (1)
        {
                send_size += write (com->fd, buf + send_size, UART_DATA_LEN);

                if (send_size == -1)
                        perror ("write");
                else if (send_size == len)
                        break;
        }
}

void show_serial_data (const char * buf, u_int32_t len)
{
        printf("recv: ");
        for (int i = 0; i < len; i++)
                printf("%c ", buf[i]);

        printf("\n");
}

void show_serial_data_hex (const u_int8_t * buf, u_int32_t len)
{
        if (len == 0)
        {
                printf ("None data.\n");
                return;
        }
        printf("recv: ");
        for (int i = 0; i < len; i++)
        {
                if (buf[i] < 0x10)
                        printf("0");
                printf("%X ", buf[i]);
        }
        printf("\n");
}

