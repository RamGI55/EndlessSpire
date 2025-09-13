// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryToolTip.h"
#include "Widgets/InventoryItemSlot.h"
#include "UObjects/ItemBase.h"
#include "Components/TextBlock.h"

void UInventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Helm: 
		ItemType->SetText(FText::FromString("Helm"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Armor: 
		ItemType->SetText(FText::FromString("Armor"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Shoes:
		ItemType->SetText(FText::FromString("Shoes"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::MainWeapon:
		ItemType->SetText(FText::FromString("MainWeapon"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Amulet:
		ItemType->SetText(FText::FromString("Amulet"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Ring:
		ItemType->SetText(FText::FromString("Ring"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Spell:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:;
	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	//SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.SellValue));
	const FString WeightInfo =
	{ "Weight : " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
	
	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo = 
		{ "Max Stack size : " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize) };

		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
