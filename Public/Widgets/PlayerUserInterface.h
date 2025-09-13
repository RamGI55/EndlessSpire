// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/PlayerStatInterface.h"
#include "Widgets/TeamStatsInterface.h"
#include "Widgets/PlayerStatusUserInterface.h"
#include "Widgets/MinimapWidget.h"
#include "Widgets/InventoryPanel.h"
#include "Widgets/EquipmentPanel.h"
#include "Widgets/CrosshairWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerUserInterface.generated.h"

/**
 * 
 */
class ABasePlayerCharacter;

UCLASS()
class ENDLESSSPIRE_API UPlayerUserInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerUserInterface(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidgetOptional))
	UTeamStatsInterface* TeamUI;

	UPROPERTY(meta = (BindWidgetOptional))
	UPlayerStatInterface* PlayerUI;

	UPROPERTY(meta = (BindWidgetOptional))
	UPlayerStatusUserInterface* StatusUI;

	UPROPERTY(meta = (BindWidgetOptional))
	UMinimapWidget* MinimapUI;

	UPROPERTY(meta = (BindWidgetOptional))
	UInventoryPanel* InventoryUI;

	UPROPERTY(meta = (BindWidgetOptional))
	UEquipmentPanel* EquipmentPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UCrosshairWidget* Crosshair;

	UPROPERTY()
	ABasePlayerCharacter* Player;


protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
