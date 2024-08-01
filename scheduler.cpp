#include "scheduler.h"

int Job::cnt = 1;

template <typename _Callable, typename... _Args>
Job::Job(ull arrival_time, ull interval, ull frequency,
         _Callable&& function, _Args&& ... args) :
        arrival_time{arrival_time},
        interval{interval},
        frequency{frequency},
        function_{bind(forward<_Callable>(function), forward<_Args>(args)...)} {
    if (interval < 1) {
        throw runtime_error("CronJob's interval can't be less than 1 second");
    }
    if (frequency < 1) {
        throw runtime_error("CronJob's frequency can't be less than 1 second");
    }
    UID = Job::cnt++;
}

Scheduler::Scheduler(string filename, string log_filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("File not found");
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        ull arrival_time, interval, frequency;
        iss >> arrival_time >> interval >> frequency;
        scheduled_jobs.push({arrival_time, std::make_shared<Job>(arrival_time, interval, frequency, [interval]() -> ull {
            return max(1ull, interval + (1ull)*(rand() % 5 - 2));
        })});
    }
    log = ofstream(log_filename);
}

void Scheduler::run(ull time) {
    while(!completed_jobs.empty() && completed_jobs.top().first <= time) {
        // log that the job is completed
        auto job = completed_jobs.top().second;
        completed_jobs.pop();
        log << "Job " << job->UID << " completed at " << time << endl;
    }
    vector<shared_ptr<Job>> jobs_to_run;
    while(!scheduled_jobs.empty() && scheduled_jobs.top().first == time) {
        auto job = scheduled_jobs.top().second;
        if(job->first_run) {
            job->first_run = false;
            log << "Job " << job->UID << " arrived at " << time << endl;
        }
        scheduled_jobs.pop();
        scheduled_jobs.push({time + job->frequency, job});  // reschedule the job
        jobs_to_run.push_back(job);
    }

#pragma omp parallel for
    for (auto job : jobs_to_run) {
#pragma omp critical
        {
            log << "Job " << job->UID << " started running at " << time << endl;
        }
        ull actual_interval= job->function_();
#pragma omp critical
        {
            completed_jobs.push({time + actual_interval, job});
        }
    }
}