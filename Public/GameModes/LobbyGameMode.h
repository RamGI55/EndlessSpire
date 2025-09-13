// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/PlayGameMode.h"
#include "UStructs/FInfoOfPlayers.h"
#include "UEnums/EClass.h"
#include "LobbyGameMode.generated.h"


/**
 * 
 */

class ABasePlayerCharacter;

UCLASS()
class ENDLESSSPIRE_API ALobbyGameMode : public APlayGameMode
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

	virtual void PostInitializeComponents() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	UWorld* world;

	UPROPERTY()
	TArray<FInfoOfPlayers> PlayerInformations;

	UPROPERTY()
	TMap<EClass, TSubclassOf<ABasePlayerCharacter>> ClassMap;


public:
	UFUNCTION(BlueprintCallable)
	virtual void ServerTravle();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnActor(APlayerController* NewPlayer, EClass playerclass);

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerGuid(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerNickname(FGuid playerguid, FString playernickname);

	UFUNCTION(BlueprintCallable)
	virtual FString GetPlayerNickname(APlayerController* playercontroller);

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerClass(FGuid playerguid, EClass playerclass, APlayerController* playercontroller);

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABasePlayerCharacter> DefaultPlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABasePlayerCharacter> WarriorPlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABasePlayerCharacter> MagePlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABasePlayerCharacter> ArcherPlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABasePlayerCharacter> ClericPlayerCharacter;

};
