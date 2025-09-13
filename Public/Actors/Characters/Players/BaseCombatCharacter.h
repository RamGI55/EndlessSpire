// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "BaseCombatCharacter.generated.h"

/**
 * 
 */

class UMMBaseSkill;
struct FAttackData;
class UMMDamagableComponent;
class AMMBaseProjectile;
class AMMMageBasicAttackProjectile;

UCLASS()
class ENDLESSSPIRE_API ABaseCombatCharacter : public ABasePlayerCharacter
{
	GENERATED_BODY()
	
public:
	ABaseCombatCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnBeginAnimation() override;

#pragma region InputResponse
public:
	virtual void OnLeftMouseBeginClick() override;
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnLeftMouseReleased(bool cancled) override;

	virtual void OnRightMouseBeginClick() override;
	virtual void OnRightMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnRightMouseReleased(bool cancled) override;

	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill1BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill1Holding(bool triggered, float ElapsedTime);
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill1Released(bool cancled);

	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill2BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill2Holding(bool triggered, float ElapsedTime);
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill2Released(bool cancled);

	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill3BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill3Holding(bool triggered, float ElapsedTime);
	UFUNCTION(Category = "InputResponse")
	virtual void OnSkill3Released(bool cancled);

	UFUNCTION(Category = "InputResponse")
	virtual void OnIdentitySkillBeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnIdentitySkillHolding(bool triggered, float ElapsedTime);
	UFUNCTION(Category = "InputResponse")
	virtual void OnIdentitySkillReleased(bool cancled);

	UFUNCTION(Category = "InputResponse")
	virtual void OnEvadeBeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnEvadeReleased();

	UFUNCTION(Category = "InputResponse")
	virtual void OnItem1BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnItem1Released();

	UFUNCTION(Category = "InputResponse")
	virtual void OnItem2BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnItem2Released();

	UFUNCTION(Category = "InputResponse")
	virtual void OnItem3BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnItem3Released();

	UFUNCTION(Category = "InputResponse")
	virtual void OnItem4BeginClick();
	UFUNCTION(Category = "InputResponse")
	virtual void OnItem4Released();


#pragma endregion

#pragma region RPCs
public:
	UFUNCTION(Server, Reliable, Category = "CharacterSkill")
	void ServerPlaySkillMontage(UAnimMontage* Montage, FName SectionName, int SkillID);

	UFUNCTION(Server, Reliable, Category = "CharacterSkill")
	void ServerPlayComboMontage(UAnimMontage* Montage, int SkillID);


	// ȸ�Ǹ�� ���ø�����Ʈ
	UFUNCTION(Server, Reliable, Category = "Evade")
	void Server_OnEvadeBeginClick(FVector StartLocation, FVector EndLocation);

	UFUNCTION(NetMulticast, Reliable, Category = "Evade")
	void Multi_OnEvadeBeginClick(FVector StartLocation, FVector EndLocation);

	// ���� ����ü
	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(const FVector& Start, const FRotator& End, const FVector& Dir, TSubclassOf<AMMBaseProjectile> InAttackProjectileClass);

	UFUNCTION(Client, Reliable)
	void ClientInitSkillIMC(int SkillID);

	UFUNCTION(Client, Reliable)
	void ClientDeleteSkillIMC(int SkillID);

#pragma endregion


public:
	UFUNCTION(Category = "CharacterSkill")
	virtual void MakeInitialSkills(); // ���� ��ų �ν��Ͻ� �������ִ� �κ�. ��ų ���� ���� �Ÿ� �ʼ� ����

	UFUNCTION(Category = "CharacterSkill")
	virtual void GetWeaponOverlappingActors(TArray<AActor*>& OutOverlappingActors, TSubclassOf<AActor> ClassFilter = nullptr);

	UFUNCTION(Category = "CharacterSkill")
	void CancleOtherPreparedSkills(UMMBaseSkill* Exception);

	UFUNCTION(Category = "CharacterSkill")
	FAttackData GetCurrentAnimatedSkillAttackData(int Index) const;

	UFUNCTION(Category = "CharacterSkill")
	void ResetSkillAttackDataIndex();
	UFUNCTION(Category = "CharacterSkill")
	void IncreaseSkillAttackDataIndex();
	UFUNCTION(Category = "CharacterSkill")
	int GetSkillAttackDataIndex() const;
	UFUNCTION(Category = "CharacterSkill")
	void SetCurrentAnimatedSkill(UMMBaseSkill* Skill);
	UFUNCTION(Category = "CharacterSkill")
	UMMBaseSkill* GetCurrentAnimatedSkill() const;

	UFUNCTION(Category = "DamageHandler")
	UMMDamagableComponent* GetDamagableComponent() const;

	UFUNCTION(Category = "Evade")
	void OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	const FVector& GetEvadeDirection() const;

	float GetEvadeDistance() const;
	void SetEvadeDistance(float value);
	void SetEvadeDuration(float value);
	UAnimMontage* GetPerfectDodgeMontage() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	TMap<int, UMMBaseSkill*> SkillMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSkill")
	UMMBaseSkill* Skill1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSkill")
	UMMBaseSkill* Skill2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSkill")
	UMMBaseSkill* Skill3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSkill")
	UMMBaseSkill* IdentitySkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* CurrentAnimatedSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	int SkillAttackDataIndex = 0;

	// ������ �� �̵��� ����� ��� ������
	// �Է¹��� ���� (����ȭ�� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade")
	FVector EvadeDirection;

	// ������ ���� ��ġ�� ��ǥ ��ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade")
	FVector EvadeStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade")
	FVector EvadeTargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* EvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* PerfectDodgeMontage;

	// ������ ���� ���� �� Ÿ�̸�(������ ��ü �ҿ� �ð�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade")
	bool bIsEvading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	float EvadeDistance = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	float EvadeDuration = 0.5f;

	// ���� �ð� (������ ���� �ð�)
	float EvadeElapsedTime = 0.f;



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageHandler", meta = (AllowPrivateAccess = "true"))
	UMMDamagableComponent* _DamagableComponent;

};
