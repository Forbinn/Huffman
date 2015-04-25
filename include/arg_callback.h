/*
** arg_callback.h for include in /home/leroy_v/Perso/Huffman/include
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 16:29:01 2015 vincent leroy
** Last update Tue Apr 07 17:15:43 2015 vincent leroy
*/

#ifndef ARG_CALLBACK_H_
# define ARG_CALLBACK_H_

#include <stdbool.h>

bool set_block_size(const char *option, const char *arg, void *userdata);
bool set_output_filename(const char *option, const char *arg, void *userdata);
bool set_thread_number(const char *option, const char *arg, void *userdata);
bool set_memlimit(const char *option, const char *arg, void *userdata);
bool set_compress(const char *option, const char *arg, void *userdata);
bool set_decompress(const char *option, const char *arg, void *userdata);

#endif /* !ARG_CALLBACK_H_ */
