#ifndef __DL_LN3X_ERR__H__
#define __DL_LN3X_ERR__H__
#include "dl_ln3x.h"
#include <string.h>
#include <stdio.h>

static void error_print (dl_ln3x_t err)
{
        switch (err)
        {
        case SUCCESS:
                fprintf (stderr, "Success.\n");
        case ALLOC_MEM_ERR:
                perror ("Allocate memory failed");
        case NONE_INIT_ERR:
                fprintf (stderr, "You haven't init the dl-ln3x before you control it.\n");
        case INVALID_ARGUMENT:
                fprintf (stderr, "Invalid argument, please check the params.\n");
        case OPEN_SERIAL_ERR:
                perror ("Open DL-LN3X Failed");
        }
}

#endif  //!__DL_LN3X_ERR__H__