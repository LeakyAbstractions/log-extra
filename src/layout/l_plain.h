/**
 *
 * @file        l_plain.h
 *
 * Plain layout header file
 *
 * @version     1.0.0.0
 * @author      Copyright (c) 2018 Guillermo Calvo
 *
 */

# ifndef LOG_PLAIN_LAYOUT_H
# define LOG_PLAIN_LAYOUT_H

# include <log.h>

/** Plain layout type */
extern struct log_layout_type log_plain_layout;

/** Plain layout configuration */
struct log_plain_layout_config{

    /** Unused field */
    char _;
};

# endif
