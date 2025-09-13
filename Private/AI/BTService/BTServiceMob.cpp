// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTServiceMob.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTServiceMob::UBTServiceMob()
{
	NodeName = "Update Distance from the Player"; 
}

void UBTServiceMob::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AActor* SelfActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	AActor* TargetPlayer = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));

	if (SelfActor && TargetPlayer)
	{
		float Distance = FVector::Dist(SelfActor->GetActorLocation(), TargetPlayer->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(fDistanceToPlayer.SelectedKeyName, Distance);
	}
}
