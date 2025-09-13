// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryItemSlot.h"
#include "Widgets/InventoryToolTip.h"
#include "Widgets/DragItemVisual.h"
#include "Widgets/ItemDragDropOperation.h"
#include "Widgets/EquipmentPanel.h"
#include "Widgets/EquipmentSlot.h"

#include "ActorComponents/MMPlayerStatComponent.h"

#include "HUDS/LobbyHUD.h"

#include "UObjects/ItemBase.h"
#include "UStructs/FItem.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		UInventoryToolTip* ToolTip = CreateWidget<UInventoryToolTip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}

}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::Unique : 
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		case EItemQuality::Epic: 
			ItemBorder->SetBrushColor(FLinearColor(238.0f, 130.0f, 238.0f, 1.0f));
			break;
		case EItemQuality::Rare : 
			ItemBorder->SetBrushColor(FLinearColor::Yellow);
			break;
		case EItemQuality::Magic : 
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::Common : 
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		default:;
		}

		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 우클릭 발생 시 HandleRightClick() 호출 (여기서 Blueprint 이벤트인 OnRightClicked도 함께 호출할 수 있습니다)

		HandleRightClick();
		// 또는 Blueprint 이벤트로 전달하고 싶다면:
	
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);
	}

	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryItemSlot::HandleRightClick()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Item slot right-clicked."));
	}
	UEquipmentSlot** FoundSlotPtr;

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		ALobbyHUD* HUD = Cast<ALobbyHUD>(PC->GetHUD());
		UEquipmentPanel* MyEquipmentPanel = HUD->PlayerUI->EquipmentPanel;

		UItemBase* CurrentItemReference = this->GetItemReference();
		EItemType ItemType = CurrentItemReference->ItemType;

		ABasePlayerCharacter* Owner = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
		UMMPlayerStatComponent* OwnerStat = Owner->GetPlayerStatComponent();

		switch (ItemType)
		{
		case EItemType::Armor:
			// Armor 타입에 대한 처리 로직
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Armor item."));
			}

			// EquipmentSlots에서 이름이 "WBP_ArmorEquipmentSlot"인 슬롯을 찾음
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_ArmorEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* ArmorSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				ArmorSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;

		case EItemType::Helm:
			// Helm 타입에 대한 처리 로직
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Helm item."));
			}

			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_HeadEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* HeadSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				HeadSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;

		case EItemType::Shoes:
			// Shoes 타입에 대한 처리 로직
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Shoes item."));
			}

			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_ShoesEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* ShoesSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				ShoesSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;
		}

		case EItemType::MainWeapon:
			// MainWeapon 타입에 대한 처리 로직
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing MainWeapon item."));
			}

			// EquipmentSlots에서 이름이 "WBP_MainWeaponEquipmentSlot"인 슬롯을 찾음
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_MainWeaponEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* MainWeaponSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				MainWeaponSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}

			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}
			break;
		}

		case EItemType::Amulet:
			// Amulet 타입에 대한 처리 로직
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Amulet item."));
			}

			// EquipmentSlots에서 이름이 "WBP_MainWeaponEquipmentSlot"인 슬롯을 찾음
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_AmuletEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* AmuletSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				AmuletSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}

			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}
			break;

		case EItemType::Ring:
			// Ring 타입에 대한 처리 로직
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Ring item."));
			}
			
			// EquipmentSlots에서 이름이 "WBP_MainWeaponEquipmentSlot"인 슬롯을 찾음
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_AmuletEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* RingSlot = *FoundSlotPtr;
				// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
				RingSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스
			}
			
			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}

			
			break;

		case EItemType::Spell:
			// Spell 타입에 대한 처리 로직
			UE_LOG(LogTemp, Log, TEXT("Processing Spell item."));
			break;

		case EItemType::Mundane:
			// Mundane 타입에 대한 처리 로직
			UE_LOG(LogTemp, Log, TEXT("Processing Mundane item."));
			break;

		case EItemType::Consumable:
			// Consumable 타입에 대한 처리 로직
			UE_LOG(LogTemp, Log, TEXT("Processing Consumable item."));
			break;

		default:
			// 기타 처리 (예: 잘못된 아이템 타입)
			UE_LOG(LogTemp, Warning, TEXT("Unknown item type."));
			break;
		}
		//MyEquipmentPanel->UpdateWidget(OwnerStat->GetPlayerStats());
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayerController is null"));
	}
}

/////////////////////////// 현재 사용 안함, 수정 예정. 0411 김선우
void UInventoryItemSlot::CallEquipItemByType(EItemType ItemType, UItemBase* Reference)
{
	UItemBase* CurrentItemReference = Reference;
	APlayerController* PC = GetOwningPlayer();
	ALobbyHUD* HUD = Cast<ALobbyHUD>(PC->GetHUD());
	UEquipmentPanel* MyEquipmentPanel = HUD->PlayerUI->EquipmentPanel;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing MainWeapon item."));
	}

	// EquipmentSlots에서 이름이 "WBP_MainWeaponEquipmentSlot"인 슬롯을 찾음
	UEquipmentSlot** FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
		{
			return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_MainWeaponEquipmentSlot"));
		});

	if (FoundSlotPtr && *FoundSlotPtr)
	{
		UEquipmentSlot* MainWeaponSlot = *FoundSlotPtr;
		// MainWeapon 슬롯에 대해 장비 장착 처리 (예를 들어 EquipItem 호출)
		MainWeaponSlot->EquipItem(CurrentItemReference);  // CurrentItem은 장착할 아이템의 레퍼런스

	}
	ABasePlayerCharacter* Owner = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	UMMPlayerStatComponent* OwnerStat = Owner->GetPlayerStatComponent();

	
	
	
}
