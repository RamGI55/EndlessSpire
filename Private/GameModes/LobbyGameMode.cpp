// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LobbyGameMode.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "GameStates/LobbyGameState.h"
#include "Actors/Characters/Players/WarriorPlayerCharacter.h"
#include "Actors/Characters/Players/MagePlayerCharacter.h"
#include "Actors/Characters/Players/ArcherPlayerCharacter.h"
#include "Actors/Characters/Players/ClericPlayerCharacter.h"
#include "PlayerStates/LobbyPlayerState.h"
#include "HUDs/LobbyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Misc/Guid.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	HUDClass = ALobbyHUD::StaticClass();
	DefaultPawnClass = ABasePlayerCharacter::StaticClass();
	
	DefaultPlayerCharacter = ABasePlayerCharacter::StaticClass();
	WarriorPlayerCharacter = AWarriorPlayerCharacter::StaticClass();
	MagePlayerCharacter = AMagePlayerCharacter::StaticClass();
	ArcherPlayerCharacter = AArcherPlayerCharacter::StaticClass();
	ClericPlayerCharacter = AClericPlayerCharacter::StaticClass();

	world = GetWorld();
}

void ALobbyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ClassMap.Empty();

	ClassMap.Add(EClass::NONE, DefaultPlayerCharacter);
	ClassMap.Add(EClass::WARRIOR, WarriorPlayerCharacter);
	ClassMap.Add(EClass::MAGE, MagePlayerCharacter);
	ClassMap.Add(EClass::ARCHER, ArcherPlayerCharacter);
	ClassMap.Add(EClass::CLERIC, ClericPlayerCharacter);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	APlayerController* CurrentPlayerController = NewPlayer;

	Super::PostLogin(NewPlayer);


	UE_LOG(LogTemp, Log, TEXT("PostLogin Up"));
	SetPlayerGuid(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("PostLogin Down"));
}

void ALobbyGameMode::ServerTravle()
{
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Stage"));
}

void ALobbyGameMode::SpawnActor(APlayerController* NewPlayer, EClass playerclass)
{

	UE_LOG(LogTemp, Log, TEXT("Spawn Up"));
	// ���� �� �÷��̾� ��ŸƮ ã��
	APlayerStart* PlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(world);It;++It)
	{
		// ù �÷��̾� ��ŸƮ ���� �� �극��ũ
		PlayerStart = *It;
		break;
	}

	if (PlayerStart) {

		// �÷��̾ŸƮ ��ġ��, ȸ���� ������ �� ���� ����
		FActorSpawnParameters spawnParams;
		FVector SpawnLocation = PlayerStart->GetActorLocation();
		FRotator SpawnRotation = PlayerStart->GetActorRotation();

		ABasePlayerCharacter* SpawnActor;
		TSubclassOf<ABasePlayerCharacter> SelectClass = ClassMap.Contains(playerclass) ? ClassMap[playerclass] : nullptr;
		if (!SelectClass) {
			UE_LOG(LogTemp, Log, TEXT("ALobbyGameMode SelectClass = NULL"));
			return;
		}
		SpawnActor = world->SpawnActor<ABasePlayerCharacter>(SelectClass, SpawnLocation, SpawnRotation, spawnParams);

		if (SpawnActor && NewPlayer)
		{
			APawn* CurrentPawn = NULL;

			if (NewPlayer->GetPawn())
			{
				CurrentPawn = NewPlayer->GetPawn();
			}

			NewPlayer->Possess(SpawnActor);


			UE_LOG(LogTemp, Log, TEXT("%s"), *NewPlayer->GetPawn()->GetName());
			if (CurrentPawn != NULL)
			{
				CurrentPawn->Destroy();
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Spawn Down"));


	// �÷��̾� ���� ����
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::SetPlayerInfo, 1.0f, false);
}

void ALobbyGameMode::SetPlayerGuid(APlayerController* NewPlayer)
{
	// Guid ����
	FGuid NewGuid = FGuid::NewGuid();
	
	UE_LOG(LogTemp, Log, TEXT("New Guid: %s"), *NewGuid.ToString());

	// �÷��̾� ���� �迭�� �߰�
	FInfoOfPlayers NewInfo(NewGuid,NewPlayer);
	
	PlayerInformations.Add(NewInfo);

	for (const FInfoOfPlayers& Player : PlayerInformations)
	{
		UE_LOG(LogTemp, Log, TEXT("GUID: %s, PC: %s"),*Player.Guid.ToString(),*Player.PlayerController->GetName());
	}

	// ���� Ŭ���̾�Ʈ ���� ���� ����
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);

	if (LobbyPC)
	{
		LobbyPC->SetGuid(NewGuid);
	}
}

// �÷��̾� �г��� ���� ����
void ALobbyGameMode::SetPlayerNickname(FGuid playerguid, FString playernickname)
{
	for (FInfoOfPlayers& Element : PlayerInformations)
	{
		if (Element.Guid == playerguid)
		{
			Element.Name = playernickname;
			UE_LOG(LogTemp, Log, TEXT("GUID: %s, PC: %s, Nickname: %s"), *Element.Guid.ToString(), *Element.PlayerController->GetName(), *Element.Name);
		}
	}
}

// �÷��̾� �г��� ��������
FString ALobbyGameMode::GetPlayerNickname(APlayerController* playercontroller)
{
	for (FInfoOfPlayers& Element : PlayerInformations)
	{
		if (Element.PlayerController == playercontroller)
		{
			return FString(Element.Name);
		}
	}
	return FString("NULL");
}


// �÷��̾� Ŭ���� ���� ���� �� UnPossess -> Spawn -> Possess
void ALobbyGameMode::SetPlayerClass(FGuid playerguid, EClass playerclass, APlayerController* playercontroller)
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EClass"));
	if (!EnumPtr) {
		UE_LOG(LogTemp, Log, TEXT("ALobbyGameMode EnumPtr = NULL"));
		return;
	}

	for (FInfoOfPlayers& Element : PlayerInformations)
	{
		if (Element.Guid == playerguid)
		{
			Element.Class = playerclass;
			UE_LOG(LogTemp, Log, TEXT("GUID: %s, PC: %s, Class: %s"), *Element.Guid.ToString(), *Element.PlayerController->GetName(),*EnumPtr->GetNameStringByValue((int32)Element.Class));

			SpawnActor(playercontroller,Element.Class);
		}
	}
}

// �÷��̾� ���� ����
void ALobbyGameMode::SetPlayerInfo()
{
	ALobbyPlayerController* LobbyPC;
	ALobbyPlayerState* LobbyPS;
	int32 index = 0;

	ALobbyGameState* LobbyGS = Cast< ALobbyGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (LobbyGS)
	{
		LobbyGS->UpdatePlayerInfo(PlayerInformations);
	}
	
	for (FInfoOfPlayers& Element : PlayerInformations)
	{
		LobbyPC = Cast<ALobbyPlayerController>(Element.PlayerController);
		LobbyPS = Cast<ALobbyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),index));

		if (!LobbyPS) {
			UE_LOG(LogTemp, Log, TEXT("LobbyGameMode LobbyPS = NULL"));
			return;
		}


		ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(Element.PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->SetMinimap(index);
			PlayerCharacter->SetPlayerStat(LobbyPS->PlayerBaseStat);
			PlayerCharacter->SetInventory(LobbyPS->PlayerItems, LobbyPS->PlayerMoney);
		}

		if (LobbyPC)
		{
			LobbyPC->ServerShowNickname(Element.Name);
			LobbyPC->OwnerSetStatus(Element.Name, Element.Class);
		}
		index++;
	}
}

