/**
 *
 * @file        f_loglevel.h
 *
 * log level formatter header file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# ifndef LOG_LEVEL_FORMATTER
# define LOG_LEVEL_FORMATTER

# include <fmt.h>

int format_log_level(struct fmt_stream * out, const char * id, const char * options, va_list * arg);

# endif
