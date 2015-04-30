/*
** parse_arg.h for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 14:24:49 2014 vincent leroy
** Last update Thu Apr 30 14:58:02 2015 vincent leroy
*/

#ifndef PARSE_ARG_H_
# define PARSE_ARG_H_

#include <stdbool.h>

#include "parse_error.h"

typedef bool (*arg_callback)(const char*, const char*, void*);

typedef enum argument
{
    NO_ARG          = 0,
    OPTIONAL_ARG    = 1,
    REQUIRED_ARG    = 2
} argument;

typedef struct opts
{
    char            short_opt;
    const char      *long_opt;
    argument        has_arg;

    arg_callback    cb;
    void            *user_data;
} opts;

int parse_arg(int *ac, char ***av, const opts options[], opt_error *error);

#endif /* !PARSE_ARG_H_ */
