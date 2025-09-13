// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "EquipmentPanel.generated.h"

/**
 * 
 */
class ABasePlayerCharacter;
class UMMInventoryComponent;
class UEquipmentSlot;
class UTextBlock;
class UWrapBox;
class UImage;


UCLASS()
class ENDLESSSPIRE_API UEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UEquipmentPanel(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	void UpdateWidget(FPlayerCharacterStat newStatus);

	UPROPERTY()
	ABasePlayerCharacter* Player;
	
	UPROPERTY()
	UMMInventoryComponent* InventoryReference;

	UPROPERTY()
	TArray<UEquipmentSlot*> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, Category = "StatInformation", meta = (BindWidget))
	UTextBlock* AttackRateValue;

	UPROPERTY(VisibleAnywhere, Category = "StatInformation", meta = (BindWidget))
	UTextBlock* DefenseRateValue;

	UPROPERTY(VisibleAnywhere, Category = "StatInformation", meta = (BindWidget))
	UTextBlock* CriticalValue;

	UPROPERTY(VisibleAnywhere, Category = "StatInformation", meta = (BindWidget))
	UTextBlock* CriticalDamageValue;
};
