// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponents/MMEnemyStatComponent.h"
#include "MonsterHealthBar.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UMonsterHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UFUNCTION()
	void UpdateState(const FNonPlayerCharacterStat& Stat);



};
