// Fill out your copyright notice in the Description page of Project Settings.


#include "Maps/MasterDungeon.h"
#include "Maps/MasterRoom.h"
#include "GameInstances/EndlessSpireGameInstance.h"
#include "Actors/MMStageStair.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMasterDungeon::AMasterDungeon()
{
	// Danger: hardcoded initialiser!
	ConstructorHelpers::FClassFinder<AActor> DungeonAsset0(TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_MasterRoom.BP_MasterRoom_C'"));
	if (DungeonAsset0.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset0.Class);
	}

	ConstructorHelpers::FClassFinder<AActor> DungeonAsset1 (TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_Room10.BP_Room10_C'"));
	if (DungeonAsset1.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset1.Class);
	}

	ConstructorHelpers::FClassFinder<AActor> DungeonAsset2(TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_Room18.BP_Room18_C'"));
	if (DungeonAsset2.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset2.Class);
	}

	ConstructorHelpers::FClassFinder<AActor> DungeonAsset3(TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_Room19.BP_Room19_C'"));
	if (DungeonAsset3.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset3.Class);
	}

	ConstructorHelpers::FClassFinder<AActor> DungeonAsset4(TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_Room20.BP_Room20_C'"));
	if (DungeonAsset4.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset4.Class);
	}

	ConstructorHelpers::FClassFinder<AActor> DungeonAsset5(TEXT("/Script/Engine.Blueprint'/Game/Maps/BP_RoomRuin.BP_RoomRuin_C'"));
	if (DungeonAsset5.Succeeded())
	{
		TSpawnMasterDungeons.Add(DungeonAsset5.Class);
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DungeonNumber = 5;
	GenerateCounter = 0;
}

void AMasterDungeon::SpawnDungeon()
{
	FActorSpawnParameters SpawnParameters;
	FRotator Rot(0, 0, 0);
	FVector Loc(0, 0, 0);

	TSpawnDungeon = (UClass*)TSpawnMasterDungeons[0];
	FActorSpawnParameters SpawnParams;
	FirstDungeon = GetWorld()->SpawnActor<AActor>(TSpawnDungeon, Loc, Rot, SpawnParameters);

	AActor* CurrentRoom = FirstDungeon;
	RoomList.Add(CurrentRoom);

	for (int i = 1; i <= DungeonNumber; i++)
	{
		if (!CurrentRoom)
		{
			break;
		}

		int Rnd = FMath::RandRange(1, TSpawnMasterDungeons.Num() - 2);
		TSpawnDungeon = (UClass*)TSpawnMasterDungeons[Rnd];

		AMasterRoom* CurrentMasterRoom = Cast<AMasterRoom>(CurrentRoom);
		if (CurrentMasterRoom)
		{
			// back to the previous room if the has now direction to go. 
			for (int j = i - 1; CurrentMasterRoom->GetRandDirection() == FVector::ZeroVector && j >= 0; --j)
			{
				CurrentRoom = RoomList[j];
				CurrentMasterRoom = Cast<AMasterRoom>(CurrentRoom);
			}

			FVector NextLoc = CurrentMasterRoom->GetRandDirection();
			NextLoc += CurrentRoom->GetActorLocation();
			AActor* NewRoom = GetWorld()->SpawnActor<AActor>(TSpawnDungeon, NextLoc, Rot, SpawnParams);

			RoomList.Add(NewRoom);
			CurrentRoom = NewRoom;

			if (i == DungeonNumber)
			{
				FVector StairLocation = CurrentRoom->GetActorLocation();
				StairLocation.Z += 100.0f;
				FRotator StairRotation = CurrentRoom->GetActorRotation();
				AActor* Stair = GetWorld()->SpawnActor<AActor>(AMMStageStair::StaticClass(), StairLocation, StairRotation, SpawnParams);
			}

			// if the collision isn't detected remove the bridge mesh. 
		}
	}
	
	AMasterRoom* Dungeon;
	for (AActor* Element : RoomList)
	{
		Dungeon = Cast<AMasterRoom>(Element);
		if (Dungeon)
		{
			if (Element != RoomList[0] && Element != RoomList.Last())
			{
				Dungeon->ActivateEvent();
				UE_LOG(LogTemp, Display, TEXT("asd %s"), *Element->GetName());
				GetWorld()->GetTimerManager().SetTimer(THHideWall, this, &AMasterDungeon::HideWall, 2.0f, false);
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Dungeon Generated; Try : %i:)"), GenerateCounter);
	GenerateCounter = 0;
}

// Called when the game starts or when spawned
void AMasterDungeon::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Before Spawn Dungeon"));

	if (HasAuthority())
	{
		UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GameInstance)
		{
			DungeonNumber += GameInstance->Stage / 2;

			UE_LOG(LogTemp, Display, TEXT("DungeonNumber %d"), DungeonNumber);

		}

		SpawnDungeon();
		UE_LOG(LogTemp, Display, TEXT("Dungeon Spawn End"));
	}

}

void AMasterDungeon::HideWall()
{
	AMasterRoom* Dungeon;

	for (AActor* Element : RoomList)
	{
		Dungeon = Cast<AMasterRoom>(Element);
		if (Dungeon)
		{
			Dungeon->HideWall();
		}
	}
}

