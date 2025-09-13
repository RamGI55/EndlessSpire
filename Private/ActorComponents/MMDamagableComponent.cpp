// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/MMDamagableComponent.h"
#include "ActorComponents/MMStatComponent.h"
#include "UObjects/BurnStatusEffect.h"
#include "UObjects/FreezingStatusEffect.h"
#include "UObjects/StunStatusEffect.h"
#include "UObjects/StatusEffect.h"
#include "UStructs/FStatusEffectData.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Miscellaneous/HitReactionLegacyCameraShake.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// 최대 데미지 전역 상수
const float UMMDamagableComponent::MaxDamage = 1000000.0f;

// Sets default values for this component's properties
UMMDamagableComponent::UMMDamagableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CameraShakeAnim = UHitReactionLegacyCameraShake::StaticClass();
	// ...
}


// Called when the game starts
void UMMDamagableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UMMDamagableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<TSubclassOf<UStatusEffect>> deleteFlags;

	// 상태이상 tick 처리 및 exit 판단
	for (auto& elem : AppliedStatusEffects) {
		elem.Value->OnTickStatus(DeltaTime);
		if (elem.Value->IsEffectEnd()) {
			deleteFlags.Add(elem.Key);
		}
	}
	// exit로 판단된 상태이상은 맵에서 제거
	for (TSubclassOf<UStatusEffect> flag : deleteFlags) {
		AppliedStatusEffects[flag]->OnExitStatus();
		AppliedStatusEffects.Remove(flag);
	}

	// ...
}

