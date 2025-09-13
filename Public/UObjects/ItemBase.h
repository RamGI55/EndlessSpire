// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UStructs/FItem.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "ItemBase.generated.h"

/**
 * 
 */

class UMMInventoryComponent;

UCLASS(BlueprintType)
class ENDLESSSPIRE_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public :
	// property & variables
	//
	// 
	// 인벤토리 업데이트 예정
	UPROPERTY(EditAnywhere, Category = "Item Data")
	UMMInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	bool bIsCopy;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	bool bIsPickUp;
	
	// Functions
	UItemBase();

	void ResetItemFlags();

	UItemBase* CreateItemCopy();

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(ABasePlayerCharacter* Charactor);

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UFUNCTION(BlueprintCallable, Category = "Item Data")
	FItemData GetItemData() const;


protected :
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}

};
