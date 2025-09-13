// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BasePlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "MagePlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMagePlayerAnimInstance : public UBasePlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float YawDelta;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Roll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Yaw;

private:
	FRotator RotationLastTick;



};