void UMMDamagableComponent::TakeDamage(AActor* Subject, UMMStatComponent* AttackerStat, const FDamageData& SkillDamageData, FVector DamageDirection)
{
	// 클라일 경우 실행 X
	if (!GetOwner()->HasAuthority()) {
		return;
	}

	// 공격 주체 null 일 경우 리턴
	if (!Subject) return;

	// 무적 판정
	if (bIsInvincible) return;

	// 싸우는 중
	if (Cast<ABasePlayerCharacter>(GetOwner())) {
		Cast<ABasePlayerCharacter>(GetOwner())->BroadcastEnterFight();
	}

	// 퍼펙트 닷지
	if (bIsEvading) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Evade eval"));
		ABaseCombatCharacter* CombatCharacter = Cast<ABaseCombatCharacter>(GetOwner());
		if (CombatCharacter) {
			CombatCharacter->BroadcastCancleCurrentNPlayMontage(CombatCharacter->GetPerfectDodgeMontage());
			return;
		}
	}

	// Equipment 추가되면 여기에 장비 스텟 처리도 추가 예정

	// 기본 피격 방향
	if (!DamageDirection.Equals(FVector(), 0.05)) {
		DamageDirection = (GetOwner()->GetActorLocation() - Subject->GetActorLocation());
		DamageDirection.Normalize();
	}

	UMMStatComponent* TempStatComponent = TryGetStatComponent();

	float Damage;
	if (AttackerStat) {
		Damage = SkillDamageData.FixedDamage + AttackerStat->GetStat("Atk") * SkillDamageData.DamageCoefficient;

		UE_LOG(LogTemp, Log, TEXT("Damage1 : %f"), Damage);

		// Damage *= 데미지 퍼센트
		float SkillDamage = AttackerStat->GetStat("SkillDamage");
		Damage *= SkillDamage > 0.0f ? SkillDamage : 1.0f;


		UE_LOG(LogTemp, Log, TEXT("Damage2 : %f"), Damage);

		// 치명 터졌을 때
		if (SkillDamageData.bCanCritical && FMath::FRandRange(0.0f, 1.0f) < AttackerStat->GetStat("CriticalChance")) {
			Damage *= AttackerStat->GetStat("CriticalHitDamage");
		}

		UE_LOG(LogTemp, Log, TEXT("Damage3 : %f"), Damage);

		// 오버플로우 처리
		if (!FMath::IsFinite(Damage)) {
			Damage = MaxDamage;
		}
	}
	else {
		Damage = SkillDamageData.FixedDamage;
	}
	

	// 방어력 적용 (빼는 거? or 따로 % 만큼 계산??)
	if (TempStatComponent) {
		Damage -= TempStatComponent->GetStat("Def");
	}
	UE_LOG(LogTemp, Log, TEXT("Damage4 : %f"), Damage);

	// 언더플로우 처리
	if (!FMath::IsFinite(Damage)) {
		Damage = 0.0f;
	}

	// 데미지 타입마다 처리


	// Damage 예외 처리
	if (Damage < 0.0f) {
		Damage = 0.0f;
	}
	if (Damage > MaxDamage) {
		Damage = MaxDamage;
	}

	// hp에 데미지 (또는 힐) 적용
	if (TempStatComponent) {
		if (SkillDamageData.DamageType & StaticCast<uint8>(ESkillDamageType::Heal)) {
			TempStatComponent->HealHP(SkillDamageData.FixedDamage);
			LastDamage = -Damage;
		}
		else {
			TempStatComponent->DamagedHp(-Damage);
			LastDamage = Damage;

			// 경직 및 넉백 처리, 화면 흔들림, 역경직 이펙트, 사운드 플레이 등 액션성 처리하는 부분
			HandleDamageReaction(Subject, SkillDamageData.DamageReactionData, DamageDirection);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Damage : %f"), Damage);

	// 상태이상 처리
	for (const FStatusEffectData& effect : SkillDamageData.StatusEffectDatas) {
		ApplyStatusEffect(effect.StatusEffectType, effect.EffectProbability, effect.EffectDuration);
	}

}

void UMMDamagableComponent::HandleDamageReaction(AActor* Subject, const FDamageReactionData& DamageReactionData, const FVector& DamageDirection)
{
	ABaseCharacter* DamagedCharacter = Cast<ABaseCharacter>(GetOwner());
	ABaseCharacter* Attacker = Cast<ABaseCharacter>(Subject);

	// 피격자
	if (DamagedCharacter) {
		// 피격 정보 세팅
		CurrentDamageReactionData = DamageReactionData;
		CurrentDamageDirection = DamageDirection;

		if (DamageReactionData.StiffType == EStiffType::Down) {
			// 다운 공격에 피격됨
			DamagedCharacter->BroadcastCancleCurrentNPlayMontage(KnockdownMontage);
			CurrentStiffMontage = KnockdownMontage;
		}
		else {
			if (bIsSuperArmorState) { // 슈퍼아머의 경우 다운 제외 역경직만 있음
				DamagedCharacter->BroadcastDamagedReaction(DamageReactionData.DamageIntense);

			}
			else { // 일반적인 경우
				switch (DamageReactionData.StiffType)
				{
				case EStiffType::None:
					DamagedCharacter->BroadcastDamagedReaction(DamageReactionData.DamageIntense);
					break;
				case EStiffType::NormalStiff:
					PlayStiffMontage(DamagedCharacter, false);
					break;
				case EStiffType::StrongStiff:
					PlayStiffMontage(DamagedCharacter, true);
					break;
				default:
					break;
				}
			}
		}
	}

	// 공격자
	if (Attacker) {
		// 역경직
		Attacker->BroadcastAttackReaction(DamageReactionData.DamageIntense);
		// 공격자가 플레이어인 경우 화면 흔들림
		ApplyScreenShaking(Attacker, DamageReactionData.DamageIntense);
	}
}

float UMMDamagableComponent::HandleTypeOfDamage(AActor* Subject, uint8 DamageType, float Damage)
{
	/*if (DamageType & StaticCast<uint8>(ESkillDamageType::Physical)) {
		
	}
	if (DamageType & StaticCast<uint8>(ESkillDamageType::Magical)) {

	}
	if (DamageType & StaticCast<uint8>(ESkillDamageType::Fire)) {

	}
	if (DamageType & StaticCast<uint8>(ESkillDamageType::Ice)) {

	}*/
	return 0;
}

UMMStatComponent* UMMDamagableComponent::TryGetStatComponent()
{
	if (!StatComponent)
	{
		StatComponent = GetOwner()->FindComponentByClass<UMMStatComponent>();
	}

	return StatComponent;
}

bool UMMDamagableComponent::IsInvincible() const
{
	return bIsInvincible;
}

bool UMMDamagableComponent::IsInSuperArmorState() const
{
	return bIsSuperArmorState;
}

void UMMDamagableComponent::SetInvincible(bool value)
{
	bIsInvincible = value;
}

void UMMDamagableComponent::SetSuperArmorState(bool value)
{
	bIsSuperArmorState = value;
}

void UMMDamagableComponent::ClearNSetTimerReleaseStiff(float InRate, bool InbLoop, float InFirstDelay)
{
	if (!GetOwner()) return;

	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(StiffTimerHandle);
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(StiffTimerHandle, this,
		&UMMDamagableComponent::ReleaseStiff, InRate, InbLoop, InFirstDelay);
}

void UMMDamagableComponent::ReleaseStiff()
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter) {
		OwnerCharacter->BroadcastResumeMontage(CurrentStiffMontage);
	}
}

const FDamageReactionData& UMMDamagableComponent::GetCurrentDamageReactionData() const
{
	return CurrentDamageReactionData;
}

const FVector& UMMDamagableComponent::GetCurrentDamageDirection() const
{
	return CurrentDamageDirection;
}

