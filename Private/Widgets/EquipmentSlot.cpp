// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EquipmentSlot.h"
#include "Widgets/EquipmentPanel.h"
#include "Widgets/InventoryToolTip.h"
#include "Widgets/ItemDragDropOperation.h"
#include "UObjects/ItemBase.h"

#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "ActorComponents/MMPlayerStatComponent.h"

#include "HUDS/LobbyHUD.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"



void UEquipmentSlot::NativeOnInitialized()
{
	// 기본 이미지 저장
	UObject* ResourceObj = ItemImage->Brush.GetResourceObject();
	BaseItem2DImage = Cast<UTexture2D>(ResourceObj);
}

void UEquipmentSlot::NativeConstruct()
{
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	// 장비창 에서 슬롯 우클릭 시 다시 인벤토리로 넣는 로직
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 우클릭 발생 시 HandleRightClick() 호출 (여기서 Blueprint 이벤트인 OnRightClicked도 함께 호출할 수 있습니다)

		HandleRightClick();
		// 또는 Blueprint 이벤트로 전달하고 싶다면:

		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);
	}

	return Reply.Unhandled();
}

void UEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}


void UEquipmentSlot::EquipItem(UItemBase* CurrentItemReference)
{
	// 아이템 레퍼런스 전달
	ItemReference = CurrentItemReference;

	if (ItemImage && CurrentItemReference && CurrentItemReference->AssetData.Icon)
	{
		ItemImage->SetBrushFromTexture(CurrentItemReference->AssetData.Icon);
	}

	APlayerController* PC = GetOwningPlayer();
	ABasePlayerCharacter* Owner = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	UMMInventoryComponent* InventoryContents = Owner->GetInventory();
	
	//InventoryContents->BroadcastRemoveItem(CurrentItemReference->ID);
	InventoryContents->ServerRemoveItem(CurrentItemReference->ID);

	//UMMPlayerStatComponent* OwnerStat = Owner->GetPlayerStatComponent();
	//OwnerStat->ModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
}

void UEquipmentSlot::HandleRightClick()
{
	APlayerController* PC = GetOwningPlayer();
	ABasePlayerCharacter* Owner= Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	ALobbyHUD* HUD = Cast<ALobbyHUD>(PC->GetHUD());
	UEquipmentPanel* MyEquipmentPanel = HUD->PlayerUI->EquipmentPanel;

	if (PC)
	{
		
		UInventoryPanel* MyInventoryPanel = HUD->PlayerUI->InventoryUI;

		UItemBase* CurrentItem = this->GetItemReference();
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing EquipmentSlot Click."));
		}

		// UI 변경
		ItemImage->SetBrushFromTexture(BaseItem2DImage);
		UMMInventoryComponent* InventoryContents = Owner->GetInventory();


		UMMPlayerStatComponent* OwnerStat = Owner->GetPlayerStatComponent();
		if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue <= 0)
		{
			OwnerStat->ServerModifyStat("Def", -ItemReference->ItemStatistics.ArmorRating);

		}
		else
		{
			OwnerStat->ServerModifyStat("Atk", -ItemReference->ItemStatistics.DamageValue);
		}
		
		FItemData ItemData = CurrentItem->GetItemData();
		
		InventoryContents->ServerNewData(ItemData);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayerController is null"));
	}
}

