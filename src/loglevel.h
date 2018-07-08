/**
 *
 * @file        loglevel.h
 *
 * log level header file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# ifndef LOG_LEVEL_H
# define LOG_LEVEL_H

# include <log.h>

# undef  LOG_EVENT
# define LOG_EVENT(LEVEL, ...) ( LOG_IS_LEVEL_DISABLED(LEVEL) ? (void)0 : \
    log_event_new((LOG_CONTEXT), LEVEL, (LOG_FUNCTION), __FILE__, __LINE__, (LOG_ERROR_OBJECT), __VA_ARGS__) )
# define LOG_FATAL(...)       LOG_EVENT(LOG_LEVEL_FATAL, __VA_ARGS__)
# define LOG_ERROR(...)       LOG_EVENT(LOG_LEVEL_ERROR, __VA_ARGS__)
# define LOG_WARN(...)        LOG_EVENT(LOG_LEVEL_WARN, __VA_ARGS__)
# define LOG_INFO(...)        LOG_EVENT(LOG_LEVEL_INFO, __VA_ARGS__)
# define LOG_VERBOSE(...)     LOG_EVENT(LOG_LEVEL_VERBOSE, __VA_ARGS__)
# define LOG_DEBUG(...)       LOG_EVENT(LOG_LEVEL_DEBUG, __VA_ARGS__)
# define LOG_TRACE(...)       LOG_EVENT(LOG_LEVEL_TRACE, __VA_ARGS__)

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
