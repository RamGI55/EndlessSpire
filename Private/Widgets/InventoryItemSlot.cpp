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
		// ��Ŭ�� �߻� �� HandleRightClick() ȣ�� (���⼭ Blueprint �̺�Ʈ�� OnRightClicked�� �Բ� ȣ���� �� �ֽ��ϴ�)

		HandleRightClick();
		// �Ǵ� Blueprint �̺�Ʈ�� �����ϰ� �ʹٸ�:
	
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
			// Armor Ÿ�Կ� ���� ó�� ����
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Armor item."));
			}

			// EquipmentSlots���� �̸��� "WBP_ArmorEquipmentSlot"�� ������ ã��
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_ArmorEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* ArmorSlot = *FoundSlotPtr;
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				ArmorSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;

		case EItemType::Helm:
			// Helm Ÿ�Կ� ���� ó�� ����
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
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				HeadSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;

		case EItemType::Shoes:
			// Shoes Ÿ�Կ� ���� ó�� ����
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
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				ShoesSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}

			if (ItemReference->ItemStatistics.ArmorRating > 0 && ItemReference->ItemStatistics.DamageValue == 0)
			{
				OwnerStat->ServerModifyStat("Def", ItemReference->ItemStatistics.ArmorRating);

			}
			break;
		}

		case EItemType::MainWeapon:
			// MainWeapon Ÿ�Կ� ���� ó�� ����
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing MainWeapon item."));
			}

			// EquipmentSlots���� �̸��� "WBP_MainWeaponEquipmentSlot"�� ������ ã��
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_MainWeaponEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* MainWeaponSlot = *FoundSlotPtr;
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				MainWeaponSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}

			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}
			break;
		}

		case EItemType::Amulet:
			// Amulet Ÿ�Կ� ���� ó�� ����
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Amulet item."));
			}

			// EquipmentSlots���� �̸��� "WBP_MainWeaponEquipmentSlot"�� ������ ã��
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_AmuletEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* AmuletSlot = *FoundSlotPtr;
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				AmuletSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}

			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}
			break;

		case EItemType::Ring:
			// Ring Ÿ�Կ� ���� ó�� ����
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Processing Ring item."));
			}
			
			// EquipmentSlots���� �̸��� "WBP_MainWeaponEquipmentSlot"�� ������ ã��
			FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
				{
					return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_AmuletEquipmentSlot"));
				});

			if (FoundSlotPtr && *FoundSlotPtr)
			{
				UEquipmentSlot* RingSlot = *FoundSlotPtr;
				// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
				RingSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���
			}
			
			if (ItemReference->ItemStatistics.ArmorRating == 0 && ItemReference->ItemStatistics.DamageValue > 0)
			{
				OwnerStat->ServerModifyStat("Atk", ItemReference->ItemStatistics.DamageValue);
			}

			
			break;

		case EItemType::Spell:
			// Spell Ÿ�Կ� ���� ó�� ����
			UE_LOG(LogTemp, Log, TEXT("Processing Spell item."));
			break;

		case EItemType::Mundane:
			// Mundane Ÿ�Կ� ���� ó�� ����
			UE_LOG(LogTemp, Log, TEXT("Processing Mundane item."));
			break;

		case EItemType::Consumable:
			// Consumable Ÿ�Կ� ���� ó�� ����
			UE_LOG(LogTemp, Log, TEXT("Processing Consumable item."));
			break;

		default:
			// ��Ÿ ó�� (��: �߸��� ������ Ÿ��)
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

/////////////////////////// ���� ��� ����, ���� ����. 0411 �輱��
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

	// EquipmentSlots���� �̸��� "WBP_MainWeaponEquipmentSlot"�� ������ ã��
	UEquipmentSlot** FoundSlotPtr = MyEquipmentPanel->EquipmentSlots.FindByPredicate([](UEquipmentSlot* FindSlot)
		{
			return FindSlot && FindSlot->GetName().Equals(TEXT("WBP_MainWeaponEquipmentSlot"));
		});

	if (FoundSlotPtr && *FoundSlotPtr)
	{
		UEquipmentSlot* MainWeaponSlot = *FoundSlotPtr;
		// MainWeapon ���Կ� ���� ��� ���� ó�� (���� ��� EquipItem ȣ��)
		MainWeaponSlot->EquipItem(CurrentItemReference);  // CurrentItem�� ������ �������� ���۷���

	}
	ABasePlayerCharacter* Owner = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	UMMPlayerStatComponent* OwnerStat = Owner->GetPlayerStatComponent();

	
	
	
}
