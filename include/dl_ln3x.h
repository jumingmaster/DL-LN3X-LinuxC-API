#ifndef __DL_LN3X__H__
#define __DL_LN3X__H__
#include "uart.h"
#include <stdarg.h>
#include <sys/types.h>

#define DEFAULT_PACKET_LEN      4
#define MIN_ADDR                0x0001
#define MAX_ADDR                0xFFFE
#define MIN_CHANNEL             0x0B
#define MAX_CHANNEL             0x1A
#define MIN_NET_ID              MIN_ADDR
#define MAX_NET_ID              MAX_ADDR
#define MAX_PACKET_SIZE         0x3F
#define MAX_VAR_DATA_SIZE       (u_int8_t)(MAX_PACKET_SIZE - DEFAULT_PACKET_LEN)
#define MIN_COM_PORT            0x80
#define MAX_COM_PORT            0xFF
#define MIN_IN_PORT             0x00
#define MAX_IN_PORT             0x7F
#define LN3X_OPEN_FlAG          RW_MODE
#define LN3X_DATA_BIT           CS8
#define LN3X_BUADRATE           B115200
#define LN3X_STOP_BIT           1
#define LN3X_HS_CON             HS_NONE
#define LN3X_PARITY             PA_NONE
#define TRUE                    1
#define FALSE                   0
#define LOCAL_ADDR              0x0000
#define BLINK_PORT              0x20
#define INFO_PORT               0x21
#define ERR_PORT                0x22
#define LINK_QUALITY_PORT       0x23
#define TTL_PIN4_PORT           0x44
#define TTL_PIN5_PORT           0x45
#define LN3X_PACKET_HEADER      0xFE
#define LN3X_PACKET_TAIL        0xFF

typedef int dl_ln3x_t;
typedef unsigned char dlboolean;

enum dl_ln3x_err
{
        SUCCESS,
        ALLOC_MEM_ERR,
        NONE_INIT_ERR,
        OPEN_SERIAL_ERR,
        MSG_FORMAT_ERR,
        CHANNEL_ERR,
        ADDR_ERR,
        COM_PORT_ERR,
        PORT_ERR,
        IN_PORT_ERR,
        NET_ID_ERR,
        INVALID_ARGUMENT,
        MSG_LEN_ERR,
        MSG_HAND_ERR,
        MSG_HEADER_ERR
};

enum dl_ln3x_set_info_response_code
{
        DONE = 0x00,
        INVALID_PORT = 0xF0,
        ERROR_CMD = 0xF8,
        ERROR_LEN = 0xF9,
        INVALID_VAL = 0xFA
};

enum dl_ln3x_set_info_cmd
{
        SET_ADDR = 0x11,
        SET_NID = 0x12,
        SET_CHN = 0x13,
        SET_BUAD = 0x14
};

enum dl_ln3x_set_info_ret_code
{
        ADDR_RET = 0x21,
        NID_RET = 0x22,
        CHN_RET = 0x23,
        BUAD_RET = 0x24
};

enum dl_ln3x_set_info_err
{
        ADDR_RET_ERR = 0x1000,
        NID_RET_ERR,
        CHN_RET_ERR,
        BUAD_RET_ERR
};

typedef enum dl_ln3x_cmd
{
        GET_ADDR,
        GET_NID,
        GET_CHN,
        GET_BUAD
}dl_ln3x_cmd_t;



enum dl_ln3x_packet_pos
{
        POS_HEADER,
        POS_LENTH,
        POS_SRC_PORT,
        POS_DEST_PORT,
        POS_ADDR,
        POS_DATA = 6
};

static const u_int8_t GET_INFO_CMD[10][8] = 
{
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x01, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x02, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x03, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x04, 0xFF}
};

#define GET_ADDR_CMD        0
#define GET_NID_CMD         1
#define GET_CHL_CMD         2
#define GET_BUAD_CMD        3


typedef struct dl_ln3x_property
{
        u_int16_t dev_addr;
        u_int8_t channel;
        u_int16_t net_ID;
        u_int32_t buadrate;
        u_int8_t com_port;       
}dl_ln3x_property;

typedef struct dl_ln3x
{
        usart_t *uart;
        dl_ln3x_property *property;
        dlboolean state;
}dl_ln3x;

typedef struct dl_ln3x_packet
{
        u_int8_t header;
        u_int8_t len;
        u_int8_t src_port;
        u_int8_t dest_port;
        u_int16_t addr;
        u_int8_t *data;
        u_int8_t tail;
}__attribute__ ((packed)) dl_ln3x_packet;

dl_ln3x_t init_dl_ln3x (dl_ln3x ** dev);

dl_ln3x_t open_dl_ln3x (dl_ln3x * dev, const char * __dev_name);

dl_ln3x_t close_dl_ln3x (dl_ln3x * dev);

dl_ln3x_t check_DLLN3x_data_len (const u_int8_t *buf);

dl_ln3x_t dl_ln3x_read_data (dl_ln3x * dev, dl_ln3x_packet *packet);

dl_ln3x_t dl_ln3x_extract_data (dl_ln3x_cmd_t cmd, dl_ln3x * dev, dl_ln3x_packet * packet);

void show_dl_ln3x_property (dl_ln3x_property * property);

#endif  //!__DL_LN3X__H__