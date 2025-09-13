// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/PlayPlayerController.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"

#include "Net/UnrealNetwork.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Constructor
APlayPlayerController::APlayPlayerController()
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> BasicControlMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/BasicControlMappingContext.BasicControlMappingContext'"));
	if (BasicControlMappingContextObject.Succeeded())
	{
		BasicControlMappingContext = BasicControlMappingContextObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> BasicMoveMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/MoveMappingContext.MoveMappingContext'"));
	if (BasicMoveMappingContextObject.Succeeded())
	{
		BasicMoveMappingContext = BasicMoveMappingContextObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> LookMappingContextObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/LookControlMappingContext.LookControlMappingContext'"));
	if (LookMappingContextObject.Succeeded())
	{
		LookMappingContext = LookMappingContextObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/IA_PlayerBasicMove.IA_PlayerBasicMove'"));
	if (MoveActionObject.Succeeded())
	{
		MoveAction = MoveActionObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> RunActionObject(TEXT("/Script/EnhancedInput.InputAction'/Game/InputMapping/BasicControl/IA_Run.IA_Run'"));
	if (RunActionObject.Succeeded())
	{
		RunAction = RunActionObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LookActionObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/IA_PlayerLook.IA_PlayerLook'"));
	if (LookActionObject.Succeeded())
	{
		LookAction = LookActionObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LeftActionObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/IA_MouseLeft.IA_MouseLeft'"));
	if (LeftActionObject.Succeeded())
	{
		LeftMouse = LeftActionObject.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> RightActionObject(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputMapping/BasicControl/IA_MouseRight.IA_MouseRight'"));
	if (RightActionObject.Succeeded())
	{
		RightMouse = RightActionObject.Object;
	}
	

}

#pragma region Overrides
void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(BasicControlMappingContext, 0);
		EnhancedInputSubsystem->AddMappingContext(BasicMoveMappingContext, 0);
		EnhancedInputSubsystem->AddMappingContext(LookMappingContext, 0);
	}

}

void APlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Set up action bindings
	if (EnhancedInputComponent) {
		// Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayPlayerController::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayPlayerController::Move);

		// Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayPlayerController::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayPlayerController::StopRunning);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayPlayerController::Look);

		// Mouse Input
		// Left
		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Started, this, &APlayPlayerController::OnLeftMouseBeginClicked);
		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Triggered, this, &APlayPlayerController::OnLeftMouseTriggered);
		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Ongoing, this, &APlayPlayerController::OnLeftMouseHolding);
		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Canceled, this, &APlayPlayerController::OnLeftMouseCancled);
		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Completed, this, &APlayPlayerController::OnLeftMouseCompleted);
		// Right
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Started, this, &APlayPlayerController::OnRightMouseBeginClicked);
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Triggered, this, &APlayPlayerController::OnRightMouseTriggered);
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Ongoing, this, &APlayPlayerController::OnRightMouseHolding);
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Canceled, this, &APlayPlayerController::OnRightMouseCancled);
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Completed, this, &APlayPlayerController::OnRightMouseCompleted);
	}
}

void APlayPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APlayPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APlayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayPlayerController, MoveInput);
}

#pragma endregion

#pragma region InputMappings
void APlayPlayerController::Move(const FInputActionValue& Value)
{
	MoveInput.X = Value.Get<FVector2D>().X;
	MoveInput.Y = Value.Get<FVector2D>().Y;

	APawn* controlledPawn = GetPawn();
	if (controlledPawn) {
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);

		// Get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		RelativeMoveInput = ForwardDirection * Value.Get<FVector2D>().Y + RightDirection * Value.Get<FVector2D>().X;

		controlledPawn->AddMovementInput(ForwardDirection, Value.Get<FVector2D>().Y);
		controlledPawn->AddMovementInput(RightDirection, Value.Get<FVector2D>().X);
	}
}

void APlayPlayerController::Run()
{
	ABasePlayerCharacter* controlledPawn = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPawn) {
		controlledPawn->ServerCastRun();
	}
}

void APlayPlayerController::StopRunning()
{
	ABasePlayerCharacter* controlledPawn = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPawn) {
		controlledPawn->ServerCastStopRunning();
	}
}

void APlayPlayerController::Look(const FInputActionValue& Value)
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn) {
		controlledPawn->AddControllerYawInput(Value.Get<FVector2D>().X);
		controlledPawn->AddControllerPitchInput(-Value.Get<FVector2D>().Y);
	}
}

