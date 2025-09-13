// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/ItemBase.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickUp(false)
{

}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickUp = false;
}

UItemBase* UItemBase::CreateItemCopy()
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy -> ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
		
		if (OwningInventory)
		{
			if (Quantity <= 0) {
				OwningInventory->RemoveSingleInstanceOfItem(this);
		 }
		}
	}
}

void UItemBase::Use(ABasePlayerCharacter* Charactor)
{
}

FItemData UItemBase::GetItemData() const
{
	FItemData Data;

	Data.ID = ID;
	Data.ItemType = ItemType;
	Data.ItemQuality = ItemQuality;
	Data.ItemStatistics = ItemStatistics;
	Data.TextData = TextData;
	Data.NumericData = NumericData;
	Data.AssetData = AssetData;

	return Data;
}
