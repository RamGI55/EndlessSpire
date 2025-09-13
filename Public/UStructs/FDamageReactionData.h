// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDamageReactionData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EStiffType : uint8
{
	None = 0,
	NormalStiff,
	StrongStiff,
	Down
};

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FDamageReactionData
{
	GENERATED_BODY()
public:

	// ���� �� �˹�, �ٿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	EStiffType StiffType = EStiffType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	float DamageIntense = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	float StiffnessTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	bool bUsePhysicsForPushing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	// bUsePhysicsForPushing�� true�� force�� ���
	float PushingDistance = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageReactionData")
	// bUsePhysicsForPushing�� true�� force�� ���
	float RisingDistance = 0.0f;

};
