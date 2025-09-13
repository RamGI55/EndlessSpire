// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PlayerAnimInstance/MagePlayerAnimInstance.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "Actors/Characters/Players/MagePlayerCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


#include "GameFramework/CharacterMovementComponent.h"

void UMagePlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMagePlayerCharacter* Player = Cast<AMagePlayerCharacter>(TryGetPawnOwner());
	EvaluateOneDirectionMoveInput();
	EvaluateEightDirectionMoveInput();

	if (Player)
	{
		bIsAccelerating = Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

		//
		FRotator CurrentRotation = Player->GetActorRotation();
		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, RotationLastTick);

		double YawChangeRate = DeltaRotator.Yaw / DeltaSeconds;
		double ScaledYawDelta = YawChangeRate / 7.0;

		YawDelta = FMath::FInterpTo(YawDelta, ScaledYawDelta, DeltaSeconds, 6.0f);

		RotationLastTick = CurrentRotation;

		Roll = Player->Roll;
		Pitch = Player->Pitch;
		Yaw = Player->Yaw;

	}


}
