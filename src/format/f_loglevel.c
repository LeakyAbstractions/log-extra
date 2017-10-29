/**
 *
 * @file        f_loglevel.c
 *
 * log level formatter source code file
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# include "f_loglevel.h"

# include <loglevel.h>

static const char * names[] = { "ALL", "FATAL", "ERROR", "WARN", "INFO", "VERBOSE", "DEBUG", "TRACE", "NONE" };

int format_log_level(struct fmt_stream * out, const char * id, const char * options, va_list * arg){

    int level = va_arg(*arg, int);
    int index = ( level <= LOG_LEVEL_ALL ? 0 : (level >= LOG_LEVEL_NONE ? 8 : (level <= LOG_LEVEL_FATAL ? 1 : level / 32) ) );

    (void)id;

    return( fmt_print_builtin(out, options && *options ? options : "%s", names[index]) );
}
