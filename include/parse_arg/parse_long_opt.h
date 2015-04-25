/*
** parse_long_opt.h for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 19:58:47 2014 vincent leroy
** Last update Thu Aug 21 14:04:58 2014 vincent leroy
*/

#ifndef PARSE_LONG_OPT_H_
# define PARSE_LONG_OPT_H_

/*
 * option : REQUIRED_ARG
 *
 * --option argument
 * --option=argument
 *
 * 'argument" must be passed to 'option' option
 */

#include "parse_arg.h"

int parse_long_opt(const char *long_opt, const char *next_opt[], const opts options[], opt_error *error);

#endif /* !PARSE_LONG_OPT_H_ */
