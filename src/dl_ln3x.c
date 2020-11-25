#include "dl_ln3x.h"
static u_int32_t trans_buadrate (u_int8_t ret_code)
{
        switch (ret_code)
        {
        case 0x00:
                return 2400;
        case 0x01:
                return 4800;
        case 0x02:
                return 9600;
        case 0x03:
                return 14400;
        case 0x04:
                return 19200;
        case 0x05:
                return 28800;
        case 0x06:
                return 38400;
        case 0x07:
                return 57600;
        case 0x08:
                return 115200;
        case 0x09:
                return 230400;
        case 0x0A:
                return 125000;
        case 0x0B:
                return 250000;
        case 0x0C:
                return 500000;
        }
}

static inline dl_ln3x_t check_dl_ln3x_com (u_int8_t com, dlboolean inside)
{
        switch (inside)
        {
        case TRUE:
                if (com > MAX_IN_PORT || com < MIN_IN_PORT)
                        return IN_PORT_ERR;
                break;
        case FALSE:
                if (com > MAX_COM_PORT || com < MIN_COM_PORT)
                        return COM_PORT_ERR;
                break;
        }
        
        return SUCCESS;
}

static inline dl_ln3x_t check_dl_ln3x_data_head (u_int8_t * data)
{
        if (!data)
                return INVALID_ARGUMENT;
        
        if (data[0] != LN3X_PACKET_HEADER)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return MSG_HEADER_ERR;
        }
        return SUCCESS;
}

static void pack_read_data (u_int8_t * data, dl_ln3x_packet * packet)
{
        dl_ln3x_t index;

        packet->header = data[POS_HEADER];
        packet->len = data[POS_LENTH];
        packet->src_port = data[POS_SRC_PORT];
        packet->dest_port = data[POS_DEST_PORT];
        packet->addr = data[POS_ADDR];
        packet->data = malloc ( packet->len - 4 );
        
        for (index = 0; index < packet->len - 4; index++)
                packet->data[index] = data[POS_DATA + index];

        packet->tail = data[POS_DATA + index];
}

static u_int8_t * pack_send_data (dl_ln3x *dev, u_int8_t *data, u_int8_t len, u_int8_t dest_port, u_int16_t dest_addr)
{
        u_int8_t packet_len = len + 4;
        
        u_int8_t *packet = malloc (sizeof (dl_ln3x_packet) - sizeof (void *) + len);
        if (!packet)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return NULL;
        }

        packet[POS_HEADER] = LN3X_PACKET_HEADER;
        packet[POS_LENTH] = packet_len;
        packet[POS_SRC_PORT] = dev->property->com_port;
        packet[POS_DEST_PORT] = dest_port;
        packet[POS_ADDR] = dest_addr >> 8;
        packet[POS_ADDR + 1] = dest_addr & 0xFF;
        memcpy (&packet[POS_DATA], data, len);
        packet[POS_DATA + len] = LN3X_PACKET_TAIL;
        return packet;
}

static void get_nID (dl_ln3x * dev)
{
        dl_ln3x_packet packet;
        u_int8_t data[MAX_PACKET_SIZE];

        write_serial_hex (dev->uart, GET_INFO_CMD[GET_NID_CMD], check_DLLN3x_data_len (GET_INFO_CMD[GET_NID_CMD]));
        usleep (100);

        dl_ln3x_read_data (dev, &packet);
        dl_ln3x_extract_data (GET_NID, dev, &packet);
}

static void get_addr (dl_ln3x * dev)
{
        dl_ln3x_packet packet;
        u_int8_t data[MAX_PACKET_SIZE];

        write_serial_hex (dev->uart, GET_INFO_CMD[GET_ADDR_CMD], check_DLLN3x_data_len (GET_INFO_CMD[GET_ADDR_CMD]));
        usleep (100);

        dl_ln3x_read_data (dev, &packet);
        dl_ln3x_extract_data (GET_ADDR, dev, &packet);
}

static void get_chn (dl_ln3x * dev)
{
        dl_ln3x_packet packet;
        u_int8_t data[MAX_PACKET_SIZE];

        write_serial_hex (dev->uart, GET_INFO_CMD[GET_CHL_CMD], check_DLLN3x_data_len (GET_INFO_CMD[GET_CHL_CMD]));
        usleep (100);

        dl_ln3x_read_data (dev, &packet);
        dl_ln3x_extract_data (GET_CHN, dev, &packet);
        
}

static void get_buadrate (dl_ln3x * dev)
{
        dl_ln3x_packet packet;
        u_int8_t data[MAX_PACKET_SIZE];

        write_serial_hex (dev->uart, GET_INFO_CMD[GET_BUAD_CMD], check_DLLN3x_data_len (GET_INFO_CMD[GET_BUAD_CMD]));
        usleep (100);

        dl_ln3x_read_data (dev, &packet);
        dl_ln3x_extract_data (GET_BUAD, dev, &packet);
        
}

