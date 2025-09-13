// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/BaseNonPlayerCharacter.h"
#include "UStructs/FNonPlayerCharacterStat.h"
#include "BaseMonsterCharacter.generated.h"

class UEnemyRandomCounter;
class UMMEnemyStatComponent;
struct FNonPlayerCharacterStat;
class UEnemyDamagableComponent;
class UBehaviorTree;
class UBlackboardComponent; 
class UEnemyBaseSkill;
class UWidgetComponent;

UCLASS()
class ENDLESSSPIRE_API ABaseMonsterCharacter : public ABaseNonPlayerCharacter
{
	GENERATED_BODY()
	
public:
	ABaseMonsterCharacter();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	void DeadEvent();

	virtual void UpdateStat(FNonPlayerCharacterStat Stat);

	UFUNCTION()
	UBehaviorTree* GetBehaviorTree() const;

	UFUNCTION()
	UBlackboardComponent* GetBlackBoard() const;
	
	UFUNCTION(Category = "Enemy Skill")
	virtual void GetWeaponOverlappingActors(TArray<AActor*>& OutOverlappingActors, TSubclassOf<AActor> ClassFilter = nullptr);

	UFUNCTION(Category = "Enemy Skill")
	// 캐릭터에 스킬 인스턴스를 생성해주는 로직을 작성하시오
	virtual void MakeInitialSkills();

protected: 
	UFUNCTION(Category = "AI Behaviour")
	void OnPawnDetected(AActor* TargetPlayer);

	UFUNCTION()
	void SetCurrentAnimatedSkill(UEnemyBaseSkill* Skill);

	void InitStat();


#pragma region RPCs

public:
	UFUNCTION(Server, Reliable, Category = "CharacterSkill")
	void ServerPlaySkillMontage(UAnimMontage* Montage, FName SectionName, int SkillID);


#pragma endregion

public:
	UPROPERTY()
	TObjectPtr<UEnemyDamagableComponent> DamagableComponent;

	UPROPERTY(EditAnywhere, Category = "CharacterStat")
	FNonPlayerCharacterStat NPCStat;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMMEnemyStatComponent> EnemyStatComp;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName DetectedActorKeyName = FName("TargetPlayer");

	UEnemyBaseSkill* GetCurrentAnimatedSkill() const;

	//드랍아이템 ID 리플
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	

	void SpawnRandomItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Skill")
	TMap<int, UEnemyBaseSkill*> SkillMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Skill")
	UEnemyBaseSkill* CurrentAnimatedSkill;

	UPROPERTY()
	int32 AttackStack; 

	UPROPERTY()
	TObjectPtr<UEnemyRandomCounter> RandomCounter;  
	
	//드랍테이블 임시
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Replicated)
	TArray<FName> SpawnItemsID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar;

};
