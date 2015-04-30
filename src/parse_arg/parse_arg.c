/*
** parse_arg.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 14:24:44 2014 vincent leroy
** Last update Thu Apr 30 15:20:13 2015 vincent leroy
*/

#include <stdlib.h>
#include <string.h>

#include "parse_arg.h"
#include "parse_short_opt.h"
#include "parse_long_opt.h"

static void _remove_option_in_av(int *ac, char ***av, int *idx_in_av_for_remove, int size_of_tab)
{
    int i;
    int j;

    for (i = 0; i < size_of_tab; ++i)
    {
        for (j = idx_in_av_for_remove[i]; j < *ac; ++j)
            (*av)[j] = (*av)[j + 1];
        --(*ac);

        for (j = i + 1; j < size_of_tab; ++j)
            --idx_in_av_for_remove[j];
    }
}

static int check_opts_arg(const opts options[], opt_error *error)
{
    char array[256] = {};
    int err = 0;
    int err_idx = -1;
    int i;
    int j;

    for (i = 0; options[i].cb != NULL && err == 0; ++i)
    {
        if (options[i].short_opt != '\0')
        {
            if (array[(unsigned char)options[i].short_opt] == 1)
            {
                err = 1;
                err_idx = i;
                break;
            }

            array[(unsigned char)options[i].short_opt] = 1;
        }

        if (options[i].long_opt != NULL)
            for (j = i + 1; options[j].cb != NULL; ++j)
            {
                if (options[j].long_opt == NULL)
                    continue;

                if (strcmp(options[i].long_opt, options[j].long_opt) == 0)
                {
                    err = 2;
                    err_idx = j;
                    break;
                }
            }
    }

    if (err != 0)
    {
        if (error != NULL)
        {
            error->err_type = INVALID_ARG;
            error->is_short_arg = ((err == 1) ? 1 : 0);
            error->idx = err_idx;
        }
        return -1;
    }

    return 0;
}

int _do_parse(char **argv, int i, int off, const opts options[], opt_error *error, int *idx_in_av_for_remove, int *idx_in_tab, int (*parse_func)(const char*, const char*[], const opts[], opt_error*))
{
    int nb_arg_parsed;
    int j;

    nb_arg_parsed = (*parse_func)(argv[i] + off, (const char**)argv + i + 1, options, error);
    if (nb_arg_parsed == -1)
    {
        free(idx_in_av_for_remove);
        return set_parse_error(error, i, error ? error->idx + off : -1, DONT_CHANGE_ERROR_TYPE, -1);
    }

    for (j = 0; j < nb_arg_parsed; ++j)
    {
        idx_in_av_for_remove[*idx_in_tab] = i + j;
        ++(*idx_in_tab);
    }

    return nb_arg_parsed;
}

int parse_arg(int *ac, char ***av, const opts options[], opt_error *error)
{
    int nb_arg_parsed = 0;
    int i;
    int *idx_in_av_for_remove;
    int idx_in_tab = 0;
    char **argv = *av;

    if (check_opts_arg(options, error) == -1)
        return -1;

    idx_in_av_for_remove = calloc(*ac, sizeof(int));
    clear_parse_error(error);

    for (i = 1; i < *ac; i += nb_arg_parsed)
    {
        if (argv[i][0] == '-' && argv[i][1] != '-' && argv[i][1] != '\0')
        {
            set_parse_error(error, -1, -1, DONT_CHANGE_ERROR_TYPE, 1);
            nb_arg_parsed = _do_parse(argv, i, 1, options, error, idx_in_av_for_remove, &idx_in_tab, &parse_short_opt);
            if (nb_arg_parsed == -1)
                return -1;
        }
        else if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            if (argv[i][2] == '\0')
            {
                nb_arg_parsed = 2;
                continue;
            }

            set_parse_error(error, -1, -1, DONT_CHANGE_ERROR_TYPE, 0);
            nb_arg_parsed = _do_parse(argv, i, 2, options, error, idx_in_av_for_remove, &idx_in_tab, &parse_long_opt);
            if (nb_arg_parsed == -1)
                return -1;
        }
        else
        {
            nb_arg_parsed = 1;
            continue;
        }
    }

    _remove_option_in_av(ac, av, idx_in_av_for_remove, idx_in_tab);
    free(idx_in_av_for_remove);
    return 0;
}
