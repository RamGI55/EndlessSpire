// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/StagePlayerController.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Constructor
AStagePlayerController::AStagePlayerController()
{
	// InputMappingContext 에셋 오브젝트 바인딩
	ConstructorHelpers::FObjectFinder<UInputMappingContext>ItemMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/UseItem/UseItemMappingContext.UseItemMappingContext'"));
	if (ItemMappingContextObject.Succeeded())
	{
		ItemMappingContext = ItemMappingContextObject.Object;
	}

	// InputAction 에셋 오브젝트 바인딩
	ConstructorHelpers::FObjectFinder<UInputAction> ItemActionObject1(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/UseItem/UseItem1.UseItem1'"));
	if (ItemActionObject1.Succeeded())
	{
		ItemAction1 = ItemActionObject1.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> ItemActionObject2(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/UseItem/UseItem2.UseItem2'"));
	if (ItemActionObject2.Succeeded())
	{
		ItemAction2 = ItemActionObject2.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> ItemActionObject3(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/UseItem/UseItem3.UseItem3'"));
	if (ItemActionObject3.Succeeded())
	{
		ItemAction3 = ItemActionObject3.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> ItemActionObject4(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/UseItem/UseItem4.UseItem4'"));
	if (ItemActionObject4.Succeeded())
	{
		ItemAction4 = ItemActionObject4.Object;
	}
}

#pragma region Overrides
void AStagePlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set MappingContext
	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(ItemMappingContext, 0);
	}
}

void AStagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Input Action Bind
	if (EnhancedInputComponent) {
		// Evade
		EnhancedInputComponent->BindAction(ItemAction1, ETriggerEvent::Started, this, &AStagePlayerController::OnItem1BeginClicked);
		EnhancedInputComponent->BindAction(ItemAction1, ETriggerEvent::Completed, this, &AStagePlayerController::OnItem1EndClicked);

		EnhancedInputComponent->BindAction(ItemAction2, ETriggerEvent::Started, this, &AStagePlayerController::OnItem2BeginClicked);
		EnhancedInputComponent->BindAction(ItemAction2, ETriggerEvent::Completed, this, &AStagePlayerController::OnItem2EndClicked);

		EnhancedInputComponent->BindAction(ItemAction3, ETriggerEvent::Started, this, &AStagePlayerController::OnItem3BeginClicked);
		EnhancedInputComponent->BindAction(ItemAction3, ETriggerEvent::Completed, this, &AStagePlayerController::OnItem3EndClicked);

		EnhancedInputComponent->BindAction(ItemAction4, ETriggerEvent::Started, this, &AStagePlayerController::OnItem4BeginClicked);
		EnhancedInputComponent->BindAction(ItemAction4, ETriggerEvent::Completed, this, &AStagePlayerController::OnItem4EndClicked);
	}
}

#pragma endregion

#pragma region InputMappings
void AStagePlayerController::OnItem1BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem1BeginClick();
	}
}

void AStagePlayerController::OnItem1EndClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem1Released();
	}
}

void AStagePlayerController::OnItem2BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem2BeginClick();
	}
}

void AStagePlayerController::OnItem2EndClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem2Released();
	}
}

void AStagePlayerController::OnItem3BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem3BeginClick();
	}
}

void AStagePlayerController::OnItem3EndClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem3Released();
	}
}

void AStagePlayerController::OnItem4BeginClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem4BeginClick();
	}
}

void AStagePlayerController::OnItem4EndClicked()
{
	ABaseCombatCharacter* controlledPlayerCharacter = Cast<ABaseCombatCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnItem4Released();
	}
}

#pragma endregion

