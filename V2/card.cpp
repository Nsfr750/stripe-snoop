/**
 * @file card.cpp
 * @brief Holds the detected/missing Tracks of a card
 * 
 * Card holds 1 or more tracks that are contained on a physcial card. It
 * also stores whether other tracks are missing, or simply couldn't be
 * detected
 *
 * @author Acidus (acidus@msblabs.org)
 * 
 * This file is part of Stripe Snoop (http://stripesnoop.sourceforge.net)
 *
 * Stripe Snoop is licensed under the GPL. See COPYING for more info
 *
 * Copyright (C) 2005 Acidus, Most Significant Bit Labs
 */
#include "card.h"

Card::Card() {
	
}

int Card::hasTrack(const int &t) const {
	//printf("Do we have track ...%d (Size %d)\n", t, trackPresent.size());	
	//we if don't know the status of any tracks at all...
	if(trackPresent.empty()) {
		//printf("We have no tracks! NO means %d\n",NO);
		return NO; //tell them we don't know
	}
	for(int i=0; i < (signed int) trackPresent.size(); i++) {
		if(trackPresent.at(i).trackNum == t) {
			//we do have knowledge of this track!
			return trackPresent.at(i).present;
		}
	}
	return UNKNOWN;
}

void Card::addMissingTrack(const int &i) {
	//printf("Missing Tracks being added");
	//do we already have this track?
	if(hasTrack(i) == YES) {
		//printf("ERROR Attempted to add missing track twice!\n");
	} else {
		//add it to our "present" list
		TrackPresent t;
		t.trackNum= i;
		t.present=NO;
		trackPresent.push_back(t);
	}
}
		
void Card::addTrack(const Track &theTrack) {
	//printf("ADDING TRACK?\n");
	//do we already have this track?
	if(hasTrack(theTrack.getNumber()) == YES) {
		printf("ERROR Attempted to add track twice!\n");
	} else {
		//add it to our "present" list
		TrackPresent t;
		t.trackNum= theTrack.getNumber();
		//printf("track num is %d\n", t.trackNum);
		t.present=YES;
		trackPresent.push_back(t);
		//add to vector
		tracks.push_back(theTrack);
	}
}

Track Card::getTrack(const int &j) const {
	//do we even have this track?
	int i;
	if(hasTrack(j) != YES) {
		printf("ERROR Attempted get nonexistant track! %d\n", j);
	} 
	for(i=0; i < (int) tracks.size(); i++) {
		if(tracks.at(i).getNumber() == j) {
			break; //we do have knowledge of this track!
		}
	}
	return tracks.at(i);
}

void Card::decodeTracks() {
	//decode all the tracks
	for(int i=0; i < (int) tracks.size(); i++) {
		tracks.at(i).decode();
	}
}

void Card::printTracks() const {
	for(int i=0; i < (int) tracks.size(); i++) {
		printf("Track %d:", tracks.at(i).getNumber());
		printf("%s:\n", tracks.at(i).getChars());
	}
}
