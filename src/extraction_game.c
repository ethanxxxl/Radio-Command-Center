#include <extraction_game.h>
#include <parser.h>

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <stdio.h>

// game of extraction, team A has intel that they have to get to the location that is transmitted.
// they lose if they do not make it to the location before the location switches, if they are all
// killed, or if the intel is stolen. (there are medics, and respawns.).
// for the command center, all it needs to do is change the current objective very x minutes
void extraction_game(float mins, int max_turns)
{
	// load in the locations
	struct Location* locs;
	int locs_size;
	load_locations("./locations.txt", &locs, &locs_size);

	srandom(time(NULL));

	int current_location;

	for ( int i=0; i < max_turns; i++ )
	{
		// selects a random spot, and makes sure that it is not the same spot as the current one
		while ( true )
		{
			int temp_loc = random() % locs_size;

			if ( temp_loc != current_location )
			{
				current_location = temp_loc;
				break;
			}
		}

		// TODO make this transmit the actual information
		// transmit this
		transmit(locs[current_location].name);
		
		// TODO add warnings, to warn of an incomming transmission.
		// sleep for min minutes
		sleep(mins);
	}

	transmit("The game is now over");

	unload_locations(locs);
}
