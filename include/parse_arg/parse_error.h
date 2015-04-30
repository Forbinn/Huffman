/*
** parse_error.h for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 18:12:11 2014 vincent leroy
** Last update Thu Apr 30 14:57:57 2015 vincent leroy
*/

#ifndef PARSE_ERROR_H_
# define PARSE_ERROR_H_

#include <stdbool.h>

typedef enum error_type
{
    NO_ERROR        = 0,
    UNKNOWN_OPT     = 1,
    ARG_NOT_PROVIDE = 2,
    CALLBACK_ERROR  = 3,
    INVALID_ARG     = 4
} error_type;

typedef struct opt_error
{
    int         argv_idx;
    int         idx;
    bool        is_short_arg;
    error_type  err_type;
} opt_error;

#define DONT_CHANGE_ERROR_TYPE   NO_ERROR

int set_parse_error(opt_error *error, int argv_idx, int idx, error_type err_type, int short_arg);
void clear_parse_error(opt_error *error);

#endif /* !PARSE_ERROR_H_ */
