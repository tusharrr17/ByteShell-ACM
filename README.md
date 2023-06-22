# ByteShell

## Project's Title
ByteShell

## Project Description
ByteShell is a simple shell program implemented in C. It provides a command-line interface where users can execute various commands and perform basic operations such as changing directories, creating directories, removing files, and echoing input.

## How to Run the Project
To run the project, follow these steps:

1. Ensure you have a C compiler installed on your system (e.g., GCC).
2. Save the provided code in a file named `byteshell.c`.
3. Compile the code using the following command:
``` 
gcc -o byteshell test.c
```
4. Run the compiled program:
```
./test
```

## Dependencies
The project does not have any external dependencies. It uses standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `unistd.h`, `sys/types.h`, `sys/wait.h`) that are usually available in C development environments.

## Internal Working of the Project
The ByteShell project is implemented as a simple shell that emulates the behavior of a typical command-line shell. It follows these steps to process user input and execute commands:

1. **Initialization:**
- The `main` function invokes the `bytsh_loop` function, which creates a loop for the shell.
- The loop runs continuously until the user chooses to exit.

2. **Prompting for Input:**
- Inside the loop, the shell displays a prompt to the user, indicating that it is ready to accept input.
- The prompt is typically represented as `> `.

3. **Reading User Input:**
- The shell uses the `bytsh_read_line` function to read the input provided by the user.
- The `bytsh_read_line` function reads the input character by character, dynamically allocating memory to store the input string as needed.
- It continues reading until it encounters the end-of-line character (`\n`) or the end-of-file character (`EOF`).
- The input string is returned to the calling function.

4. **Parsing User Input:**
- The shell uses the `bytsh_split_line` function to split the input string into individual tokens (command and arguments).
- The `bytsh_split_line` function uses whitespace characters (space, tab, newline, carriage return, and form feed) as delimiters to separate the tokens.
- The function dynamically allocates memory to store the token array as needed.
- The token array, represented as an array of strings (`char**`), is returned to the calling function.

5. **Executing Commands:**
- The shell checks if the entered command is a built-in command by comparing it with the list of built-in commands (`builtin_str`).
- If the command is a built-in command, the corresponding built-in function is executed (`bytsh_cd`, `bytsh_help`, etc.).
- If the command is not a built-in command, the shell launches a new process to execute the external command using the `bytsh_launch` function.
- The `bytsh_launch` function creates a child process using the `fork` system call.
- In the child process, the `execvp` system call is used to replace the child process with the desired command, executing it in the same environment.
- The parent process waits for the child process to complete using the `waitpid` system call.
- After the child process finishes executing, the loop continues, prompting for the next input.

6. **Exiting the Shell:**
- The shell loop continues until the user enters the "exit" command or uses another method to terminate the program.
- The `bytsh_exit` function is responsible for exiting the shell loop when the "exit" command is entered by returning a status value of 0.

## Learning Takeaways
Working on the ByteShell project can provide several learning takeaways, including:
- Understanding the basics of shell programming and command-line interfaces.
- Learning how to handle user input and execute commands in a shell environment.
- Gaining familiarity with C programming concepts and practices.
- Practicing string manipulation and memory management techniques in C.
- Exploring process creation and management in an operating system.
- Developing problem-solving and debugging skills in the context of a real-world application.

## Resources/References
During the development of the ByteShell project, the following resources/references were used:
- [Google](https://www.google.com)
- [GNU.org](https://www.gnu.org)
- [GeeksForGeeks](https://www.geeksforgeeks.org)
- [Stack Overflow](https://stackoverflow.com)
