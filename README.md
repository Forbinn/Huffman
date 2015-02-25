# Huffman coding
Implementation of the Huffman coding algorithm in c for compression and decompression of a file

In order to compile the program you just have to type **make** in the root directory. This program does not require any shared library except the standard c library of course.

### Usage

    ./ha file

If you do this the program will compress the file under the name **file.hc** and decompress it under the name **file.hc.hcd**.

The compressed file can be bigger than the uncompressed file because of the overhead caused by the *huffman tree*. This can occurs if the size of the uncompressed file is low (< 1 kB).

### Error checking

For the moment the program will crash if the compressed file is corrupted because not check is done. This will be improve in future version.
