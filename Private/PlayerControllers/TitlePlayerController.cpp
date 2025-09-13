// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/TitlePlayerController.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();


	// InputMode UI·Î¸¸
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = true;
}