dl_ln3x_t init_dl_ln3x (dl_ln3x ** dev)
{
        *dev = malloc (sizeof (dl_ln3x));
        (*dev)->property = malloc (sizeof (dl_ln3x_property));
        (*dev)->uart = malloc (sizeof (usart_t));

        if (!(*dev) || !(*dev)->uart || !(*dev)->property)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return ALLOC_MEM_ERR;
        }
        else
        {
                (*dev)->state = TRUE;
                return SUCCESS;
        }
}

dl_ln3x_t open_dl_ln3x (dl_ln3x * dev, const char * __dev_name)
{
        if (!dev)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return INVALID_ARGUMENT;
        }
        if (dev->state != TRUE)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return NONE_INIT_ERR;
        }
        dev->uart->fd = open (__dev_name, LN3X_OPEN_FlAG);
        if (dev->uart->fd == -1)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return OPEN_SERIAL_ERR;
        }

        set_usart (
                dev->uart, LN3X_DATA_BIT, LN3X_HS_CON, 
                LN3X_PARITY, LN3X_BUADRATE, LN3X_STOP_BIT, 
                1, 0);
        get_addr (dev);
        get_nID (dev);
        get_chn (dev);
        get_buadrate (dev);
        show_dl_ln3x_property (dev->property);
}

dl_ln3x_t close_dl_ln3x (dl_ln3x * dev)
{
        if (!dev)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return INVALID_ARGUMENT;
        }
        if (dev->state != TRUE)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return NONE_INIT_ERR;
        }
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
        dl_ln3x_t err;

        read_serial_hex (dev->uart, data, LN3X_PACKET_TAIL);
        err = check_dl_ln3x_data_head (data);
        if (err != SUCCESS)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return err;
        }
        pack_read_data (data, packet);
        return SUCCESS;
}

dl_ln3x_t dl_ln3x_extract_data (dl_ln3x_cmd_t cmd, dl_ln3x * dev, dl_ln3x_packet * packet)
{
        switch (cmd)
        {
        case GET_CHN:
                if (packet->data[0] != CHN_RET)
                {
                        #if ERR_DEBUG
                        fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                        #endif
                        return CHN_RET_ERR;
                }
                dev->property->channel = packet->data[1];
                break;

        case GET_ADDR:
                if (packet->data[0] != ADDR_RET)
                {
                        #if ERR_DEBUG
                        fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                        #endif
                        return ADDR_RET_ERR;
                }
                dev->property->dev_addr = packet->data[1] | (packet->data[2] << 8);
                break;
        
        case GET_NID:
                if (packet->data[0] != NID_RET)
                {
                        #if ERR_DEBUG
                        fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                        #endif
                        return NID_RET_ERR;
                }
                dev->property->net_ID = packet->data[1] | (packet->data[2] << 8);
                break;

        case GET_BUAD:
                if (packet->data[0] != BUAD_RET)
                {
                        #if ERR_DEBUG
                        fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                        #endif
                        return BUAD_RET_ERR;
                }
                dev->property->buadrate = trans_buadrate (packet->data[1]);
                break;
        }

        return SUCCESS;
}

dl_ln3x_t dl_ln3x_set_com (dl_ln3x *dev, u_int8_t com, dlboolean inside)
{
        dl_ln3x_t err;
        err = check_dl_ln3x_com (com, inside);
        if (err != SUCCESS)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return err;
        }
        
        dev->property->com_port = com;

        return SUCCESS;
}

dl_ln3x_t dl_ln3x_send_data (dl_ln3x *dev, u_int8_t *data, u_int8_t len, u_int8_t dest_port, u_int16_t dest_addr)
{
        if (!dev)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return INVALID_ARGUMENT;
        }

        u_int8_t *packet;
        packet = pack_send_data (dev, data, len, dest_port, dest_addr);
        if ( !packet)
        {
                #if ERR_DEBUG
                fprintf (stderr, "In %s (%d)%s: ",__FILE__, __LINE__, __FUNCTION__);
                #endif
                return ALLOC_MEM_ERR;
        }
        
        write_serial_hex (dev->uart, packet, check_DLLN3x_data_len (packet));
        return SUCCESS;
}


void show_dl_ln3x_property (dl_ln3x_property * property)
{
        printf ("Address : %#X\n", property->dev_addr);
        printf ("Net ID  : %#X\n", property->net_ID);
        printf ("Channel : %#X\n", property->channel);
        printf ("Buadrate: %ld\n", property->buadrate);
}