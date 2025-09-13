// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UENUM (BlueprintType)
enum EAIState
{
	Idle,
	Attacking,
	Stun,
	Patrolling,
	Dead
};

UENUM (BlueprintType)
enum EAIPerceptionSense
{
	EPS_None,
	EPS_Sight,
	EPS_Hearing,
	EPS_Damage,
	EPS_Max
};

UCLASS()
class ENDLESSSPIRE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	
};
