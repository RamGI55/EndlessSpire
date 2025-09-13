// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UMMStatComponent;
class UMMDamagableComponent;

UCLASS()
class ENDLESSSPIRE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// �ִϸ��̼� ��Ÿ���� �� ó�� ���ۺκп��� �� �ϵ�. �������� ����
	virtual void OnBeginAnimation();

	virtual void PauseMontageByDamage(float Damage);

#pragma region RPCs
public:
	UFUNCTION(Server, Reliable)
	void ServerCastPlayMontage(UAnimMontage* Montage, FName SectionName = NAME_None);
	UFUNCTION(Server, Reliable)
	// ��Ÿ�� �÷������̸� ĵ���ϰ� �� ��Ÿ�� ����
	void ServerCastPlayMontageImmediately(UAnimMontage* Montage, FName SectionName = NAME_None);
	UFUNCTION(Server, Reliable)
	void ServerCastResumeMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPlayMontage(UAnimMontage* Montage, FName SectionName = NAME_None);
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastCancleCurrentNPlayMontage(UAnimMontage* Montage, FName SectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastRotateNPlayMontage(UAnimMontage* Montage, FName SectionName = NAME_None, FRotator Rotation = FRotator());
	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastResumeMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	// �������� �׼Ǽ��� ���� ȿ�� ����
	virtual void BroadcastDamagedReaction(float Damage);
	UFUNCTION(NetMulticast, Reliable)
	// ������
	virtual void BroadcastAttackReaction(float Damage);

#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void SetCanSwitchAnimation(bool value);
	UFUNCTION(BlueprintCallable)
	bool CanSwitchAnimation() const;

protected:
	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);
	void CancleCurrentNPlayMontage(UAnimMontage* Montage, const FName& SectionName);

public:
	UMMStatComponent* TryGetStatComponent();
	UMMDamagableComponent* TryGetDamagableComponent();
	bool IsInHitStiff() const;

private:
	void CountTimeToCanclePause(float DeltaTime);

public:

	virtual void DeadEvent();

	void DestroySelf();

protected:
	
	UAnimMontage* DeathMontage;

private:
	UMMStatComponent* _StatComponent;
	UMMDamagableComponent* _DamagableComponent;
	bool _bCanSwitchAnimation = true;
	float HitStiffTimer = 0.0f;
	float HitStiffTime = 0.0f;
	bool bHitStiff = false;
	UAnimMontage* HitStiffAnim;
	FTimerHandle DeathMontageTimerHandle;
};
