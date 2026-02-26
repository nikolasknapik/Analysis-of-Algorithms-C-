#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

// Struct to represent symbol
struct Symbol {
    char character;
    int frequency;
    int codeLength;
    string code;
};

// Function to convert an integer to a binary string of a given length
string intToBinary(int value, int length) {
    string binary = "";
    for (int i = length - 1; i >= 0; i--) {
        if ((value >> i) & 1)
            binary += '1';
        else
            binary += '0';
    }
    return binary;
}

void canonicalHuffmanCoding(vector<Symbol>& symbols) {
    int n = symbols.size();
    vector<int> codeLengths(n, 0);

    // group initialization. Each symbol starts as its own group
    vector<vector<int>> groups;
    for (int i = 0; i < n; i++) {
        groups.push_back({i});
    }

    // frequencies for groups
    vector<int> groupFreqs;
    for (int i = 0; i < n; i++) {
        groupFreqs.push_back(symbols[i].frequency);
    }

    // merging groups
    while (groups.size() > 1) {
        int min1 = 0, min2 = 1;
        if (groupFreqs[min1] > groupFreqs[min2]) {
            swap(min1, min2);
        }
        for (int i = 2; i < groups.size(); i++) {
            if (groupFreqs[i] < groupFreqs[min1]) {
                min2 = min1;
                min1 = i;
            } else if (groupFreqs[i] < groupFreqs[min2]) {
                min2 = i;
            }
        }

        // Increment code lenghts for symbols in the merged groups
        for (int idx : groups[min1]) {
            codeLengths[idx]++;
        }
        for (int idx : groups[min2]) {
            codeLengths[idx]++;
        }

        // Merge the two groups
        groups[min1].insert(groups[min1].end(), groups[min2].begin(), groups[min2].end());
        groupFreqs[min1] += groupFreqs[min2];

        // remove the second group
        groups.erase(groups.begin() + min2);
        groupFreqs.erase(groupFreqs.begin() + min2);
    }

    // assign the code lengths to the symbols
    for (int i = 0; i < n; i++) {
        symbols[i].codeLength = codeLengths[i];
    }

    // sort symbols by code length, then lexicographically by character
    sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {
        if (a.codeLength == b.codeLength)
            return a.character < b.character;
        else
            return a.codeLength < b.codeLength;
    });

    // assign huffman codes
    int code = 0;
    int prevLength = 0;
    for (int i = 0; i < n; i++) {
        if (symbols[i].codeLength > prevLength) {
            code <<= (symbols[i].codeLength - prevLength);
            prevLength = symbols[i].codeLength;
        }
        symbols[i].code = intToBinary(code, symbols[i].codeLength);
        code++;
    }
}

void frequencyBasedApproximation(vector<Symbol>& symbols) {

    // calculate total frequency
    int totalFrequency = 0;
    for (const auto& sym : symbols) {
        totalFrequency += sym.frequency;
    }

    // aproximate codelenghts using 
    for (auto& sym : symbols) {
        double probability = (double)sym.frequency / totalFrequency;
        sym.codeLength = (int)ceil(-log2(probability));
    }

    // sort symbols by lenght, then lexicographically by character
    sort(symbols.begin(), symbols.end(), [](Symbol a, Symbol b) {
        if (a.codeLength == b.codeLength)
            return a.character < b.character;
        else
            return a.codeLength < b.codeLength;
    });

    // assign canonical codes
    int code = 0;
    int prevLength = 0;
    for (int i = 0; i < symbols.size(); i++) {
        if (symbols[i].codeLength > prevLength) {
            code <<= (symbols[i].codeLength - prevLength);
            prevLength = symbols[i].codeLength;
        }
        symbols[i].code = intToBinary(code, symbols[i].codeLength);
        code++;
    }
}

string encodeMessage(string message, vector<Symbol>& symbols) {
    string encodedMessage = "";
    for (int i = 0; i < message.length(); i++) {
        char ch = message[i];
        string code = "";
        // find corresponding code for each character
        for (int j = 0; j < symbols.size(); j++) {
            if (symbols[j].character == ch) {
                code = symbols[j].code;
                break;
            }
        }
        if (code != "") {
            encodedMessage += code;
        } else {
            cout << "Znak " << ch << " sa nenasiel v symboloch.\n";
        }
    }
    return encodedMessage;
}

string decodeMessage(string encodedMessage, vector<Symbol>& symbols) {
    string decodedMessage = "";
    string buffer = "";
    for (int i = 0; i < encodedMessage.length(); i++) {
        buffer += encodedMessage[i];
        bool found = false;
        // match buffer with a code
        for (int j = 0; j < symbols.size(); j++) {
            if (symbols[j].code == buffer) {
                decodedMessage += symbols[j].character;
                buffer = "";
                found = true;
                break;
            }
        }
        if (!found && buffer.length() > symbols.back().codeLength) {
            cout << "Chyba pri dekodovani: neplatny kod.\n";
            break;
        }
    }
    return decodedMessage;
}

int main() {
    // vector symbols, contains character, frequency, 0 sprace for lenght. "" space for code
    vector<Symbol> symbols = {
        {'A', 45, 0, ""},
        {'B', 13, 0, ""},
        {'C', 12, 0, ""},
        {'D', 16, 0, ""},
        {'E', 9,  0, ""},
        {'F', 5,  0, ""}
    };

    vector<Symbol> symbolsCanonical = symbols;
    vector<Symbol> symbolsApproximation = symbols;

    canonicalHuffmanCoding(symbolsCanonical);

    cout << "Kanonicke Huffmanove kody:\n";
    cout << "Znak\tFrekvencia\tDlzka kodu\tKod\n";
    for (int i = 0; i < symbolsCanonical.size(); i++) {
        cout << symbolsCanonical[i].character << "\t" << symbolsCanonical[i].frequency << "\t\t" << symbolsCanonical[i].codeLength << "\t\t" << symbolsCanonical[i].code << "\n";
    }

    frequencyBasedApproximation(symbolsApproximation);

    cout << "\nKody na zaklade aproximacie frekvencie:\n";
    cout << "Znak\tFrekvencia\tDlzka kodu\tKod\n";
    for (int i = 0; i < symbolsApproximation.size(); i++) {
        cout << symbolsApproximation[i].character << "\t" << symbolsApproximation[i].frequency << "\t\t" << symbolsApproximation[i].codeLength << "\t\t" << symbolsApproximation[i].code << "\n";
    }

    string message = "ACBDE";
    cout << "\nSprava na kodovanie: " << message << "\n";

    string encodedCanonical = encodeMessage(message, symbolsCanonical);
    cout << "\nZakodovana sprava pomocou Kanonickych Huffmanovzch kodov: " << encodedCanonical << "\n";
    string decodedCanonical = decodeMessage(encodedCanonical, symbolsCanonical);
    cout << "Dekodovaná sprava: " << decodedCanonical << "\n";

    string encodedApproximation = encodeMessage(message, symbolsApproximation);
    cout << "\nZakodovana sprava pomocou Aproximacie frekvencie: " << encodedApproximation << "\n";
    string decodedApproximation = decodeMessage(encodedApproximation, symbolsApproximation);
    cout << "Dekodovana sprava: " << decodedApproximation << "\n";

    return 0;
}
