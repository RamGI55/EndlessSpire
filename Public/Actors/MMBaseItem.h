// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseActor.h"
#include "Interface/InteractionInterface.h"
#include "MMBaseItem.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AMMBaseItem : public AMMBaseActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public :
	AMMBaseItem();
	
protected :

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteraction()override;
	virtual void EndInteraction() override;
	virtual void Interaction(ABasePlayerCharacter* Player) override;

};
