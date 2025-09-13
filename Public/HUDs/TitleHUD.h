// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/PlayHUD.h"
#include "TitleHUD.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ATitleHUD : public APlayHUD
{
	GENERATED_BODY()
	
public:
	ATitleHUD();

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ShowTitleWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	
	virtual void BeginPlay() override;

	// ���ø� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_GAME")
	TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY()
	UUserWidget* TitleWidget;
};
