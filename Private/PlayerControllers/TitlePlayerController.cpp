// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/TitlePlayerController.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();


	// InputMode UI�θ�
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = true;
}
