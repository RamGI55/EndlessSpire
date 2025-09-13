// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerStatInterface.h"
#include "GameStates/LobbyGameState.h"
#include "PlayerControllers/LobbyPlayerController.h"

void UPlayerStatInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurrentHP = 0;
	MaxHP = 1;
	CurrentMP = 0;
	MaxMP = 1;
	CurrentStamina = 0;
	MaxStamina = 1;
	CurrentEXP = 0;
	MaxEXP = 1;
	CurrentLevel = 0;

}


void UPlayerStatInterface::UpdateWidget(FPlayerCharacterStat& playerstat)
{
	CurrentHP = playerstat.CurHp;
	MaxHP = playerstat.MaxHp;
	CurrentMP = playerstat.CurMp;
	MaxMP = playerstat.MaxMp;
	CurrentStamina = playerstat.CurSp;
	MaxStamina = playerstat.MaxSp;
	CurrentEXP = playerstat.CurExp;
	MaxEXP = playerstat.MaxExp;
	CurrentLevel = playerstat.Level;

}
