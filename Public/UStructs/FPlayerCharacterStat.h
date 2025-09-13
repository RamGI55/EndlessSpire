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

	// �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMp;
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurMp;

	// �ִ� ���׹̳�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSp;
	// ���� ���׹̳�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurSp;

	// �ִ� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxExp;
	// ���� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurExp;

	// �߰� ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillRange;

	// �߰� ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillDamage;

	// �߰� ��ų ���� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillDuration;


public:

	// ��� ���� ��
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

	// ��� ���� ��
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


