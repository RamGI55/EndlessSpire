// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 *
 */
class ABasePlayerCharacter;
struct FInteractableData;
class UTextBlock;
class UProgressBar;

UCLASS()
class ENDLESSSPIRE_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public : 
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	ABasePlayerCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData) const;
protected :
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	UTextBlock* NameText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	UTextBlock* ActionText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	UTextBlock* QuantityText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	UTextBlock* KeyPressText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	UProgressBar* InteractionProgressBar;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction")
	float CurrentInteractableDuration;

	UFUNCTION(Category = "Interaction WIdget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

};
