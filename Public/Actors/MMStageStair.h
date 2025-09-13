// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MMBaseActor.h"
#include "Interface/InteractionInterface.h"
#include "MMStageStair.generated.h"

/**
 * 
 */
class UWidgetComponent;
class UPaperSpriteComponent;

UCLASS()
class ENDLESSSPIRE_API AMMStageStair : public AMMBaseActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	AMMStageStair();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);

	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteraction()override;
	virtual void EndInteraction() override;
	virtual void Interaction(ABasePlayerCharacter* Player) override;

	UPROPERTY(EditInstanceOnly)
	FInteractableData InstanceInteractableData;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// 템플릿 클래스 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> VoteWidgetClass;

	UFUNCTION(NetMulticast,Reliable)
	void VoteForNextStage();

private:

	/** Sprite **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	UPaperSpriteComponent* SpriteComponent;
};
