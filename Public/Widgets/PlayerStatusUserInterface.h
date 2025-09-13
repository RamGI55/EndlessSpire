// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStates/LobbyPlayerState.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "UStructs/FInfoOfPlayers.h"
#include "UEnums/EClass.h"
#include "PlayerStatusUserInterface.generated.h"


class UTextBlock;




/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UPlayerStatusUserInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateWidget(FPlayerCharacterStat PlayerStat);
	void UpdateWidget(const FString& playername, const EClass& playerclass);
	
protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EClass PlayerClass;
#pragma region UTextBlock

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_UserNickname;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_LevelValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ATValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_DFValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ASValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MSValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_CRCValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_CRDValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SRValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SDMValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SDTValue;

	

#pragma endregion

private: 

};
