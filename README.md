# OS-Lab
Codes for KTU S4 OS lab (CST 201).

## Experiments

### Experiment 1
- Bash scripts for;
  - calculating factorial of a number
  - finding fibanocci number
  - checking whether a number is palindrome or not
  - searching for a string file

### Experiment 3
- Simulating CPU scheduling algorithms;
  - FCFS
  - SJF
  - Priority
  - Round Robin


## How to run

For shell scripts, use the linux terminal to run the scripts. For example, to run the factorial script, use the following command:
```bash
chmod +x <file.sh>
./<file.sh>
```

For C programs, compile the program using gcc and run the executable. For example, to run the FCFS scheduling algorithm, use the following commands:
```bash
gcc fcfs.c -o fcfs
./fcfs
```

> NOTE: USE MOST C CODE ONLY WORKS POSIX SYSTEMS. WINDOWS USERS MAY NEED TO USE WSL OR CYGWIN TO RUN THE CODES.