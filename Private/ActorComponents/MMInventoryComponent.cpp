// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/MMInventoryComponent.h"
#include "UObjects/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UMMInventoryComponent::UMMInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	ConstructorHelpers::FObjectFinder<UDataTable> TempItemDataTable(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	ItemDataTable = TempItemDataTable.Object;
	// ...
	Money = 0;
}

void UMMInventoryComponent::PrintLogInventory()
{

	UE_LOG(LogTemp, Log, TEXT("%s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
	UE_LOG(LogTemp, Log, TEXT("%s : %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"), *UKismetSystemLibrary::GetDisplayName(GetOwner()));
	for (auto val : InventoryContents) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(val));
	}
}

void UMMInventoryComponent::ServerNewData_Implementation(const FItemData& NewData)
{
	BroadcastNewData(NewData);
}

void UMMInventoryComponent::BroadcastNewData_Implementation(const FItemData& NewData)
{
	if (ItemDataTable)
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(NewData.ID, NewData.ID.ToString());

		UItemBase* ItemReference = NewObject<UItemBase>(this, UItemBase::StaticClass());

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;

		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		ItemReference->SetQuantity(1);

		HandleAddItem(ItemReference);
	}
}

void UMMInventoryComponent::ServerModifyMoney_Implementation(const int32& money)
{
	BroadcastModifyMoney(money);
}

void UMMInventoryComponent::BroadcastModifyMoney_Implementation(const int32& money)
{
	Money += money;
	OnInventoryUpdated.Broadcast();
}

void UMMInventoryComponent::ServerRemoveItem_Implementation(const FName ItemID)
{
	BroadcastRemoveItem(ItemID);
}

void UMMInventoryComponent::BroadcastRemoveItem_Implementation(const FName ItemID)
{
	UItemBase* MatchingItem = FindMatchingItem(ItemID);
	if (MatchingItem)
	{
		RemoveAmountOfItem(MatchingItem, 1);
	}
}

TArray<FItemData> UMMInventoryComponent::ExtractItemDatas()
{
	TArray<FItemData> Datas;
	for (const UItemBase* Element : InventoryContents)
	{
		FItemData ItemData = FItemData();

		ItemData.ID = Element->ID;
		ItemData.ItemType = Element->ItemType;

		ItemData.ItemQuality = Element->ItemQuality;
		ItemData.NumericData = Element->NumericData;
		ItemData.TextData = Element->TextData;
		ItemData.AssetData = Element->AssetData;

		Datas.Add(ItemData);
	}

	return Datas;
}

// Called when the game starts
void UMMInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


UItemBase* UMMInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	//if (ItemIn)
	//{
	//	if (InventoryContents.Contains(ItemIn))
	//	{
	//		return ItemIn;
	//	}
	//}
	if (ItemIn)
	{
		for (UItemBase* InventoryItem : InventoryContents)
		{
			if (InventoryItem && InventoryItem->ID == ItemIn->ID) // ���� �ĺ��ڸ� ����� ��
			{
				return InventoryItem;
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FindMatchingItem :: Item to drop was somehow null"));
	}
	return nullptr;
}

UItemBase* UMMInventoryComponent::FindMatchingItem(const FName ItemID) const
{
	if (InventoryContents.IsEmpty()) {

		UE_LOG(LogTemp, Warning, TEXT("FindMatchingItem :: Empty"));
	}
	for (UItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ItemID) // ���� �ĺ��ڸ� ����� ��
		{
			return InventoryItem;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("FindMatchingItem :: Can't Find"));
	return nullptr;
}

UItemBase* UMMInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

// ���� �߰��� �Լ�: ���� ID�� ���� ������ �˻�
UItemBase* UMMInventoryComponent::FindItemByID(FName ItemID) const
{
	for (UItemBase* InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ItemID)
		{
			return InventoryItem;
		}
	}
	return nullptr;
}

UItemBase* UMMInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
			{
				return InventoryItem->ID == ItemIn->ID && InventoryItem->IsFullItemStack();
			}
		))
	{
		return *Result;
	}
	return nullptr;

}

