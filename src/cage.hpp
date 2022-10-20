/**
 *
 * @file cage.hpp
 * @brief This is the cage header file.
 *
 */

#ifndef __blickport__cage__
#define __blickport__cage__

// Cages to implement:

// 1. MapCage -- Basic fundamental properties of the world, location,
// physical space, etc. Movement aligned with classical physics.

// 2. SpawnCage -- Intention and will of dynamic entities. Movement
// and position data more aligned with intentional locomotion. Also
// data regarding health, life, homeostasis, etc. Keeps track of
// familial relationships, as well.

// 3. ItemCage -- Products and resources associated with dynamic
// entities. Aspects of the world which can be owned, bought, sold,
// moved, interacted with, related to, et al. Economy data. Etc.

// 4. CombatCage -- Keeps track of data related to physical combat
// between entities. Who hates who. Who is aligned with who. Handles
// actual tactics of immediate combat, as well as keeps track of
// general longstanding conflicts between entities.

// 5. SocialCage -- Keeps track of non-combat social activities
// between dynamic entities. Things like dialogue, cooperation, jobs,
// in-game activities. Also keeps track of social internal state data
// for entities. Jealousy, envy, pride, joy, appreciation, awe,
// wonder, etc.

// 6. Perhaps Other Cages as necessary. But these 5 are the primary
// ones.

class Cage{
public:
    virtual void event() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

#endif
