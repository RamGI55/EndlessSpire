
// Generate Date : Mar. 26, 2025		//
// Author : Hyunsoo Park				//
// Desc :  �÷��̾ �����ϴ� ���� AI BT ���� Ŭ����. //

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
