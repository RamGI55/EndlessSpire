// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/PlayHUD.h"
#include "Widgets/PlayerUserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ALobbyHUD : public APlayHUD
{
	GENERATED_BODY()
	
public:
	ALobbyHUD();

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ShowPlayerUserInterface(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION()
	void UpdateOtherPlayerInformations();

	//플레이어스테이터스UI
	UFUNCTION()
	void UpdatePlayerStatus(FPlayerCharacterStat& PlayerStat);
	UFUNCTION()
	void UpdateOtherPlayerStatus(FPlayerCharacterStat& PlayerStat, FGuid& PlayerGuid);
	
	UPlayerStatusUserInterface* GetStatusUI() { return PlayerUI->StatusUI; }

	UPROPERTY()
	UPlayerUserInterface* PlayerUI;



protected:

	virtual void BeginPlay() override;

	// 템플릿 클래스 생성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_GAME")
	TSubclassOf<UUserWidget> PlayerUIClass;

};


