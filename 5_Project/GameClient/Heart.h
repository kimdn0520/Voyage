#pragma once
#include "Item.h"

class Heart : public Item
{
public:
	Heart(Item::EItemType type, shared_ptr<Inventory>() = nullptr);
	virtual ~Heart();


};

