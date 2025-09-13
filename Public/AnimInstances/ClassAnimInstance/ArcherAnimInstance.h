// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BasePlayerAnimInstance.h"
#include "ArcherAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArcherAnimInstance : public UBasePlayerAnimInstance
{
	GENERATED_BODY()
	
public :
	UArcherAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	// 전투 모드용 에임 오프셋 블렌드스페이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* CombatAimOffset;

	// 일반 모드(Idle)용 에임 오프셋 블렌드스페이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* IdleAimOffset;

	// 카메라 yaw, pitch 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float AimPitch;

	// 전투 모드 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	bool bIsFightingOnAnim;

	// 조준 모드 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	bool bIsAimmingOnAnim;
};
