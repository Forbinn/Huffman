ha
==

This program is used to compress or decompress a file. The source code is build in an API style then you can easily write your own compression tool using the Huffman encoding.

Program description
---------------------------

**ha** compress (and decompress) a file using the [Huffman coding](http://en.wikipedia.org/wiki/Huffman_coding) technique. The input file is divided into regular piece named *block* and this block has a fixed size named *block-size* which is 10MB by default.<br>
The compression ratio it-self is not very good because using only the Huffman code is not really efficient compare to other tool like *bzip2* or *xz*. The minimal compressed size is eight of the original input size due to the nature of the Huffman code it-self but this only append if your file contain the same character.<br>
<br>
The advantage of this program is its speed:

 1. It is multi-thread using the pthread API
 2. The Huffman code is easy to compute

Program usage
-------------------

    ha input_filename -o output_filename

This is just a basic usage example but you can see all options by typing `ha -h`. With this command line you will compress the *input_filename* using a block-size of 10MB, a maximum memory usage of 100MB and *n* threads where *n* represent the number of cores on your computer.<br>
The block-size, the memory maximum usage and the number of thread can be specified either directly with the program argument or by using the environment variable.

Environment variable
----------------------------

If you are using this tool and you don't have the possibility the set the program argument (e.g. if you use it with the *tar* program) you can set them indirectly by using the following environment variable:

 - **HA_THREAD**: set the number of thread that need to be used
 - **HA_BLOCK_SIZE**: set the block-size value
 - **HA_MEMLIMIT**: set the maximum memory usage

Usage with tar
-------------------

This program is compatible with the *tar* archiver by using the following command line:

    tar -I /path/to/ha -cf archive.tar.hc dir
This command line will archive and compress the directory **dir** using the ha compression program.<br>
To decompress you can use this command line:

    tar -I /path/to/ha -xvf archive.tar.hc

Compressed file format
-------------------------------

This table describe the format of the compressed file. The file is written in little endian order.

| Name           | Size in bytes | Value          | Description               |
| :------------- | ------------: | -------------: | :------------------------ |
| Magic number   | 4             | 0x52B5218B     | Use to recognize the file |
| Version major  | 1             | 1              | Current major version     |
| Version minor  | 1             | 0              | Current minor version     |
| Version patch  | 1             | 1              | Current patch version     |

From this point **all** the fields are repeated until the end of the file (**EOF**).

| Name           | Size in bytes | Value          | Description               |
| :------------- | ------------: | -------------: | :------------------------ |
| Block size     | 4             | --block-size   | The block size of each compressed block |
| CRC-32         | 4             | Depends data   | This crc is computed using all the following field |
| Number of leaf | 1             | Depends data   | Size of the Huffman tree  |

The **2** following fields are repeated number of leaf times.

| Name           | Size in bytes | Value          | Description               |
| :------------- | ------------: | -------------: | :------------------------ |
| Weight         | 4             | Depends data   | Number of character in the block-size |
| Byte value     | 1             | 0 - 255        | Character corresponding to the weight |

| Name           | Size in bytes | Value          | Description               |
| :------------- | ------------: | -------------: | :------------------------ |
| Padding        | 1             | 0 - 7          | Number of padding bits at the end of the compressed memory |
| Memory size    | 4             | 0 - Block size | Size of the following compressed memory |
| Data           | memory size   | Depends data   | Compressed data           |

