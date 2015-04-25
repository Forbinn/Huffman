/*
** crc32.h for include in /home/leroy_v/Perso/Huffman/include
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Thu Apr 09 17:35:50 2015 vincent leroy
** Last update Sun Apr 12 13:02:33 2015 vincent leroy
*/

#ifndef CRC32_H_
# define CRC32_H_

#include <stdint.h>

#define POLY_NORMAL     0x04C11DB7
#define POLY_REVERSED   0xEDB88320
#define POLY_REV_REC    0x82608EDB

void generate_crc32_table();
uint32_t crc32(uint32_t crc, const void *buf, uint64_t len);

#endif /* !CRC32_H_ */
