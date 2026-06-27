#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// --- BASE64 ENCODER & DECODER ---
const string b64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(const string& bindata) {
    string ret;
    int i = 0, j = 0;
    unsigned char char_array_3[3], char_array_4[4];
    int in_len = bindata.length();
    const char* bytes_to_encode = bindata.c_str();

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++) ret += b64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++) char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) ret += b64_chars[char_array_4[j]];
        while((i++ < 3)) ret += '=';
    }
    return ret;
}

string base64_decode(const string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0, j = 0, in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) char_array_4[i] = b64_chars.find(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++) ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) char_array_4[j] = 0;
        for (j = 0; j < 4; j++) char_array_4[j] = b64_chars.find(char_array_4[j]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; j < i - 1; j++) ret += char_array_3[j];
    }
    return ret;
}

// --- HUFFMAN LOGIC ---
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r) : ch('\0'), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        if (l->freq == r->freq) return l->ch > r->ch;
        return l->freq > r->freq;
    }
};

void generateCodes(Node* root, const string& str, map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str.empty() ? "0" : str;
    }
    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// --- CORE FUNCTIONS ---
void compress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cerr << "Error opening input file!" << endl;
        return;
    }

    map<char, int> freq;
    char ch;
    string contents = "";
    while (in.get(ch)) {
        freq[ch]++;
        contents += ch;
    }
    in.close();

    if (contents.empty()) {
        cerr << "File is empty!" << endl;
        return;
    }

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freq) pq.push(new Node(pair.first, pair.second));

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left->freq + right->freq, left, right));
    }
    Node* root = pq.top();

    map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // Write to stringstream instead of directly to file
    stringstream outStream(ios::out | ios::binary);
    
    int mapSize = freq.size();
    outStream.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    
    for (auto pair : freq) {
        outStream.write(&pair.first, sizeof(pair.first));
        outStream.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

    int totalChars = contents.size();
    outStream.write(reinterpret_cast<const char*>(&totalChars), sizeof(totalChars));

    unsigned char bitBuffer = 0;
    int bitCount = 0;

    for (char c : contents) {
        string code = huffmanCode[c];
        for (char bit : code) {
            bitBuffer <<= 1;
            if (bit == '1') bitBuffer |= 1;
            bitCount++;

            if (bitCount == 8) {
                outStream.put(bitBuffer);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        outStream.put(bitBuffer);
    }

    freeTree(root);

    // ENCODE TO BASE64 AND WRITE TO DISK
    string rawBinary = outStream.str();
    string base64String = base64_encode(rawBinary);

    ofstream finalOut(outputFile);
    finalOut << base64String;
    finalOut.close();

    cout << "Compression successful! File is now Base64 encoded." << endl;
}

void decompress(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error opening compressed file!" << endl;
        return;
    }

    // READ BASE64, DECODE TO BINARY MEMORY STREAM
    string base64String((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    string rawBinary = base64_decode(base64String);
    stringstream in(rawBinary, ios::in | ios::binary);

    // HUFFMAN DECOMPRESSION
    int mapSize;
    in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    map<char, int> freq;
    for (int i = 0; i < mapSize; i++) {
        char ch;
        int f;
        in.read(&ch, sizeof(ch));
        in.read(reinterpret_cast<char*>(&f), sizeof(f));
        freq[ch] = f;
    }

    int totalChars;
    in.read(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freq) pq.push(new Node(pair.first, pair.second));

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left->freq + right->freq, left, right));
    }
    Node* root = pq.top();

    ofstream out(outputFile, ios::binary);
    Node* current = root;
    int charsDecoded = 0;
    char byte;

    while (in.get(byte) && charsDecoded < totalChars) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            
            if (bit == 0) current = current->left;
            else current = current->right;

            if (!current->left && !current->right) {
                out.put(current->ch);
                charsDecoded++;
                current = root;
                if (charsDecoded == totalChars) break; 
            }
        }
    }

    out.close();
    freeTree(root);
    cout << "Decompression successful! Restored from Base64." << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage:\n";
        cout << "  Compress:   " << argv[0] << " -c <input_file> <output_file>\n";
        cout << "  Decompress: " << argv[0] << " -d <input_file> <output_file>\n";
        return 1;
    }

    string flag = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (flag == "-c") compress(inputFile, outputFile);
    else if (flag == "-d") decompress(inputFile, outputFile);
    else {
        cerr << "Invalid flag. Use -c to compress or -d to decompress." << endl;
        return 1;
    }

    return 0;
}