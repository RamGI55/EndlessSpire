// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MonsterHealthBar.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "Components/WidgetComponent.h"


void UMonsterHealthBar::UpdateState(const FNonPlayerCharacterStat& Stat)
{

	MaxHP = Stat.MaxHp;   
	CurrentHP = Stat.CurHp; 

}