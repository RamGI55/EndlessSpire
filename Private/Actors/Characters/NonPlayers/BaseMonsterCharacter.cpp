// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"

#include "ActorComponents/EnemyDamagableComponent.h"
#include "ActorComponents/EnemyRandomCounter.h"
#include "ActorComponents/MMEnemyStatComponent.h"
#include "Widgets/MonsterHealthBar.h"
#include "Components/WidgetComponent.h"
#include "AI/AIController/MobAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UStructs/FItem.h"
#include "Actors/MMPickUp.h"




ABaseMonsterCharacter::ABaseMonsterCharacter()
{
	bReplicates = true;

	// Capsule Component initalisation //
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel3);
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	
	// Damagable Component initalisation // 
	DamagableComponent = CreateDefaultSubobject<UEnemyDamagableComponent>(TEXT("DamagableComponent"));
	DamagableComponent->GetComponentClassCanReplicate();
	
	EnemyStatComp = CreateDefaultSubobject<UMMEnemyStatComponent>(TEXT("EnemyStatComponent"));
	EnemyStatComp->GetComponentClassCanReplicate();

	//GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimationAsset(TEXT(""));
	//if (DeadAnimationAsset.Succeeded())
	//{
	//	// store the dead animation.
	//}

	RandomCounter = CreateDefaultSubobject<UEnemyRandomCounter>(TEXT("RandomCounter"));

	// ��������� ������ ���̺�
	ConstructorHelpers::FObjectFinder<UDataTable> TempItemDataTable(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	if (TempItemDataTable.Succeeded())
	{
		ItemDataTable = TempItemDataTable.Object;
	}

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBar->SetupAttachment(GetMesh());

	HealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);

	ConstructorHelpers::FClassFinder<UUserWidget> UI_HealthBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_MonsterHealthBar.WBP_MonsterHealthBar_C'"));
	if (UI_HealthBar.Succeeded())
	{
		HealthBar->SetWidgetClass(UI_HealthBar.Class);
		HealthBar->SetDrawSize(FVector2D(300.0f, 30.0f));
	}

}

void ABaseMonsterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitStat(); // ������Ʈ ���� �ʱ�ȭ �Ϸ�� ���� ȣ���

	
}


void ABaseMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	MakeInitialSkills();

	//��������� ���̺� ����
	if (HasAuthority())
	{

		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		int32 RandomCount = 3; //FMath::RandRange(1, 5);

		for (int i = 0; i < RandomCount; i++)
		{
			if (RowNames.Num() > 0)
			{
				// ������ �ε��� ����
				int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);
				FName RandomRowName = RowNames[RandomIndex];

				// ������ �� ��������
				FItemData* RandomRow = ItemDataTable->FindRow<FItemData>(RandomRowName, TEXT("RandomRow"));
				if (RandomRow)
				{
					SpawnItemsID.Add(RandomRowName);
				}
			}
		}
	}

	if (HealthBar)
	{

		UUserWidget* Widget = HealthBar->GetUserWidgetObject();
		if (UMonsterHealthBar* MHealthBar = Cast<UMonsterHealthBar>(Widget))
		{
			MHealthBar->UpdateState(NPCStat);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MHealthBar = Cast<UMonsterHealthBar>(HealthBar) : Cast Failed"))
		}
	}


}

void ABaseMonsterCharacter::DeadEvent()
{
	Super::DeadEvent();

	SpawnRandomItem();

}

void ABaseMonsterCharacter::UpdateStat(FNonPlayerCharacterStat Stat)
{
	NPCStat = Stat;

	if (HealthBar)
	{
		UUserWidget* Widget = HealthBar->GetUserWidgetObject();
		if (UMonsterHealthBar* MHealthBar = Cast<UMonsterHealthBar>(Widget))
		{
			MHealthBar->UpdateState(NPCStat);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MHealthBar = Cast<UMonsterHealthBar>(HealthBar) : Cast Failed"))
		}
	}




}


UBehaviorTree* ABaseMonsterCharacter::GetBehaviorTree() const
{
	return BehaviorTree; 
}

UBlackboardComponent* ABaseMonsterCharacter::GetBlackBoard() const
{
	AMobAIController* AIController = Cast<AMobAIController>(GetController());
	if (AIController)
	{
		return AIController->GetBlackboardComponent();
	}
	
	return nullptr; 
}

void ABaseMonsterCharacter::GetWeaponOverlappingActors(TArray<AActor*>& OutOverlappingActors, TSubclassOf<AActor> ClassFilter)
{

}


void ABaseMonsterCharacter::OnPawnDetected(AActor* TargetPlayer)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && Blackboard)
	{
		Blackboard->SetValueAsObject(("TargetPlayer"), TargetPlayer);
		
	}
}

void ABaseMonsterCharacter::MakeInitialSkills()
{
	/*
	* �ڽ� Ŭ�������� ������ ���� �������� �������̵� �ʿ�
	// BasicAttackSkill�� ��� ����
	if (!BasicAttackSkill) {
		BasicAttackSkill = NewObject<UEnemyBaseSkill�� ��� ���� �ֵ�>(this);
		SkillMap.Add(0, BasicAttackSkill);
	}
	BasicAttackSkill->InitializeSkill(this, 0);
	*/
}

void ABaseMonsterCharacter::ServerPlaySkillMontage_Implementation(UAnimMontage* Montage, FName SectionName, int SkillID)
{
	if (CanSwitchAnimation()) {
		UEnemyBaseSkill** FoundSkill = SkillMap.Find(SkillID);
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

UEnemyBaseSkill* ABaseMonsterCharacter::GetCurrentAnimatedSkill() const
{
	return CurrentAnimatedSkill;
}


void ABaseMonsterCharacter::SetCurrentAnimatedSkill(UEnemyBaseSkill* Skill)
{
	CurrentAnimatedSkill = Skill;
}

void ABaseMonsterCharacter::InitStat()
{
	
	if (EnemyStatComp)
	{
		EnemyStatComp->SetEnemyStat(NPCStat);
	}
}


void ABaseMonsterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseMonsterCharacter, SpawnItemsID);

}

void ABaseMonsterCharacter::SpawnRandomItem()
{

	// ������ ���̺����� ��� ���� ��������
	if (!HasAuthority())
	{
		return;
	}

	for (FName Element : SpawnItemsID)
	{
		// ���� ���������� ��ġ ����
		FVector SpawnLocation = GetActorLocation() + FVector(FMath::FRandRange(-50.f, 50.f), FMath::FRandRange(-50.f, 50.f), 100.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMMPickUp* SpawnedItem = GetWorld()->SpawnActor<AMMPickUp>(AMMPickUp::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedItem)
		{
			SpawnedItem->BroadcastItemDataInitialize(Element, 1);
		}

		if (SpawnedItem && SpawnedItem->GetRootComponent())
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnedItem  && SpawnedItem->GetRootComponent(): cast Success"));

			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(SpawnedItem->GetRootComponent());
			if (PrimComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("PrimComp : cast Success"));

				// ����: �������� 500.f ��ŭ�� ���޽��� �߰��Ͽ� �߻� ȿ�� �ο�
				FVector LaunchImpulse = FVector(0.f, 0.f, 0.f);
				//PrimComp->AddImpulse(LaunchImpulse);



			}
		}
	}


}
