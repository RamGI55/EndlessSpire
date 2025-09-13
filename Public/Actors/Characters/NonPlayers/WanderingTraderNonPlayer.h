// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/BaseNonPlayerCharacter.h"
#include "Interface/InteractionInterface.h"
#include "UStructs/FItem.h"
#include "WanderingTraderNonPlayer.generated.h"

/**
 * 
 */
class UShopWidget;

UCLASS()
class ENDLESSSPIRE_API AWanderingTraderNonPlayer : public ABaseNonPlayerCharacter, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	AWanderingTraderNonPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteraction()override;
	virtual void EndInteraction() override;
	virtual void Interaction(ABasePlayerCharacter* Player) override;

	UPROPERTY(EditInstanceOnly)
	FInteractableData InstanceInteractableData;

	// 템플릿 클래스 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ShopClass;

	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Replicated)
	TArray<FItemData> ShopItems;

	UShopWidget* ShopWidget;
	
};
