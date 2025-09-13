// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NonPlayers/MobMonsterCharacter.h"


#include "ActorComponents/EnemyDamagableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "AI/AIController/MobAIController.h"
#include "UObjects/EnemySkills/Mobs/MobsBasicAttackSkill.h"
#include "UObjects/EnemySkills/Mobs/MobsBuffSkill.h"
#include "UObjects/EnemySkills/Mobs/MobsStrongAttackSkill.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "Widgets/MonsterHealthBar.h"
#include "Components/WidgetComponent.h"


class UAISense_Sight;

AMobMonsterCharacter::AMobMonsterCharacter()
{
	
	// NPC Status //
	
	NPCStat.Atk = 15.f;
	NPCStat.DropGold = FMath::RandRange(45, 300);
	NPCStat.MaxHp = 300.f;
	NPCStat.CurHp = NPCStat.MaxHp;
	NPCStat.MoveSpeed = 600.f;
	NPCStat.Def = 0.5f;
	NPCStat.AttackSpeed = 0.5f;
	NPCStat.CriticalChance = 0.25f;
	NPCStat.CriticalHitDamage = 2.f;
	
	// Movement Component initalisation //

	GetCharacterMovement()->MaxWalkSpeed = NPCStat.MoveSpeed;

	// AI Controller initalisation //
	AIControllerClass = AMobAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> NormalStiffMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/NonPlayers/Monster/Animations/Mobs/Mobs_Stiffness_Front.Mobs_Stiffness_Front'"));
	if (NormalStiffMontageAsset.Succeeded()) {
		DamagableComponent->NormalFrontStiffMontage = NormalStiffMontageAsset.Object;
	}
	if (NormalStiffMontageAsset.Succeeded()) {
		DamagableComponent->NormalFrontStiffMontage = NormalStiffMontageAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("NormalFrontStiffMontage loaded successfully"));
	} else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load NormalFrontStiffMontage"));
	}


	ConstructorHelpers::FObjectFinder<UAnimMontage> StrongStiffMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/NonPlayers/Monster/Animations/Mobs/Mobs_Stiffness_Front_Strong.Mobs_Stiffness_Front_Strong'"));
	if (StrongStiffMontageAsset.Succeeded()) {
		DamagableComponent->StrongFrontStiffMontage = StrongStiffMontageAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("StrongFrontStiffMontage loaded successfully"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load StrongFrontStiffMontage"));
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimationAsset(TEXT("'/Game/NonPlayers/Monster/Animations/Mobs/Zombie_DeathMobAnim_Montage.Zombie_DeathMobAnim_Montage'"));
	if (DeadAnimationAsset.Succeeded())
	{
		DeathMontage = DeadAnimationAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load DeathMontage"));
	}
}

void AMobMonsterCharacter::MakeInitialSkills()
{
	if (!BasicAttackSkill)
	{
		BasicAttackSkill = NewObject<UMobsBasicAttackSkill>(this);
		SkillMap.Add(0, BasicAttackSkill);
	}
	
	if (!StrongAttackSkill)
	{
		StrongAttackSkill = NewObject<UMobsStrongAttackSkill>(this);
		SkillMap.Add(1, StrongAttackSkill);
	}
	
	if (!BuffSkill)
	{
		BuffSkill = NewObject<UMobsBuffSkill>(this);
		SkillMap.Add(2, BuffSkill);
		
	}
	
	BasicAttackSkill->InitializeSkill(this ,0);
	StrongAttackSkill->InitializeSkill(this ,1);
	BuffSkill->InitializeSkill(this ,2);
}

void AMobMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Blackboard = GetBlackBoard();


	
}

void AMobMonsterCharacter::ExcuteBaskcAttackSkill()
{
	BasicAttackSkill->OnBeginExecute(); 
}

void AMobMonsterCharacter::ExcuteStrongAttackSkill()
{
	StrongAttackSkill->OnBeginExecute();
}

void AMobMonsterCharacter::ExcuteBuffSkill()
{
	BuffSkill->OnBeginExecute();
}











