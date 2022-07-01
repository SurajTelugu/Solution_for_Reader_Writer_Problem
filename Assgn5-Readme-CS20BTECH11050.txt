Instructions: // Change output

Input in file named "inp-params.txt" should be in the following format:

 1) Number of Writer Threads (nw)
 1) Number of Reader Threads (nr)
 2) Number of times each writer thread enters the critical section (kw)
 2) Number of times each reader thread enters the critical section (kr)
 3) (ucs) which is used as average of an exponential distribution to get critical section time (randCSTime)
 4) (urem) which is used as average of an exponential distribution to get reminder section time (randRemTime)
 5) Example:
    10 10 5 4 0.1 0.2

For Linux Operating Systems:

 1) Go to the folder containing the code file and run the following commands in Linux terminal
      command1: $g++ Assn5-rw-CS20BTECH11050.cpp -lpthread
      command2:$./a.out

      command1: $g++ Assn5-frw-CS20BTECH11050.cpp -lpthread
      command2:$./a.out
    
    To name the output file run the following commands
      command1: $g++ -o <name of output> Assn5-rw-CS20BTECH11050.cpp -lpthread
      command2:$./<name of output> 

      command1: $g++ -o <name of output> Assn5-frw-CS20BTECH11050.cpp -lpthread
      command2:$./<name of output> 

    Note:"$" in commands above is just for linux terminal reference
         Do not use spaces or any symbols (except underscore(_)) in <name of output>

  2) Output Format:
     1) RW-log.txt and RW_Average_time.txt files are obtained after compiling Assn5-rw-CS20BTECH11050.cpp
     2) FRW-log.txt file and FRW_Average_time.txt files are obtained after compiling Assn5-frw-CS20BTECH11050.cpp
     
For Other Operating Systems:

 1) Use any IDE which has inbuilt linux libraries and use same commands as above

 2) You can also use the following online IDEs which uses Ubuntu(Linux)
      https://www.onlinegdb.com/online_c_compiler
      https://replit.com/~
    You can upload the source code in any of the above online IDEs 
    and run directly or you can also use the above commands to run your file 