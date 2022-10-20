/**
 * @file inventory.hpp
 * @brief items and resource management.
 *
 */

#ifndef __blickport__inventory__
#define __blickport__inventory__

#include "entity.hpp"
#include "state.hpp"
#include "cage.hpp"

struct ItemData{
	// Items are loose collections of properties and attributes. We
	// don't store logic for how to use any one specific item, because
	// items have many different usages, depending on overall game
	// state context and item attribute values.

	// Hence, we need an "item cage" which is capable of storing
	// additional information, such as property/ownership, and the
	// specific context within which an item is being invoked, as well
	// as other stuff that can be found in other cages, et al.
};

struct ItemContainer{
	// Containers contain items. They may also contain other
	// containers, as well.
};

struct EquipSlots{
	// An EquipSlots object loosely corresponds to a collection of
	// things which are attached to a dynamic game entity. So, for
	// example, a particular NPC may have an EquipSlots object which
	// contains "attachment points" to which various in game items may
	// be attached.

	// In game items, in order to be attached to a dynamic entity,
	// must be compatible with one of possibly many empty equip slots
	// that are found inside the EquipSlots object. Only one item may
	// be equipped to a single equip slot.

	// How many Equip slots a particular entity has depends on the
	// ItemState that contains the EquipSlots object.

	// Note: Not all items are compatible with all equip slots, or
	// even any equip slot.

	// However, all items CAN be stored inside ItemContainers, many of
	// which can be equipped to an equipSlot.

	// In essense, and EquipSlots object is sort of an abstraction of
	// those aspects of an entity which permit items to be moved along
	// with that entities person, as the entity moves about the world.
};

struct EstateData{
	// Estate data represents an abstraction of data regarding all the
	// collection of property which may be assigned to a dynamic
	// entity.

	// The key difference between this and, say, an EquipSlots object
	// is that even though EstateData may be attached to a dynamic
	// object, and EstateData object itself is *NOT* dynamic, and does
	// not move throughout the world along with an entity's personage.

	// EstateData can hold many different types of EquipSlot,
	// ItemContainer, and EstateData objects.
};

class ItemCage : public Cage {
	// The ItemCage handles all data regarding the
	// creation/flow/destruction of resources, items, and goods
	// throughout the game world.

	// It handles various game activities:

	// 1. Creation of items from dynamic/static resources. An entity
	// might dip their cup into the river to create a cup of river
	// water, for example.

	// 2. Transfer of ownership and/or handling requests concerning
	// what game entities have what on their person, in their estates,
	// etc. Keeping track of property owned, by who, and to what
	// extent, etc.

	// 3. Usage of items with respect to various in-world game
	// contexts. For example, swinging a sword in combat, or unlocking
	// a door with a lockpicking toolkit.

	// 4. The breaking/damage/destruction/decay of items. What happens
	// to these items when any of the aforementioned occurs?
public:
	ItemCage(){};
};

#endif
