# HuffmanCompression
Simple C++ program to compress a UTF-8 text file using Huffman tree.

This project implemets a Huffman tree using caracter compression.

If you want to learn more about how a Huffman tree works you can check a very good explanation from the website GeeksforGeeks [here](https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/).

When compressing a file the program will generate a new file called "{fileName}C.bin", which is the compressed file itself. And after the decompressing a file the program will generate a file called "{fileName}D.txt" which will be a exact replica of the original text file that was previously compressed.

The process of both compressing and decompressing can take a significant amount of time to completly finish depending on the size of the files. 
Here are some test results:

### Compression

| Original file size | Time | Size of generated file |
| --- | --- | --- |
| 1 MB | ~0.866 s | 712 kB |
| 10 MB | ~7.543 s | 6.87 MB |
| 100 MB | ~75.117 s | 68.7 MB |

With the given results we have approximately a 30% of reduction from original file size to compressed file size.

### Decompression

| Compressed file size | Time | Size of generated file |
| --- | --- | --- |
| 712 kB | ~0.749 s | 1 MB |
| 6.87 MB | ~7.515 s | 10 MB |
| 68.7 MB | ~74.092 s | 100 MB |
