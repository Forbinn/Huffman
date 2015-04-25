/*
** parse_short_opt.h for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 19:50:39 2014 vincent leroy
** Last update Thu Aug 21 14:00:41 2014 vincent leroy
*/

#ifndef PARSE_SHORT_OPT_H_
# define PARSE_SHORT_OPT_H_

/*
 * h : NO_ARG
 * a : OPTIONAL_ARG
 * l : NO_ARG
 *
 * -hal argument
 *
 * 'argument' must be pass to 'a' option
 *
 * h : NO_ARG
 * a : OPTIONAL_ARG
 * l : REQUIRED_ARG
 *
 * -lah argument
 *
 * 'argument' must be pass to 'l' option
 *
 * h : NO_ARG
 * a : OPTIONAL_ARG
 * l : OPTIONAL_ARG
 *
 * -lah argument
 *
 * 'argument' must be pass to 'a' option
 *
 * h : NO_ARG
 * l : OPTIONAL_ARG
 *
 * -l -- -h
 *
 * '-h' must be pass to 'l' option
 *
 * h : NO_ARG
 * l : NO_ARG
 *
 * -l -- -h
 *
 * '-h' is not an option in this case
 */

#include "parse_arg.h"

struct opts_arg
{
    const opts  *opt;
    const char  *arg;
};

int parse_short_opt(const char *short_opt, const char **next_opt, const opts options[], opt_error *error);

#endif /* !PARSE_SHORT_OPT_H_ */
