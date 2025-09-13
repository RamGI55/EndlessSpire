// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TitleWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

#include "GameInstances/EndlessSpireGameInstance.h"

void UTitleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// GameStartButton 블루프린트에서 가져오기
	GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_GameStartButton")));

	// 에디터 IP 가져오는 버튼 가져오기
	EditorIPButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_EditorIP")));

	// TitleName 블루프린트에서 가져오기
	TitleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_TitleName")));
	TitleName->SetText(FText::FromString("EndlessSpire"));

	// (임시) 닉네임 에딧박스 블루프린트에서 가져오기
	NicknameBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_Nickname")));
	NicknameBox->SetHintText(FText::FromString("Nickname"));

	// (임시) IP 에딧박스
	IPBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_IP")));
	IPBox->SetHintText(FText::FromString("IP"));

	// 게임 시작 버튼 클릭 시 호출될 델리게이트에 함수 등록
	GameStartButton->OnClicked.AddDynamic(this, &UTitleWidget::GameStartButtonCallback);

	// 에디터 IP 가져오는 버튼 클릭 시 호출될 델리게이트에 함수 등록
	EditorIPButton->OnClicked.AddDynamic(this, &UTitleWidget::EditorIPButtonCallback);

}

void UTitleWidget::GameStartButtonCallback()
{
	UEndlessSpireGameInstance* GameInstance = Cast<UEndlessSpireGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->Nickname = NicknameBox->GetText().ToString();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientTravel(IPBox->GetText().ToString(), ETravelType::TRAVEL_Absolute);
	}
}

void UTitleWidget::EditorIPButtonCallback()
{
	FString EditorIP = "127.0.0.1:17777";
	IPBox->SetText(FText::FromString(EditorIP));
}
