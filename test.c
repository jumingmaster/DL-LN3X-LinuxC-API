#include "dl_ln3x.h"

int main(int argc, const char **argv)
{
	if (argc < 2)
	{
		printf ("Usage: test <dev_name>\n");
		return EXIT_FAILURE;
	}
	
	dl_ln3x *dev;
	init_dl_ln3x (&dev);
	open_dl_ln3x (dev, argv[1]);
	close_usart (dev->uart);
	return 0;
}
