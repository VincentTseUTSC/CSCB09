#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"

int main(int argc, char **argv) {
	char ch;
	char path[PATHLENGTH];
	char *startdir = ".";

	while((ch = getopt(argc, argv, "d:")) != -1) {
		switch (ch) {
			case 'd':
				startdir = optarg;
				break;
			default:
				fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME]\n");
				exit(1);
		}
	}
	// Open the directory provided by the user (or current working directory)
	
	DIR *dirp;
	if((dirp = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	}
	
	/* For each entry in the directory, eliminate . and .., and check
	* to make sure that the entry is a directory, then call run_worker
	* to process the index file contained in the directory.
 	* Note that this implementation of the query engine iterates
	* sequentially through the directories, and will expect to read
	* a word from standard input for each index it checks.
	*/

	FreqRecord *master_frp = malloc((MAXRECORDS + 1) * sizeof(FreqRecord));
	check_allocate_failure(master_frp); // check if realloc successfully
	pid_t pid, fd[MAXFILES][2][2]; // fd for at most MAXFILES files, 2 pipes each and 2 file descriptors each
	int num_dir = 0;
	struct dirent *dp;
	while((dp = readdir(dirp)) != NULL) {

		if(strcmp(dp->d_name, ".") == 0 || 
		   strcmp(dp->d_name, "..") == 0 ||
		   strcmp(dp->d_name, ".svn") == 0){
			continue;
		}
		// create string for path
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

		// create string for list_file
		char *index_file = "/index";
		char list_file[strlen(path) + (strlen(index_file ) + 1)];
		strcpy(list_file, path);
		strcat(list_file, index_file );

		// create string for name_file
		char *filenames_file = "/filenames";
		char name_file[strlen(path) + (strlen(filenames_file) + 1)];
		strcpy(name_file, path);
		strcat(name_file, filenames_file);

		// create stat for checking failures
		struct stat sbuf;
		struct stat lbuf;
		struct stat nbuf;

		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 

		// Only fork and call run_worker if its a directory and files index, filenames exist in that directory
		if(S_ISDIR(sbuf.st_mode) && (stat(list_file, &lbuf) == 0) && (stat(name_file, &nbuf) == 0)) {
			num_dir ++; // increment the number of directories
			// open the pipes
			pipe(fd[num_dir - 1][0]);
			pipe(fd[num_dir - 1][1]);

			if((pid = fork()) < 0){ // failed to fork
				perror("fork");
				exit(1);
			} else if (pid == 0) { // in child
				close(fd[num_dir - 1][0][1]); // close unneeded pipes in child
				close(fd[num_dir - 1][1][0]);
				int prev_fd = 0; // close all un needed pipes that are copied from parent when forking
				while (prev_fd < (num_dir - 1)) { 
					close(fd[prev_fd][0][1]);
					close(fd[prev_fd][1][0]);
					prev_fd ++;
				} // call run worker in child and return right after its finish to avoid forking
				run_worker(path, fd[num_dir - 1][0][0], fd[num_dir - 1][1][1]);
				return 0;
			} else { // close un needed pies in parent
				close(fd[num_dir - 1][0][0]);
				close(fd[num_dir - 1][1][1]); 
			}
		}
	}
	int read_bytes, wtp_bytes, rfp_bytes;
	char user_input[MAXWORD + 1];
	FreqRecord *temp_record = malloc( sizeof(FreqRecord));
	check_allocate_failure(temp_record); // check if malloc successfully
	printf("%s\n", "Please enter a word");
	// reading from keyboard input
	while ((read_bytes = read(STDIN_FILENO, user_input, MAXWORD)) > 0) {
		// empty the array to by setting every freq to 0 and the name to null
		int empty_master;
		for(empty_master = 0; empty_master < (MAXRECORDS + 1); empty_master ++) {
			master_frp[empty_master].freq = 0;
			strcpy(master_frp[empty_master].filename, "\0");
		}
		user_input[read_bytes - 1] = '\0';
		int i; // write the user_input to every child through pipe
		for (i = 0; i < num_dir; i++) {
			if((wtp_bytes = write(fd[i][0][1], user_input, MAXWORD)) == -1) {
				perror("failed to write to pipe");
				exit(1);
			}
		}
		int j;
		for (j = 0; j < num_dir; j++) {
			// read the record from every child through pipe
			// keep reading records from child and insert each record to the master array
			while (((rfp_bytes = read(fd[j][1][0], temp_record, sizeof(FreqRecord))) > 0) && (temp_record->freq != 0)) {
				int record_index = 0; FreqRecord temp1; FreqRecord temp2;
				//insert the record into the correct position so that the array is always sorted
				while (record_index < MAXRECORDS) { // find the correct index to insert the record
					if (temp_record->freq > master_frp[record_index].freq) {
						temp1 = master_frp[record_index]; // store old record at index into temp
						master_frp[record_index] = *temp_record; // insert new record into index
						record_index ++; 
						// since we have insert a new_record, move every record to right
						while (record_index < MAXRECORDS){
							temp2 = master_frp[record_index]; // store new old-record at index into temp
							master_frp[record_index] = temp1; // insert previous old-record into index
							temp1 = temp2; // store new old-record to temp2 making it become
							record_index ++; // the previous old-record on the next iteration
						}
					}
					record_index ++;
				}
			}
			if (rfp_bytes < 0) { // if read failed
				perror("failed to read from pipe");
				exit(1);
			}
		}
		print_freq_records(master_frp); // print the master array
		printf("%s\n", "Please enter a word");
	}

	if (read_bytes < 0) { // if read failed
		perror("failed to read from keyboard");
		exit(1);
	}
	// close all open pipes from master process
	int num_pipe;
	for (num_pipe = 0; num_pipe < num_dir; num_pipe++) {
		close(fd[num_pipe][0][1]);
		close(fd[num_pipe][1][0]);
	}
	// wait for all children
    int children, status;
    for(children = 0; children < num_dir; children++) {
        wait(&status);
    }

    free(temp_record);
    free(master_frp);
	printf("End of the program!\n");
	return 0;
}

