// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

void UItemObject::CallItemUsage()
{
	switch (ItemData.ItemTypeUsage)
	{
	case EItemTypeUsage::None:
		break;
	case EItemTypeUsage::Use:
		UseItem();
		break;
	case EItemTypeUsage::Equip:
		EquipItem();
		break;
	default:
		break;
	}
}

void UItemObject::UseItem_Implementation()
{

}

void UItemObject::EquipItem_Implementation()
{

}