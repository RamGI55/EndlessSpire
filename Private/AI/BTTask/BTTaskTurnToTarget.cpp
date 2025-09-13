// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskTurnToTarget.h"
#include "AI/AIController/BaseAIController.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "AI/AIController/MobAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTaskTurnToTarget::UBTTaskTurnToTarget()
{
	NodeName=TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTaskTurnToTarget::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(Comp, NodeMemory);

	auto BasePlayerCharacter = Cast<ABasePlayerCharacter>(Comp.GetAIOwner()->GetPawn());
	if (nullptr == BasePlayerCharacter)
	{
		return EBTNodeResult::Failed;
	}
	auto Target = Cast<ABasePlayerCharacter>(Comp.GetBlackboardComponent()->GetValueAsObject(AMobAIController::TargetPlayerKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}
	FVector LookVector = Target->GetActorLocation() - BasePlayerCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); 
	BasePlayerCharacter->SetActorRotation(FMath::RInterpTo(BasePlayerCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), true));

	return EBTNodeResult::Succeeded;
}
