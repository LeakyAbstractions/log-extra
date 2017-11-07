/**
 *
 * @file        a_sqlite.h
 *
 * SQLite appender header file
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# ifndef LOG_SQLITE_APPENDER_H
# define LOG_SQLITE_APPENDER_H

# include <log.h>

# include <sqlite3.h>

/** SQLite appender type */
extern struct log_appender_type log_sqlite_appender;

/** SQLite appender configuration */
struct log_sqlite_appender_config{

    /** The path of the database */
    const char * path;
    /** The internal database pointer (automatically initialized) */
    sqlite3 * database;
    /** The internal statement pointer (automatically initialized) */
    sqlite3_stmt * statement;
};

# endif
