/*
** header.h for header in /home/leroy_v/Perso/Huffman/include/header
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Fri Apr 03 17:14:37 2015 vincent leroy
** Last update Sun Apr 26 19:40:33 2015 vincent leroy
*/

#ifndef HEADER_H_
# define HEADER_H_

#include <endian.h>
#include <stdint.h>

#define MAGIC_NUMBER    0x8b21b552
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_PATCH   1

typedef struct s_header
{
    uint32_t magic_number;

    struct
    {
        uint8_t major;
        uint8_t minor;
        uint8_t patch;
    } version;

    uint32_t block_size;
} t_header;

int write_number(int fd, const void *nb, int bits);
int read_number(int fd, void *nb, int bits);

int check_version(t_header *header);

t_header* create_header(uint32_t block_size);
void delete_header(t_header *header);
void header_init(t_header *header, uint32_t block_size);

void header_write(const t_header *header, int fd);
int header_read(t_header *header, int fd);

#endif /* !HEADER_H_ */
