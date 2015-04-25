/*
** crc32.c for src in /home/leroy_v/Perso/Huffman/src
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Thu Apr 09 17:35:45 2015 vincent leroy
** Last update Sun Apr 12 13:02:53 2015 vincent leroy
*/

#include "crc32.h"

static uint32_t crc_table[256];

void generate_crc32_table()
{
    uint32_t rem;
    int i;
    int j;

    /* Calculate CRC table. */
    for (i = 0; i < 256; i++)
    {
        rem = i;  /* remainder from polynomial division */
        for (j = 0; j < 8; j++)
        {
            if (rem & 1)
            {
                rem >>= 1;
                rem ^= POLY_REVERSED;
            }
            else
                rem >>= 1;
        }
        crc_table[i] = rem;
    }
}

uint32_t crc32(uint32_t crc, const void *buf, uint64_t len)
{
    uint8_t octet;
    const uint8_t *p;
    const uint8_t *q;

    crc = ~crc;
    q = (uint8_t*)buf + len;
    for (p = (uint8_t*)buf; p < q; p++)
    {
        octet = *p;
        crc = (crc >> 8) ^ crc_table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}
