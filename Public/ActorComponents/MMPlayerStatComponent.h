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
	// UMMStatComponent��(��) ���� ��ӵ�
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

	//����ġ ����, ���� ��
	UFUNCTION(BlueprintCallable)
	void AddExp(float Amount);

	void LevelUp();

	////�׽�Ʈ�� ����/���� ���� ����
	//UFUNCTION(BlueprintCallable)
	//void EquipmentItemUpdate(UItemBase* Item);


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StatComponent", meta = (AllowPrivateAccess = "true"))
	FPlayerCharacterStat Stat;




};
