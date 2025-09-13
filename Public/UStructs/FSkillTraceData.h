// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSkillTraceData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class ESkillTraceType : uint8
{
	None = 0,
	Line, 
	Box, 
	Capsule, 
	Sphere
};

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FSkillTraceData
{
	GENERATED_BODY()

public:
	FSkillTraceData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	bool bCanParry = false; // 패링 되는 공격인지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	bool bUseCharacterWeaponCollision = false; // 무기 콜리전을 사용할 지 커스텀 범위 사용할 지


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	ESkillTraceType TraceType = ESkillTraceType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	FVector TraceDirection; // 캐릭터 기준 트레이스 방향 (단위 벡터일 것)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	FVector TraceBeginOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	bool bIsTraceDistanceAffectedByStat = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	float TraceDistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	bool bIsTraceSizeAffectedByStat = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	FVector BoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	float Radius = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	float Height = 1.0f;


};
