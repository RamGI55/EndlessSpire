// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/StageGameMode.h"
#include "PlayerControllers/StagePlayerController.h"
#include "GameStates/StageGameState.h"
#include "PlayerStates/StagePlayerState.h"
#include "HUDs/StageHUD.h"

AStageGameMode::AStageGameMode()
{
	PlayerControllerClass = AStagePlayerController::StaticClass();
	GameStateClass = AStageGameState::StaticClass();
	PlayerStateClass = AStagePlayerState::StaticClass();
	HUDClass = AStageHUD::StaticClass();

	DefaultPawnClass = nullptr;
}
