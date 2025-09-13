// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PlayGameMode.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "GameStates/PlayGameState.h"
#include "PlayerStates/PlayPlayerState.h"
#include "HUDs/PlayHUD.h"

APlayGameMode::APlayGameMode()
{
	PlayerControllerClass = APlayPlayerController::StaticClass();
	GameStateClass = APlayGameState::StaticClass();
	PlayerStateClass = APlayPlayerState::StaticClass();
	HUDClass = APlayHUD::StaticClass();

	DefaultPawnClass = nullptr;

}
