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

In this section of the project, we added the ability to navigate through 
