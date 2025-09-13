// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskMobMove.h"
#include "NavigationSystem.h"
#include "AI/AIController/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/MobAIController.h"


UBTTaskMobMove::UBTTaskMobMove()
{
	NodeName = "MoveRandomPosition"; 
}

EBTNodeResult::Type UBTTaskMobMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 적 AI 포지션 정보 초기화 - 현재 : 적 위치 기준으로 750
	// TODO:: change the radius 750 from spawn position. 
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = AIController->GetPawn();
	FVector PawnLocation = Pawn->GetActorLocation();

	if (!AIController || !Pawn)
	{
		return EBTNodeResult::Failed;
	}
	if (ControlledPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FNavLocation NextPatrol;
	
	if (NavSystem->GetRandomPointInNavigableRadius(PawnLocation, 750.f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPos", NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	
	
	return EBTNodeResult::Failed;
}

