/**
 *
 * @file        a_rolling.h
 *
 * Rolling file appender header file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# ifndef LOG_ROLLING_FILE_APPENDER_H
# define LOG_ROLLING_FILE_APPENDER_H

# include <log.h>

# include <stdbool.h>
# include <stdio.h>

/** Rolling file appender type */
extern struct log_appender_type log_rolling_file_appender;

/** Rolling file appender configuration */
struct log_rolling_file_appender_config{

    /** The path of the file */
    const char * path;
    /** Determines if the file must be flushed after every logging event */
    bool flush;
    /** Determines the maximum file size before the file is rolled over */
    size_t max_size;
    /** The internal sequence number (automatically initialized) */
    unsigned long sequence;
    /** The internal output stream (automatically initialized) */
    struct fmt_stream stream;
};

# endif
