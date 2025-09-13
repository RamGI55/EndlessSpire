// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/MagePlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "UObjects/PlayerSkills/Mage/MageBasicComboSkill.h"
#include "UObjects/PlayerSkills/Mage/MageFireBallSkill.h"
#include "UObjects/PlayerSkills/Mage/MageIceFieldSkill.h"
#include "UObjects/PlayerSkills/Mage/MagePlasmaBeamSkill.h"
#include "UObjects/PlayerSkills/Mage/MageIdentitySkill.h"

#include "HUDs/LobbyHUD.h"
#include "Net/UnrealNetwork.h"

AMagePlayerCharacter::AMagePlayerCharacter()
{
	bReplicates = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh 위치 변경
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -97.5), FRotator(0, -90, 0));

	}

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/ParkJS/Player/ABP_MagePlayerAnimInstance.ABP_MagePlayerAnimInstance_C'"));

	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}


	ConstructorHelpers::FObjectFinder<UAnimMontage>EvadeMontageAsset(TEXT("AnimMontage'/Game/Animations/Mage/Montages/Mage_Evade.Mage_Evade'"));
	if (EvadeMontageAsset.Succeeded())
	{
		EvadeMontage = EvadeMontageAsset.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Mage/Montages/Mage_Dead.Mage_Dead'"));
	if (DeathMontageAsset.Succeeded())
	{
		DeathMontage = DeathMontageAsset.Object;
	}


}

//좌클릭
void AMagePlayerCharacter::OnLeftMouseBeginClick()
{
	Super::OnLeftMouseBeginClick();


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

	BasicAttackSkill->OnBeginExecute();
}

void AMagePlayerCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnLeftMouseHolding(triggered, ElapsedTime);

	BasicAttackSkill->OnBeginExecute();
}

void AMagePlayerCharacter::OnLeftMouseReleased(bool cancled)
{
	Super::OnLeftMouseReleased(cancled);
}

//우클릭
void AMagePlayerCharacter::OnRightMouseBeginClick()
{
	Super::OnRightMouseBeginClick();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PC->GetHUD());
		if (LobbyHUD)
		{

			LobbyHUD->PlayerUI->Crosshair->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT(" %d"), LobbyHUD->PlayerUI->Crosshair->GetVisibility());


		}
	}


}

void AMagePlayerCharacter::OnRightMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnRightMouseHolding(triggered, ElapsedTime);
}

void AMagePlayerCharacter::OnRightMouseReleased(bool cancled)
{
	Super::OnRightMouseReleased(cancled);
}



//스킬1 Q
void AMagePlayerCharacter::OnSkill1BeginClick()
{
	Super::OnSkill1BeginClick();

	Skill1->OnBeginExecute();

}

void AMagePlayerCharacter::OnSkill1Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill1Holding(triggered, ElapsedTime);

}


void AMagePlayerCharacter::OnSkill1Released(bool cancled)
{
	Super::OnSkill1Released(cancled);


}

//스킬2 E
void AMagePlayerCharacter::OnSkill2BeginClick()
{
	Super::OnSkill2BeginClick();
	Skill2->OnBeginExecute();
	
}

void AMagePlayerCharacter::OnSkill2Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill2Holding(triggered, ElapsedTime);
}

void AMagePlayerCharacter::OnSkill2Released(bool cancled)
{
	Super::OnSkill2Released(cancled);
}


//스킬3 R
void AMagePlayerCharacter::OnSkill3BeginClick()
{
	Super::OnSkill3BeginClick();
	Skill3->OnBeginExecute();
}

void AMagePlayerCharacter::OnSkill3Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill3Holding(triggered, ElapsedTime);
}

void AMagePlayerCharacter::OnSkill3Released(bool cancled)
{
	Super::OnSkill3Released(cancled);
}

void AMagePlayerCharacter::OnIdentitySkillBeginClick()
{
	Super::OnIdentitySkillBeginClick();
}

