// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/MMStatComponent.h"
#include "UStructs/FNonPlayerCharacterStat.h"
#include "MMEnemyStatComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENDLESSSPIRE_API UMMEnemyStatComponent : public UMMStatComponent
{
	GENERATED_BODY()
	
public:

	// UMMStatComponent을(를) 통해 상속됨

	UMMEnemyStatComponent();

	void UpdateState() override;

	void InitStat(FNonPlayerCharacterStat NewStat);

	bool DamagedHp(float DeltaHp) override;

	float GetStat(FName StatName) const override;

	void ModifyStat(FName StatName, float Amount) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	FNonPlayerCharacterStat GetNonPlayerStats() const;

	void SetEnemyStat(FNonPlayerCharacterStat InStat);

public:


	UFUNCTION()
	void OnRep_Stat();


private:

	UPROPERTY(ReplicatedUsing = OnRep_Stat, VisibleAnywhere)
	FNonPlayerCharacterStat Stat;

};
