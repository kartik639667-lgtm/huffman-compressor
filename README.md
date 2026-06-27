# CLI Huffman File Compressor

A C++ command-line tool that compresses text files using Huffman Coding and Base64 encoding for safe data transmission.

## How to Compile
g++ -O3 main.cpp -o huffman

## How to Run
Compress a file:
huffman -c input.txt compressed.txt

Decompress a file:
huffman -d compressed.txt output.txt