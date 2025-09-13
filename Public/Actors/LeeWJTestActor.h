// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseActor.h"
#include "LeeWJTestActor.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ALeeWJTestActor : public AMMBaseActor
{
	GENERATED_BODY()
	
public:
	ALeeWJTestActor();
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Fuck();

	USceneComponent* scene;
};
