#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "freq_list.h"
#include "worker.h"

/* Print to standard output the frequency records for a word.
* Used for testing.
*/
void print_freq_records(FreqRecord *frp) {
	int i = 0;
	while(frp != NULL && frp[i].freq != 0) {
		printf("%d    %s\n", frp[i].freq, frp[i].filename);
		i++;
	}
}

/* check_allocate_failure
* - check if memory is allocated successfully
*/
void check_allocate_failure(FreqRecord *frp){
	if(frp == NULL){
        fprintf(stderr, "Error: %s\n", "allocate memory failed");
        exit(EXIT_FAILURE);
    }
}

/* get_word
* - given an array of filenames and a index (represented as a linked list)
* - find the word in the index and return an array of FreqRecords 
* - every FreqRecords has a non-zero freq value and a file name
*/
FreqRecord *get_word(const char *word, Node *head, char **filenames) {
	// initailize the variables
	FreqRecord *frp = malloc( sizeof(FreqRecord) * (MAXFILES + 1));
	check_allocate_failure(frp); // check if malloc successfully
	int record_index = 0; int num_records = 1;
	Node *current = head;
	// loop through the linked list until we find the word
	while ((current != NULL) && (strcmp(current->word, word) != 0)) {
		current = current->next;
	}// if the word is found
	if (current != NULL) {
		int i = 0; // append every non-zero freq to the list and increase the size
		while(i < MAXFILES && filenames[i] != NULL) {
			if (current->freq[i] != 0) { // set the freq and the filename
				frp[record_index].freq = current->freq[i];
				strcpy(frp[record_index].filename, filenames[i]);
				record_index ++; num_records ++;
			}
			i ++;
		}
	}// add a sentinel record to the end to indicate the end of the array
	frp[record_index].freq = 0;
	frp[record_index].filename[0] = '\0';
	frp = realloc(frp, sizeof(FreqRecord) * num_records);
	check_allocate_failure(frp); // check if realloc successfully
	return frp;
}

/* write_record
* - write the record to out and check for error
*/
void write_record(int out, FreqRecord *record){
	if (write(out, record, sizeof(FreqRecord)) != sizeof(FreqRecord)){
		perror("write");
		exit(1);
	}
}

/* run_worker
* - load the index found in dirname
* - read a word from the file descriptor "in"
* - find the word in the index list
* - write the frequency records to the file descriptor "out"
*/
void run_worker(char *dirname, int in, int out){
	// initailize the variables
	Node *head = NULL;
	char **filenames = init_filenames();
	FreqRecord *frp = NULL;
	char *index_file = "/index";
	char *filenames_file = "/filenames";
	
	// create string for index
	char list_file[strlen(dirname) + (strlen(index_file) + 1)];
	strcpy(list_file, dirname);
	strcat(list_file, index_file);

	// create string for filenames
	char name_file[strlen(dirname) + (strlen(filenames_file) + 1)];
	strcpy(name_file, dirname);
	strcat(name_file, filenames_file);
	
	// create the data structures,  index_list and filenames_array
	read_list(list_file, name_file, &head, filenames);
	//read_list(index_file, filenames_file, &head, filenames);
	char word[MAXWORD];
	int nbytes;
	// keep reading words and find the word from index
	while((nbytes = read(in, word, MAXWORD)) > 0) {
		printf("%s\n", word);
		word[nbytes-1] = '\0';
		frp = get_word(word, head, filenames);
		int i = 0;
		// write every individual freqrecord to stdout include sentinel one
		while(frp[i].freq != 0) {
			write_record(out, &frp[i]);
			i++;
		}
		// sentinel record
		write_record(out, &frp[i]);
		free(frp);
	}

	if (nbytes < 0) { // if read failed
		perror("fail to read word");
		exit(1);
	}
}
