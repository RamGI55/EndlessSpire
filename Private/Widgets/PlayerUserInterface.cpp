// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerUserInterface.h"
#include "Widgets/ItemDragDropOperation.h"
#include "UObjects/ItemBase.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"



UPlayerUserInterface::UPlayerUserInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}


void UPlayerUserInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TeamUI = Cast<UTeamStatsInterface>(this->GetWidgetFromName(TEXT("WBP_TeamStats")));
	PlayerUI = Cast<UPlayerStatInterface>(this->GetWidgetFromName(TEXT("WBP_PlayerStat")));
	MinimapUI = Cast<UMinimapWidget>(this->GetWidgetFromName(TEXT("WBP_Minimap")));
	StatusUI = Cast<UPlayerStatusUserInterface>(this->GetWidgetFromName(TEXT("WBP_PlayerStatus")));
	InventoryUI = Cast<UInventoryPanel>(this->GetWidgetFromName(TEXT("WBP_InventoryPanel")));
	Crosshair = Cast<UCrosshairWidget>(this->GetWidgetFromName(TEXT("WBP_CrosshairWidget")));
	EquipmentPanel = Cast<UEquipmentPanel>(this->GetWidgetFromName(TEXT("WBP_EquipmentPanel")));
}

void UPlayerUserInterface::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());

}

bool UPlayerUserInterface::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (Player && ItemDragDrop->SourceItem)
	{
		Player->ServerDropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		
		return true;
	}

	return false;
}

