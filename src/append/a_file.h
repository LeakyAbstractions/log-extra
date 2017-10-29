/**
 *
 * @file        a_file.h
 *
 * File appender header file
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# ifndef LOG_FILE_APPENDER_H
# define LOG_FILE_APPENDER_H

# include <log.h>

/** File appender type */
extern struct log_appender_type log_file_appender;

/** File appender configuration */
struct log_file_appender_config{

    /** The path of the file */
    const char * path;
    /** Determines if the file will be truncated */
    bool append;
    /** Determines if the file must be flushed after every logging event */
    bool flush;
    /** The internal output stream (automatically initialized) */
    struct fmt_stream stream;
};

# endif
