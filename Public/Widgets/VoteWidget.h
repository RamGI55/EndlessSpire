// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "VoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UVoteWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ALobbyGameState* LobbyGS;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_VoteTitle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox_Vote1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox_Vote2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox_Vote3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox_Vote4;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Vote1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Vote2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Vote3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Vote4;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Yes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_No;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBox_ButtonBox;

private:

	UFUNCTION(BlueprintCallable)
	void YesButtonCallback();

	UFUNCTION(BlueprintCallable)
	void NoButtonCallback();

	UFUNCTION(BlueprintCallable)
	void RemoveWidget();

	UFUNCTION(BlueprintCallable)
	int32 GetVoteArray(int32 index);

};
