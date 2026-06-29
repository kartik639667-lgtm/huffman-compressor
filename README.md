# 🗜️ CLI Huffman File Compressor

A C++ command-line tool for **lossless text file compression** using the Huffman Coding algorithm, with Base64 encoding for safe data transmission.

---

## ✨ Features

- 📦 **Lossless Compression** — Compress text files with zero data loss using Huffman Coding
- 🌳 **Optimal Prefix Codes** — Builds a Huffman tree using a Min-Heap (Priority Queue) for optimal code generation
- 🔢 **Bitwise Serialization** — Uses low-level bitwise operations for efficient binary data packing
- 🔐 **Base64 Encoding** — Encodes compressed binary output to Base64 for safe storage and network transmission
- ⚡ **Fast CLI Interface** — Simple compress and decompress commands via command-line flags

---

## 🛠️ Tech Stack

- **Language:** C++ (C++17)
- **Data Structures:** Binary Tree, Min-Heap (Priority Queue)
- **Concepts:** Huffman Coding, File I/O, Bitwise Operations, Base64 Encoding

---

## 📁 Folder Structure

```
huffman-compressor/
├── main.cpp       # Complete implementation
├── input.txt      # Sample input file
└── README.md
```

---

## ⚙️ How It Works

1. **Frequency Analysis** — Counts character frequencies in the input file
2. **Build Min-Heap** — Inserts all characters as nodes into a priority queue ordered by frequency
3. **Construct Huffman Tree** — Repeatedly merges the two lowest-frequency nodes until one root remains
4. **Generate Prefix Codes** — Traverses the tree to assign unique binary codes to each character
5. **Encode & Pack** — Replaces each character with its binary code and packs bits into bytes using bitwise operations
6. **Base64 Encode** — Converts binary output to Base64 for safe transmission
7. **Decompress** — Reverses the process: Base64 decode → bit unpack → Huffman tree traversal → original text

---

## 🚀 Compile & Run

### Compile

```bash
g++ -O3 -std=c++17 main.cpp -o huffman
```

### Compress a file

```bash
./huffman -c input.txt compressed.txt
```

### Decompress a file

```bash
./huffman -d compressed.txt output.txt
```

---

## 📊 Example

```
Input file:    input.txt     (1,200 bytes)
Compressed:    compressed.txt (~600 bytes)
Compression ratio: ~50%
```

Actual compression ratio depends on character frequency distribution in the input file. Files with fewer unique characters compress better.

---

## 🧠 Algorithm Details

| Step | Data Structure | Purpose |
|------|---------------|---------|
| Frequency count | `unordered_map` | Count character occurrences |
| Build tree | `priority_queue` (Min-Heap) | Always merge lowest-frequency nodes first |
| Code generation | Binary Tree traversal | Assign 0/1 codes via DFS |
| Bit packing | Bitwise ops (`<<`, `>>`, `&`, `\|`) | Pack codes into bytes efficiently |
| Safe encoding | Base64 | Ensure binary data is transmission-safe |

---

## 📋 Requirements

- C++17 or later
- Any C++ compiler (g++, clang++, MSVC)

---

## 👤 Author

**Kartik Pal** — [GitHub](https://github.com/kartik639667-lgtm) | [LinkedIn](https://www.linkedin.com/in/kartik-pal-193824316/)
