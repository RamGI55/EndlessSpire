// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/ArcherPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "Animinstances/ClassAnimInstance/ArcherAnimInstance.h"
#include "UObjects/PlayerSkills/Archer/ArcherBasicSkill.h"
#include "UObjects/PlayerSkills/Archer/ArcherBuff.h"
#include "UObjects/PlayerSkills/Archer/ArcherRainsPoem.h"
#include "UObjects/PlayerSkills/Archer/Archer_ArrowMeteor.h"
#include "Animation/AnimMontage.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "HUDs/LobbyHUD.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

AArcherPlayerCharacter::AArcherPlayerCharacter()
{
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/Raven/Meshes/Sparrow_Raven.Sparrow_Raven'"));
    GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	 //ĳ���� AnimInstance ����
	ConstructorHelpers::FClassFinder<UAnimInstance> ArcherAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/Archer/ABP_ArcherAnim.ABP_ArcherAnim_C'"));

	if (ArcherAnimInstance.Succeeded())
	{
		// �ִϸ��̼Ǹ�� ����
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		GetMesh()->SetAnimInstanceClass(ArcherAnimInstance.Class);
	}

	// ���� ������ �ּ� �ε�
	static ConstructorHelpers::FObjectFinder<UBlendSpace> CombatAimOffsetFinder(TEXT("/Script/Engine.AimOffsetBlendSpace'/Game/Animations/Archer/AO_idle.AO_idle'"));
	static ConstructorHelpers::FObjectFinder<UBlendSpace> IdleAimOffsetFinder(TEXT("/Script/Engine.AimOffsetBlendSpace'/Game/Animations/Archer/AO_TravelMode.AO_TravelMode'"));
	if (CombatAimOffsetFinder.Succeeded() && IdleAimOffsetFinder.Succeeded())
	{
		CombatAimOffsetAsset = CombatAimOffsetFinder.Object;
		IdleAimOffsetAsset = IdleAimOffsetFinder.Object;
	}

	// �߻� (Shot) ��Ÿ�� ����.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Archer/Montage/AM_Shot.AM_Shot'"));
	if (ShotMontageFinder.Succeeded())
	{
		ShotMontage = ShotMontageFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Archer/Roll/MT_Evade.MT_Evade'"));
	if (EvadeMontageFinder.Succeeded())
	{
		EvadeMontage = EvadeMontageFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Archer/Montage/AM_Death.AM_Death'"));
	if (DeadMontageFinder.Succeeded())
	{
		DeadMontage = DeadMontageFinder.Object;
	}
}

void AArcherPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

	// ���� ũ�ν���� Ȱ��ȭ
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PC->GetHUD());
		if (LobbyHUD)
		{
			LobbyHUD->PlayerUI->Crosshair->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT(" %d"), LobbyHUD->PlayerUI->Crosshair->GetVisibility());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" Cast<ALobbyHUD>(PC->GetHUD() is Failed"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast<APlayerController>(GetController()) is Failed"))
	}
}

void AArcherPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��Ʈ�ѷ��� ȸ����(ī�޶��� ȸ��)�� �����ɴϴ�.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FRotator ControlRot = PC->GetControlRotation();

		// �ʿ��� ���, ĳ���� ���� �������� ��ȯ
		FRotator LocalRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, GetActorRotation());

		if (UArcherAnimInstance* AnimInst = Cast<UArcherAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInst->AimYaw = LocalRot.Yaw;
			AnimInst->AimPitch = LocalRot.Pitch;
			AnimInst->bIsFightingOnAnim = bIsFighting;  // ���� ��� ���� ������Ʈ
			AnimInst->bIsAimmingOnAnim = bIsAimming;
		}
	}

}

void AArcherPlayerCharacter::OnLeftMouseBeginClick()
{
	if (bIsFighting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left Mouse Click"));
		bIsAimming = true;
		AimmingStart();
	}
	else
	{
		// �Ϲ� ��忡�� ���� ���� ��ȯ�ϴ� ������ ���Ե� ���� �Լ��� ȣ��
		Super::OnLeftMouseBeginClick();

		// ���� ���� ��ȯ�Ǿ��ٰ� �����ϰ� �÷��� ����
		if (bIsFighting) // ���� ȣ�� �� ���� ��� ���°� ����Ǿ��ٸ�
		{
			bCombatModeJustEntered = true;
		}
	}
}

void AArcherPlayerCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	if (bIsFighting)
	{
		// ���� ���� �� ��ȯ�Ǿ��� ���� �Է� ����
		if (bCombatModeJustEntered)
		{
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Left Mouse Holding"));
		Aimming();
	}
}

