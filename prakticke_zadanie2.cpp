#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Struct to represent a job
struct Job {
    int id;
    int deadline;
    int profit;
};

bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

class DS {
private:
    vector<int> parent;

public:
    // Constructor: Initializes the disjoint set with `n` elements.

    DS(int n) {
        parent.resize(n + 1);
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    // Returns the root of the set containing `x`.
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        } return parent[x];
    }

    // Union operation: Merges the sets containing `u` and `v`.
    void unionSets(int u, int v) {
        parent[find(u)] = find(v);
        for (int i=0; i < parent.size(); i++){
            cout << parent[i] << " ";
        }
        cout << endl;
    }
};

void scheduleJobs(vector<Job>& jobs) {
    sort(jobs.begin(), jobs.end(), compare);
    int maxDeadline = 0;
    for (const auto& job : jobs) maxDeadline = max(maxDeadline, job.deadline);

    // initialize disjoint set
    DS ds(maxDeadline);
    vector<int> schedule(maxDeadline + 1, -1);
    int totalProfit = 0;

    for (int i=0; i < schedule.size(); i++){
        cout << schedule[i] << " ";
    }
    cout << endl;

    for (const auto& job : jobs) {
        // find latest available slot for the job
        int availableSlot = ds.find(min(maxDeadline, job.deadline));
        if (availableSlot > 0) {
            schedule[availableSlot] = job.id;
            for (int i=0; i < schedule.size(); i++){
                cout << schedule[i] << " ";
            }
            cout << endl;
            
            totalProfit += job.profit;
            ds.unionSets(availableSlot, availableSlot - 1);
        }
    }
    // Output
    cout << "Optimal sequence of jobs: ";
    for (int i = 1; i <= maxDeadline; i++) {
        if (schedule[i] != -1)
            cout << "Job" << schedule[i] << " ";
    } cout << "\nTotal profit: " << totalProfit << endl;
}

int main() {
    vector<Job> jobs = { {1, 2, 40}, {2, 4, 15}, {3, 3, 60}, {4, 2, 20}, {5, 3, 10}, {6, 1, 45}, {7, 1, 55} };
    scheduleJobs(jobs);
    return 0;
}
