/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

// Please enter your name and McGill ID below
// Name: Erick Zhao
// McGill ID: 260687719

// all the header files you would require
#include <stdio.h>    // for standard IO
#include <unistd.h>   // for execvp/dup/dup2
#include <string.h>   // for string manipulation
#include <stdlib.h>   // for fork  
#include <ctype.h>    // for character type check (isnum,isalpha,isupper etc)
#include <sys/wait.h> // for waitpid
#include <fcntl.h>    // open function to open a file. type "man 2 open" in terminal
#include <time.h>     // to handle time

// pointer to Linked list head
struct node *head_job = NULL;

// pointer to current job in Linked list
struct node *current_job = NULL;

// Global variable used to store process id of process
// that has been just created.
// Can be used while adding a job to linked list.
pid_t process_id;

// flag variable to check if redirection of output is required
int isred = 0;

// counter to keep track of job numbers
// used to set new job numbers
int next_job_number = 0;

// structure of a single node
// donot modify this structure
struct node
{
    int number;        //the job number
    int pid;           //the process id of the process
    char *cmd;         //string to store the command name
    time_t spawn;      //time to store the time it was spawned
    struct node *next; //when another process is called you add to the end of the linked list
};

// Add a job to the linked list
void addToJobList(char *args[])
{
    // allocate memory for the new job
    struct node *job = malloc(sizeof(struct node));
    // If the job list is empty, create a new head
    if (head_job == NULL)
    {
        // if no jobs, reset global job counter to 1
        next_job_number = 1;

        // init the job number with 1
        job->number = next_job_number;
        // set its pid from the global variable process_id
        job->pid = process_id;
        // cmd can be set to args[0]
        job->cmd = args[0];
        // set the job->next to point to NULL.
        job->next = NULL;
        // set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);
        // set head_job to be the job
        head_job = job;
        // set current_job to be head_job
        current_job = head_job;
    }
    // Otherwise, create a new job node and link the current node to it
    else
    {
        // init all values of the job like above num, pid, cmd, spawn
        struct node *job = malloc(sizeof(struct node));
        // init the job number
        job->number = next_job_number;
        // set new pid
        job->pid = process_id;
        // cmd can be set to arg[0]
        job->cmd = args[0];
        // set the job->next to point to NULL.
        job->next = NULL;
        // set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);
        
        // make next of current_job point to job
        current_job->next = job;
        // make job to be current_job
        current_job = job;
        // set the next of job to be NULL
        job->next = NULL;
    }
    next_job_number++;
}

// Function to refresh job list
// Run through jobs in linked list and check
// if they are done executing then remove it
void refreshJobList()
{
    // pointer require to perform operation on linked list
    struct node *current_job;
    struct node *prev_job;
    
    //variable to store returned pid 
    pid_t ret_pid;

    // init pointers
    current_job = head_job;
    prev_job = head_job;

    // traverse through the linked list
    while (current_job != NULL)
    {
        // use waitpid to init return pid variable
        // NOHANG - doesn't wait for process to finish
        ret_pid = waitpid(current_job->pid, NULL, WNOHANG);

        if (ret_pid == 0) // process has not exited, continue traversal
        {
            prev_job = current_job;
            current_job = current_job->next;
        }
        else // process has exited, remove it from list
        {

            // if head job, change head to second element
            if (current_job == head_job) {
                head_job = current_job->next;
            // otherwise, connect previous node to next
            // to remove current from chain
            } else {
                prev_job->next = current_job->next;
                current_job = current_job->next;
            }
        }
    }
    return;
}

// Function that lists all the jobs
void listAllJobs()
{
    struct node *current_job;
    int ret_pid;

    // refresh the linked list
    refreshJobList();

    // init current_job with head_job
    current_job = head_job;

    // heading row print only once.
    printf("\nID\tPID\tCmd\tstatus\tspawn-time\n");
        
    // traverse the linked list and print using the following statement for each job
    while (current_job != NULL) {
        printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn)));
        current_job = current_job->next;
    }
    
    return;
}

