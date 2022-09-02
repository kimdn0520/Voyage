#include "pch.h"
#include "Item.h"
#include "Inventory.h"

Inventory::Inventory(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{
}

Inventory::~Inventory()
{
}

void Inventory::Start()
{
	

}

void Inventory::Update()
{

}

//std::shared_ptr<Item> Inventory::FindItem(Item::EItemType type)const
//{
//	for (const auto& item : inventory)
//	{
//		if (item.first->GetType() == type)
//		{
//			return item.first;
//		}
//	}
//
//	return nullptr;
//}
//
//UINT Inventory::GetItemCount(const Item::EItemType type) const
//{
//	auto _iter = inventory.begin();
//	for (; _iter != inventory.end(); ++_iter)
//	{
//		if ((*_iter).first->GetType() == type)
//		{
//			return(*_iter).second;
//		}
//	}
//	return 0;
//}
//
//void Inventory::SetItemCount(const Item::EItemType type, UINT count)
//{
//	auto _iter = inventory.begin();
//
//	for (; _iter != inventory.end(); ++_iter)
//	{
//		if ((*_iter).first->GetType() == type)
//		{
//			(*_iter).second = count;
//
//			if ((*_iter).second < 0)
//			{
//				(*_iter).second = 0;
//			}
//		}
//	}
//}
//
//UINT Inventory::IncreaseItemCount(const Item::EItemType type, const UINT val)
//{
//	auto _iter = inventory.begin();
//
//	for (; _iter != inventory.end(); ++_iter)
//	{
//		if ((*_iter).first->GetType() == type)
//		{
//			(*_iter).second += val;
//
//			break;
//		}
//	}
//
//	return (*_iter).second; 
//}
//
//UINT Inventory::DecreaseItemCount(const Item::EItemType type, const UINT val)
//{
//	auto _iter = inventory.begin();
//
//	for (; _iter != inventory.end(); ++_iter)
//	{
//		if ((*_iter).first->GetType() == type)
//		{
//			(*_iter).second -= val;
//
//			break;
//		}
//	}
//
//	return (*_iter).second;
//}
