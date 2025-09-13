// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/MMEnemyStatComponent.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "Net/UnrealNetwork.h"

UMMEnemyStatComponent::UMMEnemyStatComponent()
{
	SetIsReplicatedByDefault(true);

}

void UMMEnemyStatComponent::UpdateState()
{
	ABaseMonsterCharacter* Owner = Cast<ABaseMonsterCharacter>(GetOwner());
	if (Owner)
	{
		Owner->UpdateStat(Stat);
	}

}

FNonPlayerCharacterStat UMMEnemyStatComponent::GetNonPlayerStats() const
{
	return Stat;
}

void UMMEnemyStatComponent::SetEnemyStat(FNonPlayerCharacterStat InStat)
{
	Stat = InStat;
}

void UMMEnemyStatComponent::InitStat(FNonPlayerCharacterStat NewStat)
{
	Stat = NewStat;

	UpdateState();


}

bool UMMEnemyStatComponent::DamagedHp(float DeltaHp)
{
	if (bIsDead || DeltaHp == 0.0f)
	{
		return bIsDead;
	}


	Stat.CurHp = FMath::Clamp(Stat.CurHp + DeltaHp, 0.0f, Stat.MaxHp);


	if (Stat.CurHp <= 0.0f)
	{
		bIsDead = true;
		if (bIsDead)
		{
			ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
			if (Character)
			{
				Character->DeadEvent();
			}
		}
		
	}

	UpdateState();

	return bIsDead;
}

float UMMEnemyStatComponent::GetStat(FName StatName) const
{
	if (StatName == "Level") return Stat.Level;
	if (StatName == "MaxHp") return Stat.MaxHp;
	if (StatName == "CurHp") return Stat.CurHp;
	if (StatName == "Atk") return Stat.Atk;
	if (StatName == "Def") return Stat.Def;
	if (StatName == "CriticalChance") return Stat.CriticalChance;
	if (StatName == "CriticalHitDamage") return Stat.CriticalHitDamage;
	if (StatName == "MoveSpeed") return Stat.MoveSpeed;
	if (StatName == "AttackSpeed") return Stat.AttackSpeed;

	return 0.0f;
}

void UMMEnemyStatComponent::ModifyStat(FName StatName, float Amount)
{
	if (StatName == "Level") Stat.Level += Amount;
	else if (StatName == "MaxHp") Stat.MaxHp += Amount;
	else if (StatName == "CurHp") Stat.CurHp += Amount;
	else if (StatName == "Atk") Stat.Atk += Amount;
	else if (StatName == "Def") Stat.Def += Amount;
	else if (StatName == "CriticalChance") Stat.CriticalChance += Amount;
	else if (StatName == "CriticalDamage") Stat.CriticalHitDamage += Amount;
	else if (StatName == "MoveSpeed") Stat.MoveSpeed += Amount;
	else if (StatName == "AttackSpeed") Stat.AttackSpeed += Amount;

}


void UMMEnemyStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMMEnemyStatComponent, Stat);
}



void UMMEnemyStatComponent::OnRep_Stat()
{
	UpdateState();
}
