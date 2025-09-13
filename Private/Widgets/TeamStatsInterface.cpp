// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TeamStatsInterface.h"
#include "GameStates/LobbyGameState.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"




void UTeamStatsInterface::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TeamPlayer1NicknameBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_TeamPlayer1Nickname")));
	TeamPlayer2NicknameBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_TeamPlayer2Nickname")));
	TeamPlayer3NicknameBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_TeamPlayer3Nickname")));

}

void UTeamStatsInterface::UpdateInfoWidget()
{
	ALobbyGameState* GameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(GetWorld()));
	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!GameState) {
		UE_LOG(LogTemp, Log, TEXT("UTeamStatsInterface GameState = NULL"));
		return;
	}

	for (FInfoOfPlayers& Element : GameState->PlayerInformations)
	{
		if (GameState && PlayerController)
		{
			if (Element.Guid != PlayerController->PlayerGuid )
			{
				UE_LOG(LogTemp, Log, TEXT("Update -> Guid: %s Nickname: %s"),*Element.Guid.ToString(), *Element.Name);
				OtherPlayerGuidArray.AddUnique(Element.Guid);
				OtherPlayerNicknameMap.Add(Element.Guid, Element.Name);
				OtherPlayerClassMap.Add(Element.Guid, Element.Class);
			}
		}
	}
}

void UTeamStatsInterface::UpdateStatWidget(FPlayerCharacterStat& playerstat, FGuid& playerguid)
{
	OtherPlayerStatMap.Add(playerguid, playerstat);
}

FText UTeamStatsInterface::GetOtherPlayerNickname(int index)
{	
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		return FText::FromString(*OtherPlayerNicknameMap.Find(OtherPlayerGuidArray[index]));
	}
	return FText::FromString("");
}

EClass UTeamStatsInterface::GetOtherPlayerClass(int index)
{
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		return EClass(*OtherPlayerClassMap.Find(OtherPlayerGuidArray[index]));
	}
	return EClass(EClass::NONE);
}

FText UTeamStatsInterface::GetOtherPlayerLevel(int index)
{
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		return FText::AsNumber(OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->Level);
	}
	return FText::AsNumber(0);
}

float UTeamStatsInterface::GetOtherPlayerHealth(int index)
{
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		float CurrentHP = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->CurHp;
		float MaxHP = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->MaxHp;

		if (CurrentHP != 0 && MaxHP != 0)
		{
			return CurrentHP / MaxHP;
		}
	}
	return 0;
}

float UTeamStatsInterface::GetOtherPlayerMana(int index)
{
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		float CurrentMP = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->CurMp;
		float MaxMP = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->MaxMp;

		if (CurrentMP != 0 && MaxMP != 0)
		{
			return CurrentMP / MaxMP;
		}

	}
	return 0;
}

float UTeamStatsInterface::GetOtherPlayerExperience(int index)
{
	if (OtherPlayerGuidArray.Num() > index && OtherPlayerStatMap.Num() > index)
	{
		float CurrentExp = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->CurExp;
		float MaxExp = OtherPlayerStatMap.Find(OtherPlayerGuidArray[index])->MaxExp;

		if (CurrentExp != 0 && MaxExp != 0)
		{
			return CurrentExp / MaxExp;
		}

	}
	return 0;
}

