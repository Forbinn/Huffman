# Huffman coding
Implementation of the Huffman coding algorithm in c for compression and decompression of a file

In order to compile the program you just have to type **make** in the root directory. This program does not require any shared library except the standard c library of course.

### Usage

    ./ha [-h] [-g [image_file]] [-c | -d] -i input_file [-o output_file]

The **-c** option is used to compress a file and the **-d** option is used to decompress a file.
For more information type *./ha -h*.

You can also draw the huffman graph if you add the **-g** option.

The compressed file can be bigger than the uncompressed file because of the overhead caused by the *huffman tree*. This can occurs if the size of the uncompressed file is low (< 1 kB).

### Error checking

For the moment the program will crash if the compressed file is corrupted because not check is done. This will be improve in future version.
