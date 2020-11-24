/****************************
 *     author: Chienheng
 *      data :  2020/11/20 
*****************************/
#ifndef __UART__H__
#define __UART__H__
#include "usart_property.h"

/*
 *@func         open_uart
 *@description: 
 *              open the uart dev
 *@param        uart: uart's structure
 *@param        dev: uart's name(from /dev/)
 *@param        flag: open mode
 *              RW_MODE, WR_MODE, RD_MODE
*/
void open_uart (usart_t *uart, const char *dev,int flag);

/*
 * @func        set_usart 
 * @description: 
 *              set the uart's attr
 * @param       usart: uart's structure
 * @param       databit: set the databit
 *                      CS7, CS8
 * @param       hs_ctl: set the stream control
 *                      HARDWARE, SOFTWARE, HS_NONE
 * @param       parity: set the parity bits
 *                      EVEN, ODD, PA_NONE
 * @param       buadrate: set the buadrate
 *                      BXXXXX, e.g: B115200
 * @param       stop_bit: 1, 2
 * @param       vmin: the minium byte for read
 * @param       vtime: the minium sec for read
 * 
*/
void set_usart 
(
        usart_t * usart, u_int8_t databit, u_int8_t hs_ctl,
        u_int8_t parity, speed_t buadrate, u_int8_t stop_bit, 
        u_int8_t vmin, u_int8_t vtime);

void close_usart (usart_t * usart);
/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void read_serial_char (usart_t *com, char * buf, char end);

/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void read_serial_hex (usart_t *com, u_int8_t * buf, u_int8_t end);

/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void write_serial_char (usart_t *com, const char * buf, u_int32_t len);

/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void write_serial_hex (usart_t *com, const u_int8_t * buf, u_int32_t len);

/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void show_serial_data (const char * buf, u_int32_t len);

/*
 * @func
 * @description
 * @param
 * @param
 * @param
 * 
*/
void show_serial_data_hex (const u_int8_t * buf, u_int32_t len);




#endif  //!__UART__H__