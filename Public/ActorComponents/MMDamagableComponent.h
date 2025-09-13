// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UStructs/FCharacterStat.h"
#include "UStructs/FDamageData.h"

#include "MMDamagableComponent.generated.h"

class ABaseCharacter;
class UMMStatComponent;
class UStatusEffect;
class UHitReactionLegacyCameraShake;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSSPIRE_API UMMDamagableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMMDamagableComponent();

#pragma region Statics
private:
	static const float MaxDamage;

#pragma endregion

#pragma region Overrides
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

#pragma region RPCs


#pragma endregion

public:
	UFUNCTION()
	// 서버에서만 실행됨 (HasAuthority)
	virtual void TakeDamage(AActor* Subject, UMMStatComponent* AttackerStat, const FDamageData& SkillDamageData, FVector DamageDirection = FVector());

protected:
	virtual void HandleDamageReaction(AActor* Subject, const FDamageReactionData& DamageReactionData, const FVector& DamageDirection);

	// 데미지 타입 저항에 따라 딜 편차 만들기 Chain of responsibility로 바꿔도 됨(데미지 타입 늘릴 거라면)
	virtual float HandleTypeOfDamage(AActor* Subject, uint8 DamageType, float Damage);

public:
	UFUNCTION()
	UMMStatComponent* TryGetStatComponent();
	UFUNCTION()
	bool IsInvincible() const;
	UFUNCTION()
	bool IsInSuperArmorState() const;
	UFUNCTION()
	const FDamageReactionData& GetCurrentDamageReactionData() const;
	UFUNCTION()
	const FVector& GetCurrentDamageDirection() const;
	UFUNCTION()
	void SetInvincible(bool value);
	UFUNCTION()
	void SetSuperArmorState(bool value);

public:
	void ClearNSetTimerReleaseStiff(float InRate, bool InbLoop = false, float InFirstDelay = -1.f);
	void ReleaseStiff();

protected:
	void ApplyStatusEffect(const TSubclassOf<UStatusEffect>& Effect, float Probability, float duration);
	void ApplyScreenShaking(ABaseCharacter* SubjectCharacter, float Damage);
	void PlayStiffMontage(ABaseCharacter* Character, bool bIsStrongAttack);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* NormalFrontStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* StrongFrontStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* NormalBackStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* StrongBackStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* NormalSideStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* StrongSideStiffMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAnimation")
	UAnimMontage* KnockdownMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamagableComponent")
	bool bIsInvincible = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamagableComponent")
	bool bIsSuperArmorState = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamagableComponent")
	bool bIsEvading = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	float LastDamage = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	TMap<TSubclassOf<UStatusEffect>, UStatusEffect*> AppliedStatusEffects;

	UMMStatComponent* StatComponent;

	TSubclassOf<class UCameraShakeBase> CameraShakeAnim;

	FDamageReactionData CurrentDamageReactionData;
	FVector CurrentDamageDirection;

	FTimerHandle StiffTimerHandle;
	UAnimMontage* CurrentStiffMontage;
};
