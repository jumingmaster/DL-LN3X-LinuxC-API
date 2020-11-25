#include "dl_ln3x.h"
#include "dl_ln3x_err.h"

int main(int argc, const char **argv)
{
	if (argc < 2)
	{
		printf ("Usage: test  <dev_name>\n");
		return EXIT_FAILURE;
	}
	dl_ln3x_t err;
	dl_ln3x *dev;

	err = init_dl_ln3x (&dev);
	if (err)
		error_print (err);

	err = open_dl_ln3x (dev, argv[1]);
	if (err)
		error_print (err);

	err = dl_ln3x_set_com (dev, 0x90, FALSE);
	if (err)
		error_print (err);
	
	int i = 0;
	u_int8_t val[1];
	val[0] = 20;
	dl_ln3x_packet packet;
	while (i < 5)
	{
		err = dl_ln3x_send_data (dev, val, 1, 0x90, 0x6562);
		if (err)
			error_print (err);
		dl_ln3x_read_data (dev, &packet);
		show_serial_data_hex (packet.data, LN3X_VAR_DATA_LEN (packet.len));
		sleep (5);
		val[0] += 2;
		i++;
	}
	
	close_usart (dev->uart);
	return 0;
}
