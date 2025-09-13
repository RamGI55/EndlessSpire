// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController/SkeletonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Characters/NonPlayers/SkeletonMonsterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"


ASkeletonAIController::ASkeletonAIController()
{

}

void ASkeletonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Log, TEXT("ParkJS_OnPossess : Success"));

	// 0.2초 딜레이 후 InitBehaviorTree 호출
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ASkeletonAIController::InitBehaviorTree, 0.2f, false);
}

void ASkeletonAIController::InitBehaviorTree()
{
	ASkeletonMonsterCharacter* SkeletonMonster = Cast<ASkeletonMonsterCharacter>(GetPawn());
	if (SkeletonMonster && SkeletonMonster->BehaviorTree)
	{
		RunBehaviorTree(SkeletonMonster->BehaviorTree);
		UE_LOG(LogTemp, Log, TEXT("ParkJS_BehaviorTree Run: Success"));

		if (UBlackboardComponent* BBComp = GetBlackboardComponent())
		{
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (Player)
			{
				BBComp->SetValueAsObject(AttackTargetKeyName, Player);
				UE_LOG(LogTemp, Log, TEXT("ParkJS_TargetPlayer Set: %s"), *Player->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ParkJS_Player is null!"));
			}
		}
	}
}

void ASkeletonAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ASkeletonAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);



}
