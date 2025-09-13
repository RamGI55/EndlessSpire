// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskPlayAnimation.h"
#include "HeadMountedDisplayTypes.h"
#include "Actors/Characters/NonPlayers/BaseNonPlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "Navigation/PathFollowingComponent.h"
#include "Actors/Characters/BaseCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskPlayAnimation::UBTTaskPlayAnimation():IsPlaying(false)
{
	NodeName = TEXT("Multicast the Montage");
	bCreateNodeInstance = true;
	UE_LOG(LogTemp, Warning, TEXT("BTTaskPlayAnimation constructor called"));
}

EBTNodeResult::Type UBTTaskPlayAnimation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* Controller = Comp.GetAIOwner();
	UE_LOG(LogTemp, Warning, TEXT("ExecuteTask called for %s"), *GetNodeName());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}
	ABaseCharacter* Character = Cast<ABaseCharacter>(Controller->GetPawn());
	if (!Character|| !AnimMontage)
	{
		return EBTNodeResult::Failed;
	}
	MyComp = &Comp;
	MyCharacter = Character; // ?? 
	if (Character->HasAuthority())
	{
		Character->ServerCastPlayMontage(AnimMontage);
	}
	
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance()) // TODO::Animinstance must be changed to the data from the data table.
		// TODO :: GetPawn -> GetSkill with index -> Datatable.
	{
		IsPlaying = true;
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageEnded);
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageInterrupted);
		
		AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTaskPlayAnimation::OnMontageEnded);
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UBTTaskPlayAnimation::OnMontageInterrupted);
		
		UE_LOG(LogTemp, Warning, TEXT("Attempting to play montage: %s"), *AnimMontage->GetName());

		return EBTNodeResult::InProgress; // animation is interrupeted when the enemy get hit. 
	}
	return EBTNodeResult::Failed; 
}

void UBTTaskPlayAnimation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	if (MyCharacter)
	{
		if (UAnimInstance* AnimInstance = MyCharacter->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageEnded);
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageInterrupted);
		}
	}
	// Reset references 
	MyComp = nullptr;
	MyCharacter = nullptr;
	IsPlaying = false; 
}

void UBTTaskPlayAnimation::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimMontage && MyComp)
	{
		if (MyCharacter)
		{
			if (UAnimInstance* AnimInstance = MyCharacter->GetMesh()->GetAnimInstance())
			{
				AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageEnded);

				AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageInterrupted);
			}
		}
		FinishLatentTask(*MyComp, EBTNodeResult::Succeeded);
	}
}

void UBTTaskPlayAnimation::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimMontage && MyComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage %s finished. Interrupted: %s"),
			   *Montage->GetName(),
			   bInterrupted ? TEXT("YES") : TEXT("NO"));

		// Restart BT only if it was interrupted
		if (bInterrupted && IsPlaying)
		{
			IsPlaying = false;
			if (AAIController* AIController = MyComp->GetAIOwner())
			{
				if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
				{
					if (UBlackboardComponent* BlackboardComponent = BTComp->GetBlackboardComponent())
					{
						// For example, set an "AnimationCooldown" boolean to true
						// BlackboardComp->SetValueAsBool(FName("AnimationCooldown"), true);
					}
					// Instead of immediately restarting, consider just finishing the task
					// BTComp->RestartTree();
				}
				
			}
		}
		if (MyCharacter)
		{
			if (UAnimInstance* AnimInstance = MyCharacter->GetMesh()->GetAnimInstance())
			{
				AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageEnded);
				AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &UBTTaskPlayAnimation::OnMontageInterrupted);
			}
		}
		FinishLatentTask(*MyComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
	}
}

