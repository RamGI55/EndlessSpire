// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/BaseNonPlayerCharacter.h"
#include "Interface/InteractionInterface.h"
#include "ClassGuideNonPlayerCharacter.generated.h"

/**
 * 
 */

class UWidgetComponent;
class UPaperSpriteComponent;


UCLASS()
class ENDLESSSPIRE_API AClassGuideNonPlayerCharacter : public ABaseNonPlayerCharacter, public IInteractionInterface
{
	GENERATED_BODY()
	

public:
	AClassGuideNonPlayerCharacter();

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

	// 템플릿 클래스 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ClassSelectionClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* DefaultAnimSequence;

private:

	/** Sprite **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	UPaperSpriteComponent* SpriteComponent;

	/** Widget **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NameWidget;

	
};
