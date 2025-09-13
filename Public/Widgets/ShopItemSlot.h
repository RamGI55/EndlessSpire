// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStructs/FItem.h"
#include "ShopItemSlot.generated.h"

/**
 * 
 */

class UBorder;
class UTextBlock;
class UImage;
class UShopWidget;

UCLASS()
class ENDLESSSPIRE_API UShopItemSlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetItemInfo(const FItemData& Element, UShopWidget* Parent);


protected:
	UShopWidget* ParentWidget;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* ItemPrice_Buy;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* ItemPrice_Sell;

	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

	FItemData Item;
	
};
