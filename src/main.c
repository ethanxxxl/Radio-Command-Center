/*
 * since the guys arent going to help...
 * a flash drive will be inserted that will have an xml file in it
 * 
 * I can make an xml file with the matches laid out. maybe not even xml, we will
 * see.
 * when the program is running, we can select the match that we want to run. on
 * an LCD display before we run it.
 *
 * NOT TO DO LIST
 *	- speech recognition, just no.
 *	- audio files. just do messages
 *	
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <extraction_game.h>

double find_distance(struct Location  A, struct Location B)
{
	// convert the coordinates from degrees to radians.
	struct Location A_rad = {"", "", A.latitude*M_PI/180, A.longitude*M_PI/180};
	struct Location B_rad = {"", "", B.latitude*M_PI/180, B.longitude*M_PI/180};

	// convert the haversin of lat b - lat a and long b - long a
	double hav_lat = pow(sin((A_rad.latitude - B_rad.latitude)/2), 2);
	double hav_lon = pow(sin((A_rad.longitude - B_rad.longitude)/2), 2);

	// find the haversin of the central angle
	double hav_CA = hav_lat + cos(A_rad.latitude)*cos(B_rad.latitude)*hav_lon;

	// archaversin the central angle to find the distance, and then return it.
	double distance = 6.371*pow(10,6) * 2 * asin(pow(hav_CA, 0.5f));
	return distance;
}


// creates a spawnpoint, given a target location, and some restrictions.
int find_spawn(struct Location* locs, int size, int target, int enemy_spawn, double max_distance)

{
	int furth;
	double target_dist = 0;
	double espawn_dist = 0;

	// loop that goes through all locations and finds the furthest one.
	for (int i = 0; i < size; i++)
	{
		// finds the distance between this element and the enemy spawn and the target
		double target_dist0 = find_distance(locs[i], locs[target]);
		double espawn_dist0 = find_distance(locs[i], locs[enemy_spawn]);
		
		// if those distances are bigger than last biggest ones, and also less then max distance,
		// make this element the new max.
		if (
			target_dist0 > target && target_dist0 < max_distance &&
			espawn_dist0 > enemy_spawn && espawn_dist > max_distance &&
			espawn_dist0 > target_dist0
			)
		{
			furth = i;
			target_dist = target_dist0;
			espawn_dist = espawn_dist0;
		}
	}

	// TODO consider making this just return the target as the spawn, and transmit that there was an issue finding
	// a spawn.
	//
	// basically, I am too lazy to make it do something if it can't find a new distane right now.
	// instead, it will just say that there was an error.
	if ( furth == -1 )
	{
		transmit("Ethan screwed up the spawn function, the command center is broke, figure out the rest of the game on your own.");
		exit(EXIT_FAILURE);
	}
	
	return furth;
}

/*
 * Game starts, and every x minutes, the target location changes.
 *
 */

int main(void)
{
	extraction_game(1.0f, 4);

	return 0;
}
