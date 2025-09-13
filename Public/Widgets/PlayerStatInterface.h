// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "UEnums/EClass.h"
#include "PlayerStatInterface.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UPlayerStatInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateWidget(FPlayerCharacterStat& playerstat);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentLevel;


protected:

	virtual void NativeOnInitialized();

};
