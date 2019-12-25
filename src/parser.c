#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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
	*locations = malloc(*size);

	for (int i=0; i < *size; i++)
	{
		// TODO so, these need to be changed to char*. they will make your life easier.
		// get the name and description strings
		fgets((*locations)[i].name, MESSAGE_LENGTH, locations_file);
		fgets((*locations)[i].description, MESSAGE_LENGTH, locations_file);

		strtok((*locations)[i].name, "\n");
		strtok((*locations)[i].description, "\n");

		// get the latitude and longitude
		fscanf(locations_file, "%lf%lf", &(*locations)[i].latitude, &(*locations)[i].longitude);

		// move past the whitespace
		fseek(locations_file, sizeof(char)*2, SEEK_CUR);
	}

	// close the locations file
	fclose(locations_file);
}
