// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/BasePlayerController.h"
#include "PlayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API APlayPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	APlayPlayerController();

// Fuctions
#pragma region Overrides
protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion
	
#pragma region InputMappingFunctions
public:
	UFUNCTION(Category = "InputMapping")
	virtual void Move(const struct FInputActionValue& Value);

	UFUNCTION(Category = "InputMapping")
	virtual void Run();

	UFUNCTION(Category = "InputMapping")
	virtual void StopRunning();

	UFUNCTION(Category = "InputMapping")
	virtual void Look(const FInputActionValue& Value);

	UFUNCTION(Category = "InputMapping")
	virtual void Jump();

	UFUNCTION(Category = "InputMapping")
	virtual void StopJumping();

	UFUNCTION(Category = "InputMapping")
	virtual void OnLeftMouseBeginClicked();

	UFUNCTION(Category = "InputMapping")
	virtual void OnLeftMouseTriggered(const FInputActionInstance& Instance);

	UFUNCTION(Category = "InputMapping")
	virtual void OnLeftMouseHolding(const struct FInputActionInstance& Instance);

	UFUNCTION(Category = "InputMapping")
	virtual void OnLeftMouseCancled();

	UFUNCTION(Category = "InputMapping")
	virtual void OnLeftMouseCompleted();
	
	UFUNCTION(Category = "InputMapping")
	virtual void OnRightMouseBeginClicked();

	UFUNCTION(Category = "InputMapping")
	virtual void OnRightMouseTriggered(const FInputActionInstance& Instance);

	UFUNCTION(Category = "InputMapping")
	virtual void OnRightMouseHolding(const FInputActionInstance& Instance);

	UFUNCTION(Category = "InputMapping")
	virtual void OnRightMouseCancled();

	UFUNCTION(Category = "InputMapping")
	virtual void OnRightMouseCompleted();

#pragma endregion
public:
	UFUNCTION()
	virtual void InitAllIMC();
	UFUNCTION()
	virtual void DeleteAllIMC();

	UFUNCTION()
	void InitBasicControllIMC();
	UFUNCTION()
	void DeleteBasicControllIMC();

	UFUNCTION()
	void InitBasicMoveIMC();
	UFUNCTION()
	void DeleteBasicMoveIMC();

	UFUNCTION()
	void InitLookIMC();
	UFUNCTION()
	void DeleteLookIMC();

public:
	FVector2D GetMoveInput() const;
	FVector GetRelativeMoveInput() const;

// Properties
public:
#pragma region InputMappingProperties
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* BasicControlMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* LookMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* BasicMoveMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RunAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LeftMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RightMouse;

protected:
	class UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	class UEnhancedInputComponent* EnhancedInputComponent;

#pragma endregion

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Input)
	FVector2D MoveInput;
	FVector RelativeMoveInput;

};
