// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/MMStatComponent.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "MMPlayerStatComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENDLESSSPIRE_API UMMPlayerStatComponent : public UMMStatComponent
{
	GENERATED_BODY()
	
public:
	// UMMStatComponent을(를) 통해 상속됨
	void InitStat(FPlayerCharacterStat NewStat);

	bool DamagedHp(float DeltaHp) override;

	void HealHP(float Amount) override;

	void UseStamina(float Amount);

	void HealStamina(float Amount);

	float GetStat(FName StatName) const override;

	UFUNCTION(Server, Reliable)
	void ServerModifyStat(FName StatName, float Amount);

	void ModifyStat(FName StatName, float Amount) override;

	void RestoreStat(FName StatName, float Amount) override;

	UFUNCTION(BlueprintCallable)
	void UpdateState() override;

public:


	FPlayerCharacterStat GetPlayerStats() const;

	//경험치 증가, 레벨 업
	UFUNCTION(BlueprintCallable)
	void AddExp(float Amount);

	void LevelUp();

	////테스트용 장착/해제 스탯 증감
	//UFUNCTION(BlueprintCallable)
	//void EquipmentItemUpdate(UItemBase* Item);


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StatComponent", meta = (AllowPrivateAccess = "true"))
	FPlayerCharacterStat Stat;




};
