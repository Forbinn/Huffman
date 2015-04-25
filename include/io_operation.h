/*
** io_operation.h for include in /home/leroy_v/Perso/Huffman/include
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 16:33:08 2015 vincent leroy
** Last update Sun Apr 12 14:24:33 2015 vincent leroy
*/

#ifndef IO_OPERATION_H_
# define IO_OPERATION_H_

#include "decompression_threaded.h"
#include "compression_threaded.h"

uint32_t read_raw_data(t_compress_arg *arg, void *userdata);
uint32_t read_compressed_data(t_decompress_arg *arg, void *userdata);

uint32_t write_compressed_data(const t_compress_ret *ret, void *userdata);
uint32_t write_raw_data(const t_decompress_ret *ret, void *userdata);

#endif /* !IO_OPERATION_H_ */
