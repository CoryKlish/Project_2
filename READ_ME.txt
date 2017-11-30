PROGRAM OVERVIEW

//Cory Klish
//Josh Pineda

///////////////////////////~~PART 0~~//////////////////////////////////////////

Our program consists of 2 major 'C' files: "sorter.c" and "mergesort.c".
In "sorter.c" we handled the reading in of the CSV file through stdin using
strtok to read in the sort types, strsep to fill in struct attributes, 
getline to parse each row, and a dynamically allocated array for
structures using malloc. The hardest part in this process of "sorter.c"
was dealing with commas contained inside movie titles. For example,
"The Good, The Bad, and The Ugly" would be read in as "The Good The Bad
and The Ugly" without commas. We were able read in the field by changing 
the delimeter from ',' to quotes, but scanning in would not take in the
commas. 

Moving on to "mergesort.c", we originally implemented 2 simple mergesort 
algoritms, 1 to sort an array of strings, and one to sort an array of numbers.
Later, we had an idea to possibly sort an array of structs. We thought this would
be a huge undertaking but it turns out the change was only a few lines of code.
So our mergesort consists of a sort by number, and a sort by string function; 
each taking in the array of structs. The mergesort we implemented uses strcmp
and strcasecmp. It will sort in lexicographical order, ignoring case, UNLESS
2 strings are exactly the same. In this case 'A' would come before 'a' using
strcmp.

To use the code, a user inputs all lines of the CSV, and types in the way
they want to sort it. The program reads in these fields, and sorts using 
mergesort.

Compile normally: "gcc sorter.c mergesort.c -o sorter"

///////////////////////////~~PART 1~~//////////////////////////////////////////

In this section of the project, we added the ability to navigate through directories
and their respective ".csv" files through using the fork() system call to sort each 
file. We accomplished this using many libraries and 3 main ones: dirent.h (used to navigate 
directories, recognize new files, and open them), unistd.h & sys/wait.h (allowing us to call
fork() and wait() on our spawned processes. We used a recursive method to find and open all
directories and CSVs with the "dirent" library. The hardest part for us was definitely
summing the total number of processes. We were able to do this in the trivial case, where
we waited in the parent for each child (within the loop) and used WEXITSTATUS. 

We intended to wait outside of the while loop, but we were unable to.
It created errors in printing PIDS of individual processes (perhaps because it was overwriting data by other processes?)
Instead, we waited for each child process in the parent processes
Currently, it shows the total number of processes as the number of csv files and it does not factor in
directories when it accumulates the total number of processes. 


/////////////////////////~~PART 2~~//////////////////////////////////////////

For this part of the project, we were tasked with changing our fork() calls to pthread_create() 
calls. Along with this, we had to change how the sorting actually happend. We would first find
valid CSVs and sort them. Next, we would put all these sorted files into one big data structure
and sort, outputting to a bigger file. This project was fundamentally more difficult than the
fork() project because of all the synchronization constructs that needed to be implemented. There 
are many locks in the program so no 2 threads access certain areas simultaneously. This process
took a lot of thinking with pencil and paper before we actually got to coding. 

The biggest challenge was between 2 things: 1) knowing when all threads are finished so we can join
without other threads being created, and 2) a memory corruption error when certain amount of files
were used. We (hopefully) fixed the joining error, so that it will print the correct number of threads
roughly 99% of the time. We are currently still working on fixing the error on the memory corruption. 
So far, these directory structures work fine:

















