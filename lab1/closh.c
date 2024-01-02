// closh.c - CS 377, Fall 2012
// YOUR NAME HERE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char *cmd, char **cmdTokens)
{
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens))
    {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar()
{
    char c = getchar();
    while (getchar() != '\n')
        ;
    return c;
}

void executeParallely(int count, int timeout, char **cmdTokens)
{
    pid_t pids[count];
    pid_t cid = fork();
    /*
        Creates all child processes at once then waits/terminates
    */
    for (int i = 0; i < count; i++)
    {

        if (cid == 0)
        {
            printf("Child process_id(pid) = %d \n", getpid());

            execvp(cmdTokens[0], cmdTokens);
            printf("Can't execute %s\n", cmdTokens[0]);
        }
        else
        {
            pids[i] = cid;
            cid = fork();
        }
    }

    // Only after all processes are made does waiting/termination begin
    if (timeout == 0)
    {
        for (int i = 0; i < count; i++)
        {
            waitpid(pids[i], 0, 0);
        }
    }
    // Kills all the processes when the timeout runs out
    else
    {
        sleep(timeout);
        for (int i = 0; i < count; i++)
        {
            kill(pids[i], SIGKILL);
        }
    }
}

void executeSequentially(int count, int timeout, char **cmdTokens)
{
    int cid = fork();
    /*
        Creates each process one after the other. Once one child is finished or terminated only then will another child be made 
    */
    for (int i = 0; i < count; i++)
    {
        if (cid == 0)
        {
            printf("process_id(pid) = %d \n", getpid());

            execvp(cmdTokens[0], cmdTokens);
            printf("Can't execute %s\n", cmdTokens[0]);
        }
        else
        {
            // if there is no timeout wait for the process to finish
            if (timeout == 0)
            {
                waitpid(cid, 0, 0);
                cid = fork();
            }
            // After the timeout time the parent will kill the child and start a new one
            else
            {
                sleep(timeout);
                if (!kill(cid, SIGKILL)){ 
                    printf("Process successfully terminated for taking too long");
                }
                cid = fork();
            }
        }
    }
}
// main method - program entry point
int main()
{
    char cmd[81];        // array of chars (a string)
    char *cmdTokens[20]; // array of strings
    int count;           // number of times to execute command
    int parallel;        // whether to run in parallel or sequentially
    int timeout;         // max seconds to run set of commands (parallel) or each command (sequentially)

    while (TRUE)
    { // main shell input loop

        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n')
            continue;
        readCmdTokens(cmd, cmdTokens);
        do
        {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);

        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do
        {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);

        if (count > 1)
        {
            if (parallel)
            {
                executeParallely(count, timeout, cmdTokens);
            }
            else
            {
                executeSequentially(count, timeout, cmdTokens);
            }
        }
        else
        {
            execvp(cmdTokens[0], cmdTokens);
            printf("Can't execute %s\n", cmdTokens[0]);
        }
        exit(1);
    }
}
