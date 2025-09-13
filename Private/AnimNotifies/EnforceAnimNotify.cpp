// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/EnforceAnimNotify.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "TimerManager.h"

void UEnforceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;

	Character = Cast<ABaseCombatCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		for (const TPair<FName, float>& Element : ModifyStats)
		{
			Character->GetPlayerStatComponent()->ModifyStat(Element.Key, Element.Value);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, Element.Key.ToString() + " " + FString::SanitizeFloat(Element.Value));
		}

		if (UWorld* World = MeshComp->GetWorld())
		{
			// Duration이 0보다 큰 경우에만 타이머를 설정
			if (Duration > 0)
			{
				FTimerHandle TimerHandle;
				World->GetTimerManager().SetTimer(TimerHandle, this, &UEnforceAnimNotify::EndEnforce, Duration, false);
			}
		}
	}
}

void UEnforceAnimNotify::EndEnforce()
{
	for (const TPair<FName, float>& Element : ModifyStats)
	{
		Character->GetPlayerStatComponent()->ModifyStat(Element.Key, -Element.Value);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Element.Key.ToString() + " " + FString::SanitizeFloat(-Element.Value));
	}
}
