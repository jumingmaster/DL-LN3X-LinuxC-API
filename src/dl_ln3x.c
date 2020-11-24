#include "dl_ln3x.h"
static dl_ln3x_t check_dl_ln3x_data (u_int8_t * data)
{
        if (!data)
                return INVALID_ARGUMENT;
        
        dl_ln3x_t index = 0;
        
        if (data[index] != LN3X_PACKET_HEADER)
                return MSG_HEADER_ERR;
        else
        {
                u_int8_t packet_len =  data[++index];
        }
}

static void get_nID (dl_ln3x * dev)
{

}

static void get_addr (dl_ln3x * dev)
{

}

static void get_chn (dl_ln3x * dev)
{
        dl_ln3x_packet packet;
        u_int8_t data[MAX_PACKET_SIZE];

        write_serial_hex (dev->uart, GET_INFO_CMD[GET_CHL_CMD], check_DLLN3x_data_len (GET_INFO_CMD[GET_CHL_CMD]));
        usleep (100);
        read_serial_hex (dev->uart, data, LN3X_PACKET_TAIL);
        
        
}

dl_ln3x_t init_dl_ln3x (dl_ln3x ** dev)
{
        *dev = malloc (sizeof (dl_ln3x));
        (*dev)->property = malloc (sizeof (dl_ln3x_property));
        (*dev)->uart = malloc (sizeof (usart_t));

        if (!(*dev) || !(*dev)->uart || !(*dev)->property)
                return ALLOC_MEM_ERR;
        else
        {
                (*dev)->state = TRUE;
                return SUCCESS;
        }
}

dl_ln3x_t open_dl_ln3x (dl_ln3x * dev, const char * __dev_name)
{
        if (!dev)
                return INVALID_ARGUMENT;
        if (dev->state != TRUE)
                return NONE_INIT_ERR;
        
        dev->uart->fd = open (__dev_name, LN3X_OPEN_FlAG);
        if (dev->uart->fd == -1)
        {
                perror ("Open DL-LN3X Failed");
                return OPEN_SERIAL_ERR;
        }

        set_usart (
                dev->uart, LN3X_DATA_BIT, LN3X_HS_CON, 
                LN3X_PARITY, LN3X_BUADRATE, LN3X_STOP_BIT, 
                1, 0);
        
        get_chn (dev);

}

dl_ln3x_t close_dl_ln3x (dl_ln3x * dev)
{
        if (!dev)
                return INVALID_ARGUMENT;
        if (dev->state != TRUE)
                return NONE_INIT_ERR;
        
        close_usart (dev->uart);

}

dl_ln3x_t check_DLLN3x_data_len (const u_int8_t *buf)
{
        if (buf[0] != LN3X_PACKET_HEADER)
                return 0;

        int len = 0;
        for (int i = 0; i < MAX_BUFFER; i++)
        {
                if (buf[i] == 0xFF)
                {
                        len++;
                        break;
                }
                else 
                        len++;
        }

        return len;
}

dl_ln3x_t dl_ln3x_read_data (dl_ln3x * dev, dl_ln3x_packet *packet)
{
        u_int8_t data[MAX_PACKET_SIZE];
        read_serial_hex (dev->uart, data, LN3X_PACKET_TAIL);
        
}