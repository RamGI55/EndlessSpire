// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NicknameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UNicknameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetNickname(const FString& nickname);

	UFUNCTION(BlueprintCallable)
	FText GetNicknameText() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Nickname;


protected:

	virtual void NativeOnInitialized() ;
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "true"))
	class UTextBlock* PlayerNickname;

};
