// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/BaseCharacter.h"
#include "ActorComponents/MMStatComponent.h"
#include "ActorComponents/MMDamagableComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementReplication.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CountTimeToCanclePause(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::OnBeginAnimation()
{
	// �ִϸ��̼� ��Ÿ���� �� ó�� ���ۺκп��� �� �ϵ�. �������� ����
}


void ABaseCharacter::PauseMontageByDamage(float Damage)
{
	UAnimInstance* TempAnimInstance = GetMesh()->GetAnimInstance();
	// �̹� �������� �ִϸ��̼��� ���� ��� ���� ����
	if (HitStiffAnim) {
		TempAnimInstance->Montage_Resume(HitStiffAnim);
	}
	HitStiffAnim = TempAnimInstance->GetCurrentActiveMontage();

	// ���� ������� �ִϸ��̼� ����
	if (TempAnimInstance->IsAnyMontagePlaying()) {
		TempAnimInstance->Montage_Pause(HitStiffAnim);
		
		HitStiffTimer = 0.0f;
		bHitStiff = true;

		// �������� ���� N�� �� Ÿ�� ����(������) ����
		// �����ѹ� �ٲٱ�. DT���� ��������?
		HitStiffTime = FMath::Clamp(Damage, 100.0f, 5000.0f); // �����ѹ� �ٲٱ�. �ּҶ� �ִ�
		HitStiffTime = FMath::Lerp(0.05f, 0.3f, (HitStiffTime - 100.0f) / 4900.0f);
	}
}
void ABaseCharacter::CountTimeToCanclePause(float DeltaTime)
{
	if (!bHitStiff) return;

	if (HitStiffTimer >= HitStiffTime) {
		GetMesh()->GetAnimInstance()->Montage_Resume(HitStiffAnim);

		HitStiffAnim = nullptr;
		bHitStiff = false;
		HitStiffTimer = 0.0f;
		HitStiffTime = 0.0f;
	}

	HitStiffTimer += DeltaTime;
}

void ABaseCharacter::DeadEvent()
{
	// 사망, 피격등의 상태 처리는 델리게이트로도 충분히 가능. 현재 직접적인 커플링으로 애니메이션을 동작하는중. 
	ServerCastPlayMontageImmediately(DeathMontage, "Default");

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst && DeathMontage)
	{
		float MontageDuration = DeathMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(DeathMontageTimerHandle, this, &ABaseCharacter::DestroySelf, MontageDuration, false);
	}
	else
	{
		
		Destroy();
	}

}
void ABaseCharacter::DestroySelf()
{
	Destroy();
}

// Montage 클래스는 각각 따로 클래스를 만들것이 아니라 하나의 플레이 -> 레플리케이션 로직으로 통합해야함. 
void ABaseCharacter::ServerCastPlayMontage_Implementation(UAnimMontage* Montage, FName SectionName)
{
	BroadcastPlayMontage(Montage, SectionName);
}

void ABaseCharacter::ServerCastPlayMontageImmediately_Implementation(UAnimMontage* Montage, FName SectionName)
{
	BroadcastCancleCurrentNPlayMontage(Montage, SectionName);
}

void ABaseCharacter::ServerCastResumeMontage_Implementation(UAnimMontage* Montage)
{
	BroadcastResumeMontage(Montage);
}

void ABaseCharacter::BroadcastPlayMontage_Implementation(UAnimMontage* Montage, FName SectionName)
{
	PlayMontage(Montage, SectionName);
}

void ABaseCharacter::BroadcastCancleCurrentNPlayMontage_Implementation(UAnimMontage* Montage, FName SectionName)
{
	CancleCurrentNPlayMontage(Montage, SectionName);
}

void ABaseCharacter::BroadcastRotateNPlayMontage_Implementation(UAnimMontage* Montage, FName SectionName, FRotator Rotation)
{
	SetActorRotation(Rotation);
	CancleCurrentNPlayMontage(Montage, SectionName);
}

void ABaseCharacter::BroadcastResumeMontage_Implementation(UAnimMontage* Montage)
{
	if (!Montage) return;

	GetMesh()->GetAnimInstance()->Montage_Resume(Montage);
}

void ABaseCharacter::BroadcastDamagedReaction_Implementation(float Damage)
{
	PauseMontageByDamage(Damage);
}

void ABaseCharacter::BroadcastAttackReaction_Implementation(float Damage)
{
	PauseMontageByDamage(Damage);
}

void ABaseCharacter::SetCanSwitchAnimation(bool value)
{
	_bCanSwitchAnimation = value;
}

bool ABaseCharacter::CanSwitchAnimation() const
{
	return _bCanSwitchAnimation;
}

void ABaseCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	if (!Montage) return;

	UMMStatComponent* StatComp = TryGetStatComponent();

	if (StatComp) {
		PlayAnimMontage(Montage, StatComp->GetStat("AttackSpeed"), SectionName); // Speed 역시 맞는지 확인 필요. 1/60 해야 스피드가 나옴. 정확한 스피드가 아니라면 인덱스 값을 주는것도 좋음. 
	}
	else {
		PlayAnimMontage(Montage, 1.0f, SectionName);
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(Montage));
}

void ABaseCharacter::CancleCurrentNPlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* TempAnimInstance = GetMesh()->GetAnimInstance();

	if (TempAnimInstance->IsAnyMontagePlaying()) {
		TempAnimInstance->StopAllMontages(0.1f);
	}

	PlayMontage(Montage, SectionName);
}

UMMStatComponent* ABaseCharacter::TryGetStatComponent()
{
	if (!_StatComponent) {
		_StatComponent = FindComponentByClass<UMMStatComponent>();	// 좋은 예외처리. 
	}

	return _StatComponent;
}

UMMDamagableComponent* ABaseCharacter::TryGetDamagableComponent()
{
	if (!_DamagableComponent) {
		_DamagableComponent = FindComponentByClass<UMMDamagableComponent>();
	}
	return _DamagableComponent;
}

bool ABaseCharacter::IsInHitStiff() const
{
	return bHitStiff;
}
