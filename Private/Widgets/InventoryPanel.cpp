// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryPanel.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "Widgets/InventoryItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Player = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryReference = Player->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: Bound OnInventoryUpdated delegate successfully"));
			SetInfoText();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: InventoryReference is null"));
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue{
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight(),1) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity(),1)
	};

	const FString CapacityInfoValue{
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotsCapacity())
	};

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
	MoneyText->SetText(FText::Format(NSLOCTEXT("YourNamespace", "MoneyText", "$ {0}"), FText::AsNumber(InventoryReference->Money)));
		
}

void UInventoryPanel::RefreshInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("RefreshInventory() called"));
	if (InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
