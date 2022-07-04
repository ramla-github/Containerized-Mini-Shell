Ramla Mohamed moha0864@umn.edu ID: 4943358

This Lab revolves around creating a shell script written using C language, it utilizes clone functions and fork to create a new namespace and network. The program is uses two functions to handle reading in input which are void parsing(char* line) and char *read_line(). The other functions such as void changeRoot() and void changeHostname() help perform basic commands which are executed by void commandExec(), void commandExec() also carries out the ability to quit the program and display the PID. 

The main function houses the clone function which creates the new namespace, utsname and network while the child_fn function starts up the shell and executes the input given by the user. 

The only issues with the program lie in the ability to perform the clone operations specified in the instructions such as 'ip link' or 'readlink' as for some reason the input is read in correctly but the execvp function won't carry out them out even though it performed the other built in commands such as 'ps' or 'ls'. Another issue I ran into was being able to use built in commands after changing the root, after I changed the root it wouldn't let me use any commands that weren't predefined in commandExec() function. 

Overall, through this Lab 1 I was able to get a better understanding of the functionality that accompanied creating a shell. 