#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Job {
    int id;
    int deadline;
    int profit;
};

bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

void scheduleJobs(vector<Job>& jobs) {
    // sorting jobs by profit
    sort(jobs.begin(), jobs.end(), compare);

    // getting max deadline out of jobs
    int maxDeadline = 0;
    for (const auto& job : jobs) {
        maxDeadline = max(maxDeadline, job.deadline) + 1;
    }

    //schedule vector with maxdeadline size
    vector<int> schedule(maxDeadline, -1);
    int totalProfit = 0;

    // scheduling jobs
    for (int i = 0; i < jobs.size(); i++) {
        if (schedule[jobs[i].deadline] == -1) {
            schedule[jobs[i].deadline] = jobs[i].id;
            totalProfit += jobs[i].profit;
        }
    }

    // printing sequence
    cout << "Optimal sequence of jobs: ";
    for (int i = 0; i < maxDeadline; i++) {
        if (schedule[i] != -1)
            cout << "Job" << schedule[i] << " ";
    }
    cout << "\nTotal profit: " << totalProfit << endl;
}

int main() {
    vector<Job> jobs = { {1, 2, 40}, {2, 4, 15}, {3, 3, 60}, {4, 2, 20}, {5, 3, 10}, {6, 1, 45}, {7, 1, 55}};
    
    scheduleJobs(jobs);

    return 0;
}
