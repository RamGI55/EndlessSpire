// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/NPCAnimInstance/MonsterSkeletonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


UMonsterSkeletonAnimInstance::UMonsterSkeletonAnimInstance()
{
}

void UMonsterSkeletonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* PawnOwner = TryGetPawnOwner();
	if (!PawnOwner) return;

	if (!SkeletonMonster)
	{
		SkeletonMonster = Cast<ASkeletonMonsterCharacter>(TryGetPawnOwner());
	}
	
	if (!SkeletonMonster) return;


	fSpeed = SkeletonMonster->GetVelocity().Size();
	bIsFalling = SkeletonMonster->GetCharacterMovement()->IsFalling();


	const float MinMoveSpeed = 3.0f;
	//const bool bHasAcceleration = SkeletonMonster->GetCharacterMovement()->GetCurrentAcceleration().SizeSquared() > 0.0f;
	const bool bAboveThreshold = fSpeed > MinMoveSpeed;

	bShouldMove = bAboveThreshold;


}
