// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/MMInteractionComponent.h"
#include "InteractionComponent.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UInteractionComponent : public UMMInteractionComponent
{
	GENERATED_BODY()
	
public :

	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteraction();
	virtual void EndInteraction();
	virtual void Interaction();

	
};

