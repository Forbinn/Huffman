/*
** decompression_threaded.h for decompression in /home/leroy_v/Perso/Huffman/include/decompression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 02:01:57 2015 vincent leroy
** Last update Sat Apr 25 20:59:54 2015 vincent leroy
*/

#ifndef UNCOMPRESSION_THREADED_H_
# define UNCOMPRESSION_THREADED_H_

#include "huffmantree.h"

typedef enum e_d_error
{
    D_NO_ERROR      = 0,
    D_READ_ERROR    = -1,
    D_WRITE_ERROR   = -2,
    D_CRC_ERROR     = -3
} t_d_error;

typedef struct s_u_thread_param
{
    uint32_t block_size;
    int number_of_thread;
    uint64_t memory_max;
} t_u_thread_param;

typedef struct s_decompress_arg
{
    const t_huffmantree *tree;

    uint8_t *mem;
    uint32_t mem_size;
    uint8_t padding;
    uint32_t block_size;
    uint32_t crc;
} t_decompress_arg;

typedef struct s_decompress_ret
{
    uint8_t *mem;
    uint32_t mem_size;
} t_decompress_ret;

typedef uint32_t (*d_input)(t_decompress_arg*, void*);
typedef uint32_t (*d_output)(const t_decompress_ret*, void*);

t_d_error decompress_with_thread(const t_u_thread_param *param, d_input in, d_output out, void *in_userdata, void *out_userdata);

#endif /* !UNCOMPRESSION_THREADED_H_ */
