// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStructs/FInfoOfPlayers.h"
#include "UStructs/FPlayerCharacterStat.h"
#include "TeamStatsInterface.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UTeamStatsInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateInfoWidget();
	UFUNCTION()
	void UpdateStatWidget(FPlayerCharacterStat& playerstat, FGuid& playerguid);

protected:

	virtual void NativeOnInitialized() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UTextBlock* TeamPlayer1NicknameBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UTextBlock* TeamPlayer2NicknameBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UTextBlock* TeamPlayer3NicknameBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	TArray<FGuid> OtherPlayerGuidArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	TMap<FGuid, FString> OtherPlayerNicknameMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	TMap<FGuid, EClass> OtherPlayerClassMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	TMap<FGuid, FPlayerCharacterStat> OtherPlayerStatMap;


	UFUNCTION(BlueprintCallable)
	FText GetOtherPlayerNickname(int index);

	UFUNCTION(BlueprintCallable)
	EClass GetOtherPlayerClass(int index);

	UFUNCTION(BlueprintCallable)
	FText GetOtherPlayerLevel(int index);

	UFUNCTION(BlueprintCallable)
	float GetOtherPlayerHealth(int index);

	UFUNCTION(BlueprintCallable)
	float GetOtherPlayerMana(int index);

	UFUNCTION(BlueprintCallable)
	float GetOtherPlayerExperience(int index);

};
