// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEnums/EClass.h"
#include "FInfoOfPlayers.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Atomic)
struct FInfoOfPlayers
{
	GENERATED_USTRUCT_BODY()

public:

	FInfoOfPlayers()
		: Guid(FGuid::NewGuid())           // 새로운 GUID 생성
		, PlayerController(nullptr)        // 널 포인터로 초기화
		, Name("")                         // 빈 문자열로 초기화
		, Class(EClass::NONE)              // EClass 열거형의 기본값을 None으로 설정
	{
	}

	FInfoOfPlayers(const FGuid& InGuid, APlayerController* InPlayerController)
		: Guid(InGuid)
		, PlayerController(InPlayerController)
		, Name("")           // 필요에 따라 다른 기본값 지정 가능
		, Class(EClass::NONE) // EClass 열거형의 기본값 지정
	{
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FGuid Guid;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	FString Name = "";

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Stats")
	EClass Class = EClass::NONE;

};
