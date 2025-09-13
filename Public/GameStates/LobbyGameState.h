// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStates/PlayGameState.h"
#include "UStructs/FInfoOfPlayers.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ALobbyGameState : public APlayGameState
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

public:
	ALobbyGameState();

	UFUNCTION()
	void OpenLevel();

	UPROPERTY(Replicated)
	int32 Stage;

	UFUNCTION()
	void UpdatePlayerInfo(const TArray<FInfoOfPlayers>& playerinformations);

	UPROPERTY(ReplicatedUsing = OnRep_PlayerInformations)
	TArray<FInfoOfPlayers> PlayerInformations;

	UFUNCTION()
	void OnRep_PlayerInformations();
	
	UPROPERTY(Replicated)
	TArray<int32> VoteArray;

	UFUNCTION()
	void AddVoteArray(int32 result);
};