// wait till the linked list is empty
// you would have to look for a place 
// where you would call this function.
// donot modify this function
void waitForEmptyLL(int nice, int bg)
{
    if (nice == 1 && bg == 0)
    {
        while (head_job != NULL)
        {
            sleep(1);
            refreshJobList();
        }
    }
    return;
}
// checks if a character is whitespace or not
// whitespace: space, tab, or newline
int isWhitespace (char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

// function to perform word count
 int wordCount(char *filename,char* flag)
{
    int     cnt;
    char    buf;

    // open file with read only flag
    int fd = open(filename, O_RDONLY, 0666);
    cnt = 0;

    if (!flag) {
        printf("Please enter a flag for the wc command!\n");
        return -1;
    } else if (!strcmp(flag, "l")) {
        // if flag is l, count the number of lines in the file
        while(1) {
            // break on EOF
            if (!read(fd, &buf, 1)) {
                break;
            };
            // if newline encountered, increment count;
            if (buf == '\n') {
                cnt++;
            }
        }
    } else if (!strcmp(flag, "w")) {
        // if flag is w, count the number of words in the file
    
        // keep flag to track if we're in a word or not
        int in_word = 0; 
        while(1) {
            // break on EOF
            if (!read(fd, &buf, 1)) {
                break;
            };

            if (in_word && isWhitespace(buf)) {
                // if in word and whitespace encountered, then set NOT IN WORD
                in_word = 0;
            } else if (!in_word && !isWhitespace(buf)) {
                // if not in word and not whitespace encountered, then set IN WORD
                // new word encountered -> increase word count
                in_word = 1;
                cnt++;
            }
        }
    } else {
        printf("Flag %s not recognized!\n", flag);
        return -1;
    }

    return cnt;
}

// function to augment waiting times for a process
// donot modify this function
void performAugmentedWait()
{
    int w, rem;
    time_t now;
    srand((unsigned int)(time(&now)));
    w = rand() % 15;
    printf("sleeping for %d\n", w);
    rem = sleep(w);
    return;
}

// simulates running process to foreground
// by making the parent process wait for
// a particular process id.
int waitforjob(char *jobnc)
{
    struct node *trv;
    int jobn = (*jobnc) - '0';
    trv = head_job;
    // traverse through linked list and find the corresponding job
    // hint : traversal done in other functions too

    // if corresponding job is found 
    // use its pid to make the parent process wait.
    // waitpid with proper argument needed here
    while (trv != NULL) {
        // matching job numbers
        if (trv->number == jobn) {
            int status;
            waitpid(trv->pid, &status, WUNTRACED); // wait until termination
            break;
        }
        trv = trv->next;
    }
    return 0;
}

// splits whatever the user enters and sets the background/nice flag variable
// and returns the number of tokens processed
// donot modify this function
int getcmd(char *prompt, char *args[], int *background, int *nice)
{
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0)
    {
        exit(-1);
    }
    // Check if background is specified..
    if ((loc = index(line, '&')) != NULL)
    {
        *background = 1;
        *loc = ' ';
    }
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL)
    {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
        {
            if (!strcmp("nice", token))
            {
                *nice = 1;
            }
            else
            {
                args[i++] = token;
            }
        }
    }
    return i;
}

// this initialises the args to All null.
// donot modify
void initialize(char *args[])
{
    for (int i = 0; i < 20; i++)
    {
        args[i] = NULL;
    }
    return;
}

