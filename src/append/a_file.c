/**
 *
 * @file        a_file.c
 *
 * File appender source code file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# include "a_file.h"

# include <stdio.h>
# include <stdlib.h>

# ifndef LOG_FILE_APPENDER_PATH_MAX_SIZE
#   define LOG_FILE_APPENDER_PATH_MAX_SIZE 512
# endif

static bool append(struct log_appender * appender, struct log_layout * layout, struct log_logger * logger, struct log_event * event){

    int written = FMT_ERROR;

    if(appender && appender->configuration && layout){

        struct log_file_appender_config * config = appender->configuration;

        /* output event through layout */
        written = layout->type->format(layout, &config->stream, logger, event);

        /* flush file */
        if(config->flush && fflush(config->stream.file) != 0){
            written = FMT_ERROR;
        }
    }

    return(written != FMT_ERROR);
}

static bool initialize(struct log_appender * appender){

    FILE * file = NULL;

    if(appender && appender->configuration){

        struct log_file_appender_config * config = appender->configuration;
        char path[LOG_FILE_APPENDER_PATH_MAX_SIZE];
        struct fmt_stream tmp = {0};

        /* format datetime */
        fmt_stream_buffer(&tmp, path, LOG_FILE_APPENDER_PATH_MAX_SIZE);
        fmt_print(&tmp, config->path, NULL);

        /* open the file in append or truncate mode */
        file = fopen(path, config->append ? "a" : "w");

        fmt_stream_file(&config->stream, file);
    }

    return(file != NULL);
}

static bool finalize(struct log_appender * appender){

    if(appender && appender->configuration){

        struct log_file_appender_config * config = appender->configuration;

        return( fclose(config->stream.file) == 0 );
    }

    return(false);
}

struct log_appender_type log_file_appender = { initialize, finalize, append };
