#include "scheduler.h"

// Initialize the static data member.
int Job::cnt = 1;

template <typename _Callable, typename... _Args>
Job::Job(ull arrival_time, ull interval, ull frequency,
         _Callable&& function, _Args&& ... args) :
        arrival_time{arrival_time},
        interval{interval},
        frequency{frequency},
        function_{bind(forward<_Callable>(function), forward<_Args>(args)...)} {
    // Validation that each job should have running time or frequency greater than 1 second.
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
        // On reading each line in the file make a new job with the info given in the file and make a call back function that returns a 
        // randomized running time for each function where it takes the given interval and adds a random number from -2 to 2.
        scheduled_jobs.push({arrival_time, std::make_shared<Job>(arrival_time, interval, frequency, [interval]() -> ull {
            return max(1ull, interval + (1ull)*(rand() % 5 - 2));
        })});
    }
    log = ofstream(log_filename);
}

void Scheduler::run(ull time) {
    // Check for all the completed jobs if they finished then we should log in the logging file.
    while(!completed_jobs.empty() && completed_jobs.top().first <= time) {
        // log that the job is completed
        auto job = completed_jobs.top().second;
        completed_jobs.pop();
        log << "Job " << job->UID << " completed at " << time << endl;
    }
    // jobs_to_run is a temporary vector that holds all the new Jobs that will go from the scheduled_jobs to completed_jobs.
    vector<shared_ptr<Job>> jobs_to_run;
    // For each new time check the top scheduled job if its time to run is now then it is moved to the jobs_to_run and then
    // requeued in the scheduled_jobs with current time + frequency.
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

    // Run all the jobs in parallel to ensure concurrency
#pragma omp parallel for
    for (auto job : jobs_to_run) {
        // Making the logging in here in a critical section to make sure that each job logs itself and don't overwrite any other job.
#pragma omp critical
        {
            log << "Job " << job->UID << " started running at " << time << endl;
        }
        // Running the actual function in each Job.
        ull actual_interval= job->function_();
        // Pushing the job in the completed_jobs with the time it will finish, also making sure that this is done in critical section
        // to make sure that each jobs is queued without overwriting other jobs.
#pragma omp critical
        {
            completed_jobs.push({time + actual_interval, job});
        }
    }
}