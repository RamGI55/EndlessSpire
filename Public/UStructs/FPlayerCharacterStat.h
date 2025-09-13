// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FCharacterStat.h"

#include "FPlayerCharacterStat.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FPlayerCharacterStat : public FCharacterStat
{
public:
	GENERATED_USTRUCT_BODY()


	FPlayerCharacterStat()
		: MaxMp(100),
		CurMp(100),
		MaxSp(100),
		CurSp(100),
		MaxExp(100),
		CurExp(0),
		SkillRange(1.0f),
		SkillDamage(1.0f),
		SkillDuration(1.0f)
	{};

public:

	// 최대 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMp;
	// 현재 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurMp;

	// 최대 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSp;
	// 현재 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurSp;

	// 최대 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxExp;
	// 현재 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurExp;

	// 추가 스킬 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillRange;

	// 추가 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillDamage;

	// 추가 스킬 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillDuration;


public:

	// 장비 장착 시
	FPlayerCharacterStat& operator+=(const FPlayerCharacterStat& Other)
	{
		this->Atk += Other.Atk;
		this->AttackSpeed += Other.AttackSpeed;
		this->CriticalChance += Other.CriticalChance;
		this->CriticalHitDamage += Other.CriticalHitDamage;
		this->CurExp += Other.CurExp;
		this->CurHp += Other.CurHp;
		this->CurMp += Other.CurMp;
		this->CurSp += Other.CurSp;
		this->Def += Other.Def;
		this->Level += Other.Level;
		this->MaxExp += Other.MaxExp;
		this->MaxHp += Other.MaxHp;
		this->MaxMp += Other.MaxMp;
		this->CurSp += Other.CurSp;
		this->MoveSpeed += Other.MoveSpeed;
		this->SkillDamage += Other.SkillDamage;
		this->SkillDuration += Other.SkillDuration;
		this->SkillRange += Other.SkillRange;

		return *this;

	}

	// 장비 해제 시
	FPlayerCharacterStat& operator-=(const FPlayerCharacterStat& Other)
	{
		this->Atk -= Other.Atk;
		this->AttackSpeed -= Other.AttackSpeed;
		this->CriticalChance -= Other.CriticalChance;
		this->CriticalHitDamage -= Other.CriticalHitDamage;
		this->CurExp -= Other.CurExp;
		this->CurHp -= Other.CurHp;
		this->CurMp -= Other.CurMp;
		this->CurSp -= Other.CurSp;
		this->Def -= Other.Def;
		this->Level -= Other.Level;
		this->MaxExp -= Other.MaxExp;
		this->MaxHp -= Other.MaxHp;
		this->MaxMp -= Other.MaxMp;
		this->MaxSp -= Other.MaxSp;
		this->MoveSpeed -= Other.MoveSpeed;
		this->SkillDamage -= Other.SkillDamage;
		this->SkillDuration -= Other.SkillDuration;
		this->SkillRange -= Other.SkillRange;

		return *this;

	}

};


