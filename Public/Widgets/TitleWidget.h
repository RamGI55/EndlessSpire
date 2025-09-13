// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeOnInitialized();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UButton* GameStartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UButton* EditorIPButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UTextBlock* TitleName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UEditableTextBox* NicknameBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UEditableTextBox* IPBox;

private:

	UFUNCTION(BlueprintCallable)
	void GameStartButtonCallback();

	UFUNCTION(BlueprintCallable)
	void EditorIPButtonCallback();
	
};
