// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MMBaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "GameFramework/PlayerController.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "ActorComponents/MMStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "UStructs/FSkill.h"





AMMBaseProjectile::AMMBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBox"));
	CollisionBox->InitSphereRadius(10.f);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionBox;


    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    if (ProjectileMovement)
    {
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->bShouldBounce = false;
        ProjectileMovement->InitialSpeed = 2000.f;
        ProjectileMovement->MaxSpeed = 2000.f;
        ProjectileMovement->ProjectileGravityScale = 0.f;
    }

	// RootComponent�� �浹 �̺�Ʈ�� ���� �� �ֵ��� UPrimitiveComponent�� ĳ����
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(RootComponent))
	{
		PrimComp->OnComponentBeginOverlap.AddDynamic(this, &AMMBaseProjectile::OnProjectileBeginOverlap);
	}

	

}

void AMMBaseProjectile::InitProjectileVelocity(const FVector& ShootDir)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
    }
}

void AMMBaseProjectile::SetShooterActor(ABaseCombatCharacter* NewShooter)
{
	
	ShooterActor = NewShooter;
	Skill = NewShooter->GetCurrentAnimatedSkill();
	
}


void AMMBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ShooterActor == nullptr)
	{
		ShooterActor = Cast<ABaseCombatCharacter>(GetOwner());
	}


}

void AMMBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMMBaseProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == ShooterActor) return;

	 //�浹 ����Ʈ�� �����Ǿ� ������ ���̾ư��� ����Ʈ�� ����
	if (CollisionNiaEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			CollisionNiaEffect,
			SweepResult.ImpactPoint,      // �浹 ������ ����
			FRotator::ZeroRotator,        // ȸ���� (�ʿ信 ���� ����)
			FVector(1.f),                 // ������ (�ʿ信 ���� ����)
			true,                         // auto-destroy
			true,                         // auto-activate
			ENCPoolMethod::None,          // Ǯ�� �ɼ� (�ʿ信 ���� ����)
			true                          // PreCullCheck
		);
	}

	if (CollisionCacheEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			CollisionCacheEffect,
			GetActorLocation(),   // ���� ��ġ
			GetActorRotation()	  // ���� ȸ��
		);
	}

	// �浹 �� ����ü�� ������ �� �ֽ��ϴ�.
	if (HasAuthority()&& Skill)
	{
		if (ShooterActor)
		{
			int AttackDataIndex = ShooterActor->GetSkillAttackDataIndex();

			TArray<AActor*> OutOverlappingActors;

			Skill->GetOverlappingActorsToDamage(OutOverlappingActors, AttackDataIndex, GetActorLocation());

			UMMDamagableComponent* DamageComp = Cast<UMMDamagableComponent>(OtherActor->FindComponentByClass(UMMDamagableComponent::StaticClass()));
			if (DamageComp)
			{
				UMMStatComponent* StatComp = Cast<UMMStatComponent>(ShooterActor->FindComponentByClass(UMMStatComponent::StaticClass()));
				DamageComp->TakeDamage(ShooterActor, StatComp, Skill->GetSkillData().AttackDatas[0].DamageData);
			}

		}
	
	}



	HideProjectile();
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AMMBaseProjectile::DelayedDestroy, 0.3f, false); // 0.3�� �� ����

}

void AMMBaseProjectile::TryDamage(AActor* Target, int AttackDataIndex)
{
	DamagedActors.Empty();

	UMMDamagableComponent* DamageHandler = Target->FindComponentByClass<UMMDamagableComponent>();
	if (DamageHandler) {
		UMMStatComponent* StatComponent = ShooterActor->TryGetStatComponent();
		if (StatComponent) {
			if (ShooterActor->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
				DamageHandler->TakeDamage(ShooterActor, StatComponent,
					ShooterActor->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas[AttackDataIndex].DamageData);
				DamagedActors.Add(Target);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("%s has no DamagableComponent"), *UKismetSystemLibrary::GetDisplayName(Target));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT(""));
		DamagedActors.Add(Target);
	}


}

void AMMBaseProjectile::HideProjectile()
{
	// �浹 ����
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���־� ����
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	// �̵� ����
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}
}

void AMMBaseProjectile::DelayedDestroy()
{
	Destroy();
}
