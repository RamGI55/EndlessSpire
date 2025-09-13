// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates/PlayPlayerState.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "UStructs/FInfoOfPlayers.h"
#include "UStructs/FItem.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API ALobbyPlayerState : public APlayPlayerState
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerState();

	UFUNCTION()
	void SetPlayerStat(FPlayerCharacterStat NewStat);

	const FPlayerCharacterStat& GetPlayerBaseStat() const;

	UPROPERTY()
	FGuid PlayerGuid;

	UPROPERTY()
	TArray<FItemData> PlayerItems;

	UPROPERTY()
	int32 PlayerMoney;

	UPROPERTY(Replicated)
	FPlayerCharacterStat PlayerBaseStat;


protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
};
