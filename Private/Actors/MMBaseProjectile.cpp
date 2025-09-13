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

	// RootComponent가 충돌 이벤트를 받을 수 있도록 UPrimitiveComponent로 캐스팅
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

	 //충돌 이펙트가 설정되어 있으면 나이아가라 이펙트를 스폰
	if (CollisionNiaEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			CollisionNiaEffect,
			SweepResult.ImpactPoint,      // 충돌 지점에 스폰
			FRotator::ZeroRotator,        // 회전값 (필요에 따라 조정)
			FVector(1.f),                 // 스케일 (필요에 따라 조정)
			true,                         // auto-destroy
			true,                         // auto-activate
			ENCPoolMethod::None,          // 풀링 옵션 (필요에 따라 설정)
			true                          // PreCullCheck
		);
	}

	if (CollisionCacheEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			CollisionCacheEffect,
			GetActorLocation(),   // 스폰 위치
			GetActorRotation()	  // 스폰 회전
		);
	}

	// 충돌 후 투사체를 제거할 수 있습니다.
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
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AMMBaseProjectile::DelayedDestroy, 0.3f, false); // 0.3초 뒤 삭제

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
	// 충돌 방지
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 비주얼 제거
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	// 이동 정지
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}
}

void AMMBaseProjectile::DelayedDestroy()
{
	Destroy();
}
