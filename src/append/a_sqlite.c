/**
 *
 * @file        a_sqlite.c
 *
 * SQLite appender source code file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# include "a_sqlite.h"

# include <stdio.h>
# include <stdlib.h>
# include <f_tmspec.h>
# include <f_loglevel.h>

# define TABLE_NAME "events"
# define TABLE(COLUMN) \
    COLUMN(0, 1,    id,             NIL,    INTEGER PRIMARY KEY, _) \
    COLUMN(0, 2,    timestamp,      TXT,    NVARCHAR(28),       timestamp) \
    COLUMN(0, 3,    logger,         TXT,    NVARCHAR(32),       logger->id) \
    COLUMN(0, 4,    sequence,       I64,    INTEGER,            logger->sequence) \
    COLUMN(0, 5,    level_id,       INT,    INTEGER,            event->level) \
    COLUMN(0, 6,    level,          TXT,    NVARCHAR(8),        level) \
    COLUMN(0, 7,    module,         TXT,    NVARCHAR(32),       event->module) \
    COLUMN(0, 8,    function,       TXT,    NVARCHAR(32),       event->function) \
    COLUMN(0, 9,    file,           TXT,    NVARCHAR(64),       event->file) \
    COLUMN(0, 10,   line,           INT,    INTEGER,            event->line) \
    COLUMN(0, 11,   error_number,   INT,    INTEGER,            event->error_number) \
    COLUMN(0, 12,   error_object,   TXT,    NVARCHAR(256),      error) \
    COLUMN(1, 13,   message,        TXT,    NVARCHAR(1024),     message)
# define TABLE_INDICES \
    INDEX(timestamp) \
    INDEX(level_id) \
    INDEX(module)

# define COLUMNS(LAST, INDEX, NAME, TYPE, SQL, VALUE)      "  "  #NAME LAST_COLUMN_##LAST "\n"
# define PARAMETERS(LAST, INDEX, NAME, TYPE, SQL, VALUE)   "  :" #NAME LAST_COLUMN_##LAST "\n"
# define DEFINITION(LAST, INDEX, NAME, TYPE, SQL, VALUE)   "  "  #NAME " " #SQL LAST_COLUMN_##LAST "\n"
# define BIND_ALL(LAST, INDEX, NAME, TYPE, SQL, VALUE) for(result=BIND_##TYPE(INDEX,VALUE);result;){goto binding_error;}
# define BIND_NIL(INDEX, VALUE) sqlite3_bind_null(config->statement, INDEX)
# define BIND_INT(INDEX, VALUE) sqlite3_bind_int(config->statement, INDEX, VALUE)
# define BIND_I64(INDEX, VALUE) sqlite3_bind_int64(config->statement, INDEX, VALUE)
# define BIND_TXT(INDEX, VALUE) sqlite3_bind_text(config->statement, INDEX, VALUE, -1, NULL)
# define LAST_COLUMN_0 ","
# define LAST_COLUMN_1
# define INDEX(COLUMN) "CREATE INDEX IF NOT EXISTS index_" #COLUMN " ON " TABLE_NAME "(" #COLUMN ");\n"

# define FATAL_ERROR "SQLite error (%d) %s.\n\t%s\n"

static const char SQL_TABLE[] = "CREATE TABLE IF NOT EXISTS " TABLE_NAME " (\n" TABLE(DEFINITION) ");\n" TABLE_INDICES;
static const char SQL_INSERT[] = "INSERT INTO " TABLE_NAME " (\n" TABLE(COLUMNS) ") VALUES (\n" TABLE(PARAMETERS) ");";

static bool append(struct log_appender * appender, struct log_layout * layout, struct log_logger * logger, struct log_event * event){

    char message[1024] = "", error[256] = "", timestamp[28] = "", level[8] = "";
    struct log_sqlite_appender_config * config;
    struct fmt_stream tmp = {0};
    const char * error_message;
    int result;

    (void)layout;

    if(!appender || !appender->configuration){
        return(false);
    }

    config = appender->configuration;

    /* Format timestamp */
    fmt_stream_buffer(&tmp, timestamp, sizeof(timestamp));
    result = fmt_print(&tmp, "%{@}", format_timespec, &event->timestamp);
    if(result == FMT_ERROR){
        (void)fprintf(stderr, FATAL_ERROR, result, "while formatting timestamp", timestamp);
        *timestamp = '\0';
    }

    /* Format logging level */
    fmt_stream_buffer(&tmp, level, sizeof(level));
    result = fmt_print(&tmp, "%{@}", format_log_level, event->level);
    if(result == FMT_ERROR){
        (void)fprintf(stderr, FATAL_ERROR, result, "while formatting logging level", level);
        *level = '\0';
    }

    /* Format error object */
    fmt_stream_buffer(&tmp, error, sizeof(error));
    result = fmt_print(&tmp, "%s", event->error_object);
    if(result == FMT_ERROR){
        (void)fprintf(stderr, FATAL_ERROR, result, "while formatting error object", error);
        *error = '\0';
    }

    /* Format logging message */
    fmt_stream_buffer(&tmp, message, sizeof(message));
    result = fmt_print(&tmp, "%{*}", event->format, event->arg);
    if(result == FMT_ERROR){
        (void)fprintf(stderr, FATAL_ERROR, result, "while formatting logging message", message);
        *message = '\0';
    }

    /* Prepare statement */
    TABLE(BIND_ALL)
    if(result != SQLITE_OK){
        binding_error:
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "before inserting a new row", error_message);
        return(false);
    }

    /* Evaluate the statement */
    result = sqlite3_step(config->statement);
    if(result != SQLITE_DONE){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while inserting a new row", error_message);
        return(false);
    }

    /* Reset all bindings */
    result = sqlite3_clear_bindings(config->statement);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while resetting all bindings", error_message);
    }

    /* Reset the statement */
    result = sqlite3_reset(config->statement);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "after inserting a new row", error_message);
        return(false);
    }

    return(true);
}

