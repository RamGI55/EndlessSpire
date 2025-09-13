// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UEnums/EDungeonEvent.h"
#include "DungeonEventComponent.generated.h"

class AMasterDungeon;
class AWanderingTraderNonPlayer;
class AMobMonsterCharacter;
class AInteractChest;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSSPIRE_API UDungeonEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDungeonEventComponent();
	UFUNCTION(BlueprintCallable)
	void GetEvent();

	UFUNCTION(BlueprintCallable)
	void StartEvent();

protected:

public:
	UPROPERTY(VisibleAnywhere)
	AMasterDungeon* MasterDungeon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDungeonEvent EventName;

	AWanderingTraderNonPlayer* WanderingTrader;
	TSubclassOf<AMobMonsterCharacter> MonsterClass;
	AInteractChest* Chest;
		
	
};
