
// Generate Date : Mar. 26, 2025		//
// Author : Hyunsoo Park				//
// Desc :  플레이어를 감지하는 공용 AI BT 서비스 클레스. //

#include "AI/BTService/BTServiceDetect.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTServiceDetect::UBTServiceDetect()
{
	bNotifyTick = true;
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTServiceDetect::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetPlayer"));

		if (TargetActor)
		{
			UE_LOG(LogTemp, Display, TEXT("Detect: TargetPlayer - %s"), *TargetActor->GetName());
		}
	}
}

void UBTServiceDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetPlayer"));
		if (TargetActor&&!IsValid(TargetActor))
		{
			Blackboard->SetValueAsObject("TargetPlayer", nullptr); 
		}
	}
	
}
