/**
 *
 * @file        a_rolling.c
 *
 * Rolling file appender source code file
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# include "a_rolling.h"

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

# ifndef LOG_ROLLING_FILE_APPENDER_PATH_MAX_SIZE
#   define LOG_ROLLING_FILE_APPENDER_PATH_MAX_SIZE 512
# endif

# define file_close(FILE) ( fclose(FILE) == 0 )
# define file_rename(OLD, NEW) ( rename((OLD), (NEW)) == 0 )

static bool file_exists(const char * path){

    FILE * file = fopen(path, "r+");

    if( file == NULL || !file_close(file) ){

        errno = 0;
    }

    return(file != NULL);
}

static bool rename_next(struct log_rolling_file_appender_config * config){

    char next_path[LOG_ROLLING_FILE_APPENDER_PATH_MAX_SIZE];

    for(config->sequence++; config->sequence > 0; config->sequence++){

        snprintf(next_path, LOG_ROLLING_FILE_APPENDER_PATH_MAX_SIZE, "%s.%lu", config->path, config->sequence);

        if( !file_exists(next_path) ){ break; }
    }

    return( file_rename(config->path, next_path) );
}

static bool roll_over(struct log_rolling_file_appender_config * config){

    FILE * file = NULL;
    bool success = false;

    if(config->stream.file == NULL){

        success = !file_exists(config->path) || rename_next(config);

    }else if( file_close(config->stream.file) ){

        success = rename_next(config);
    }

    if(success){

        file = fopen(config->path, "w");
        fmt_stream_file(&config->stream, file);
    }

    return(file != NULL);
}

static bool append(struct log_appender * appender, struct log_layout * layout, struct log_logger * logger, struct log_event * event){

    struct log_rolling_file_appender_config * config = (void *)appender->configuration;
    int written;

    if(config->max_size != 0 && config->max_size <= config->stream.written){

        if( !roll_over(config) ){

            return(false);
        }
    }

    written = layout->type->format(layout, &config->stream, logger, event);

    if(config->flush != 0){

        fflush(config->stream.file);
    }

    return(written != FMT_ERROR);
}

static bool initialize(struct log_appender * appender){

    bool initialized = false;

    if(appender && appender->configuration){

        struct log_rolling_file_appender_config * config = appender->configuration;

        config->sequence = 0;
        config->stream.file = NULL;

        initialized = roll_over(config);
    }

    return(initialized);
}

static bool finalize(struct log_appender * appender){

    bool finalized = false;

    if(appender && appender->configuration){

        struct log_rolling_file_appender_config * config = appender->configuration;

        finalized = ( file_close(config->stream.file) );
    }

    return(finalized);
}

struct log_appender_type log_rolling_file_appender = { initialize, finalize, append };
