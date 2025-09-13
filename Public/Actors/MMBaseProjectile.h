// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseActor.h"
#include "MMBaseProjectile.generated.h"

class UProjectileMovementComponent;
class UPrimitiveComponent;
class UNiagaraSystem;
class USphereComponent;
class UParticleSystem;
class UMMBaseSkill;
class ABaseCombatCharacter;
/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AMMBaseProjectile : public AMMBaseActor
{
	GENERATED_BODY()
	
public:

	AMMBaseProjectile();

	void InitProjectileVelocity(const FVector& ShootDir);
	
	void SetShooterActor(ABaseCombatCharacter* NewShooter);

	void TryDamage(AActor* Target, int AttackDataIndex);

	void HideProjectile();

	void DelayedDestroy();

	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* CollisionNiaEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* CollisionCacheEffect;

	


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	ABaseCombatCharacter* ShooterActor;

	UMMBaseSkill* Skill;
	TArray<AActor*> DamagedActors;
	FTimerHandle DestroyHandle;
};
