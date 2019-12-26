#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>

// the last parameter MUST be NULL
void transmit(char* message, ...)
{
	int pid = fork();

	if ( pid == 0 )
	{
		va_list ap;
		va_start(ap, message);

		// calculate the total size of the resultant strung, so we
		// know how much memory to allocate
		int total_length = 0;
		total_length += strlen(message);
		for (;;)
		{
			char* c = va_arg(ap, char*);
			if ( c == NULL )
				break;
			else
				total_length += strlen(c);
		}

		// restart ap
		va_end(ap);
		va_start(ap, message);

		// create the buffer
		char* buff = malloc(total_length*sizeof(char));

		// concatenate the string
		strcat(buff, message);
		for (;;)
		{
			char* c = va_arg(ap, char*);
			if ( c == NULL )
				break;
			else
				strcat(buff, c);
		}
		
		va_end(ap);

		// arguments for espeak
		char *args[] = {"espeak", buff, "-ven-us", NULL};

		// ironically, espeak is too verbose, and this is the only way to make it shut up.
		fclose(stderr);

		// run the espeak
		execvp(args[0], args);

		// memory leaks are bad
		free(buff);
		
		// exit successfully (from this fork)
		exit(EXIT_SUCCESS);
	}
	else
	{
		// wait for espeak to finish up before continuing.
		// also makes sure that there are no zombie processes.
		wait(NULL);
	}
}

// this allocates memory pointed to by buff, and fills it in the next line
// in a file
static char* allocate_and_read(FILE* locs_file, char** buff)
{
	// find the size of the line.
	int count = 0;
	for (;;)
	{
		// increments before the test, because there needs to be
		// at least one memory space for a \0
		count++;
		char c = fgetc(locs_file);

		// go until it comes across an EOF or newline
		if ( c == EOF || c == '\n' )
			break;
	}

	// push back count places
	fseek(locs_file, -(count), SEEK_CUR);

	// allocate memory for the buffer. this is freed in the unload_locations function
	*buff = malloc(count * sizeof(char));

	// this shouldn't get to any \n or EOF, because it should be one less
	fgets(*buff, count, locs_file);

	// move to the next line
	fseek(locs_file, 1L, SEEK_CUR);

	return *buff;
}

void load_locations(char* path, struct Location** locations, int* size)
{
	// open the locations file
	FILE* locations_file = fopen(path, "r");

	//   the total # of lines / 4
	// find the size of the file
	*size = 0;
	for (int c=getc(locations_file); c!=EOF; c=getc(locations_file))
	{
		if ( c == '\n' ) (*size)++;
	}
	*size = (*size)/4;

	rewind(locations_file);

	// allocate memory for the locations list
	// the size is divided by four because 4 lines correspond to one entry
	*locations = malloc(*size * sizeof(struct Location));

	// fill in the structure
	for (int i=0; i < *size; i++)
	{
		allocate_and_read(locations_file, &(*locations)[i].name);
		allocate_and_read(locations_file, &(*locations)[i].description);

		// because the coordinates are on the same line, they need to be split
		// up.
		char* temp1, * temp2;
		allocate_and_read(locations_file, &temp1);
		temp1 = strtok(temp1, " ");
		temp2 = strtok(NULL, " ");

		// find the size of each coordinate
		int size_x = strlen(temp1);
		int size_y = strlen(temp2);

		// allocate space in the struct for the coordinates
		(*locations)[i].x = malloc(sizeof(char)*size_x);
		(*locations)[i].y = malloc(sizeof(char)*size_y);

		// copy the strings from the temporary locations into the struct
		strcpy((*locations)[i].x, temp1);
		strcpy((*locations)[i].y, temp2);

		// temp1 should still point to the original location in memory.
		free(temp1);

		fseek(locations_file, 1L, SEEK_CUR);
	}

	// close the locations file
	fclose(locations_file);
}

// goes through and frees all the memory that is contained in
// all of the structs, and then frees the memory containing the struct list
void unload_locations(struct Location* loc, int size)
{
	for ( int i=0; i < size; i++ )
	{
		free(loc[i].name);
		free(loc[i].description);
		free(loc[i].x);
		free(loc[i].y);
	}

	free(loc);
}
