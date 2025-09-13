// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskChangeTheSpeed.h"
#include"Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "AI/AIController/BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTTaskChangeTheSpeed::SetTheWalkSpeed()
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	if (!OwnerComp)
	{
		return; 
	}
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp->GetOwner());
	if (!AIController)
	{
		return; 
	}
	EnemyCharacter = Cast<ABaseMonsterCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
	{
		return;
	}
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 1650; // must change to the variable.
	
	
}
