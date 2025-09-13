// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NonPlayers/SkeletonMonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/MMEnemyStatComponent.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "AI/AIController/SkeletonAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASkeletonMonsterCharacter::ASkeletonMonsterCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// 컴포넌트
	/*EnemyStatCompoenet = CreateDefaultSubobject<UMMEnemyStatComponent>(TEXT("EnemyStatCompoenet"));
	EnemyStatCompoenet->GetComponentClassCanReplicate();
	*/

	/*DamagableComponent = CreateDefaultSubobject<UMMDamagableComponent>(TEXT("DamagableComponent"));
	DamagableComponent->GetComponentClassCanReplicate();*/

	// 스탯
	Stat.Atk = 15.f;
	Stat.DropGold = FMath::RandRange(45, 300);
	Stat.MaxHp = 300.f;
	Stat.CurHp = Stat.MaxHp;
	Stat.MoveSpeed = 410.f;
	Stat.Def = 0.5f;
	Stat.AttackSpeed = 0.5f;
	Stat.CriticalChance = 0.25f;
	Stat.CriticalHitDamage = 35.f;

	GetCharacterMovement()->MaxWalkSpeed = Stat.MoveSpeed;

	// 부위 별 메쉬 설정
#pragma region SkeletonByParts
	//부위별로
	BaseBone = GetMesh(); // 메인 메시 (AnimInstance 붙는 곳)
	BaseBone->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	BaseBone->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	BaseBone->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Game/Animations/Skeleton/ABP_MonsterSkeletonAnimInstance.ABP_MonsterSkeletonAnimInstance_C"));
	//if (AnimRef.Succeeded())
	//{
	//	BaseBone->SetAnimInstanceClass(AnimRef.Class);
	//}


	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("SkeletalMesh'/Game/Assets/Skeleton_Guard/Mesh_UE4/Full/SKM_Skeleton_Guard_Body.SKM_Skeleton_Guard_Body'"));
	if (MeshRef.Succeeded())
	{
		BaseBone->SetSkeletalMesh(MeshRef.Object);
	}
	*/


	/*ShinLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShinLt"));
	ShinLt->SetupAttachment(RootComponent);
	ShinLt->SetLeaderPoseComponent(BaseBone);
	ShinLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ShinLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShinLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	ShinRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShinRt"));
	ShinRt->SetupAttachment(RootComponent);
	ShinRt->SetLeaderPoseComponent(BaseBone);
	ShinRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ShinRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShinRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	ElbowLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ElbowLt"));
	ElbowLt->SetupAttachment(RootComponent);
	ElbowLt->SetLeaderPoseComponent(BaseBone);
	ElbowLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ElbowLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ElbowLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	ShoulderRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoulderRt"));
	ShoulderRt->SetupAttachment(RootComponent);
	ShoulderRt->SetLeaderPoseComponent(BaseBone);
	ShoulderRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ShoulderRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShoulderRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	TibiaLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TibiaLt"));
	TibiaLt->SetupAttachment(RootComponent);
	TibiaLt->SetLeaderPoseComponent(BaseBone);
	TibiaLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	TibiaLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	TibiaLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	TibiaRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TibiaRt"));
	TibiaRt->SetupAttachment(RootComponent);
	TibiaRt->SetLeaderPoseComponent(BaseBone);
	TibiaRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	TibiaRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	TibiaRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	WristsLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WristsLt"));
	WristsLt->SetupAttachment(RootComponent);
	WristsLt->SetLeaderPoseComponent(BaseBone);
	WristsLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	WristsLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	WristsLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	WristsRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WristsRt"));
	WristsRt->SetupAttachment(RootComponent);
	WristsRt->SetLeaderPoseComponent(BaseBone);
	WristsRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	WristsRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	WristsRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	CollarboneLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CollarboneLt"));
	CollarboneLt->SetupAttachment(RootComponent);
	CollarboneLt->SetLeaderPoseComponent(BaseBone);
	CollarboneLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	CollarboneLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CollarboneLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	Jaw = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jaw"));
	Jaw->SetupAttachment(RootComponent);
	Jaw->SetLeaderPoseComponent(BaseBone);
	Jaw->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	Jaw->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Jaw->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	CollarboneRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CollarboneRt"));
	CollarboneRt->SetupAttachment(RootComponent);
	CollarboneRt->SetLeaderPoseComponent(BaseBone);
	CollarboneRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	CollarboneRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CollarboneRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	HipRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HipRt"));
	HipRt->SetupAttachment(RootComponent);
	HipRt->SetLeaderPoseComponent(BaseBone);
	HipRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	HipRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	HipRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	HipRt1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HipRt1"));
	HipRt1->SetupAttachment(RootComponent);
	HipRt1->SetLeaderPoseComponent(BaseBone);
	HipRt1->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	HipRt1->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	HipRt1->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	HipLt1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HipLt1"));
	HipLt1->SetupAttachment(RootComponent);
	HipLt1->SetLeaderPoseComponent(BaseBone);
	HipLt1->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	HipLt1->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	HipLt1->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	Skull = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skull"));
	Skull->SetupAttachment(RootComponent);
	Skull->SetLeaderPoseComponent(BaseBone);
	Skull->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	Skull->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Skull->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	ShoulderLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoulderLt"));
	ShoulderLt->SetupAttachment(RootComponent);
	ShoulderLt->SetLeaderPoseComponent(BaseBone);
	ShoulderLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ShoulderLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShoulderLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	HipLt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HipLt"));
	HipLt->SetupAttachment(RootComponent);
	HipLt->SetLeaderPoseComponent(BaseBone);
	HipLt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	HipLt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	HipLt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


	ElbowRt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ElbowRt"));
	ElbowRt->SetupAttachment(RootComponent);
	ElbowRt->SetLeaderPoseComponent(BaseBone);
	ElbowRt->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	ElbowRt->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ElbowRt->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));*/

#pragma endregion
	
	// AI
	AIControllerClass = ASkeletonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



}

void ASkeletonMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();


}
