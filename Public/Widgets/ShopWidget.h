// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStructs/FItem.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
class ALobbyHUD;
class AWanderingTraderNonPlayer;
class ABasePlayerCharacter;
class UShopItemSlot;
class UMMInventoryComponent;
class UWrapBox;
class UButton;
class UVerticalBox;

UCLASS()
class ENDLESSSPIRE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_ShopWrapBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UShopItemSlot> ShopItemSlotClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VerticalBox_Buttons;

	FItemData SelectItem;

	void SetOwner(AWanderingTraderNonPlayer* Owner);
	void UpdateItems(TArray<FItemData>& Items);

protected:

	virtual void NativeOnInitialized();

	UPROPERTY()
	AWanderingTraderNonPlayer* WanderingTrader;

	UPROPERTY()
	ABasePlayerCharacter* Player;

	UPROPERTY()
	UMMInventoryComponent* InventoryReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Buy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Sell;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Exit;


private:

	UFUNCTION(BlueprintCallable)
	void BuyButtonCallback();

	UFUNCTION(BlueprintCallable)
	void SellButtonCallback();

	UFUNCTION(BlueprintCallable)
	void ExitButtonCallback();

	UFUNCTION(BlueprintCallable)
	FText GetSelectItemName();

	UPROPERTY()
	ALobbyHUD* LobbyHUD;

};
