#include <iostream>
#include <bits/stdc++.h>
//#include <omp.h>
using namespace std;

#define ull unsigned long long
#ifndef CRONSCHEDULER_SCHEDULER_H
#define CRONSCHEDULER_SCHEDULER_H

class Job {
    int UID;
    ull arrival_time;
    ull interval;
    ull frequency;
    bool first_run = true;
    function<ull()> function_;
    friend class Scheduler;
public:
    static int cnt;
    template <typename _Callable, typename... _Args>
    Job(ull arrival_time, ull interval, ull frequency,
                 _Callable&& function, _Args&& ... args);
};

class Scheduler {
//    map<ull, vector<Job>> scheduled_jobs;
//    map<ull, vector<Job>> running_jobs;
    priority_queue<pair<ull, shared_ptr<Job>> , vector<pair<ull, shared_ptr<Job>>>, greater<pair<ull, shared_ptr<Job>>>> scheduled_jobs;
    priority_queue<pair<ull, shared_ptr<Job>> , vector<pair<ull, shared_ptr<Job>>>, greater<pair<ull, shared_ptr<Job>>>> completed_jobs;
    ofstream log;
public:
    Scheduler() = default;
    // Copy constructor and assignment operator are deleted
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    // Move constructor and assignment operator are deleted
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    Scheduler(string filename, string log_filename = "log.txt");
    void run(ull time);
};


#endif //CRONSCHEDULER_SCHEDULER_H