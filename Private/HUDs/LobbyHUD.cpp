// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/LobbyHUD.h"
#include "Blueprint/UserWidget.h"

ALobbyHUD::ALobbyHUD()
{
	// 블루프린트 클래스 불러오기
	//static ConstructorHelpers::FClassFinder<UUserWidget> PlayerUIAsset(TEXT("WidgetBlueprint'/Game/UI/WBP_PlayerUI.WBP_PlayerUI_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerUIAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_PlayerUI.WBP_PlayerUI_C'"));

	// 템플릿 클래스에 블루프린트 클래스 넣기
	if (PlayerUIAsset.Succeeded())
	{
		PlayerUIClass = PlayerUIAsset.Class;
	}
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowPlayerUserInterface(PlayerUIClass);
}

void ALobbyHUD::ShowPlayerUserInterface(TSubclassOf<UUserWidget> NewWidgetClass)
{
	UUserWidget* Widget;

	if (NewWidgetClass != nullptr)
	{
		Widget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), NewWidgetClass);
		if (Widget != nullptr)
		{
			PlayerUI = Cast<UPlayerUserInterface>(Widget);
			Widget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("PlayerUI Created"));
		}
	}


}

void ALobbyHUD::UpdateOtherPlayerInformations()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateOtherPlayer"));

	if (!PlayerUI) {

		UE_LOG(LogTemp, Log, TEXT("LobbyHUD PlayerUI = NULL"));
		return;
	}
	if (!PlayerUI->TeamUI) {

		UE_LOG(LogTemp, Log, TEXT("LobbyHUD PlayerUI->TeamUI = NULL"));
		return;
	}

	PlayerUI->TeamUI->UpdateInfoWidget();
}

void ALobbyHUD::UpdatePlayerStatus(FPlayerCharacterStat& PlayerStat)
{
	if (!PlayerUI) {
		UE_LOG(LogTemp, Warning, TEXT("ALobbyHUD::UpdatePlayerStatus PlayerUI = NULL"));
		return;
	}
	if (!PlayerUI->StatusUI)
	{
		UE_LOG(LogTemp, Error, TEXT("ALobbyHUD::UpdatePlayerStatus() failed - Widget is not UPlayerStatusUserInterface!"));
		return;
	}
	if (!PlayerUI->TeamUI) {
		UE_LOG(LogTemp, Warning, TEXT("ALobbyHUD::UpdatePlayerStatus PlayerUI->TeamUI = NULL"));
		return;
	}

	//  UI가 올바르게 캐스팅되었으면 저장 후 업데이트 실행
	PlayerUI->PlayerUI->UpdateWidget(PlayerStat);
	PlayerUI->StatusUI->UpdateWidget(PlayerStat);
	PlayerUI->EquipmentPanel->UpdateWidget(PlayerStat);
	UE_LOG(LogTemp, Warning, TEXT("ALobbyHUD::UpdatePlayerStatus() succeeded!"));
}

void ALobbyHUD::UpdateOtherPlayerStatus(FPlayerCharacterStat& PlayerStat, FGuid& PlayerGuid)
{

	if (!PlayerUI) {
		UE_LOG(LogTemp, Warning, TEXT("ALobbyHUD::UpdateOtherPlayerStatus PlayerUI = NULL"));
		
		return;
	}
	if (!PlayerUI->TeamUI) {
		UE_LOG(LogTemp, Warning, TEXT("ALobbyHUD::UpdateOtherPlayerStatus PlayerUI->TeamUI = NULL"));

		return;
	}
	PlayerUI->TeamUI->UpdateStatWidget(PlayerStat, PlayerGuid);
}