int main(void)
{
    //args is a array of character pointers
    //where each pointer points to a string
    //which may be command , flag or filename
    char *args[20];

    //flag variables for background, status and nice
    //bg set to 1 if the command is to executed in background
    //nice set to 1 if the command is nice
    //status  
    int bg, status, nice;

    //variable to store the process id.
    pid_t pid;

    //variable to store the file des
    //helpful in output redirection
    int fd1, fd2;

    //your terminal executes endlessly unless 
    //exit command is received
    while (1)
    {
        //init background to zero
        bg = 0;
        //init nice to zero
        nice = 0;
        //init args to null
        initialize(args);
        //get the user input command
        int cnt = getcmd("\n>> ", args, &bg, &nice);
        //keep asking unless the user enters something
        while (!(cnt >= 1))
            cnt = getcmd("\n>> ", args, &bg, &nice);

        //use the if-else ladder to handle built-in commands
        //built in commands don't need redirection
        //also no need to add them to jobs linked list
        //as they always run in foreground
        if (!strcmp("jobs", args[0]))
        {
            //call the listalljobs function
            listAllJobs();
        }
        else if (!strcmp("exit", args[0]))
        {
            //exit the execution of endless while loop 
            exit(0);
        }
        else if (!strcmp("fg", args[0]))
        {
            //bring a background process to foreground
            waitforjob(args[1]);
        }
        else if (!strcmp("cd", args[0]))
        {
            int result = 0;
            // if no destination directory given 
            // change to home directory
            if (args[1] == NULL) {
                chdir(getenv("HOME"));
            } else {
                //if given directory does not exist
                //print directory does not exist
                //if everything is fine 
                //change to destination directory 

                // attempt cd system call

                if (chdir(args[1]) == -1) { // if directory does not exist
                    printf("Directory %s does not exist!", args[1]);
                }
            }
        }
        else if (!strcmp("pwd", args[0]))
        {
            //use getcwd and print the current working directory
            char buf[1024];
            getcwd(buf, 1024);
            printf("%s\n", buf);
        }
        else if(!strcmp("wc",args[0]))
        {
            //call the word count function
            int count = wordCount(args[1],args[2]);

            if (count >= 0) {
                printf("%d\n", count);
            }
        }
        else
        {
            //Now handle the executable commands here 
            /* the steps can be..:
            (1) fork a child process using fork()
            (2) the child process will invoke execvp()
            (3) if background is not specified, the parent will wait,
                otherwise parent starts the next command... */


            //hint : samosas are nice but often there 
            //is a long waiting line for it.

            // if nice command, wait for empty linked list before executing
            if (nice) {
                waitForEmptyLL(nice,bg);
            }

            // create a child
            pid = fork();

            // to check if it is parent
            if (pid > 0)
            {
                // in parent
                if (bg == 0)
                {
                    // FOREGROUND
                    // wait until process ends
                    waitpid(pid, NULL, 0);
                }
                else
                {
                    // BACKGROUND
                    // don't wait; just add job to list
                    process_id = pid;
                    addToJobList(args);
                }
            }
            else
            {
                // we are inside the child

                //introducing augmented delay
                performAugmentedWait();

                // check for redirection
                // now you know what does args store
                // check if args has ">"
                // if yes set isred to 1
                // else set isred to 0

                int i;
                i = 1;
                while (args[i] != NULL) {
                    if (!strcmp(args[i], ">")) {
                        isred = 1;
                        break;
                    }
                    i++;
                }

                // if redirection is enabled
                if (isred == 1)
                {
                    // open file and change output from stdout to that  
                    // make sure you use all the read write exec authorisation flags
                    // while you use open (man 2 open) to open file

                    int stdout_cpy = dup(STDOUT_FILENO);
                    int fd = open(args[i+1], O_WRONLY | O_CREAT, 0666);
                    dup2(fd, STDOUT_FILENO);

                    //set ">" and redirected filename to NULL
                    args[i] = NULL;
                    args[i + 1] = NULL;

                    //run your command
                    execvp(args[0], args);

                    //restore to stdout
                    fflush(stdout);
                }
                else
                {
                    //simply execute the command.
                    if (execvp(args[0], args) == -1) {
                        printf("No such command!");
                    };
                }
            }
        }
    }

    return 0;
}