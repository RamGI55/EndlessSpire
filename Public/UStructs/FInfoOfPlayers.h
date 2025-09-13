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
		: Guid(FGuid::NewGuid())           // ���ο� GUID ����
		, PlayerController(nullptr)        // �� �����ͷ� �ʱ�ȭ
		, Name("")                         // �� ���ڿ��� �ʱ�ȭ
		, Class(EClass::NONE)              // EClass �������� �⺻���� None���� ����
	{
	}

	FInfoOfPlayers(const FGuid& InGuid, APlayerController* InPlayerController)
		: Guid(InGuid)
		, PlayerController(InPlayerController)
		, Name("")           // �ʿ信 ���� �ٸ� �⺻�� ���� ����
		, Class(EClass::NONE) // EClass �������� �⺻�� ����
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