void AArcherPlayerCharacter::OnLeftMouseReleased(bool canceled)
{
	if (bIsFighting)
	{
		// ���� ���� ó�� ���� ��, Released �̺�Ʈ ���� �� �÷��� �ʱ�ȭ
		if (bCombatModeJustEntered)
		{
			bCombatModeJustEntered = false;
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Left Mouse Released"));
	AimmingEnd();
}

void AArcherPlayerCharacter::OnShotMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	// ��Ÿ�ְ� ������ �ƿ� �ܰ迡 �� �� ȣ��˴ϴ�.
	// ���⼭ ���� ��ȯ�̳� ���� �ִϸ��̼������� ��ȯ�� ����ϵ��� ������ ������ �� �ֽ��ϴ�.
	
	UE_LOG(LogTemp, Log, TEXT("Shot montage is blending out"));
}

void AArcherPlayerCharacter::OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	// ��Ÿ�ְ� ������ �ƿ� �ܰ迡 �� �� ȣ��˴ϴ�.
	// ���⼭ ���� ��ȯ�̳� ���� �ִϸ��̼������� ��ȯ�� ����ϵ��� ������ ������ �� �ֽ��ϴ�.
	GetDamagableComponent()->bIsInvincible = false;

	
	UE_LOG(LogTemp, Log, TEXT("Evade montage is blending out"));
}

void AArcherPlayerCharacter::AimmingStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Aimming Start"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bIsAimming = true;
}

void AArcherPlayerCharacter::Aimming()
{
	UE_LOG(LogTemp, Warning, TEXT("Aimming"));
	// ���� ����
}

void AArcherPlayerCharacter::AimmingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Aimming End"));
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	Shot();
}

void AArcherPlayerCharacter::Shot()
{
	if (ShotMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// ��Ÿ�� ��� (������ ��/�ƿ� �ð��� ��Ÿ�� ���¿� �����Ǿ� ����)
			float MontageDuration = AnimInstance->Montage_Play(ShotMontage, 1.0f, EMontagePlayReturnType::Duration, 0.f, false);
			if (MontageDuration > 0.f)
			{
				// ��Ÿ�ְ� ����Ǳ� ������ ȣ��� ��������Ʈ�� �����մϴ�.
				FOnMontageBlendingOutStarted BlendingOutDelegate;
				BlendingOutDelegate.BindUObject(this, &AArcherPlayerCharacter::OnShotMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, ShotMontage);

				// (�ɼ�) Anim Blueprint ������ �⺻ �ִϸ��̼� ���� ��ȯ ��������
				//Montage_IsPlaying(ShotMontage) üũ�� �߰��Ͽ�, ��Ÿ�ְ� ��� ���̸� �⺻ ���¸� ����� �ʵ��� �մϴ�.
				if (AnimInstance->Montage_IsPlaying(ShotMontage))
				{
					return;
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Shot montage failed to play"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ShotMontage is not set"));
	}
}

void AArcherPlayerCharacter::DeadEvent()
{
	Super::DeadEvent(); 
	/*// 몽타지의 플레이는 이미 Super::DeadEvent() 에 있음. 굳이 이걸 또? 
	if (DeadMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// ��Ÿ�� ��� (������ ��/�ƿ� �ð��� ��Ÿ�� ���¿� �����Ǿ� ����)
			float MontageDuration = AnimInstance->Montage_Play(DeadMontage, 1.0f, EMontagePlayReturnType::Duration, 0.f, false);
			if (MontageDuration > 0.f)
			{
				// ��Ÿ�ְ� ����Ǳ� ������ ȣ��� ��������Ʈ�� �����մϴ�.
				FOnMontageBlendingOutStarted BlendingOutDelegate;
				BlendingOutDelegate.BindUObject(this, &AArcherPlayerCharacter::OnShotMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, DeadMontage);

				// (�ɼ�) Anim Blueprint ������ �⺻ �ִϸ��̼� ���� ��ȯ ��������
				//Montage_IsPlaying(DeadMontage) üũ�� �߰��Ͽ�, ��Ÿ�ְ� ��� ���̸� �⺻ ���¸� ����� �ʵ��� �մϴ�.
				if (AnimInstance->Montage_IsPlaying(DeadMontage))
				{
					return;
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DeadMontage failed to play"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeadMontage is not set"));
	}
	
	APlayPlayerController* PC = Cast<APlayPlayerController>(GetController());
	PC->DeleteAllIMC();*/
	
}

void AArcherPlayerCharacter::OnEvadeBeginClick()
{
	Super::OnEvadeBeginClick();
}

void AArcherPlayerCharacter::OnEvadeReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Evade Released on Archer"));

}

void AArcherPlayerCharacter::MakeInitialSkills()
{
	if (!Skill1) {
		Skill1 = NewObject<UArcherBasicSkill>(this);
		SkillMap.Add(1, Skill1);
	}
	Skill1->InitializeSkill(this, 1);

	if (!Skill2) {
		Skill2 = NewObject<UArcherBuff>(this);
		SkillMap.Add(2, Skill2);
	}
	Skill2->InitializeSkill(this, 2);

	if (!Skill3) {
		Skill3 = NewObject<UArcherRainsPoem>(this);
		SkillMap.Add(3, Skill3);
	}
	Skill3->InitializeSkill(this, 3);

	if (!IdentitySkill) {
		IdentitySkill = NewObject<UArcher_ArrowMeteor>(this);
		SkillMap.Add(0, IdentitySkill);
	}
	IdentitySkill->InitializeSkill(this, 0);
}

