// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DungeonEventComponent.h"
#include "Actors/Characters/NonPlayers/WanderingTraderNonPlayer.h"
#include "Actors/Characters/NonPlayers/MobMonsterCharacter.h"
#include "Actors/InteractActors/InteractChest.h"
#include "Maps/MasterDungeon.h"

// Sets default values for this component's properties
UDungeonEventComponent::UDungeonEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AMobMonsterCharacter> MonsterAsset(TEXT("/Script/Engine.Blueprint'/Game/NonPlayers/Monster/BPMobMonsterCharacter.BPMobMonsterCharacter_C'"));
	if (MonsterAsset.Succeeded())
	{
		MonsterClass = MonsterAsset.Class;
	}
}

void UDungeonEventComponent::GetEvent()
{
	//Get a random event name
	int Rnd = FMath::RandRange(0, 1000);
	if (Rnd < 350)
	{
		EventName = EDungeonEvent::ENEMY;
	}
	else if (350 <= Rnd && Rnd < 550)
	{
		EventName = EDungeonEvent::TREASURE;
	}
	else if (550 <= Rnd && Rnd < 670)
	{
		EventName = EDungeonEvent::TRAP;
	}
	else if (670 <= Rnd && Rnd < 770)
	{
		EventName = EDungeonEvent::SHOP;
	}
	else if (770 <= Rnd && Rnd < 820)
	{
		EventName = EDungeonEvent::BOSS;
	}
	else
	{
		EventName = EDungeonEvent::NONE;
	}

	//int32 i = FMath::RandRange(0, EventNames.Num() - 1);
	UE_LOG(LogTemp, Display, TEXT("EventName %i"), Rnd);

	// using Case.
	
}

void UDungeonEventComponent::StartEvent()
{
	int Rnd;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector Location = GetOwner()->GetActorLocation();
	Location.Z += 200.0f;
	FRotator Rotation = GetOwner()->GetActorRotation();

	switch (EventName)
	{
	case EDungeonEvent::NONE:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("NONE"));
		break;
	case EDungeonEvent::ENEMY:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ENEMY"));

		Rnd = FMath::RandRange(1, 3);

		for (int i = 0; i < Rnd; i++)
		{
			GetWorld()->SpawnActor<AMobMonsterCharacter>(MonsterClass, Location, Rotation, SpawnParams);
		}

		break;

	case EDungeonEvent::TREASURE:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("TREASURE"));

		Location.Z -= 100.0f;
		Chest = GetWorld()->SpawnActor<AInteractChest>(AInteractChest::StaticClass(), Location, Rotation, SpawnParams);
		break;

	case EDungeonEvent::TRAP:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("TRAP"));
		break;

	case EDungeonEvent::SHOP:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SHOP"));

		WanderingTrader = GetWorld()->SpawnActor<AWanderingTraderNonPlayer>(AWanderingTraderNonPlayer::StaticClass(), Location, Rotation, SpawnParams);

		break;

	case EDungeonEvent::BOSS:
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("BOSS"));

		break;

	default:
		break;
	}
}




