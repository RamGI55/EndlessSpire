// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/WarriorPlayerCharacter.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "UObjects/PlayerSkills/WarriorBasicComboSkill.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "UObjects/PlayerSkills/Warrior/WarriorIdentitySkill.h"
#include "Actors/MMParticleActor.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "UObjects/PlayerSkills/Warrior/WarriorProjectileSkill.h"

#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AWarriorPlayerCharacter::AWarriorPlayerCharacter()
{
	// 캐릭터 Mesh 적용
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh 위치 변경
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -97.5), FRotator(0, -90, 0));

	}

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/Warrior/ABP_Warrior.ABP_Warrior_C'"));
	if (TempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}
}

void AWarriorPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) {
		IdentitySkillParticleActor = GetWorld()->SpawnActor<AMMParticleActor>();
		IdentitySkillParticleActor->GetParticleSystemComponent()->SetTemplate(IdentitySkillParticle);
		SetIdentitySkillEffectEnabled(false);
	}
}

void AWarriorPlayerCharacter::OnLeftMouseBeginClick()
{
	Super::OnLeftMouseBeginClick();
	BasicAttackSkill->OnBeginExecute();
}

void AWarriorPlayerCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnLeftMouseHolding(triggered, ElapsedTime);
}

void AWarriorPlayerCharacter::OnLeftMouseReleased(bool cancled)
{
	Super::OnLeftMouseReleased(cancled);
}

void AWarriorPlayerCharacter::OnRightMouseBeginClick()
{
	Super::OnRightMouseBeginClick();
}

void AWarriorPlayerCharacter::OnRightMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnRightMouseHolding(triggered, ElapsedTime);

}

void AWarriorPlayerCharacter::OnRightMouseReleased(bool cancled)
{
	Super::OnRightMouseReleased(cancled);

}

void AWarriorPlayerCharacter::OnEvadeBeginClick()
{
	// 여기서 입력으로 방향 판단해서 서버 캐스트로 해야 됨

	APlayPlayerController* PlayController = Cast<APlayPlayerController>(GetController());
	if (PlayController) {
		const FVector2D& MoveInput = PlayController->GetMoveInput();

		if (MoveInput.Y < 0.0f) { // 후방

			EvadeDirection = -GetActorForwardVector();
			ServerEvade(Dodge, FName("Back"), -GetActorForwardVector());
		}
		else {
			if (MoveInput.X > 0.0f) { // 좌
				EvadeDirection = GetActorRightVector();
				ServerEvade(Dodge, FName("Right"), EvadeDirection);
			}
			else if (MoveInput.X < 0.0f) { // 우
				EvadeDirection = -GetActorRightVector();
				ServerEvade(Dodge, FName("Left"), EvadeDirection);
			}
			else { // 기타 예외들

				EvadeDirection = -GetActorForwardVector();
				ServerEvade(Dodge, FName("Back"), -GetActorForwardVector());
			}
		}
	}
}

void AWarriorPlayerCharacter::OnEvadeReleased()
{

}

void AWarriorPlayerCharacter::MakeInitialSkills()
{
	if (!BasicAttackSkill) {
		BasicAttackSkill = NewObject<UWarriorBasicComboSkill>(this);
		SkillMap.Add(0, BasicAttackSkill);
	}
	BasicAttackSkill->InitializeSkill(this, 0);

	if (!Skill1) {
		Skill1 = NewObject<UWarriorProjectileSkill>(this);
		SkillMap.Add(1, Skill1);
	}
	Skill1->InitializeSkill(this, 1);

	if (!Skill2) {
		Skill2 = NewObject<UWarriorIdentitySkill>(this);
		SkillMap.Add(2, Skill2);
	}
	Skill2->InitializeSkill(this, 2);

	if (!Skill3) {
		Skill3 = NewObject<UWarriorIdentitySkill>(this);
		SkillMap.Add(3, Skill3);
	}
	Skill3->InitializeSkill(this, 3);

	if (!IdentitySkill) {
		IdentitySkill = NewObject<UWarriorIdentitySkill>(this);
		SkillMap.Add(3, IdentitySkill);
	}
	IdentitySkill->InitializeSkill(this, 3);
}

void AWarriorPlayerCharacter::ServerCastEndIDSkill_Implementation()
{
	BroadcastEndIDSkill();
}

void AWarriorPlayerCharacter::BroadcastEndIDSkill_Implementation()
{
	SetIdentitySkillEffectEnabled(false);
	SetActorHiddenInGame(false);
	GetDamagableComponent()->SetInvincible(false);
	IdentitySkill->DeleteSkillMappingContext();
}

void AWarriorPlayerCharacter::InterruptIDSkill()
{
	BroadcastInterruptIDSkill();
}

void AWarriorPlayerCharacter::ServerIDSkillEvade_Implementation(const FVector& Direction, float EvadeDashDistance)
{
	BroadcastSpawnSkillEvadeEffect();

	APlayPlayerController* PlayController = Cast<APlayPlayerController>(GetController());
	if (PlayController) {
		GetCharacterMovement()->MoveUpdatedComponent(
			Direction * EvadeDashDistance,
			GetControlRotation(), true);
	}
}

void AWarriorPlayerCharacter::BroadcastSpawnSkillEvadeEffect_Implementation()
{
	if (HasAuthority()) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IdentitySkillEvadeParticle, IdentitySkillParticleActor->GetActorLocation());
}

void AWarriorPlayerCharacter::BroadcastInterruptIDSkill_Implementation()
{
	// GetMesh()->GetAnimInstance()->Montage_Resume(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
	GetMesh()->GetAnimInstance()->StopAllMontages(0.05f);
	GetCameraBoom()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	GetCameraBoom()->SetRelativeLocation(FVector());
	SetIdentitySkillEffectEnabled(false);
	SetActorHiddenInGame(false);
	GetDamagableComponent()->SetInvincible(false);
	GetDamagableComponent()->SetSuperArmorState(false);
	IdentitySkill->DeleteSkillMappingContext();
	UE_LOG(LogTemp, Log, TEXT("============================================="));
}

void AWarriorPlayerCharacter::SetIdentitySkillEffectEnabled(bool bEnable)
{
	if (!IdentitySkillParticleActor) return;

	if (bEnable) {
		IdentitySkillParticleActor->FollowTarget = this;
		IdentitySkillParticleActor->FollowSocketName = FName("pelvis");
		IdentitySkillParticleActor->SetActorHiddenInGame(false);
		IdentitySkillParticleActor->GetParticleSystemComponent()->Activate();

	}
	else {
		IdentitySkillParticleActor->FollowTarget = nullptr;
		IdentitySkillParticleActor->FollowSocketName = FName("pelvis");
		IdentitySkillParticleActor->SetActorHiddenInGame(true);
		IdentitySkillParticleActor->GetParticleSystemComponent()->Deactivate();
	}
}

void AWarriorPlayerCharacter::InitIDSkillMappingContext()
{
	Cast<UWarriorIdentitySkill>(IdentitySkill)->InitIDSkillMappingContext();
}

void AWarriorPlayerCharacter::ServerEvade_Implementation(UAnimMontage* Montage, FName SectionName, FVector MoveDirection)
{
	if (CurrentAnimatedSkill) {
		if (CurrentAnimatedSkill != BasicAttackSkill && !CanSwitchAnimation()) {
			return;
		}
	}
	else {
		if (!CanSwitchAnimation()) return;
	}
	

	SetCurrentAnimatedSkill(nullptr);

	EvadeDirection = MoveDirection;

	BroadcastCancleCurrentNPlayMontage(Montage, SectionName);

}
