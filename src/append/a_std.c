/**
 *
 * @file        a_std.c
 *
 * Standard stream appender source code file
 *
 * @version     1.0.0
 * @author      Copyright (c) 2017 Guillermo Calvo
 *
 */

# include "a_std.h"

# include <stdio.h>

static bool append(struct log_appender * appender, struct log_layout * layout, struct log_logger * logger, struct log_event * event){

    int written = FMT_ERROR;

    if(appender && appender->configuration && layout){

        struct log_std_appender_config * config = appender->configuration;

        written = layout->type->format(layout, &config->stream, logger, event);
    }

    return(written != FMT_ERROR);
}

static bool initialize_stdout(struct log_appender * appender){

    struct fmt_stream * stream = NULL;

    if(appender && appender->configuration){

        struct log_std_appender_config * config = appender->configuration;

        stream = &config->stream;
        fmt_stream_file(stream, stdout);
    }

    return(stream != NULL);
}

static bool initialize_stderr(struct log_appender * appender){

    struct fmt_stream * stream = NULL;

    if(appender && appender->configuration){

        struct log_std_appender_config * config = appender->configuration;

        stream = &config->stream;
        fmt_stream_file(stream, stderr);
    }

    return(stream != NULL);
}

struct log_appender_type log_stdout_appender = { initialize_stdout, NULL, append };
struct log_appender_type log_stderr_appender = { initialize_stderr, NULL, append };