void UMMDamagableComponent::ApplyStatusEffect(const TSubclassOf<class UStatusEffect>& Effect, float Probability, float duration)
{
	// 나중엔 상태이상 저항까지 고려

	if (FMath::FRand() * 100.0f <= Probability) { // 확률 처리
		if (AppliedStatusEffects.Contains(Effect)) { // 이미 적용된 상태이상일 경우
			AppliedStatusEffects[Effect]->OnEnterStatus(false, duration);
		}
		else {
			// 여기 할당을 풀링으로 바꾸자
			AppliedStatusEffects[Effect] = NewObject<UStatusEffect>(GetOwner()->GetOuter(), Effect);
			AppliedStatusEffects[Effect]->EffectOwner = GetOwner();
			AppliedStatusEffects[Effect]->OnEnterStatus(true, duration);
		}
	}
}

void UMMDamagableComponent::ApplyScreenShaking(ABaseCharacter* SubjectCharacter, float Damage)
{
	APlayerController* SubjectController = Cast<APlayerController>(SubjectCharacter->GetController());
	if (SubjectController) {
		float ShakeIntense = FMath::Clamp(Damage, 100.0f, 5000.0f); // 매직넘버 바꾸기. 최소랑 최대
		ShakeIntense = FMath::Lerp(0.75f, 5.5f, (ShakeIntense - 100.0f) / 4900.0f);
		SubjectController->ClientStartCameraShake(CameraShakeAnim, ShakeIntense);

	}
}

void UMMDamagableComponent::PlayStiffMontage(ABaseCharacter* Character, bool bIsStrongAttack)
{
	bool RotateFlag = false;
	FRotator Rotation;
	FName SectionName = FName("Default");
	const FVector& DamageDirection = -CurrentDamageDirection;
	if (bIsStrongAttack) {
		if (abs(DamageDirection.X) > abs(DamageDirection.Y)) { // 정면 혹은 후면
			if (DamageDirection.X < 0.0f) { // 정면
				RotateFlag = true;
				CurrentStiffMontage = StrongFrontStiffMontage;
			}
			else { // 후면
				if (StrongBackStiffMontage) {
					CurrentStiffMontage = StrongBackStiffMontage;
				}
				else {// 혹시 후방 피격 애니메이션이 없다면 정면으로 대체
					RotateFlag = true;
					CurrentStiffMontage = StrongFrontStiffMontage;
				}
			}
		}
		else { // 측면
			if (StrongSideStiffMontage) {
				if (DamageDirection.Y > 0.0f) { // 좌측
					CurrentStiffMontage = StrongSideStiffMontage;
					SectionName = FName("Left");
				}
				else { // 우측
					CurrentStiffMontage = StrongSideStiffMontage;
					SectionName = FName("Right");
				}
			}
			else { // 혹시 측면 피격 애니메이션이 없다면 정면으로 대체
				RotateFlag = true;
				CurrentStiffMontage = StrongFrontStiffMontage;
			}
		}
	}
	else {
		if (abs(DamageDirection.X) > abs(DamageDirection.Y)) { // 정면 혹은 후면
			if (DamageDirection.X < 0.0f) { // 정면
				RotateFlag = true;
				CurrentStiffMontage = NormalFrontStiffMontage;
			}
			else { // 후면
				if (NormalBackStiffMontage) {
					CurrentStiffMontage = NormalBackStiffMontage;
				}
				else { // 혹시 후방 피격 애니메이션이 없다면 정면으로 대체
					RotateFlag = true;
					CurrentStiffMontage = NormalFrontStiffMontage;
				}
			}
		}
		else { // 측면
			if (NormalSideStiffMontage) {
				if (DamageDirection.Y > 0.0f) { // 좌측
					CurrentStiffMontage = NormalSideStiffMontage;
					SectionName = FName("Left");
				}
				else { // 우측
					CurrentStiffMontage = NormalSideStiffMontage;
					SectionName = FName("Right");
				}
			}
			else { // 혹시 측면 피격 애니메이션이 없다면 정면으로 대체
				RotateFlag = true;
				CurrentStiffMontage = NormalFrontStiffMontage;
			}
		}
	}

	if (Cast<ABaseCombatCharacter>(Character)) {
		Cast<ABaseCombatCharacter>(Character)->SetCurrentAnimatedSkill(nullptr);
	}

	if (RotateFlag) {
		Rotation = UKismetMathLibrary::FindLookAtRotation(
			Character->GetActorLocation(), Character->GetActorLocation() - CurrentDamageDirection);

		Character->BroadcastRotateNPlayMontage(CurrentStiffMontage, SectionName, Rotation);
	}
	else {
		Character->BroadcastCancleCurrentNPlayMontage(CurrentStiffMontage, SectionName);
	}
}

