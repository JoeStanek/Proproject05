/*
 * Auth: Josef Stanek
 * Date: 10/23/2025 (Due: 10/26/2025 )
 * Course: CSCI4500
 * Preproj#: 5 (Batch Line Parser)
 * Desc: Implementation of a Simple Batch Parser, for Later Use In The Simple Shell Program.
 */

/* ================================= Includes ========================================= */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/* ================================= Defines ========================================== */
#define MAX_CMDLINE_SIZE 256
#define MAX_JOBS_PER_LINE 10
#define MAX_SIMPLE_CMDS 10
#define MAX_SEQOPS MAX_SIMPLE_CMDS
#define MAX_ARGS_PER_CMD 10

/* ================================= Prototypes ======================================= */
void show_prompt(char *prompt);
int split_into_jobs(char *cmdline, char *jobs[], size_t count);
int scan_seqops(int seqops[], char *jobstr, size_t count);
int extract_simple_cmds(char *jobstr, char *simple_cmds[], size_t count);
int extract_cmd_args(char *simple_cmd, char **cmd, char *cmdargs[], size_t count);
int readline(int fd, char *buf, int bufsz);

/* ================================= Functions ======================================== */
int main(int argc, char *argv[])
{
    char cmdline[MAX_CMDLINE_SIZE];     // For storing the input string from the user.
    char *jobs[MAX_JOBS_PER_LINE];      // For storing 'job' substrings.
    int seqops[MAX_SEQOPS];             // For storing sequence operator codes
                                        //  for the current job being processed.
    char *simple_cmds[MAX_SIMPLE_CMDS]; // For storing simple command strings.
    char *cmd;                          // For storing the 'command word'.
    char *cmdargs[MAX_ARGS_PER_CMD];    // For storing additional 'argument words'.

    while (1)
    {
        show_prompt(">>: ");

        bytes_read = readline(0, cmdline, MAX_CMDLINE_SIZE);

        // If we read anything (even an empty line).
        if (bytes_read > 0)
        {

            // Split input commandline by 'jobs'.
            total_jobs = split_into_jobs(cmdline, jobs, MAX_JOBS_PER_LINE);

            // For _each_ job...
            for (int i = 0; i < total_jobs; ++i)
            {

                // Scan for sequence operators.
                seqops_cnt = scan_seqops(seqops, jobs[i], MAX_SIMPLE_CMDS);

                // Split the job into simple commands...
                total_simple_cmds = extract_simple_cmds(jobs[i], simple_cmds, MAX_SIMPLE_CMDS);

                // For _each_ simple comand...
                for (int j = 0; j < total_simple_cmds; ++j)
                {

                    // Extract the commands and the arguments.
                    total_tokens = extract_cmd_args(simple_cmds[j], &cmd, cmdargs, MAX_ARGS_PER_CMD);

                } // end for( each simple command )

            } // end for( each job )
        }
        else
        {
            printf("readline(): Returned with code = %d\n", bytes_read);
        } // end if-else()

    } // end while()

    ...

        return (EXIT_SUCCESS);
}


void show_prompt(char *prompt)
{
    printf("%s ", prompt);
    fflush(stdout);
}

/* Readline code copied from Pre-project02 */
int readline(int fd, char *buf, int bufsz)
{
    /* Local Declares */
    int rval = 0;
    char c;
    ssize_t bytes_read;
    int count = 0;

    while (count < bufsz - 1)
    {
        /*Take in a single character*/
        bytes_read = read(fd, (void *)&c, 1);

        if (bytes_read < 0)
        {
            /*Error*/
            rval = -1;
        }
        else if (bytes_read == 0 || (c == '\n' && count == 0))
        {
            /*End of File*/
            buf[count] = '\0';

            break;
        }
        else if (c == '\n')
        {
            /*Newline Check*/
            buf[count] = '\0';
            count++;
            break;
        }

        else
        {

            /*Regular Character*/
            buf[count] = c;
            count++;
        }
    }

    rval = count;

    /*must return number of characters written into the user-specified buffer,
      which must include the NULL character.*/
    return rval;

} /* end readline() */
