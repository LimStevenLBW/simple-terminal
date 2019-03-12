Steven Lim
012763177
CECS 326 Lab 2

This is a simple terminal that can execute the ls,cp, and kill commands.

It begins by executing a prompt loop that reads the users input into a pointer
to pointer array. The input is tokenized and then analyzed for matches with implemented
functions and then executes them.

To test the cp command, src.txt is provided as a simple file to copy.
To test kill command, use top within a shell to find some unimportant process's PID.
Use the PID as the first argument, for example: kill 1111
