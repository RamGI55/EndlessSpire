// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/MonsterMobAnimInstance.h"
#include "Animation/AnimInstance.h"


UMonsterMobAnimInstance::UMonsterMobAnimInstance()
{
	
}

void UMonsterMobAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	MobMonster = Cast<AMobMonsterCharacter>(TryGetPawnOwner());
	if (MobMonster)
	{
		fSpeed = MobMonster->GetVelocity().Size();
	}

	
}
