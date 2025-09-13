// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMStatComponent.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Actors/MMBaseProjectile.h"
#include "Actors/Projectile/Mage/MMMageBasicAttackProjectile.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"

ABaseCombatCharacter::ABaseCombatCharacter()
{
	// 데미지 처리 컴포넌트 부착
	_DamagableComponent = CreateDefaultSubobject<UMMDamagableComponent>(TEXT("Damage Handler"));

	// 기본 피격 애니메이션 설정
	ConstructorHelpers::FObjectFinder<UAnimMontage> NormalStiffMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Test/Montages/Stiffness_Front.Stiffness_Front'"));
	if (NormalStiffMontageAsset.Succeeded()) {
		_DamagableComponent->NormalFrontStiffMontage = NormalStiffMontageAsset.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> StrongStiffMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Test/Montages/Stiffness_Front_Strong.Stiffness_Front_Strong'"));
	if (StrongStiffMontageAsset.Succeeded()) {
		_DamagableComponent->StrongFrontStiffMontage = StrongStiffMontageAsset.Object;
	}

}

void ABaseCombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	MakeInitialSkills();
}

void ABaseCombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEvading)
	{
		EvadeElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(EvadeElapsedTime / EvadeDuration, 0.f, 1.f);
		FVector NewLocation = FMath::Lerp(EvadeStartLocation, EvadeTargetLocation, Alpha);
		SetActorLocation(NewLocation, true);

		if (Alpha >= 1.f)
		{
			// 회전, 컨트롤러 입력 복원 등 추가 처리
			bIsEvading = false;
			GetDamagableComponent()->bIsInvincible = false;
			bUseControllerRotationYaw = true;  // 원래 값으로 복원 (상황에 따라 다르게 설정)
			GetCharacterMovement()->bOrientRotationToMovement = false; // 원래 값 복원
		}
	}
}

void ABaseCombatCharacter::OnBeginAnimation()
{
	// 애니메이션 몽타주의 맨 처음 시작부분에서 할 일들. 서버에서 실행
	Super::OnBeginAnimation();
	ResetSkillAttackDataIndex();

	if (CurrentAnimatedSkill) {
		CurrentAnimatedSkill->SetComboPassed(true);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Can't Find CurrentAnimatedSkill"));
	}
}

#pragma region InputResponse

void ABaseCombatCharacter::OnLeftMouseBeginClick()
{
	Super::OnLeftMouseBeginClick();
	// 스킬 컨펌 로직이랑 취소 로직 넣을 것
}

void ABaseCombatCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnLeftMouseHolding(triggered, ElapsedTime);
}

void ABaseCombatCharacter::OnLeftMouseReleased(bool cancled)
{
	Super::OnLeftMouseReleased(cancled);
}

void ABaseCombatCharacter::OnRightMouseBeginClick()
{
	Super::OnRightMouseBeginClick();
}

void ABaseCombatCharacter::OnRightMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnRightMouseHolding(triggered, ElapsedTime);
}

void ABaseCombatCharacter::OnRightMouseReleased(bool cancled)
{
	Super::OnRightMouseReleased(cancled);
}

void ABaseCombatCharacter::OnSkill1BeginClick()
{
	if (!Skill1) {
		UE_LOG(LogTemp, Log, TEXT("Skill1 was not setted"));
		return;
	}

	ServerCastEnterFight();

	UE_LOG(LogTemp, Log, TEXT("Skill1BeginClick"));
	if (Skill1->IsExecutePrepared()) {
		Skill1->OnConfirmExecute();
	}
	else {
		Skill1->OnBeginExecute();
	}
}

