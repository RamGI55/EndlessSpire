// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "UStructs/FSkillTraceData.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMStatComponent.h"

#include "Kismet/KismetSystemLibrary.h"

UMMBaseSkill::UMMBaseSkill()
{
	ConstructorHelpers::FObjectFinder<UDataTable> TempSkillDataTable(TEXT("/Script/Engine.DataTable'/Game/Data/DataTable/DT_PlayerSkills.DT_PlayerSkills'"));
	SkillDataTable = TempSkillDataTable.Object;
}

void UMMBaseSkill::InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID)
{
	this->Player = InPlayer;
	this->SkillID = InSkillID;
}

void UMMBaseSkill::OnBeginExecute()
{

}

void UMMBaseSkill::OnHoldExecute(float ElapsedTime)
{

}

void UMMBaseSkill::OnConfirmExecute()
{

}

void UMMBaseSkill::OnReleaseExcute()
{

}

void UMMBaseSkill::PrepareExecute()
{
	bIsExecutePrepared = true;
	Player->CancleOtherPreparedSkills(this);
}

void UMMBaseSkill::CanclePreparedExecute()
{
	bIsExecutePrepared = false;
}

void UMMBaseSkill::InitSkillMappingContext()
{

}

void UMMBaseSkill::DeleteSkillMappingContext()
{

}

void UMMBaseSkill::PlaySkillMontageWithServer(UAnimMontage* Montage, FName SectionName)
{
	// 클릭 할 때 마다 실행되는 경우도 있기 때문에 
	// 여기 로직을 서버에서 처리하면 패킷 양이 많아지고, 클라에서 처리하면 변조 가능...

	if (Player->CanSwitchAnimation()) {
		Player->ServerPlaySkillMontage(Montage, SectionName, SkillID);
	}
}

void UMMBaseSkill::PlayComboMontageWithServer(UAnimMontage* Montage)
{
	Player->ServerPlayComboMontage(Montage, SkillID);
}

void UMMBaseSkill::GetOverlappingActorsToDamage(TArray<AActor*>& OutOverlappingActors, int AttackDataIndex, const FVector& TraceStart)
{
	if (!Player) return;
	if (!Player->HasAuthority()) return;

	OutOverlappingActors.Empty();
	if (SkillData.AttackDatas.IsValidIndex(AttackDataIndex)) {
		if (SkillData.AttackDatas[AttackDataIndex].TraceData.bUseCharacterWeaponCollision) { // 무기 콜리전 사용할 때
			if (Player) Player->GetWeaponOverlappingActors(OutOverlappingActors);
		}
		else { // 트레이스 사용할 때
			TArray<FHitResult> Hits;
			TArray<AActor*> Ignores;
			Ignores.Add(Player);
			ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(SkillData.AttackDatas[AttackDataIndex].TraceData.TraceChannel);

			UMMStatComponent* TempStatComponent = Player->TryGetStatComponent();

			// Trace Distance 계산
			FVector TraceEnd;
			if (TempStatComponent) {
				TraceEnd = TraceStart +
					(SkillData.AttackDatas[AttackDataIndex].TraceData
						.TraceDirection.RotateAngleAxis(Player->GetControlRotation().Euler().Z, FVector::UpVector)
						* SkillData.AttackDatas[AttackDataIndex].TraceData.TraceDistance
						* (SkillData.AttackDatas[AttackDataIndex].TraceData.bIsTraceDistanceAffectedByStat ? TempStatComponent->GetStat("SkillRange") : 1.0f));
			}
			else {
				TraceEnd = TraceStart +
					(SkillData.AttackDatas[AttackDataIndex].TraceData
						.TraceDirection.RotateAngleAxis(Player->GetControlRotation().Euler().Z, FVector::UpVector)
						* SkillData.AttackDatas[AttackDataIndex].TraceData.TraceDistance);
			}

			// 스킬 영향 받는 범위 설정
			FVector HalfSize = SkillData.AttackDatas[AttackDataIndex].TraceData.BoxExtent * 0.5f;
			float Radius = SkillData.AttackDatas[AttackDataIndex].TraceData.Radius;;
			float HalfHeight = SkillData.AttackDatas[AttackDataIndex].TraceData.Height * 0.5f;

			if (TempStatComponent) {
				HalfSize *= SkillData.AttackDatas[AttackDataIndex].TraceData.bIsTraceDistanceAffectedByStat
					? TempStatComponent->GetStat("SkillRange") : 1.0f;
				Radius *= SkillData.AttackDatas[AttackDataIndex].TraceData.bIsTraceDistanceAffectedByStat
					? TempStatComponent->GetStat("SkillRange") : 1.0f;
				HalfHeight *= SkillData.AttackDatas[AttackDataIndex].TraceData.bIsTraceDistanceAffectedByStat
					? TempStatComponent->GetStat("SkillRange") : 1.0f;
			}

			// 트레이스 타입에 따른 트레이스
			switch (SkillData.AttackDatas[AttackDataIndex].TraceData.TraceType)
			{
			case ESkillTraceType::Line: // Line Trace
				UKismetSystemLibrary::LineTraceMulti(Player->GetWorld(), TraceStart, TraceEnd, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Box: // Box Trace
				UKismetSystemLibrary::BoxTraceMulti(Player->GetWorld(), TraceStart, TraceEnd, 
					HalfSize, FRotator(0.0f, Player->GetControlRotation().Euler().Z, 0.0f), 
					TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Capsule: // Capsule Trace
				UKismetSystemLibrary::CapsuleTraceMulti(Player->GetWorld(), TraceStart, TraceEnd, 
					Radius,	HalfHeight, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			case ESkillTraceType::Sphere: // Sphere Trace
				UKismetSystemLibrary::SphereTraceMulti(Player->GetWorld(), TraceStart, TraceEnd, 
					Radius, TraceChannel, false, Ignores, EDrawDebugTrace::ForOneFrame, Hits, true);
				break;

			default: // None 등 예외
				UE_LOG(LogTemp, Log, TEXT("Please Check SkillTraceType. Is TraceType None?"));
				break;
			}

			for (const FHitResult& hit : Hits) { // 트레이스 결과 아웃풋에 추가
				OutOverlappingActors.Add(hit.GetActor());
			}
		}
	}
}

bool UMMBaseSkill::IsExecutePrepared() const
{
	return bIsExecutePrepared;
}

FSkill UMMBaseSkill::GetSkillData() const
{
	return SkillData;
}

void UMMBaseSkill::InitComboIndex()
{
	_ComboIndex = 0;
}

void UMMBaseSkill::IncreaseComboIndex()
{
	++_ComboIndex;
}

int UMMBaseSkill::GetComboIndex() const
{
	return _ComboIndex;
}

void UMMBaseSkill::SetComboPassed(bool value)
{
	_bIsComboPassed = value;
}

bool UMMBaseSkill::IsComboPassed() const
{

	return _bIsComboPassed;
}

void UMMBaseSkill::ClearTimerHandle()
{
	GetWorld()->GetTimerManager().ClearTimer(CoolTimerHandle);
}

int UMMBaseSkill::GetSkillID() const
{
	return SkillID;
}