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
	// ���� ���� ���� ������ ���彺���̽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* CombatAimOffset;

	// �Ϲ� ���(Idle)�� ���� ������ ���彺���̽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* IdleAimOffset;

	// ī�޶� yaw, pitch ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float AimPitch;

	// ���� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	bool bIsFightingOnAnim;

	// ���� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	bool bIsAimmingOnAnim;
};
