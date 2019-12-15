/*
 * Radio Command Center
 * this program is designed to be uploaded to a Raspberry Pi, which is connected
 * to a baofeng walkie talkie microphone input, with a GPIO pin controlling the
 * PTT.
 */

#include <math.h>
#include <stdio.h>

struct Coordinate
{
	double latitude;
	double longitude;
};

// there has to be a predefined list of locations, each with a spawn location
struct LZ
{
	const char* name;
	struct Coordinate coords;
	
};

// struct that defines an objective.
struct Objeective
{
	const char* name;
	// the message will either constain a message that will be played, or a path
	// to an audio file which will be played.
	const char* message;

	// the location that the objective is at.
	struct LZ lz;

	// time to complete the objective
	int time;
};

double find_distance(struct Coordinate  A, struct Coordinate B)
{
	// convert the coordinates from degrees to radians.
	struct Coordinate A_rad = {A.latitude*M_PI/180, A.longitude*M_PI/180};
	struct Coordinate B_rad = {B.latitude*M_PI/180, B.longitude*M_PI/180};

	// convert the haversin of lat b - lat a and long b - long a
	double hav_lat = pow(sin((A_rad.latitude - B_rad.latitude)/2), 2);
	double hav_lon = pow(sin((A_rad.longitude - B_rad.longitude)/2), 2);

	// find the haversin of the central angle
	double hav_CA = hav_lat + cos(A_rad.latitude)*cos(B_rad.latitude)*hav_lon;

	// archaversin the central angle to find the distance, and then return it.
	double distance = 6.371*pow(10,6) * 2 * asin(pow(hav_CA, 0.5f));
	return distance;
}

// there needs to be a function that will transmit a message, generated in real
//   time. if time permits, have the functionality to instead play an audio file
//   associated with the objective.

// a list of objectives, that can be pulled from "randomly"

int main(void)
{
	return 0;
}