int32 UMMInventoryComponent::CaculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UMMInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UMMInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UMMInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	//const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	//ItemIn->SetQuantity(ItemIn->Quantity = ActualAmountToRemove);

	//InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	//OnInventoryUpdated.Broadcast();

	//return ActualAmountToRemove;

	// ���� ������ ���� ���
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	// ���� ���� ���
	int32 NewQuantity = ItemIn->Quantity - ActualAmountToRemove;
	ItemIn->SetQuantity(NewQuantity);

	// ���� ������ 0 �����̸� �κ��丮���� �ش� ������ ���� (�ʿ��� ���)
	if (NewQuantity <= 0)
	{
		InventoryContents.RemoveSingle(ItemIn);
	}

	// ��ü ���� ������Ʈ
	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	// �κ��丮 ���� ������ UI�� �ݿ��ϵ��� �̺�Ʈ ��ε�ĳ��Ʈ
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UMMInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}


FItemAddResult UMMInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// check if input item has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight value"), InputItem->TextData.Name));
	}

	// will the item weight overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would ovewrflow weight limit"), InputItem->TextData.Name));
	}

	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All Inventory slots are full"), InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added {0} to the inventory."), InputItem->TextData.Name));
}

int32 UMMInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	// ���� ���ѿ� ���� �߰� ������ �ִ� ���� ���
	int32 AllowedByWeight = CaculateWeightAddAmount(ItemIn, RequestedAddAmount);
	if (AllowedByWeight <= 0)
	{
		return 0;
	}
	RequestedAddAmount = FMath::Min(RequestedAddAmount, AllowedByWeight);

	int32 TotalAdded = 0;
	int32 AmountLeft = RequestedAddAmount;

	// 1. ���� ���ÿ� �߰�: ������ ID�� ���� ������ �� ���� �ִ� ���� ������ �������� ���� ������ ã�� ä��ϴ�.
	for (UItemBase* ExistingItem : InventoryContents)
	{
		if (ExistingItem->ID == ItemIn->ID && ExistingItem->NumericData.bIsStackable)
		{
			int32 CurrentQuantity = ExistingItem->Quantity;
			int32 MaxStackSize = ExistingItem->NumericData.MaxStackSize;

			// ������ �� ���� ���� ��쿡��
			if (CurrentQuantity < MaxStackSize)
			{
				int32 CanAdd = MaxStackSize - CurrentQuantity;
				int32 AddNow = FMath::Min(CanAdd, AmountLeft);

				ExistingItem->SetQuantity(CurrentQuantity + AddNow);
				TotalAdded += AddNow;
				AmountLeft -= AddNow;
				InventoryTotalWeight += AddNow * ItemIn->GetItemSingleWeight();

				// ��� ������ �߰��Ǿ����� �ݺ� ����
				if (AmountLeft <= 0)
				{
					break;
				}
			}
		}
	}

	// 2. ���� ������ ���� �� ���� ����: �κ��丮 ������ �����ִٸ� �� �׸��� �����մϴ�.
	while (AmountLeft > 0 && InventoryContents.Num() < InventorySlotsCapacity)
	{
		// �� ���Կ� �� �� �ִ� �ִ� ������ MaxStackSize
		int32 NewStackAmount = FMath::Min(AmountLeft, ItemIn->NumericData.MaxStackSize);

		AddNewItem(ItemIn, NewStackAmount);
		TotalAdded += NewStackAmount;
		AmountLeft -= NewStackAmount;

		// AddNewItem ���ο��� InventoryTotalWeight�� OnInventoryUpdated.Broadcast()�� ȣ��˴ϴ�.
	}

	// ���������� �κ��丮 ������Ʈ �̺�Ʈ�� ��ε�ĳ��Ʈ�Ͽ� UI�� ������ �˸�
	OnInventoryUpdated.Broadcast();

	return TotalAdded;
}

FItemAddResult UMMInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable items
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handel stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			// return added all result
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} to the invenntory."),
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			// return added partial result
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} to the invenntory. Numer added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			// return added none result
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fall through error. Get Owner() check somehow failed"));
}
void UMMInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickUp)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();

	UE_LOG(LogTemp, Warning, TEXT("Inventory AddNewItem: Total items now: %d"), InventoryContents.Num());
	OnInventoryUpdated.Broadcast();
}


