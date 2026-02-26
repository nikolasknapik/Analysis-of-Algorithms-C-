#include <iostream>
#include <cstdlib>
#include <unordered_map>
using namespace std;

#define MAX_TREE_HT 100  // Maximálna výška Huffmanovho stromu

// Huffmanov stromový uzol
struct MinHeapNode {
    char data;            // Znak
    unsigned freq;        // Frekvencia znaku
    struct MinHeapNode *left, *right;  // Ľavý a pravý podstrom
};

// MinHeap je prioritná fronta, ktorá obsahuje Huffmanove stromové uzly
struct MinHeap {
    unsigned size;        // Aktuálna veľkosť min-heapu
    unsigned capacity;    // Kapacita min-heapu
    struct MinHeapNode** array;  // Pole ukazovateľov na uzly
};

// Vytvorenie nového Huffmanovho uzla
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;  // Nový uzol nemá žiadne deti
    temp->data = data;  // Nastavuje znak
    temp->freq = freq;  // Nastavuje frekvenciu
    return temp;
}

// Vytvorenie min-heapu s danou kapacitou
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;  // Začíname s prázdnym heapom
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));  // Pole pre uzly
    return minHeap;
}

// Pomocná funkcia na výmenu dvoch uzlov v min-heape
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Funkcia minHeapify zabezpečuje, že strom dodržiava vlastnosti min-heapu
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Vyberá najmenšieho potomka
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    // Ak je nájdený najmenší, vymeníme uzly a pokračujeme
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Kontrola, či v heapu ostal len jeden uzol (pre dokončenie Huffmanovho stromu)
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extrakcia minimálneho uzla z min-heapu
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];  // Koreň je minimálny uzol
    minHeap->array[0] = minHeap->array[minHeap->size - 1];  // Posledný uzol sa presunie na miesto koreňa
    --minHeap->size;  // Zmenšíme veľkosť heapu
    minHeapify(minHeap, 0);  // Upravíme min-heap
    return temp;
}

// Vloženie nového uzla do min-heapu
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;  // Vloženie uzla na správne miesto
}

// Vytvorenie a úprava min-heapu podľa dát a frekvencií znakov
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Kontrola, či je uzol listový (bez detí)
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Vytvorenie min-heapu a vloženie všetkých znakov do heapu
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);  // Každý znak má svoj uzol
    minHeap->size = size;
    buildMinHeap(minHeap);  // Postavíme min-heap
    return minHeap;
}

// Hlavná funkcia, ktorá vytvára Huffmanov strom
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Kým v heapu ostane len jeden uzol
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);  // Najmenší prvý
        right = extractMin(minHeap);  // Druhý najmenší

        // Vytvorenie nového vnútorného uzla ako kombinácia dvoch najmenších
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);  // Vložíme späť do heapu
    }

    return extractMin(minHeap);  // Koreň stromu
}

// Ukladanie kódov do mapy podľa znakov
void storeCodes(struct MinHeapNode* root, int arr[], int top, unordered_map<char, string> &huffmanCodeMap) {
    if (root->left) {
        arr[top] = 0;  // Cesta doľava je 0
        storeCodes(root->left, arr, top + 1, huffmanCodeMap);
    }
    if (root->right) {
        arr[top] = 1;  // Cesta doprava je 1
        storeCodes(root->right, arr, top + 1, huffmanCodeMap);
    }
    if (isLeaf(root)) {  // Ak sme v listovom uzle, uložíme kód
        string code = "";
        for (int i = 0; i < top; ++i)
            code += to_string(arr[i]);
        huffmanCodeMap[root->data] = code;
    }
}

// Generuje Huffmanove kódy pre znaky na základe stromu
unordered_map<char, string> generateHuffmanCodes(char data[], int freq[], int size) {
    unordered_map<char, string> huffmanCodeMap;
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    storeCodes(root, arr, top, huffmanCodeMap);  // Uloží kódy do mapy
    return huffmanCodeMap;
}

// Dekódovanie zakódovaného reťazca pomocou Huffmanovho stromu
string decodeHuffman(struct MinHeapNode* root, string encodedStr) {
    string decodedStr = "";
    struct MinHeapNode* current = root;
    for (char bit : encodedStr) {
        if (bit == '0') {
            current = current->left;  // Cesta doľava
        } else {
            current = current->right;  // Cesta doprava
        }

        // Ak sa dostaneme na listový uzol, pridáme znak a začneme od koreňa
        if (isLeaf(current)) {
            decodedStr += current->data;
            current = root;
        }
    }
    return decodedStr;
}

// Hlavná funkcia (Driver code)
int main() {
    string word, encodedWord;
    int choice;
    
    // Preddefinované znaky a ich frekvencie
    char arr[] = { 'c', 'e', 'i', 'r', 's', 't', 'x' };  // Príklad znakov
    int freq[] = { 11, 22, 16, 12, 15, 10, 14};  // Frekvencie znakov
    int size = sizeof(arr) / sizeof(arr[0]);

    // Generovanie Huffmanových kódov
    unordered_map<char, string> huffmanCodeMap = generateHuffmanCodes(arr, freq, size);
    struct MinHeapNode* huffmanTreeRoot = buildHuffmanTree(arr, freq, size);

    // Nekonečná slučka na výber medzi zakódovaním a dekódovaním
    while (true) {
        cout << "\nChoose an option (1 for encoding, 2 for decoding, 0 to exit): ";
        cin >> choice;

        if (choice == 1) {
            // Zakódovanie
            cout << "Enter a word to encode: ";
            cin >> word;
            encodedWord = "";

            // Pre každý znak v slove nájdeme kód a zobrazíme ho
            cout << "Encoded word: ";
            for (char ch : word) {
                if (huffmanCodeMap.find(ch) != huffmanCodeMap.end()) {
                    encodedWord += huffmanCodeMap[ch];
                    cout << huffmanCodeMap[ch];
                } else {
                    cout << "?";  // Ak znak nie je v mape, zobrazíme '?'
                }
            }
            cout << endl;
        } 
        else if (choice == 2) {
            // Dekódovanie
            cout << "Enter the encoded word: ";
            cin >> encodedWord;

            // Dekódujeme zakódovaný reťazec naspäť na pôvodné slovo
            string decodedWord = decodeHuffman(huffmanTreeRoot, encodedWord);
            cout << "Decoded word: " << decodedWord << endl;
        } 
        else if (choice == 0) {
            // Ukončenie programu
            cout << "Exiting the program..." << endl;
            break;
        } 
        else {
            cout << "Invalid choice. Please select 1 for encoding, 2 for decoding, or 0 to exit." << endl;
        }
    }

    return 0;
}
