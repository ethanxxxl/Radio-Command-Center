#ifndef PARSER_H
#define PARSER_H

#define TRIGGER 14

struct Location
{
	char* name;
	char* description;
	char* x;
	char* y;
};

void transmit(char* message, ...);

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

/* Unloads the array of locations given by loc
 * 
 */
void unload_locations(struct Location* loc, int size);

#endif
