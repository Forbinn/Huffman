/*
** compression_threaded.h for compression in /home/leroy_v/Perso/Huffman/include/compression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Fri Apr 03 16:24:25 2015 vincent leroy
** Last update Sat Apr 25 20:55:00 2015 vincent leroy
*/

#ifndef COMPRESSION_THREADED_H_
# define COMPRESSION_THREADED_H_

#include "huffmantree.h"

typedef enum e_c_error
{
    C_NO_ERROR    = 0,
    C_READ_ERROR  = -1,
    C_WRITE_ERROR = -2
} t_c_error;

typedef struct s_c_thread_param
{
    uint32_t block_size;
    int number_of_thread;
    uint64_t memory_max;
} t_c_thread_param;

typedef struct s_compress_arg
{
    uint8_t *mem;
    uint32_t mem_size;
} t_compress_arg;

typedef struct s_compress_ret
{
    uint8_t *mem;

    uint32_t mem_size;
    uint8_t padding;

    const t_huffmantree *tree;

    uint32_t crc;
} t_compress_ret;

typedef uint32_t (*c_input)(t_compress_arg*, void*);
typedef uint32_t (*c_output)(const t_compress_ret*, void*);

t_c_error compress_with_thread(const t_c_thread_param *param, c_input in, c_output out, void *in_userdata, void *out_userdata);

#endif /* !COMPRESSION_THREADED_H_ */
