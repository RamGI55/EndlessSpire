// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerStatusUserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UPlayerStatusUserInterface::UpdateWidget(FPlayerCharacterStat newStatus)
{
		

	if (TextBlock_LevelValue)
	{
		TextBlock_LevelValue->SetText(FText::AsNumber(newStatus.Level));
	}
	if (TextBlock_ATValue)
	{
		TextBlock_ATValue->SetText(FText::AsNumber(newStatus.Atk));
	}	
	if (TextBlock_DFValue)
	{
		TextBlock_DFValue->SetText(FText::AsNumber(newStatus.Def));
	}
	if (TextBlock_ASValue)
	{
		TextBlock_ASValue->SetText(FText::AsNumber(newStatus.AttackSpeed));
	}
	if (TextBlock_MSValue)
	{
		TextBlock_MSValue->SetText(FText::AsNumber(newStatus.MoveSpeed));
	}
	if (TextBlock_CRCValue)
	{
		TextBlock_CRCValue->SetText(FText::AsNumber(newStatus.CriticalChance));
	}
	if (TextBlock_CRDValue)
	{
		TextBlock_CRDValue->SetText(FText::AsNumber(newStatus.CriticalHitDamage));
	}
	if (TextBlock_SRValue)
	{
		TextBlock_SRValue->SetText(FText::AsNumber(newStatus.SkillRange));
	}
	if (TextBlock_SDMValue)
	{
		TextBlock_SDMValue->SetText(FText::AsNumber(newStatus.SkillDamage));
	}
	if (TextBlock_SDTValue)
	{
		TextBlock_SDTValue->SetText(FText::AsNumber(newStatus.SkillDuration));
	}
}

void UPlayerStatusUserInterface::UpdateWidget(const FString& playername, const EClass& playerclass)
{
	if (TextBlock_UserNickname)
	{
		TextBlock_UserNickname->SetText(FText::FromString(playername));
	}
	
	PlayerClass = playerclass;
	
}

void UPlayerStatusUserInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}