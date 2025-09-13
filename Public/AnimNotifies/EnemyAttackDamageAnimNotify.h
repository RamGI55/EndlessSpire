// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnemyAttackDamageAnimNotify.generated.h"

/**
 * 
 */
class ABaseMonsterCharacter; 
UCLASS()
class ENDLESSSPIRE_API UEnemyAttackDamageAnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	void TryDamage(AActor* Target);
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDataIndex = 0;
	
	ABaseMonsterCharacter* Character;
	TArray<AActor*> DamagedActors;
	
};
