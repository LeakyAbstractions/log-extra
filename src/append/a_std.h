/**
 *
 * @file        a_std.h
 *
 * Standard stream appender header file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# ifndef LOG_STANDARD_STREAM_APPENDER_H
# define LOG_STANDARD_STREAM_APPENDER_H

# include <log.h>

/** Standard output stream appender type */
extern struct log_appender_type log_stdout_appender;

/** Standard error stream appender type */
extern struct log_appender_type log_stderr_appender;

/** Standard stream appender configuration */
struct log_std_appender_config{

    /** The internal output stream (automatically initialized) */
    struct fmt_stream stream;
};

/** Standard output stream appender configuration */
struct log_stdout_appender_config{

    /** The standard stream appender configuration */
    struct log_std_appender_config _;
};

/** Standard error stream appender configuration */
struct log_stderr_appender_config{
    /** The standard stream appender configuration */
    struct log_std_appender_config _;
};

# endif
