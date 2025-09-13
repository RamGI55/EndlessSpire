// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Warrior/WarriorIdentitySkill.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Actors/Characters/Players/WarriorPlayerCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UWarriorIdentitySkill::UWarriorIdentitySkill()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IDSkillMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/Warrior/IDSkillMappingContext.IDSkillMappingContext'"));
	if (IDSkillMappingContextObject.Succeeded())
	{
		IDSkillMappingContext = IDSkillMappingContextObject.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> IDSkillEvadeActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/Warrior/IA_WarriorIDEvade.IA_WarriorIDEvade'"));
	if (IDSkillEvadeActionObject.Succeeded())
	{
		IDSkillEvadeInputAction = IDSkillEvadeActionObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IDSkillConfirmActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/Warrior/IA_WarriorIDConfirm.IA_WarriorIDConfirm'"));
	if (IDSkillConfirmActionObject.Succeeded())
	{
		IDSkillConfirmInputAction = IDSkillConfirmActionObject.Object;
	}

	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Warrior_IdentitySkill"), FString("WarriorIdentitySkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UWarriorIdentitySkill::InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID)
{
	Super::InitializeSkill(InPlayer, InSkillID);

	if (InPlayer->HasAuthority()) return;
	if (!InPlayer->GetController()) return;

	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InPlayer->GetWorld()->GetFirstPlayerController()->GetLocalPlayer());
	
	InPlayer->GetWorld()->GetTimerManager().ClearTimer(BindInputActionsTimerHandle);
	InPlayer->GetWorld()->GetTimerManager().SetTimer(BindInputActionsTimerHandle, this, 
		&UWarriorIdentitySkill::BindInputActions, 0.5f, false);
}

void UWarriorIdentitySkill::OnBeginExecute()
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}
}

void UWarriorIdentitySkill::InitSkillMappingContext()
{
	InitIDSkillMappingContext();
}

void UWarriorIdentitySkill::DeleteSkillMappingContext()
{
	DeleteIDSkillMappingContext();
}

void UWarriorIdentitySkill::OnIDSkillEvade()
{
	if (!bIsIDSkillOnProccess) return;

	AWarriorPlayerCharacter* Warrior = Cast<AWarriorPlayerCharacter>(Player);
	if (Warrior) {
		if (EvadeCount < 2) {
			APlayPlayerController* PlayController = Cast<APlayPlayerController>(Warrior->GetController());
			if (PlayController) {
				Warrior->ServerIDSkillEvade(PlayController->GetRelativeMoveInput().GetSafeNormal(), EvadeDashDistance);
			}
			
			
			++EvadeCount;
		}


	}
}

void UWarriorIdentitySkill::OnIDSkillConfirm()
{
	if (!bIsIDSkillOnProccess) return;

	AWarriorPlayerCharacter* Warrior = Cast<AWarriorPlayerCharacter>(Player);
	if (Warrior) {
		Warrior->ServerCastEndIDSkill();

		UAnimMontage* Montage = Warrior->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
		Warrior->ServerCastResumeMontage(Montage);

		DeleteIDSkillMappingContext();
	}
}

void UWarriorIdentitySkill::BindInputActions()
{
	if (!Player->InputComponent) {
		Player->GetWorld()->GetTimerManager().ClearTimer(BindInputActionsTimerHandle);
		Player->GetWorld()->GetTimerManager().SetTimer(BindInputActionsTimerHandle, this,
			&UWarriorIdentitySkill::BindInputActions, 0.5f, false);
		return;
	}
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Player->InputComponent);
	if (EnhancedInputComponent) {
		EnhancedInputComponent->BindAction(IDSkillEvadeInputAction, ETriggerEvent::Started, this, &UWarriorIdentitySkill::OnIDSkillEvade);
		EnhancedInputComponent->BindAction(IDSkillConfirmInputAction, ETriggerEvent::Started, this, &UWarriorIdentitySkill::OnIDSkillConfirm);
	}
}

void UWarriorIdentitySkill::InitIDSkillMappingContext()
{
	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(IDSkillMappingContext, 10);
		bIsIDSkillOnProccess = true;
		EvadeCount = 0;
	}
}

void UWarriorIdentitySkill::DeleteIDSkillMappingContext()
{
	if (EnhancedInputSubsystem)
	{
		while (EnhancedInputSubsystem->HasMappingContext(IDSkillMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(IDSkillMappingContext);
		}
		bIsIDSkillOnProccess = false;
		EvadeCount = 0;
	}
}
