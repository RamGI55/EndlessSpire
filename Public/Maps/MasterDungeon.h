// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/DungeonEventComponent.h"
#include "Maps/MasterRoom.h"
#include "GameFramework/Actor.h"
#include "MasterDungeon.generated.h"

UCLASS()
class ENDLESSSPIRE_API AMasterDungeon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMasterDungeon();
	UFUNCTION(BlueprintCallable)
	void SpawnDungeon();


private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HideWall();

	FTimerHandle THHideWall;

	AMasterRoom* MasterRoom;

public:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<AActor>> TSpawnMasterDungeons;
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	AActor* FirstDungeon;
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	AActor* PreviousDungeon;
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TSubclassOf<class UObject> TSpawnDungeon;
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TArray<AActor*> RoomList;
	UPROPERTY(EditAnywhere, Category = "Dungeon")
	int32 DungeonNumber;
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	int32 GenerateCounter;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TArray<FVector> RoadLocations;
	
};
