// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStates/LobbyGameState.h"
#include "GameModes/LobbyGameMode.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "GameInstances/EndlessSpireGameInstance.h"
#include "HUDs/LobbyHUD.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
}

void ALobbyGameState::OpenLevel()
{
	ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGM)
	{
		LobbyGM->ServerTravle();
	}
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, PlayerInformations);
	DOREPLIFETIME(ALobbyGameState, VoteArray);
	DOREPLIFETIME(ALobbyGameState, Stage);
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		Stage = GameInstance->Stage;
	}
}

void ALobbyGameState::UpdatePlayerInfo(const TArray<FInfoOfPlayers>& playerinformations)
{
	PlayerInformations = playerinformations;
}

void ALobbyGameState::OnRep_PlayerInformations()
{
	if (HasAuthority())
	{
		// 서버 처리
	}
	else 
	{
		if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			UE_LOG(LogTemp, Log, TEXT("LobbyGameState PlayerController = NULL"));
			return;
		}
		// 클라이언트 처리
		ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD());
		if (LobbyHUD)
		{
			LobbyHUD->UpdateOtherPlayerInformations();
		}
	}
}

void ALobbyGameState::AddVoteArray(int32 result)
{
	if (VoteArray.Num() < 4)
	{
		VoteArray.Add(result);	
	}

	if (VoteArray.Num() == PlayerInformations.Num())
	{
		for (const int32 Element : VoteArray)
		{
			if (Element == 0)
			{
				return;
			}
		}

		// 이 곳에 서버트래블 함수 호출
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Let's go ServerTravle"));
		ALobbyPlayerController* LobbyPC;
		UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GameInstance)
		{
			GameInstance->Stage = Stage + 1;
			UE_LOG(LogTemp, Log, TEXT("Stage Up"));
		}

		for (const FInfoOfPlayers& Element : PlayerInformations)
		{
			LobbyPC = Cast<ALobbyPlayerController>(Element.PlayerController);
			if (LobbyPC)
			{
				LobbyPC->OwnerSaveInfo(Element.Class);
			}
		}


		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyGameState::OpenLevel, 3.0f, false);
	}
}



