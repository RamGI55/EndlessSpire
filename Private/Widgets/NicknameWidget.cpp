// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NicknameWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "GameInstances/EndlessSpireGameInstance.h"


void UNicknameWidget::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	// ´Ð³×ÀÓ 
	PlayerNickname = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_PlayerNickname")));
}

void UNicknameWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FText UNicknameWidget::GetNicknameText() const
{
	return FText::FromString(Nickname);
}

void UNicknameWidget::SetNickname(const FString& nickname)
{
	Nickname = nickname;
	UE_LOG(LogTemp, Log, TEXT("Set Nickname Prev"));
	UE_LOG(LogTemp, Log, TEXT("Set Nickname %s"), *nickname);
}
