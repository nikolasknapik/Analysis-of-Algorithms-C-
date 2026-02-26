# AZA Practical Assignments (C++)

This repository contains several C++ practice programs focused on greedy algorithms and Huffman coding.

## Files

- `prakticke_zadanie.cpp`  
  Job sequencing by profit and deadline (basic greedy version).

- `prakticke_zadanie2.cpp`  
  Job sequencing using a disjoint-set (union-find) structure for slot assignment.

- `prakticke_zadanie3.cpp`  
  Minimal file merge cost (optimal merge pattern with a min-heap).

- `prakticke_zadanie4.cpp`  
  Canonical Huffman coding and frequency-based code-length approximation, including encode/decode examples.

- `HufCode.cpp`  
  Interactive Huffman encoder/decoder using a manually built Huffman tree.

- `Nikolas_Knapik_AZA_practical_dokumetation.pdf`  
  Project documentation.

## Requirements

- C++ compiler with C++11 or newer support (e.g., `g++`)

## Build and Run

Compile any source file:

```bash
g++ -std=c++11 -O2 -Wall -Wextra -o app <source_file.cpp>
```

Examples:

```bash
g++ -std=c++11 -O2 -Wall -Wextra -o zad1 prakticke_zadanie.cpp
./zad1

g++ -std=c++11 -O2 -Wall -Wextra -o zad2 prakticke_zadanie2.cpp
./zad2

g++ -std=c++11 -O2 -Wall -Wextra -o zad3 prakticke_zadanie3.cpp
./zad3

g++ -std=c++11 -O2 -Wall -Wextra -o zad4 prakticke_zadanie4.cpp
./zad4

g++ -std=c++11 -O2 -Wall -Wextra -o huf HufCode.cpp
./huf
```
