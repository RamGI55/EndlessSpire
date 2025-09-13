
// Generate Date : Mar. 26, 2025		//
// Author : Hyunsoo Park				//
// Desc :  플레이어를 감지하는 공용 AI BT 서비스 클레스. //

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceDetect.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBTServiceDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTServiceDetect();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKeyName = FName("TargetActor");
	
};
