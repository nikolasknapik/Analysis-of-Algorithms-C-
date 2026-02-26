#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int minimizeMoves(vector<int>& fileSizes) {
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int size : fileSizes) {
        minHeap.push(size);
    }

    int totalMoves = 0;

    while (minHeap.size() > 1) {
        int first = minHeap.top(); minHeap.pop();
        int second = minHeap.top(); minHeap.pop();
        int mergedSize = first + second;
        totalMoves += mergedSize;
        minHeap.push(mergedSize);
    }

    return totalMoves;
}

int main() {
    vector<int> files = {5,6,9,20,50,3,70,5,1,30};

    cout << "Minimal number of moves for files1: " << minimizeMoves(files) << endl;

    return 0;
}
