/*
 * Class Card
 *
 * Holds all info about a card
 */

#ifndef CARD_H
#define CARD_H
#include "track.h"
#include <vector>


//----------------defines
//defines for hasTrack Functions
#define UNKNOWN 0	//reader didn't support this track, so we don't
			//know if card contains this track
#define YES 1		//yes, we can read it and yes, it was there
#define NO 2		//yes, we can read it and no, it was not there
class TrackPresent {
public:
	int trackNum;
	int present;
	
};

typedef std::vector<Track>  TrackVec;

typedef std::vector<TrackPresent> TrackPresentVec;


class Card {
public:
	Card();
	int hasTrack(const int &) const;
	void addMissingTrack(const int&);
	void addTrack(const Track &);
	Track getTrack(const int&) const;
	void decodeTracks(void);
	void printTracks(void) const;
private:
	
	TrackVec tracks;
	TrackPresentVec trackPresent;
};
#endif
