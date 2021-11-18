#include <iostream>
#include <cstring>
#include <unordered_map> // To use hashmaps
#include <queue>         // To use priority queue
#include <bitset>        // For printing binary representation of data
#include <fstream>       // To access and read files

using namespace std;

// To calculate compression ratio, we define the following two global variables
int bitOriginal = 0;
int bitHuffman = 0;

// Huffman Tree node structure
struct TreeNode
{
    char ch;
    int freq;
    TreeNode *leftChild, *rightChild;
};

// Dynamic allocation of new tree node
TreeNode *Node(char ch, int freq, TreeNode *leftChild, TreeNode *rightChild)
{
    TreeNode *newNode = new TreeNode();

    newNode->ch = ch;
    newNode->freq = freq;
    newNode->leftChild = leftChild;
    newNode->rightChild = rightChild;

    return newNode;
}

// Custom comparator for priority queue
// Swapping will take place when frequency of first node is greater than that of second
class myCompare
{
public:
    bool operator()(TreeNode *leftChild, TreeNode *rightChild)
    {
        return leftChild->freq > rightChild->freq;
    }
};

// Traversing Huffman Tree and storing the codes in a hashmap for easy access
void createHuffmanCode(TreeNode *root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == NULL)
    {
        return;
    }

    if (!root->leftChild && !root->rightChild) // Leaf node reached and thus no path to move ahead
    {
        huffmanCode[root->ch] = str;
    }

    createHuffmanCode(root->leftChild, str + "0", huffmanCode);  // 0 is assigned to left
    createHuffmanCode(root->rightChild, str + "1", huffmanCode); // 1 is assigned to right
}

// Traversing the Huffman Tree to decompress the encoded string
// Since this is a prefix coding technique, we will never encounter a case when a prefix of one code is a code for another character
void deCompress(TreeNode *root, int &index, string str)
{
    if (root == NULL)
    {
        return;
    }

    if (!root->leftChild && !root->rightChild)
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] == '0')
    {
        deCompress(root->leftChild, index, str);
    }
    else
    {
        deCompress(root->rightChild, index, str);
    }
}

void HuffmanTree(string text)
{
    unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }

    // Min priority queue
    priority_queue<TreeNode *, vector<TreeNode *>, myCompare> minHeap;

    // Creating node for each character
    for (auto pair : freq)
    {
        minHeap.push(Node(pair.first, pair.second, NULL, NULL));
    }

    while (minHeap.size() != 1)
    {
        // Selecting two nodes of lowest frequency, removing them from the queue, adding their frequencies to create a new node and finally push this new node.
        TreeNode *leftChild = minHeap.top();
        minHeap.pop();
        TreeNode *rightChild = minHeap.top();
        minHeap.pop();

        int sum = leftChild->freq + rightChild->freq;
        minHeap.push(Node('\0', sum, leftChild, rightChild));
    }

    TreeNode *root = minHeap.top();

    unordered_map<char, string> huffmanCode;
    createHuffmanCode(root, "", huffmanCode);

    cout << "Huffman codes generated for characters in the file are as follows: " << endl;

    for (auto pair : huffmanCode)
    {
        cout << pair.first << " " << pair.second << endl;
    }

    cout << endl
         << "Original string was: " << endl
         << text << endl;

    string str = "";
    for (char ch : text)
    {
        str += huffmanCode[ch];
    }

    cout << endl
         << "The compressed code is: " << endl
         << str << endl;
    cout << endl
         << "Bit requirement after compression: " << str.length() << endl;
    bitHuffman = str.length();

    int index = -1;
    cout << endl
         << "The decompressed text is: " << endl;
    while (index < (int)str.size() - 2)
    {
        deCompress(root, index, str);
    }
    cout << endl
         << "Bit by bit match obtained after decompression." << endl;
}

void stringToBinary(string s)
{
    cout << endl
         << "Binary representation of the original data is: " << endl;

    for (int i = 0; i <= s.length(); i++)
    {
        int binValue = int(s[i]);

        bitset<8> bin(binValue);

        cout << bin << " ";
    }
    // Bit requirement of original data
    cout << endl;
    cout << endl
         << "Bit requirement before compression: " << 8 * s.length() << endl;
    cout << endl;
    bitOriginal = 8 * s.length();
}

string fileToString(const string &filepath)
{
    ifstream inputFile(filepath);
    if (!inputFile.is_open())
    {
        cerr << "Could not open your file!!! Please check your path." << endl;
        exit(0);
    }
    return string((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
}

int main()
{
    string input("input.txt");

    string fileText;

    fileText = fileToString(input);

    fileText.erase(remove(fileText.begin(), fileText.end(), '\n'), fileText.end()); // To remove newline character for demo

    stringToBinary(fileText);

    HuffmanTree(fileText);

    cout << endl
         << "Compression ratio is: " << (float)bitOriginal / bitHuffman << endl;
    cout << endl;
    cout << "Code written by Sajal Aggarwal IT-124 for IT-201 Data Structures MTE Project.";

    return 0;
}