void AMagePlayerCharacter::OnIdentitySkillHolding(bool triggered, float ElapsedTime)
{
	Super::OnIdentitySkillHolding(triggered, ElapsedTime);
}

void AMagePlayerCharacter::OnIdentitySkillReleased(bool cancled)
{
	Super::OnIdentitySkillReleased(cancled);
}


// 회피
void AMagePlayerCharacter::OnEvadeBeginClick()
{
	Super::OnEvadeBeginClick();

	APlayPlayerController* PlayController = Cast<APlayPlayerController>(GetController());
	if (PlayController) {
		const FVector2D& MoveInput = PlayController->GetMoveInput();

		if (MoveInput.Y < 0.0f) { // 후방
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(),
				GetActorLocation() + (-GetActorForwardVector() + GetActorRightVector() * MoveInput.X) * 100.0f);

			ServerEvade(EvadeMontage, FName("Default"), true, Rotation);
		}
		else {
			if (MoveInput.X > 0.0f) { // 좌
				ServerEvade(EvadeMontage, FName("Default"));
			}
			else if (MoveInput.X < 0.0f) { // 우
				ServerEvade(EvadeMontage, FName("Default"));
			}
			else { // 기타 예외들
				ServerEvade(EvadeMontage, FName("Default"));
			}
		}
	}


}

void AMagePlayerCharacter::OnEvadeReleased()
{
	Super::OnEvadeReleased();

}

// Init스킬
void AMagePlayerCharacter::MakeInitialSkills()
{
	Super::MakeInitialSkills();

	if (!BasicAttackSkill) {
		BasicAttackSkill = NewObject<UMageBasicComboSkill>(this);
		SkillMap.Add(0, BasicAttackSkill);
	}
	BasicAttackSkill->InitializeSkill(this, 0);

	if (!Skill1)
	{
		Skill1 = NewObject<UMageFireBallSkill>(this);
		SkillMap.Add(1, Skill1);
	}
	Skill1->InitializeSkill(this, 1);

	if (!Skill2)
	{
		Skill2 = NewObject<UMageIceFieldSkill>(this);
		SkillMap.Add(2, Skill2);
	}
	Skill2->InitializeSkill(this, 2);

	if (!Skill3)
	{
		Skill3 = NewObject<UMagePlasmaBeamSkill>(this);
		SkillMap.Add(3, Skill3);
	}
	Skill3->InitializeSkill(this, 3);

	if (!IdentitySkill)
	{
		IdentitySkill = NewObject<UMageIdentitySkill>(this);
		SkillMap.Add(4, IdentitySkill);
	}
	IdentitySkill->InitializeSkill(this, 4);


}

void AMagePlayerCharacter::DeadEvent()
{
	Super::DeadEvent();


}



void AMagePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator BaseAimRotation = this->GetBaseAimRotation();
	FRotator ActorRotation = this->GetActorRotation();

	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(BaseAimRotation, ActorRotation);

	if (IsLocallyControlled())
	{
		Roll = DeltaRot.Roll;
		Pitch = DeltaRot.Pitch;
		Yaw = DeltaRot.Yaw;

	
	}


}


void AMagePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagePlayerCharacter, Roll);
	DOREPLIFETIME(AMagePlayerCharacter, Pitch);
	DOREPLIFETIME(AMagePlayerCharacter, Yaw);
	DOREPLIFETIME(AMagePlayerCharacter, CameraAiming);

}

void AMagePlayerCharacter::ServerEvade_Implementation(UAnimMontage* Montage, FName SectionName, bool bDoRotate, FRotator Rotation)
{
	if (CurrentAnimatedSkill != BasicAttackSkill && !CanSwitchAnimation()) {
		return;
	}



	if (bDoRotate) {
		BroadcastRotateNPlayMontage(Montage, SectionName, Rotation);
	}
	else {
		BroadcastCancleCurrentNPlayMontage(Montage, SectionName);
	}

}



