// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "StagePlayerController.generated.h"

/**
 * 
 */
 
UCLASS()
class ENDLESSSPIRE_API AStagePlayerController : public ALobbyPlayerController
{
	GENERATED_BODY()
	
public:
	AStagePlayerController();

// Functions
#pragma region Overrides
protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

#pragma endregion

#pragma region InputMappingFunctions
public:
	UFUNCTION()
	virtual void OnItem1BeginClicked();
	UFUNCTION()
	virtual void OnItem1EndClicked();

	UFUNCTION()
	virtual void OnItem2BeginClicked();
	UFUNCTION()
	virtual void OnItem2EndClicked();

	UFUNCTION()
	virtual void OnItem3BeginClicked();
	UFUNCTION()
	virtual void OnItem3EndClicked();

	UFUNCTION()
	virtual void OnItem4BeginClicked();
	UFUNCTION()
	virtual void OnItem4EndClicked();

#pragma endregion

// Properties
public:
#pragma region InputMappingProperties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ItemMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ItemAction1;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ItemAction2;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ItemAction3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ItemAction4;

#pragma endregion


};
