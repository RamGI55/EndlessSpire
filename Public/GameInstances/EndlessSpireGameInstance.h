// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "UStructs/FItem.h"
#include "UEnums/EClass.h"
#include "EndlessSpireGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UEndlessSpireGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UEndlessSpireGameInstance();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FGuid Guid;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FString Nickname;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	EClass Class;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FPlayerCharacterStat PlayerStat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	TArray<FItemData> PlayerItems;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	int32 PlayerMoney;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stage")
	int32 Stage;

};
