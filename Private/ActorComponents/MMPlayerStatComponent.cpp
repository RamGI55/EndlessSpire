// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/MMPlayerStatComponent.h"
#include "PlayerStates/LobbyPlayerState.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "TimerManager.h"

void UMMPlayerStatComponent::InitStat(FPlayerCharacterStat NewStat)
{
	Stat = NewStat;
	UpdateState();
}


bool UMMPlayerStatComponent::DamagedHp(float DeltaHp)
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
			ABasePlayerCharacter* Character = Cast<ABasePlayerCharacter>(GetOwner());
			if (Character)
			{
				Character->DeadEvent();
			}
		}
	}

	UpdateState();

	return bIsDead;
}

void UMMPlayerStatComponent::HealHP(float Amount)
{
	if (Amount < 0.0f) {
		Amount = 0.0f;
	}

	Stat.CurHp = FMath::Clamp(Stat.CurHp + Amount, 0.0f, Stat.MaxHp);

	UpdateState();
}

void UMMPlayerStatComponent::UseStamina(float Amount)
{
	if (Amount < 0.0f) {
		Amount = 0.0f;
	}

	Stat.CurSp = FMath::Clamp(Stat.CurSp - Amount, 0.0f, Stat.MaxSp);

	UpdateState();
}

void UMMPlayerStatComponent::HealStamina(float Amount)
{
	if (Amount < 0.0f) {
		Amount = 0.0f;
	}

	Stat.CurSp = FMath::Clamp(Stat.CurSp + Amount, 0.0f, Stat.MaxSp);

	UpdateState();
}


float UMMPlayerStatComponent::GetStat(FName StatName) const
{
	if (StatName == "Level") return Stat.Level;
	if (StatName == "MaxHp") return Stat.MaxHp;
	if (StatName == "CurHp") return Stat.CurHp;
	if (StatName == "Atk") return Stat.Atk;
	if (StatName == "Def") return Stat.Def;
	if (StatName == "CurSp") return Stat.CurSp;
	if (StatName == "Def") return Stat.Def;
	if (StatName == "CriticalChance") return Stat.CriticalChance;
	if (StatName == "CriticalHitDamage") return Stat.CriticalHitDamage;
	if (StatName == "MoveSpeed") return Stat.MoveSpeed;
	if (StatName == "AttackSpeed") return Stat.AttackSpeed;
	if (StatName == "MaxMp") return Stat.MaxMp;
	if (StatName == "CurMp") return Stat.CurMp;
	if (StatName == "MaxExp") return Stat.MaxExp;
	if (StatName == "CurExp") return Stat.CurExp;
	if (StatName == "SkillRange") return Stat.SkillRange;
	if (StatName == "SkillDamage") return Stat.SkillDamage;
	if (StatName == "SkillDuration") return Stat.SkillDuration;

	return 0.0f;
}

void UMMPlayerStatComponent::ServerModifyStat_Implementation(FName StatName, float Amount)
{
	ModifyStat(StatName, Amount);
}

void UMMPlayerStatComponent::ModifyStat(FName StatName, float Amount)
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
		else if (StatName == "MaxMp") Stat.MaxMp += Amount;
		else if (StatName == "CurMp") Stat.CurMp += Amount;
		else if (StatName == "MaxExp") Stat.MaxExp += Amount;
		else if (StatName == "CurExp") Stat.CurExp += Amount;
		else if (StatName == "SkillRange") Stat.SkillRange += Amount;
		else if (StatName == "SkillDamage") Stat.SkillDamage += Amount;
		else if (StatName == "SkillDuration") Stat.SkillDuration += Amount;
	

	UpdateState();
}

void UMMPlayerStatComponent::RestoreStat(FName StatName, float Amount)
{

	if (StatName == "Level") Stat.Level -= Amount;
	else if (StatName == "MaxHp") Stat.MaxHp -= Amount;
	else if (StatName == "CurHp") Stat.CurHp -= Amount;
	else if (StatName == "Atk") Stat.Atk -= Amount;
	else if (StatName == "Def") Stat.Def -= Amount;
	else if (StatName == "CriticalChance") Stat.CriticalChance -= Amount;
	else if (StatName == "CriticalDamage") Stat.CriticalHitDamage -= Amount;
	else if (StatName == "MoveSpeed") Stat.MoveSpeed -= Amount;
	else if (StatName == "AttackSpeed") Stat.AttackSpeed -= Amount;
	else if (StatName == "MaxMp") Stat.MaxMp -= Amount;
	else if (StatName == "CurMp") Stat.CurMp -= Amount;
	else if (StatName == "MaxExp") Stat.MaxExp -= Amount;
	else if (StatName == "CurExp") Stat.CurExp -= Amount;
	else if (StatName == "SkillRange") Stat.SkillRange -= Amount;
	else if (StatName == "SkillDamage") Stat.SkillDamage -= Amount;
	else if (StatName == "SkillDuration") Stat.SkillDuration -= Amount;


	UpdateState();
}

void UMMPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMMPlayerStatComponent::AddExp(float Amount)
{

	Stat.CurExp += Amount;

	if (Stat.CurExp >= Stat.MaxExp)
	{
		LevelUp();
	}


	UE_LOG(LogTemp, Log, TEXT("AddExp : %f"), Amount);
	UpdateState();
}

void UMMPlayerStatComponent::LevelUp()
{
	Stat.CurExp -= Stat.MaxExp;

	Stat.Level++;

	Stat.MaxExp = FMath::FloorToInt(Stat.MaxExp * 1.2f);



	Stat.MaxHp += 5;
	Stat.CurHp += 5;
	Stat.Atk += 2;
	Stat.Def += 1;


	if (Stat.CurExp >= Stat.MaxExp)
	{
		LevelUp();
	}
}

//void UMMPlayerStatComponent::EquipmentItemUpdate(UItemBase* Item)
//{
//	if (!Item)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("EquipmentItemUpdate is nullptr!"));
//		return;
//	}
//
//	if (!(Item->GetEquipped()))
//	{
//		Stat += Item->ItemStatistics.ItemStat;
//		Item->SetEquipeed(true);
//	}
//	else
//	{
//		Stat -= Item->ItemStatistics.ItemStat;
//		Item->SetEquipeed(false);
//	}
//
//	UpdateState();
//}

void UMMPlayerStatComponent::UpdateState()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerState: Owner is nullptr!"));
		return;
	}

	AController* Controller = Cast<AController>(Owner->GetInstigatorController());
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerState: Controller is nullptr! Retrying later."));
		return;
	}

	ALobbyPlayerState* PlayerState = Controller->GetPlayerState<ALobbyPlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerState: PlayerState is nullptr! Retrying later."));
		return;
	}

	PlayerState->SetPlayerStat(GetPlayerStats());

}


FPlayerCharacterStat UMMPlayerStatComponent::GetPlayerStats() const
{
	return Stat;
}


