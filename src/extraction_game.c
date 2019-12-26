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
	printf("game init\n");
	// load in the locations
	struct Location* locs;
	int locs_size;
	load_locations("./locations.txt", &locs, &locs_size);

	srandom(time(NULL));

	int current_location;

	// TODO add game start transmission and delay
	
	transmit("The game mode is Extraction. The game will start in 15 minutes", NULL);

	//sleep(10*60);

	transmit("The game will start in 5 minutes", NULL);

	//sleep(4*60);

	transmit("The game will start in 1 minute", NULL);

	//sleep(30);

	transmit("30 seconds", NULL);

	//sleep(20);

	for ( int i=10; i >= 0; i-- )
	{
		char c[5];
		sprintf(c, "%d", i);
		transmit(c, NULL);
		usleep(500);
	}

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

		int secs = mins * 60;

		if ( i == 0 )
			goto transmit;

		if ( mins >= 7 )
		{
			printf("time is more than 7\n");
			// 5 minute warning
			sleep(secs - 5*60);
			transmit("Warning, the location will be changing in 5 minutes", NULL);
			
			// 1 minute warning
			sleep(4 * 60);
			transmit("Warning, the location will be changing in 1 minute", NULL);

			sleep(60);
		}
		else if ( mins >= 5 )
		{
			printf("5 <= time <=\n");
			// 1 minute warning.
			sleep(secs - 60);
			transmit("Warning, the location will be changing in 1 minute", NULL);

			sleep(60);
		}
		else
		{
			printf("time < 5\n");
			// if the time is too short, then there is no warning
			sleep(secs);
		}

		transmit:
		if ( i == max_turns - 1 )
			transmit("The game is now over", NULL);
		else
			transmit("The new location is: ", locs[current_location].name,
					". It is located at: ", locs[current_location].x, ", ", locs[current_location].y, ".",
					NULL);
	}

	unload_locations(locs, locs_size);
}
