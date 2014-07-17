/*Damon Edge
 *Project 1
*/

#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <string.h>

#define MAX_LINE 80 													/*max length command*/
#define HIST_MAX_SIZE 10 												/*max length history*/

static char *history[HIST_MAX_SIZE];
static int history_count = 0;

/* 
 * Line2Array function and function style was helped by the example from 
 * http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/
void Input2Array(char *in_line, char **args) {
	while (*in_line != '\0' || *in_line != '\n') {						/* End of line? */ 
		while (*in_line == ' ' || *in_line == '\n')
			*in_line++ = '\0';											/* Replace white spaces with 0 */
        *args++ = in_line;         										/* Save the position */
        while (*in_line != '\0' && *in_line != ' ' && *in_line != '\n') 
			in_line++;													/* Skip argument */
	}
	*args = '\0';                 										/* End of argument list */
}

void  execute(char **args, int TFwait) {
	pid_t pid;															/* Process ID data type to pid */

	pid = fork();
    if (pid < 0) {     													/* Error making child */
		printf("-- Error: Could not fork a child process\n");
		exit(1);
	}
	else if (pid == 0) {												/* Child process statements */
		if (execvp(*args, args) < 0) {									/* Execute the command */
			printf("-- Error: Could not execute command\n");
			exit(1);
		}
	}
	else {																/* Parent process statements */
		if (TFwait)														/* If (true or false) TFwait is true */
			wait(NULL);													/* Wait for completion  */
	}
}

void add_to_history(char *command){
	if (history_count < HIST_MAX_SIZE) {
		history[history_count++] = strdup(command);
	}
	else{
		free(history[0]);
		int idx;
		for (idx = 1; idx < HIST_MAX_SIZE; idx++) {
			history[idx-1] = history[idx];
		}
		history[HIST_MAX_SIZE-1] = strdup(command);
	}
}

int main(void){
	char in_line[1024];													/* Input */
	char hist_line[1024];												/* Input to History */
	char *args[MAX_LINE / 2 + 1];										/* Command line args */
	/*char *args2[MAX_LINE / 2 + 1];*/										/* History pull array */
	
	int should_run = 1;
	int num_cmd_n_hist = 0;
	int TFwait = 1;														/* Initialize TFwait to true so it can wait */
	int counter = 0;
	
	while (should_run) {												/* Repeat until exit */
		printf("osh> ");												/* Prompt display */
		gets(in_line);              									/* Gets input */
		strcpy(hist_line,in_line);
		printf("\n");
		Input2Array(in_line, args);										/* Reads input into args[] */
		/* printf("%s", hist_line); */
		int i = 0;
		while (args[i] != NULL){
			i+=1;
		}
		if (args[i-1][0] == '&') {										/* Determines whether to wait or not */
			TFwait = 0;													/* Don't wait */
		}
		else {								
			TFwait = 1;													/* Wait */
		}
		args[i-1] = NULL
		if (strcmp(args[0], "exit") != 0) {								/* Exit if args[0] is exit command */
			if (strcmp(args[0], "history") == 0) {
				int i;
				for (i = HIST_MAX_SIZE-1; i>-1;i--){
					int counterint = counter;
					if (history[i] != NULL)
						printf("%d %s\n", counterint, history[i]);
						counterint -= 1;
					printf("\n");
				}
			}
			/* -- Doesn't work yet --
			else if (strcmp(args[0], "!!") == 0) {
				counter += 1
				printf("%s", history[num_cmd_n_hist]);
				if (history[HIS_MAX_SIZE-1] != NULL) {
					Input2Array(history[num_cmd_n_hist],args2);
					execute(args2);
				}
				else {
					printf("No commands in history\n");
				}
			}
			else if (strcmp(args[0], ("!%d",num)) == 0) {
				counter += 1
			}
			*/
			else {
				add_to_history(hist_line);
				counter += 1;
				if (num_cmd_n_hist<HIST_MAX_SIZE)
					num_cmd_n_hist += 1;
				execute(args, TFwait);									/* Execute() function */
			}
		}
		else
			should_run = 0;
	}
	return 0;
}
