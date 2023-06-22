#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int bytsh_cd(char **args);
int bytsh_help(char **args);
int bytsh_exit(char **args);
int bytsh_mkdir(char **args);
int bytsh_rm(char **args);
int bytsh_echo(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "mkdir",
    "rm",
    "echo"};
    
int (*builtin_func[])(char **) = {
    &bytsh_cd,
    &bytsh_help,
    &bytsh_exit,
    &bytsh_mkdir,
    &bytsh_rm,
    &bytsh_echo};

int bytsh_count_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int bytsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "bytsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("bytsh");
        }
    }
    return 1;
}


int bytsh_help(char **args)
{
    int i;
    printf("This is ByteShell\n");
    printf("Type the command and its arguments, then press enter.\n");
    printf("The following commands are built in:\n");

    for (i = 0; i < bytsh_count_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("You may use the 'man' command for information about the builtin as well as other commands\n");
    return 1;
}


int bytsh_exit(char **args)
{
    return 0;
}

int bytsh_mkdir(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "bytsh: expected argument to \"mkdir\"\n");
    }
    else
    {
        if (mkdir(args[1], 0777) != 0)
        {
            perror("bytsh");
        }
    }
    return 1;
}

int bytsh_rm(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "bytsh: expected argument to \"rm\"\n");
    }
    else
    {
        if (remove(args[1]) != 0)
        {
            fprintf(stderr, "bytsh: failed to remove file '%s'\n", args[1]);
        }
        else
        {
            printf("File removed: %s\n", args[1]);
        }
    }
    return 1;
}

int bytsh_echo(char **args)
{
    int i = 1;
    while (args[i] != NULL)
    {
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");
    return 1;
}

int bytsh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("bytsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("bytsh");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int bytsh_execute(char **args)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    for (int i = 0; i < bytsh_count_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return bytsh_launch(args);
}

#define bytsh_MAXBUFF 1024
char *bytsh_read_line(void)
{
    int bufsize = bytsh_MAXBUFF, index = 0, chr;
    char *buff = malloc(sizeof(char) * bufsize);

    if (!buff)
    {
        fprintf(stderr, "bytsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        chr = getchar();

        if (chr == EOF || chr == '\n')
        {
            buff[index] = '\0';
            return buff;
        }
        else
        {
            buff[index] = chr;
        }

        index++;

        if (index >= bufsize)
        {
            bufsize += bytsh_MAXBUFF;
            buff = realloc(buff, bufsize);
            if (!buff)
            {
                fprintf(stderr, "bytsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define bytsh_TOK_MAXBUFF 64
#define bytsh_TOK_DELIM " \t\r\n\a"
char **bytsh_split_line(char *line)
{
    int bufsize = bytsh_TOK_MAXBUFF, index = 0;
    char **alltokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_backup;

    if (!alltokens)
    {
        fprintf(stderr, "bytsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, bytsh_TOK_DELIM);
    while (token != NULL)
    {
        alltokens[index] = token;
        index++;

        if (index >= bufsize)
        {
            bufsize += bytsh_TOK_MAXBUFF;
            tokens_backup = alltokens;
            alltokens = realloc(alltokens, bufsize * sizeof(char *));
            if (!alltokens)
            {
                free(tokens_backup);
                fprintf(stderr, "bytsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, bytsh_TOK_DELIM);
    }
    alltokens[index] = NULL;
    return alltokens;
}

void bytsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("> ");
        line = bytsh_read_line();
        args = bytsh_split_line(line);
        status = bytsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{
    bytsh_loop();
    return EXIT_SUCCESS;
}
