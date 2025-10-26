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
    char cmdline[MAX_CMDLINE_SIZE];     /* For storing the input string from the user.*/
    char *jobs[MAX_JOBS_PER_LINE];      /* For storing 'job' substrings.*/
    int seqops[MAX_SEQOPS];             /* For storing sequence operator codes*/
                                        /* For the current job being processed.*/
    char *simple_cmds[MAX_SIMPLE_CMDS]; /* For storing simple command strings.*/
    char *cmd;                          /* For storing the 'command word'.*/
    char *cmdargs[MAX_ARGS_PER_CMD];    /* For storing additional 'argument words'.*/

    while (1)
    {
        show_prompt(">>: ");

        int bytes_read = readline(0, cmdline, MAX_CMDLINE_SIZE);
        /* print what readline() obtained */
        printf("\nReadline() got: \"%s\" (rval = %d)\n", cmdline, bytes_read);

        /* If we read anything (even an empty line).*/
        if (bytes_read > 0)
        {

            /* Split input commandline by 'jobs'.*/
            int total_jobs = split_into_jobs(cmdline, jobs, MAX_JOBS_PER_LINE);
            /* For _each_ job... */
            for (int i = 0; i < total_jobs; ++i)
            {
                /* Print out the JOB we are scanning. */
                printf("\nJob #%d: \"%s\"\n", jobs[i]);


                /* Scan for SEQUENCE OPERATORS. */
                int seqops_cnt = scan_seqops(seqops, jobs[i], MAX_SIMPLE_CMDS);
                /* Print out the seqops codes */
                printf("\tSeqops[] = [ ");
                for (int j = 0; j < strlen(seqops); j++)
                {
                    /* If statement covers if the index is initialized to 0 */
                    if (seqops[j] != 0)
                    {
                        printf("%d ", seqops[j]);
                    }
                }
                printf("]\n");


                /* Split the job into SIMPLE COMMANDS... */
                int total_simple_cmds = extract_simple_cmds(jobs[i], simple_cmds, MAX_SIMPLE_CMDS);
                /* For _each_ simple comand... */
                for (int j = 0; j < total_simple_cmds; ++j)
                {
                    /*print out the simple commands*/
                    printf("\tSimple Command #%d = %s\n",j,simple_cmds[j]);

                    /* Extract the commands and the arguments. */
                    int total_tokens = extract_cmd_args(simple_cmds[j], &cmd, cmdargs, MAX_ARGS_PER_CMD);
                    /* print out cmd and arguments*/
                    printf("\t\tcmd = \"%s\"\n",cmd);
                    for(int k = 0; k<total_tokens)
                    {
                        printf("\t\targ[%d] = \"%s\"",k,cmdargs[k]);
                    }

                } /* end for( each simple command ) */

            } /* end for( each job ) */
        }
        else
        {
            printf("readline(): Returned with code = %d\n", bytes_read);
        } /* end if-else() */

    } /* end while() */

    return (EXIT_SUCCESS);
}

void show_prompt(char *prompt)
{
    printf("%s ", prompt);
    fflush(stdout);
}

int split_into_jobs(char *cmdline, char *jobs[], size_t count)
{
    /* This function splits a given command line into jobs via the ';' delimiter */
    char *delims = ";";
    int jobAmt;  /* Integer just set for future for loop usage */
    char *token; /* Final Tokenized job to be added to tokenList */

    /* Tokenization */
    for (jobAmt = 0, token = strtok(cmdline, delims); token != NULL && jobAmt < count;
         token = strtok(NULL, delims), ++jobAmt)
    {
        jobs[jobAmt] = token;
    }

    return jobAmt;
}

int scan_seqops(int seqops[], char *jobstr, size_t count)
{
    /* This function scans a given job string for the & or | characters
       and returns the # of seqops while storing them into seqops[] */
    size_t len = strlen(jobstr);
    int i; /* for loop index variable */
    int j; /* seqops index */
    int seqOpsAmt;

    for (i = 0; i < count; i++)
    {
        seqops[i] = 0;
    }

    /*Loop through the given jobstr*/
    j = 0;
    seqOpsAmt = 0;
    for (i = 0; i < len; i++)
    {
        /* Check if i+1 is in bound, then check if the current and next index are & or | */
        if (i + 1 < len && jobstr[i] == '&' && jobstr[i + 1] == '&')
        {
            /* Increment I to skip the next character as weve verified it is & */
            i++;
            /* set the current index of seqops to 1, and increment */
            seqops[j++] = 1;
            /* Increment the seqOpsAmt */
            seqOpsAmt++;
        }
        else if (i + 1 < len && jobstr[i] == '|' && jobstr[i + 1] == '|')
        {
            /* Increment I to skip the next character as weve verified it is | */
            i++;
            /* set the current index of seqops to 2, and increment */
            seqops[j++] = 2;
            /* Increment the seqOpsAmt */
            seqOpsAmt++;
        }
    }

    return seqOpsAmt;
}

int extract_simple_cmds(char *jobstr, char *simple_cmds[], size_t count)
{
    /* This function splits a given job string into the seperate commands via the '&' and '|' delimiters */
    char *delims = "&|";
    int simpleCmdAmt; /* Integer just set for future for loop usage */
    char *token;      /* Final Tokenized command to be added to simple_cmds */

    /* Tokenization */
    for (simpleCmdAmt = 0, token = strtok(jobstr, delims); token != NULL && simpleCmdAmt < count;
         token = strtok(NULL, delims), ++simpleCmdAmt)
    {
        simple_cmds[simpleCmdAmt] = token;
    }

    return simpleCmdAmt;
}

int extract_cmd_args(char *simple_cmd, char **cmd, char *cmdargs[], size_t count)
{
    /* This function splits a given command line into jobs via the ' ' delimiter */
    char *delims = " ";
    int argAmt;  /* Integer just set for future for loop usage */
    char *token; /* Final Tokenized argument to be added to cmdargs[] */

    /* Tokenization */
    /*tokenize and assign the first string "command" to cmd*/
    *cmd = strtok(simple_cmd, delims);

    /*tokenize the rest as usual, starting from arg2*/
    for (argAmt = 1, token = strtok(simple_cmd, delims); token != NULL && argAmt < count;
         token = strtok(NULL, delims), ++argAmt)
    {
        cmdargs[argAmt] = token;
    }

    return argAmt;
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
