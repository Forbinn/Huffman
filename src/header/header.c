/*
** header.c for header in /home/leroy_v/Perso/Huffman/src/header
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Fri Apr 03 17:14:31 2015 vincent leroy
** Last update Sat Apr 25 20:27:41 2015 vincent leroy
*/

#include <stdlib.h>
#include <unistd.h>

#include "header.h"

static int write_u8(int fd, const void *nb)
{
    uint8_t val = *(const uint8_t*)nb;
    return write(fd, &val, sizeof(uint8_t));
}

static int write_u16(int fd, const void *nb)
{
    uint16_t val = htole16(*(const uint16_t*)nb);
    return write(fd, &val, sizeof(uint16_t));
}

static int write_u32(int fd, const void *nb)
{
    uint32_t val = htole32(*(const uint32_t*)nb);
    return write(fd, &val, sizeof(uint32_t));
}

static int write_u64(int fd, const void *nb)
{
    uint64_t val = htole64(*(const uint64_t*)nb);
    return write(fd, &val, sizeof(uint64_t));
}

static int read_u8(int fd, void *nb)
{
    uint8_t *val = (uint8_t*)nb;
    return read(fd, val, sizeof(uint8_t));
}

static int read_u16(int fd, void *nb)
{
    uint16_t *val = (uint16_t*)nb;
    int res = read(fd, val, sizeof(uint16_t));
    *val = le16toh(*val);
    return res;
}

static int read_u32(int fd, void *nb)
{
    uint32_t *val = (uint32_t*)nb;
    int res = read(fd, val, sizeof(uint32_t));
    *val = le32toh(*val);
    return res;
}

static int read_u64(int fd, void *nb)
{
    uint64_t *val = (uint64_t*)nb;
    int res = read(fd, val, sizeof(uint64_t));
    *val = le64toh(*val);
    return res;
}

int write_number(int fd, const void *nb, int bits)
{
    struct s_bits_to_write
    {
        int (*func)(int, const void*);
        int bits;
    };
    static const struct s_bits_to_write tab[] = {
        {&write_u8, 8},
        {&write_u16, 16},
        {&write_u32, 32},
        {&write_u64, 64}
    };

    uint32_t i;
    for (i = 0; i < (sizeof(tab) / sizeof(*tab)); ++i)
        if (bits == tab[i].bits)
            return (*tab[i].func)(fd, nb);

    return -1;
}

int read_number(int fd, void *nb, int bits)
{
    struct s_bits_to_write
    {
        int (*func)(int, void*);
        int bits;
    };
    static const struct s_bits_to_write tab[] = {
        {&read_u8, 8},
        {&read_u16, 16},
        {&read_u32, 32},
        {&read_u64, 64}
    };

    uint32_t i;
    for (i = 0; i < (sizeof(tab) / sizeof(*tab)); ++i)
        if (bits == tab[i].bits)
            return (*tab[i].func)(fd, nb);

    return -1;
}

int check_version(t_header *header)
{
    if (header->version.major > VERSION_MAJOR)
        return -1;
    else if (header->version.major == VERSION_MAJOR)
    {
        if (header->version.minor > VERSION_MINOR)
            return -1;
    }
    return 0;
}

t_header* create_header(uint32_t block_size)
{
    t_header *header = malloc(sizeof(t_header));
    header_init(header, block_size);

    return header;
}

void delete_header(t_header *header)
{
    if (header == NULL)
        return ;

    free(header);
}

void header_init(t_header *header, uint32_t block_size)
{
    header->magic_number = MAGIC_NUMBER;
    header->version.major = VERSION_MAJOR;
    header->version.minor = VERSION_MINOR;
    header->version.patch = VERSION_PATCH;
    header->block_size = block_size;
}

void header_write(const t_header *header, int fd)
{
    write_number(fd, &header->magic_number, 32);
    write(fd, &header->version, sizeof(header->version));
    write_number(fd, &header->block_size, 32);
}

int header_read(t_header *header, int fd)
{
    if (read_number(fd, &header->magic_number, 32) < (int)sizeof(uint32_t))
        return -1;
    if (header->magic_number != MAGIC_NUMBER)
        return -1;
    if (read(fd, &header->version, sizeof(header->version)) < (int)sizeof(header->version))
        return -1;
    if (read_number(fd, &header->block_size, 32) < (int)sizeof(uint32_t))
        return -1;
    return 0;
}
