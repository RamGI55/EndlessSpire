// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TitleGameMode.h"
#include "PlayerControllers/TitlePlayerController.h"
#include "HUDs/TitleHUD.h"

ATitleGameMode::ATitleGameMode()
{
	PlayerControllerClass = ATitlePlayerController::StaticClass();
	HUDClass = ATitleHUD::StaticClass();
}