void APlayPlayerController::Jump()
{
	ACharacter* controlledCharacter = Cast<ACharacter>(GetPawn());
	if (controlledCharacter) {
		controlledCharacter->Jump();
	}
}

void APlayPlayerController::StopJumping()
{
	ACharacter* controlledCharacter = Cast<ACharacter>(GetPawn());
	if (controlledCharacter) {
		controlledCharacter->StopJumping();
	}
}

void APlayPlayerController::OnLeftMouseBeginClicked()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnLeftMouseBeginClick();
	}
}

void APlayPlayerController::OnLeftMouseTriggered(const FInputActionInstance& Instance)
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnLeftMouseHolding(true, Instance.GetElapsedTime());
	}
}

void APlayPlayerController::OnLeftMouseHolding(const FInputActionInstance& Instance)
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnLeftMouseHolding(false, Instance.GetElapsedTime());
	}
}

void APlayPlayerController::OnLeftMouseCancled()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnLeftMouseReleased(true);
	}
}

void APlayPlayerController::OnLeftMouseCompleted()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnLeftMouseReleased(false);
	}
}

void APlayPlayerController::OnRightMouseBeginClicked()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnRightMouseBeginClick();
	}
}

void APlayPlayerController::OnRightMouseTriggered(const FInputActionInstance& Instance)
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnRightMouseHolding(true, Instance.GetElapsedTime());
	}
}

void APlayPlayerController::OnRightMouseHolding(const FInputActionInstance& Instance)
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnRightMouseHolding(false, Instance.GetElapsedTime());
	}
}

void APlayPlayerController::OnRightMouseCancled()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnRightMouseReleased(true);
	}
}

void APlayPlayerController::OnRightMouseCompleted()
{
	ABasePlayerCharacter* controlledPlayerCharacter = Cast<ABasePlayerCharacter>(GetPawn());
	if (controlledPlayerCharacter) {
		controlledPlayerCharacter->OnRightMouseReleased(false);
	}
}

void APlayPlayerController::InitAllIMC()
{
	InitBasicControllIMC();
	InitBasicMoveIMC();
	InitLookIMC();
}

void APlayPlayerController::DeleteAllIMC()
{
	DeleteBasicControllIMC();
	DeleteBasicMoveIMC();
	DeleteLookIMC();
}

void APlayPlayerController::InitBasicControllIMC()
{
	if (EnhancedInputSubsystem)
	{
		if (!EnhancedInputSubsystem->HasMappingContext(BasicControlMappingContext)) {
			EnhancedInputSubsystem->AddMappingContext(BasicControlMappingContext, 0);
		}
	}
}

void APlayPlayerController::DeleteBasicControllIMC()
{
	if (EnhancedInputSubsystem)
	{
		while (EnhancedInputSubsystem->HasMappingContext(BasicControlMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(BasicControlMappingContext);
		}
	}
}

void APlayPlayerController::InitBasicMoveIMC()
{
	if (EnhancedInputSubsystem)
	{
		if (!EnhancedInputSubsystem->HasMappingContext(BasicMoveMappingContext)) {
			EnhancedInputSubsystem->AddMappingContext(BasicMoveMappingContext, 0);

		}
	}
}

void APlayPlayerController::DeleteBasicMoveIMC()
{
	if (EnhancedInputSubsystem)
	{
		while (EnhancedInputSubsystem->HasMappingContext(BasicMoveMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(BasicMoveMappingContext);

			UE_LOG(LogTemp, Log, TEXT("%s : MoveCancle"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		}
	}
}

void APlayPlayerController::InitLookIMC()
{
	if (EnhancedInputSubsystem)
	{
		if (!EnhancedInputSubsystem->HasMappingContext(LookMappingContext)) {
			EnhancedInputSubsystem->AddMappingContext(LookMappingContext, 0);
		}
	}
}

void APlayPlayerController::DeleteLookIMC()
{
	if (EnhancedInputSubsystem)
	{
		while (EnhancedInputSubsystem->HasMappingContext(LookMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(LookMappingContext);
		}
	}
}

FVector2D APlayPlayerController::GetMoveInput() const
{
	return MoveInput;
}

FVector APlayPlayerController::GetRelativeMoveInput() const
{
	return RelativeMoveInput;
}

#pragma endregion


