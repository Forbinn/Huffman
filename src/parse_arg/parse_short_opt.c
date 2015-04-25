/*
** parse_short_opt.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 19:50:33 2014 vincent leroy
** Last update Sat Apr 25 20:34:25 2015 vincent leroy
*/

#include <string.h>
#include <stdlib.h>

#include "parse_short_opt.h"

static int _get_opts_idx_for_opt(char short_opt, const opts options[])
{
    int i;

    for (i = 0; options[i].cb != NULL; ++i)
        if (options[i].short_opt == short_opt)
            return i;

    return -1;
}

static int _get_real_arg(const char *next_opt[], const char **real_arg)
{
    *real_arg = next_opt[0];
    if (*real_arg != NULL && (*real_arg)[0] == '-')
    { // The next argument is an option
        if ((*real_arg)[1] == '-' && (*real_arg)[2] == '\0')
        { // The next argument is '--' so take the next argument
            *real_arg = next_opt[1];
            return 2;
        }
        else
        { // Option canno't be take as argument (except if '--' is present)
            *real_arg = NULL;
            return 0;
        }
    }

    return *real_arg != NULL ? 1 : 0;
}

static int _short_opt_to_opts_arg(const char *short_opt, const opts options[], struct opts_arg opts_arg[], opt_error *error)
{
    int i;
    int idx;

    for (i = 0; short_opt[i] != '\0'; ++i)
    {
        idx = _get_opts_idx_for_opt(short_opt[i], options);
        if (idx == -1)
            return set_parse_error(error, -1, i, UNKNOWN_OPT, -1);

        opts_arg[i].opt = &options[idx];
    }

    return 0;
}

static int _give_arg_to_opts_arg(const char *arg, struct opts_arg opts_arg[], opt_error *error)
{
    int save_i;
    int i;
    bool arg_already_used_by_required = false;
    bool arg_already_used = false;

    for (i = 0; opts_arg[i].opt != NULL; ++i)
    {
        if (opts_arg[i].opt->has_arg == REQUIRED_ARG)
        {
            if (arg == NULL || arg_already_used_by_required)
                return set_parse_error(error, -1, i, ARG_NOT_PROVIDE, -1);
            else if (arg_already_used) // arg is used by an optional argument
                opts_arg[save_i].arg = NULL; // remove it

            opts_arg[i].arg = arg;
            arg_already_used = true;
            arg_already_used_by_required = true;
        }
        else if (opts_arg[i].opt->has_arg == OPTIONAL_ARG)
        {
            if (arg_already_used)
                continue;

            opts_arg[i].arg = arg;
            arg_already_used = true;
            save_i = i;
        }
    }

    return (arg_already_used_by_required || arg_already_used) ? 1 : 0;
}

static int _call_cb(const struct opts_arg opts_arg[], opt_error *error)
{
    int i;
    char str[2] = {};

    for (i = 0; opts_arg[i].opt != NULL; ++i)
    {
        str[0] = opts_arg[i].opt->short_opt;
        if (!(opts_arg[i].opt->cb(str, opts_arg[i].arg, opts_arg[i].opt->user_data)))
            return set_parse_error(error, -1, i, CALLBACK_ERROR, -1);
    }

    return 0;
}

int parse_short_opt(const char *short_opt, const char *next_opt[], const opts options[], opt_error *error)
{
    const char *arg;
    int return_value = 0;
    int nb_arg_used = _get_real_arg(next_opt, &arg) + 1;
    struct opts_arg *opts_arg = calloc(strlen(short_opt) + 1, sizeof(struct opts_arg));

    if ((return_value = _short_opt_to_opts_arg(short_opt, options, opts_arg, error)) != -1)
        if ((return_value = _give_arg_to_opts_arg(arg, opts_arg, error)) != -1)
        {
            if (return_value == 0) // arg is not used
                nb_arg_used = 1; // Only the option has been parsed
            return_value = _call_cb(opts_arg, error);
        }

    free(opts_arg);
    return return_value == -1 ? -1 : nb_arg_used;
}