static bool initialize(struct log_appender * appender){

    struct log_sqlite_appender_config * config;
    const char * error_message;
    char * tmp;
    int result;

    if(!appender || !appender->configuration){
        return(false);
    }

    config = appender->configuration;

    /* Open database */
    result = sqlite3_open(config->path, &config->database);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while opening the database file", error_message);
        return(false);
    }

    /* Create table (if it does not exist yet) */
    result = sqlite3_exec(config->database, SQL_TABLE, NULL, NULL, &tmp);
    if(result != SQLITE_OK){
        (void)fprintf(stderr, FATAL_ERROR, result, "while creating the table for logging events", tmp);
        sqlite3_free(tmp);
        (void)sqlite3_close(config->database);
        return(false);
    }

    /* Prepare INSERT statement */
    result = sqlite3_prepare_v2(config->database, SQL_INSERT, sizeof(SQL_INSERT), &config->statement, NULL);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while preparing the SQL statement", error_message);
        (void)sqlite3_close(config->database);
        return(false);
    }

    return(true);
}

static bool finalize(struct log_appender * appender){

    struct log_sqlite_appender_config * config;
    const char * error_message;
    int result;

    if(!appender || !appender->configuration){
        return(false);
    }

    config = appender->configuration;

    /* Finalize statement */
    result = sqlite3_finalize(config->statement);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while finalizing the SQL statement", error_message);
        return(false);
    }

    /* Close database */
    result = sqlite3_close(config->database);
    if(result != SQLITE_OK){
        error_message = sqlite3_errstr(result);
        (void)fprintf(stderr, FATAL_ERROR, result, "while closing the database file", error_message);
        return(false);
    }

    return(true);
}

struct log_appender_type log_sqlite_appender = { initialize, finalize, append };
