// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ATitlePlayerController : public APlayPlayerController
{
	GENERATED_BODY()
	
	
protected:
	virtual void BeginPlay();
	
};
