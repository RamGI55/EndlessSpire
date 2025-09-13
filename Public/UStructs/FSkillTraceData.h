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
	bool bCanParry = false; // �и� �Ǵ� ��������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	bool bUseCharacterWeaponCollision = false; // ���� �ݸ����� ����� �� Ŀ���� ���� ����� ��


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	ESkillTraceType TraceType = ESkillTraceType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTraceData")
	FVector TraceDirection; // ĳ���� ���� Ʈ���̽� ���� (���� ������ ��)
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
