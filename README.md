# Soultion Description
- First of all, the main idea was to maintain a Priority Queue containing all the initial jobs with their arrival time.
- Then, time is simulated second by second in a main function.
- In each second the Priority Queue is checked if the top job has arrival time which equals the current time simulated in the main, if so this job is added to a temporary vector to be executed and is rescheduled with its new arrival time (old arrival time + frequency).
- Each job adds itself to completed jobs' priority queue after finishing.
- Logging is maintained throughout all actions, when a job arrives, starts running and completes.

# Reasoning behind your technical decisions.
- The use of a Priority Queue is justified because it allows us to directly access the most recently arrived job.
- Parallelizing loop of running the jobs to handle concurrent job execution.
- Using Shared Pointers to automatically de-allocate memory
- Used critical sections to avoid race conditions on adding completed jobs in Priority Queue or during logging.


# Trade-offs made, if any.
- Used Priority Queue instead of map containing the jobs in order to directly access the most recently arrived job.
- Used critical sections instead of locking on the log file to have one central logging system.


# Example usage snippet(s).
- First of all, make two `.txt` files one named `input.txt` and the other `log.txt`.
- In the input file each line represents a job, and each line has 3 entries (Arrival time, Execution interval, Frequency) respectively. 

    <b>below is an example for an input file containing 2 jobs</b>
```
1 2 4
2 2 1
```
- After running the program the output appears in the `log.txt`.

    <b>below is an example for an output file</b>
```
Job 1 arrived at 1
Job 1 started running at 1
Job 1 completed at 2
Job 2 arrived at 2
Job 2 started running at 2
Job 2 started running at 3
Job 2 completed at 4
```


# Possible future improvements.
- Make an event loop for the upcoming jobs instead of having an input file and using it as input for one time only.
- Use forking instead of OMP parallel to make each job a child process were it signals the main process when it finishes.