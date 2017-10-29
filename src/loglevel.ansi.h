/**
 *
 * @file        loglevel.ansi.h
 *
 * log level header file (ANSI C compatible)
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# ifndef LOG_LEVEL_ANSI_H
# define LOG_LEVEL_ANSI_H

# include <log.h>

# define LOG_FATAL(message)   LOG_EVENT(LOG_LEVEL_FATAL, message)
# define LOG_ERROR(message)   LOG_EVENT(LOG_LEVEL_ERROR, message)
# define LOG_WARN(message)    LOG_EVENT(LOG_LEVEL_WARN, message)
# define LOG_INFO(message)    LOG_EVENT(LOG_LEVEL_INFO, message)
# define LOG_VERBOSE(message) LOG_EVENT(LOG_LEVEL_VERBOSE, message)
# define LOG_DEBUG(message)   LOG_EVENT(LOG_LEVEL_DEBUG, message)
# define LOG_TRACE(message)   LOG_EVENT(LOG_LEVEL_TRACE, message)

/** Represents a standard logging level */
enum log_level{

    /** This special level is intended to turn on all logging */
    LOG_LEVEL_ALL = LOG_LEVEL_MIN,

    /** This level designates very severe error events that will presumably lead the application to abort */
    LOG_LEVEL_FATAL = 0x20,

    /** This level designates error events that might still allow the application to continue running */
    LOG_LEVEL_ERROR = 0x40,

    /** This level designates potentially harmful situations */
    LOG_LEVEL_WARN = 0x60,

    /** This level designates normal but significant situations */
    LOG_LEVEL_INFO = 0x80,

    /** This level designates informational messages that highlight the progress of the application at coarse-grained level */
    LOG_LEVEL_VERBOSE = 0xA0,

    /** This level designates fine-grained informational events that are most useful to debug an application */
    LOG_LEVEL_DEBUG = 0xC0,

    /** This level designates finer-grained informational events than the DEBUG */
    LOG_LEVEL_TRACE = 0xE0,

    /** This special level is intended to turn off logging */
    LOG_LEVEL_NONE = LOG_LEVEL_MAX
};

# endif
