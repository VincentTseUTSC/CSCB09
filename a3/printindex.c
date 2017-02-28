#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "worker.c"

int main(int argc, char **argv)
{
  Node *head = NULL;
  char **filenames = init_filenames();
  char *listfile = "index";
  char *namefile = "filenames";


  read_list(listfile, namefile, &head, filenames);
	FreqRecord *aaa = get_word("afternoon",head,filenames);
	print_freq_records(aaa);
  return 0;
}
