// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Minimap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Stage;
};
