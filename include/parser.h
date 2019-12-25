#ifndef PARSER_H
#define PARSER_H

#define MESSAGE_LENGTH 256

struct Location
{
	char name[MESSAGE_LENGTH];
	char description[MESSAGE_LENGTH];
	double latitude, longitude;
};

/* Function loads all the locations from a given locations file.
 * locations should be in the following format:
 *
 * Name of Location
 * Description
 * Latitude Longitude
 *
 * ...
 *
 * there should be a line separating entries.
 */
void load_locations(char* path, struct Location** loc, int* size);

#endif
