// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasePlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBasePlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBasePlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation")
	float EvaluateOneDirectionMoveInput();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation")
	FVector2D EvaluateEightDirectionMoveInput();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsFighting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	FVector2D MoveInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	FVector2D PreMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float MoveLerpAlpha = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float BlendSpaceRunValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float BlendSpaceWalkValue = 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed = 0.0f;

};
