// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameStates/LobbyGameState.h"
#include "PlayerControllers/LobbyPlayerController.h"

ALobbyPlayerState::ALobbyPlayerState()
{
    bReplicates = true;
}

// 리플리케이션
void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerBaseStat);
}


void ALobbyPlayerState::SetPlayerStat(FPlayerCharacterStat NewStat)
{
    PlayerBaseStat = NewStat;

    ALobbyPlayerController* LobbyPC;

    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("Server Level : %d"), PlayerBaseStat.Level);
        ALobbyGameState* LobbyGS = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (LobbyGS)
        {
            for (FInfoOfPlayers& Element : LobbyGS->PlayerInformations)
            {
                LobbyPC = Cast<ALobbyPlayerController>(Element.PlayerController);

                if (Element.PlayerController->GetPlayerState<ALobbyPlayerState>() == this)
                {
                    UE_LOG(LogTemp, Log, TEXT("OnRep_PlayerState    ME!!!!"));
                    LobbyPC->OwnerUpdatePlayerStat(PlayerBaseStat);

                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("OnRep_PlayerState    OTHER!!!!"));
                    LobbyPC->OwnerUpdateOtherPlayerStat(PlayerBaseStat, PlayerGuid);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Client Level : %d"), PlayerBaseStat.Level);
    }
}

const FPlayerCharacterStat& ALobbyPlayerState::GetPlayerBaseStat() const
{
	return PlayerBaseStat;
}

