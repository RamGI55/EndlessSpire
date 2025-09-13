// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseActor.h"
#include "Interface/InteractionInterface.h"
#include "InteractChest.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AInteractChest : public AMMBaseActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:

	AInteractChest();

	virtual void BeginPlay() override;


	void BeginFocus() override;
	void EndFocus()override;
	void BeginInteraction()override;
	void EndInteraction()override;
	void Interaction(ABasePlayerCharacter* Player)override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* CloseMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* OpenedMesh;

	UPROPERTY(Replicated)
	bool bOpened;

	TObjectPtr<UDataTable> ItemDataTable;

public:

	void SpawnRandomItem();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OpenChest();

protected:

	UPROPERTY(Replicated)
	TArray<FName> SpawnItemsID;



};
