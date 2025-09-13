// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/EnemyRandomCounter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values for this component's properties
UEnemyRandomCounter::UEnemyRandomCounter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyRandomCounter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	float CombatInitalDelay = FMath::RandRange(0.f, CounterUpdateInterval); 
	GetWorld()->GetTimerManager().SetTimer(CombatTimerHandle, this, &UEnemyRandomCounter::UpdateCombatCounter,
		CounterUpdateInterval, true, CombatInitalDelay); 
}

void UEnemyRandomCounter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CombatTimerHandle);
	}
	
}

void UEnemyRandomCounter::SetBlackBoardRef()
{

	TObjectPtr<AActor> EnemyActor = GetOwner();
	if (!EnemyActor ) return;

	OwnerController = Cast<AAIController>(EnemyActor );
	if (!OwnerController)
	{
		OwnerController = Cast<AAIController>(EnemyActor ->GetInstigatorController());
		
	}
	if (OwnerController)
	{
		BlackBoardComp = OwnerController->GetBlackboardComponent();
	}
	
	
}

void UEnemyRandomCounter::UpdateCombatCounter()
{
	if (!BlackBoardComp || !OwnerController)
	{
		SetBlackBoardRef();
	}
	if (BlackBoardComp)
	{
		BlackBoardComp->SetValueAsInt(CombatCounterKeyName, FMath::RandRange(0, 100));
	}
}




