// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskFocusTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTaskFocusTarget::UBTTaskFocusTarget()
{
	NodeName = TEXT("Set Focus on Target");
	bCreateNodeInstance = true;

	TargetPlayerKey.SelectedKeyName = FName("TargetPlayer"); 
	TargetLockedKey.SelectedKeyName = FName("isTargetLocked");
}

EBTNodeResult::Type UBTTaskFocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	UE_LOG (LogTemp, Warning, TEXT("UBTTaskFocus ExecuteTask Called"));
	AAIController * AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("No AI Controller Found"));
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Blackboard Component"));
		return EBTNodeResult::Failed;
	}
	
	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetPlayerKey.SelectedKeyName);
	// UE_LOG(LogTemp, Warning, TEXT("ExecuteTask: Raw TargetPlayer in Blackboard: %s"), *GetNameSafe(TargetObject));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	
	if (!TargetActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("No Vaild Target Actor Found in Blackboard" ));
		return EBTNodeResult::Failed;
	}
	
	AIController->SetFocus(TargetActor);
	// UE_LOG(LogTemp, Warning, TEXT("Set Focus On Target : %s "), *TargetActor->GetName());
	BlackboardComp->SetValueAsBool("isTargetLocked", true);
	return EBTNodeResult::Succeeded;
}

void UBTTaskFocusTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	// UE_LOG(LogTemp, Warning, TEXT("BTTaskSetFocus task finished with result: %d"), (int32)TaskResult);
}

