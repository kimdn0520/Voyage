#pragma once
#include "MonoBehaviour.h"


class Inventory : public MonoBehaviour
{
public:
	Inventory(shared_ptr<GameObject> gameObject);
	virtual ~Inventory();

	virtual void Start()override;
	virtual void Update()override;

public:
	/*void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
	void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;*/

public:
	/*std::shared_ptr<Item> FindItem(Item::EItemType type) const;
	UINT GetItemCount(const Item::EItemType type) const;
	void SetItemCount(const Item::EItemType type, UINT count);
	UINT IncreaseItemCount(const Item::EItemType type, const UINT val);
	UINT DecreaseItemCount(const Item::EItemType type, const UINT val);*/

public:
	//std::vector<std::pair<std::shared_ptr<Item>, int>> inventory;
};