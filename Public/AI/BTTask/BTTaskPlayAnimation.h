// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskPlayAnimation.generated.h"


/**
 * 
 */
class ABaseMonsterCharacter;
class ABaseCharacter; 
UCLASS()
class ENDLESSSPIRE_API UBTTaskPlayAnimation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTaskPlayAnimation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	// need the function for intrupted situation. 

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (AllowPrivateAccess = true))
	FName MontageRowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage* AnimMontage; // depricated; must use the animmontage from the datatable.  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool IsPlaying;
	
	// 레퍼런스 저장용.
	UPROPERTY() 
	TObjectPtr<UBehaviorTreeComponent> MyComp;
	
	UPROPERTY()
	TObjectPtr<ABaseCharacter> MyCharacter;	 // for get the skill montage. Owner()->Character->skill->datatable. 
};
