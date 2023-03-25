CS5323 - DESIGN AND IMPLEMENTATION OF OPERATING SYSTEM II
ASSIGNMENT 02
SUBMITTED BY - S M RAFIUDDIN
CWID: A20387294



HOW TO RUN THE CODE:

1. LOGIN TO CSX SERVER.
2. UPLOAD THE CODE TO THE SERVER YOU LOGGED IN.
3. COMPILE THE CODE IN THE CMD USING THE FOLLOWING COMMAND-

>> gcc A02.c -lpthread

4. THEN EXECUTE THE CODE USING THE FOLLOWING COMMAND-

>> ./a.out 4


INPUT:

GIVEN IN THE ARGUMENT
(Here 4 is the number of readers passed from command prompt)

EXPECTED OUTPUT:

Creating Thread 1
Creating Thread 2
Creating WRITER thread
READER 1 thread has started its execution
Critical Section has started
Creating Thread 3
Creating Thread 4
WRITER thread has started its execution.
Exiting from main loop
WRITER thread has completed its execution.
Critical section has ended
READER 1 thread has completed its execution
READER 2 thread has started its execution
Critical Section has started
Critical section has ended
READER 2 thread has completed its execution
READER 3 thread has started its execution
Critical Section has started
Critical section has ended
READER 3 thread has completed its execution
READER 4 thread has started its execution
Critical Section has started
Critical section has ended
READER 4 thread has completed its execution