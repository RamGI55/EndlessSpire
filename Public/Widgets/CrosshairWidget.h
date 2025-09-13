// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCrosshairWidget(const FObjectInitializer& ObjectInitializer);

 protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Crosshair;
	
};
