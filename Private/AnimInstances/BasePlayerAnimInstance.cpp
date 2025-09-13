// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BasePlayerAnimInstance.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"

UBasePlayerAnimInstance::UBasePlayerAnimInstance()
{

}

void UBasePlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(TryGetPawnOwner());

	if (Player) {
		bIsRunning = Player->IsRunning();
		bIsFighting = Player->IsFighting();
		bIsFalling = Player->GetCharacterMovement()->IsFalling();

		APlayPlayerController* PlayerController = Cast<APlayPlayerController>(Player->GetController());
		if (PlayerController) {
			MoveInput = PlayerController->GetMoveInput();
		}

		EvaluateOneDirectionMoveInput();
		EvaluateEightDirectionMoveInput();

	}

	
}

float UBasePlayerAnimInstance::EvaluateOneDirectionMoveInput()
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(TryGetPawnOwner());
	
	if (Player) {
		float NewSpeed = FVector2D(Player->GetCharacterMovement()->GetVelocityForRVOConsideration()).Length() / Player->GetCharacterMovement()->MaxWalkSpeed;
		NewSpeed *= bIsRunning ? BlendSpaceRunValue : BlendSpaceWalkValue; // Blend Space 값

		Speed = FMath::Lerp(Speed, NewSpeed, MoveLerpAlpha);
		return Speed;
	}
	return 0.0f;
}

FVector2D UBasePlayerAnimInstance::EvaluateEightDirectionMoveInput()
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(TryGetPawnOwner());

	if (Player) {
		// 이동방향 벡터 정규화
		FVector Move = Player->GetCharacterMovement()->GetVelocityForRVOConsideration() / Player->GetCharacterMovement()->MaxWalkSpeed;

		// 플레이어 바라보는 방향 기준 이동 방향
		Move = Move.RotateAngleAxis(-Player->GetControlRotation().Euler().Z, FVector::UpVector);

		FVector2D NewMove;
		NewMove.Y = FMath::Clamp(Move.X, -1.0, 1.0);
		NewMove.X = FMath::Clamp(Move.Y, -1.0, 1.0);

		NewMove *= (bIsRunning ? BlendSpaceRunValue : BlendSpaceWalkValue);

		PreMove = FMath::Lerp(PreMove, NewMove, MoveLerpAlpha);

		return PreMove;
	}
	return FVector2D();
}

