// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "ActorComponents/MMStatComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "Kismet/KismetSystemLibrary.h"

UEnemyBaseSkill::UEnemyBaseSkill()
{
	ConstructorHelpers::FObjectFinder<UDataTable>TempSkillDataTable(TEXT("/Script/Engine.DataTable'/Game/Data/DataTable/DT_EnemySkills.DT_EnemySkills'"));
	SkillDataTable = TempSkillDataTable.Object;
	
}

void UEnemyBaseSkill::InitializeSkill(ABaseMonsterCharacter* InOwnerCharacter, int InSkillID)
{
	this->OwnerCharacter = InOwnerCharacter;
	this->SkillID = InSkillID;
}

void UEnemyBaseSkill::OnBeginExecute()
{
	
}

void UEnemyBaseSkill::OnHoldExecute(float DeltaTime)
{

}

void UEnemyBaseSkill::OnFinishExcute()
{

	
}

void UEnemyBaseSkill::GetOverlappingActorsToDamage(TArray<AActor*>& OutOverlappingActors, int AttackDataIndex, const FVector& TraceStart)
{
	if (!OwnerCharacter) return;
	if (!OwnerCharacter->HasAuthority()) return;

	OutOverlappingActors.Empty();
	if (SkillData.AttackDatas.IsValidIndex(AttackDataIndex)) {
		if (SkillData.AttackDatas[AttackDataIndex].TraceData.bUseCharacterWeaponCollision) { // ���� �ݸ��� ����� ��
			if (OwnerCharacter) OwnerCharacter->GetWeaponOverlappingActors(OutOverlappingActors);
		}
		else { // Ʈ���̽� ����� ��
			TArray<FHitResult> Hits;
			TArray<AActor*> Ignores;
			Ignores.Add(OwnerCharacter);
			ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(SkillData.AttackDatas[AttackDataIndex].TraceData.TraceChannel);

			UMMStatComponent* TempStatComponent = OwnerCharacter->TryGetStatComponent();

			// Trace Distance ���
			FVector TraceEnd;
			TraceEnd = TraceStart +
				(SkillData.AttackDatas[AttackDataIndex].TraceData
					.TraceDirection.RotateAngleAxis(OwnerCharacter->GetControlRotation().Euler().Z, FVector::UpVector)
					* SkillData.AttackDatas[AttackDataIndex].TraceData.TraceDistance);

			// ��ų ���� �޴� ���� ����
			FVector HalfSize = SkillData.AttackDatas[AttackDataIndex].TraceData.BoxExtent * 0.5f;
			float Radius = SkillData.AttackDatas[AttackDataIndex].TraceData.Radius;;
			float HalfHeight = SkillData.AttackDatas[AttackDataIndex].TraceData.Height * 0.5f;


			// Ʈ���̽� Ÿ�Կ� ���� Ʈ���̽�
			switch (SkillData.AttackDatas[AttackDataIndex].TraceData.TraceType)
			{
			case ESkillTraceType::Line: // Line Trace
				UKismetSystemLibrary::LineTraceMulti(OwnerCharacter->GetWorld(), TraceStart, TraceEnd, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Box: // Box Trace
				UKismetSystemLibrary::BoxTraceMulti(OwnerCharacter->GetWorld(), TraceStart, TraceEnd,
					HalfSize, FRotator(0.0f, OwnerCharacter->GetControlRotation().Euler().Z, 0.0f),
					TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Capsule: // Capsule Trace
				UKismetSystemLibrary::CapsuleTraceMulti(OwnerCharacter->GetWorld(), TraceStart, TraceEnd,
					Radius, HalfHeight, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Sphere: // Sphere Trace
				UKismetSystemLibrary::SphereTraceMulti(OwnerCharacter->GetWorld(), TraceStart, TraceEnd,
					Radius, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			default: // None �� ����
				UE_LOG(LogTemp, Log, TEXT("Please Check SkillTraceType. Is TraceType None?"));
				break;
			}

			for (const FHitResult& hit : Hits) { // Ʈ���̽� ��� �ƿ�ǲ�� �߰�
				OutOverlappingActors.Add(hit.GetActor());
				
			}
		}
	}
}

FSkill UEnemyBaseSkill::GetSkillData() const
{
	return SkillData;
}


void UEnemyBaseSkill::PlaySkillMontageWithServer(UAnimMontage* Montage, FName SectionName)
{
	if (OwnerCharacter->CanSwitchAnimation()) {
		OwnerCharacter->ServerPlaySkillMontage(Montage, SectionName, SkillID);
	}
}