void ABaseCombatCharacter::OnSkill1Holding(bool triggered, float ElapsedTime)
{
	if (!Skill1) {
		UE_LOG(LogTemp, Log, TEXT("Skill1 was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Skill1 Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
	Skill1->OnHoldExecute(ElapsedTime);
}

void ABaseCombatCharacter::OnSkill1Released(bool cancled)
{
	if (!Skill1) {
		UE_LOG(LogTemp, Log, TEXT("Skill1 was not setted"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Skill1 Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
	Skill1->OnReleaseExcute();
}

void ABaseCombatCharacter::OnSkill2BeginClick()
{
	if (!Skill2) {
		UE_LOG(LogTemp, Log, TEXT("Skill2 was not setted"));
		return;
	}

	ServerCastEnterFight();

	UE_LOG(LogTemp, Log, TEXT("Skill2BeginClick"));
	if (Skill2->IsExecutePrepared()) {
		Skill2->OnConfirmExecute();
	}
	else {
		Skill2->OnBeginExecute();
	}

}

void ABaseCombatCharacter::OnSkill2Holding(bool triggered, float ElapsedTime)
{
	if (!Skill2) {
		UE_LOG(LogTemp, Log, TEXT("Skill2 was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Skill2 Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
	Skill2->OnHoldExecute(ElapsedTime);
}

void ABaseCombatCharacter::OnSkill2Released(bool cancled)
{
	if (!Skill2) {
		UE_LOG(LogTemp, Log, TEXT("Skill2 was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Skill2 Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
	Skill2->OnReleaseExcute();

}

void ABaseCombatCharacter::OnSkill3BeginClick()
{
	if (!Skill3) {
		UE_LOG(LogTemp, Log, TEXT("Skill3 was not setted"));
		return;
	}

	ServerCastEnterFight();

	UE_LOG(LogTemp, Log, TEXT("Skill3BeginClick"));
	if (Skill3->IsExecutePrepared()) {
		Skill3->OnConfirmExecute();
	}
	else {
		Skill3->OnBeginExecute();
	}

}

void ABaseCombatCharacter::OnSkill3Holding(bool triggered, float ElapsedTime)
{
	if (!Skill3) {
		UE_LOG(LogTemp, Log, TEXT("Skill3 was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Skill3 Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
	Skill3->OnHoldExecute(ElapsedTime);
}

void ABaseCombatCharacter::OnSkill3Released(bool cancled)
{
	if (!Skill3) {
		UE_LOG(LogTemp, Log, TEXT("Skill3 was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Skill3 Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
	Skill3->OnReleaseExcute();
}

void ABaseCombatCharacter::OnIdentitySkillBeginClick()
{
	if (!IdentitySkill) {
		UE_LOG(LogTemp, Log, TEXT("IdentitySkill was not setted"));
		return;
	}

	ServerCastEnterFight();

	UE_LOG(LogTemp, Log, TEXT("IdentitySkillBeginClick"));
	if (IdentitySkill->IsExecutePrepared()) {
		IdentitySkill->OnConfirmExecute();
	}
	else {
		IdentitySkill->OnBeginExecute();
	}
}

void ABaseCombatCharacter::OnIdentitySkillHolding(bool triggered, float ElapsedTime)
{
	if (!IdentitySkill) {
		UE_LOG(LogTemp, Log, TEXT("IdentitySkill was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("IdentitySkill Hold, triggered = %s"), triggered ? TEXT("True") : TEXT("False"));
	IdentitySkill->OnHoldExecute(ElapsedTime);

}

void ABaseCombatCharacter::OnIdentitySkillReleased(bool cancled)
{
	if (!IdentitySkill) {
		UE_LOG(LogTemp, Log, TEXT("IdentitySkill was not setted"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("IdentitySkill Released, Cancled = %s"), cancled ? TEXT("True") : TEXT("False"));
	IdentitySkill->OnReleaseExcute();
}

void ABaseCombatCharacter::OnEvadeBeginClick()
{


	if (!HasAuthority())
	{
		// 만약 이동 입력이 없다면 컨트롤러의 방향을 사용합니다.
		EvadeDirection = GetLastMovementInputVector();
		if (EvadeDirection.IsNearlyZero())
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				// 컨트롤러 회전에서 피치 값을 제거한 후 방향 계산
				FRotator ControlRot = PC->GetControlRotation();
				ControlRot.Pitch = 0.0f; // 피치 제거
				EvadeDirection = ControlRot.Vector();
			}
		}
		EvadeDirection.Normalize();

		// 구르기 시작과 목표 위치 계산
		EvadeStartLocation = GetActorLocation();
		EvadeTargetLocation = EvadeStartLocation + (EvadeDirection * EvadeDistance);
		
		Server_OnEvadeBeginClick(EvadeStartLocation, EvadeTargetLocation);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Evade BeginClick"));
}

void ABaseCombatCharacter::Server_OnEvadeBeginClick_Implementation(FVector StartLocation, FVector EndLocation)
{
	// 입력받은 방향을 저장합니다.
	

	Multi_OnEvadeBeginClick(StartLocation, EndLocation);
}

void ABaseCombatCharacter::Multi_OnEvadeBeginClick_Implementation(FVector StartLocation, FVector EndLocation)
{
	if (bIsEvading)
	{
		return;
	}

	GetDamagableComponent()->bIsInvincible = true;
	bIsEvading = true;
	EvadeElapsedTime = 0.f;
	EvadeStartLocation = StartLocation;    // 서버가 결정한 시작 위치
	EvadeTargetLocation = EndLocation;       // 서버가 결정한 목표 위치

	// 몽타주 재생 및 기타 초기 설정은 그대로 유지
	if (EvadeMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;

			float MontageDuration = AnimInstance->Montage_Play(EvadeMontage, 1.0f, EMontagePlayReturnType::Duration, 0.f, false);
			if (MontageDuration > 0.f)
			{
				FOnMontageBlendingOutStarted BlendingOutDelegate;
				BlendingOutDelegate.BindUObject(this, &ABaseCombatCharacter::OnEvadeMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, EvadeMontage);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Evade montage failed to play"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Evade montage is not set"));
	}
}

void ABaseCombatCharacter::OnEvadeReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Evade Released"));

}

void ABaseCombatCharacter::OnItem1BeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Item1 BeginClick"));

}

void ABaseCombatCharacter::OnItem1Released()
{
	UE_LOG(LogTemp, Log, TEXT("Item1 Released"));

}

void ABaseCombatCharacter::OnItem2BeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Item2 BeginClick"));

}

void ABaseCombatCharacter::OnItem2Released()
{
	UE_LOG(LogTemp, Log, TEXT("Item2 Released"));

}

void ABaseCombatCharacter::OnItem3BeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Item3 BeginClick"));

}

void ABaseCombatCharacter::OnItem3Released()
{
	UE_LOG(LogTemp, Log, TEXT("Item3 Released"));

}

void ABaseCombatCharacter::OnItem4BeginClick()
{
	UE_LOG(LogTemp, Log, TEXT("Item4 BeginClick"));

}

void ABaseCombatCharacter::OnItem4Released()
{
	UE_LOG(LogTemp, Log, TEXT("Item4 Released"));

}

#pragma endregion

#pragma region RPCs

void ABaseCombatCharacter::ServerPlaySkillMontage_Implementation(UAnimMontage* Montage, FName SectionName, int SkillID)
{
	if (CanSwitchAnimation()) {
		UMMBaseSkill** FoundSkill = SkillMap.Find(SkillID);
		if (FoundSkill) {
			SetCurrentAnimatedSkill(*FoundSkill);
		}
		if (CurrentAnimatedSkill) {
			BroadcastPlayMontage(Montage, SectionName);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Server : %s Can't Find Skill Instance"), *UKismetSystemLibrary::GetDisplayName(this));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Server : %s Can't switch animation"), *UKismetSystemLibrary::GetDisplayName(this));
	}
}

void ABaseCombatCharacter::Server_SpawnProjectile_Implementation(const FVector& Start, const FRotator& End, const FVector& Dir, TSubclassOf<AMMBaseProjectile> InAttackProjectileClass)
{
	if (!InAttackProjectileClass) {
		UE_LOG(LogTemp, Error, TEXT("InAttackProjectileClass is null"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Error, TEXT("World is null"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	

	AMMBaseProjectile* Projectile = this->GetWorld()->SpawnActor<AMMBaseProjectile>
		(
		InAttackProjectileClass,
		Start,
		End,
		SpawnParams
		);
	if (Projectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile : Success"))
		// 투사체 이동 방향 초기화 (투사체 내부에 ProjectileMovementComponent가 있다면)
		Projectile->SetShooterActor(this);
		Projectile->InitProjectileVelocity(Dir);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile : Fail"))
	}
	

}

void ABaseCombatCharacter::ServerPlayComboMontage_Implementation(UAnimMontage* Montage, int SkillID)
{
	UMMBaseSkill* PrevSkill = CurrentAnimatedSkill;
	UMMBaseSkill** FoundSkill = SkillMap.Find(SkillID);
	if (!FoundSkill) {
		return;
	}

	if (*FoundSkill) {
		
		if (((*FoundSkill)->bCanComboContinue && !(*FoundSkill)->IsComboPassed()) || ((*FoundSkill)->GetComboIndex() == 0 && CanSwitchAnimation())) {
			SetCurrentAnimatedSkill(*FoundSkill);
			FString ComboString = FString("Combo");
			ComboString.AppendInt(CurrentAnimatedSkill->GetComboIndex());
			BroadcastPlayMontage(Montage, FName(ComboString));
		}
		else {
			CurrentAnimatedSkill = PrevSkill;
			UE_LOG(LogTemp, Warning, TEXT("Server : %s Can't Continue Combo"), *UKismetSystemLibrary::GetDisplayName(this));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Server : %s Can't Find Skill Instance"), *UKismetSystemLibrary::GetDisplayName(this));
	}
}

void ABaseCombatCharacter::ClientInitSkillIMC_Implementation(int SkillID)
{
	if (SkillMap.Contains(SkillID)) {
		SkillMap[SkillID]->InitSkillMappingContext();
	}
}

void ABaseCombatCharacter::ClientDeleteSkillIMC_Implementation(int SkillID)
{
	if (SkillMap.Contains(SkillID)) {
		SkillMap[SkillID]->DeleteSkillMappingContext();
	}
}
#pragma endregion

void ABaseCombatCharacter::MakeInitialSkills()
{

}

void ABaseCombatCharacter::GetWeaponOverlappingActors(TArray<AActor*>& OutOverlappingActors, TSubclassOf<AActor> ClassFilter)
{
	OutOverlappingActors.Empty();
	UE_LOG(LogTemp, Warning, TEXT("%s Has No Weapon Collision"), *UKismetSystemLibrary::GetDisplayName(this));
}

void ABaseCombatCharacter::CancleOtherPreparedSkills(UMMBaseSkill* Exception)
{
	if (Skill1 && Skill1 != Exception) {
		Skill1->CanclePreparedExecute();
	}
	if (Skill2 && Skill2 != Exception) {
		Skill2->CanclePreparedExecute();
	}
	if (Skill3 && Skill3 != Exception) {
		Skill3->CanclePreparedExecute();
	}
	if (IdentitySkill && IdentitySkill != Exception) {
		IdentitySkill->CanclePreparedExecute();
	}
}

FAttackData ABaseCombatCharacter::GetCurrentAnimatedSkillAttackData(int Index) const
{
	if (CurrentAnimatedSkill) {
		const FSkill& TempSkillData = CurrentAnimatedSkill->GetSkillData();
		if (CurrentAnimatedSkill) {
			if (TempSkillData.AttackDatas.IsValidIndex(Index)) {
				return TempSkillData.AttackDatas[Index];
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Invalid Index. Or CurrentAnimatedSkillAttackData is null"));
	return FAttackData();
}

void ABaseCombatCharacter::ResetSkillAttackDataIndex()
{
	SkillAttackDataIndex = 0;
}

void ABaseCombatCharacter::IncreaseSkillAttackDataIndex()
{
	++SkillAttackDataIndex;
}

int ABaseCombatCharacter::GetSkillAttackDataIndex() const
{
	return SkillAttackDataIndex;
}

void ABaseCombatCharacter::SetCurrentAnimatedSkill(UMMBaseSkill* Skill)
{
	if (CurrentAnimatedSkill && CurrentAnimatedSkill != Skill) {
		CurrentAnimatedSkill->InitComboIndex();
		CurrentAnimatedSkill->SetComboPassed(false);
		CurrentAnimatedSkill->bCanComboContinue = false;
	}
	CurrentAnimatedSkill = Skill;
}

UMMBaseSkill* ABaseCombatCharacter::GetCurrentAnimatedSkill() const
{
	return CurrentAnimatedSkill;
}

UMMDamagableComponent* ABaseCombatCharacter::GetDamagableComponent() const
{
	return _DamagableComponent;
}

void ABaseCombatCharacter::OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	
}

const FVector& ABaseCombatCharacter::GetEvadeDirection() const
{
	return EvadeDirection;
}

float ABaseCombatCharacter::GetEvadeDistance() const
{
	return EvadeDistance;
}

void ABaseCombatCharacter::SetEvadeDistance(float value)
{
	EvadeDistance = FMath::Clamp(value, 50.0f, 1500.0f);
}

void ABaseCombatCharacter::SetEvadeDuration(float value)
{
	EvadeDuration = FMath::Clamp(value, 0.05f, 3.0f);
}

UAnimMontage* ABaseCombatCharacter::GetPerfectDodgeMontage() const
{
	return PerfectDodgeMontage;
}
