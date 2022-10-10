/**
 * @file clock.hpp
 * @brief Internal world game clock
 *
 */

#ifndef __blickport__clock__
#define __blickport__clock__

#include "entity.hpp"
    
class GsClock{
    int currentTick;
    int lastTick;

    // Keep track of how many times the GsClock has overflowed
    // over. This is important for things like level traversal, or
    // stuff that needs to keep track of long periods of time.
    long int numCycles;
	long int lastCycle;
	
    const int MAX_TICKS;
public:
    GsClock():MAX_TICKS(1000){
        currentTick = 0;
        lastTick = 0;
		numCycles = 0;
		lastCycle = 0;
    };
    
    void update(){};
    void modTime(int ticks){
		if(ticks <= 0){
			return;
		} else {
			lastTick = currentTick;
			lastCycle = numCycles;
		}
		
		if(currentTick + ticks > MAX_TICKS){
			numCycles += (currentTick + ticks) / MAX_TICKS;
			currentTick = (currentTick + ticks) % MAX_TICKS;
		} else {
			currentTick = (currentTick + ticks);
		}
    }

    int getTime(){ return currentTick; }
    int getOldtime(){ return lastTick; }
    
    int getElapsed(){
		return (MAX_TICKS * (numCycles - lastCycle) + currentTick - lastTick);
    };
};

#endif
