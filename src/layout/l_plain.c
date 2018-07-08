/**
 *
 * @file        l_plain.c
 *
 * Plain layout source code file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# include "l_plain.h"

# include <stdio.h>
# include <string.h>

static int format(struct log_layout * layout, struct fmt_stream * stream, struct log_logger * logger, struct log_event * event){

    (void)layout;
    (void)logger;

    return( fmt_vprint(stream, event->format, event->arg) );
}

struct log_layout_type log_plain_layout = { NULL, NULL, format };
