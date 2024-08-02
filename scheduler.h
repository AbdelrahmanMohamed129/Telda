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

    // Function pointer to pass a call back function while creating a job for the first time. 
    function<ull()> function_;
    // using a friend class to make Scheduler access the private data members of Job.
    friend class Scheduler;
public:
    // Count the is used to give each new Job a unique UID.
    static int cnt;

    // Constructor to create a Job with a function pointer.
    template <typename _Callable, typename... _Args>
    Job(ull arrival_time, ull interval, ull frequency,
                 _Callable&& function, _Args&& ... args);
};

class Scheduler {
//    map<ull, vector<Job>> scheduled_jobs;
//    map<ull, vector<Job>> running_jobs;
    // Pri Q that holds a pair of arrival time and a Job pointer
    priority_queue<pair<ull, shared_ptr<Job>> , vector<pair<ull, shared_ptr<Job>>>, greater<pair<ull, shared_ptr<Job>>>> scheduled_jobs;
    // Pri Q that holds a pair of completion time and a Job pointer
    priority_queue<pair<ull, shared_ptr<Job>> , vector<pair<ull, shared_ptr<Job>>>, greater<pair<ull, shared_ptr<Job>>>> completed_jobs;
    // Holds the logging to print in the output file
    ofstream log;
public:
    // Use a default constructor.
    Scheduler() = default;
    // Copy constructor and assignment operator are deleted
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    // Move constructor and assignment operator are deleted
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    // Constructor that takes the input file name and the log file name.
    Scheduler(string filename, string log_filename = "log.txt");
    // The main function that simulates the scheduler.
    void run(ull time);
};


#endif //CRONSCHEDULER_SCHEDULER_H