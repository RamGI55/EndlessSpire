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

	// GameStartButton �������Ʈ���� ��������
	GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_GameStartButton")));

	// ������ IP �������� ��ư ��������
	EditorIPButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_EditorIP")));

	// TitleName �������Ʈ���� ��������
	TitleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_TitleName")));
	TitleName->SetText(FText::FromString("EndlessSpire"));

	// (�ӽ�) �г��� �����ڽ� �������Ʈ���� ��������
	NicknameBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_Nickname")));
	NicknameBox->SetHintText(FText::FromString("Nickname"));

	// (�ӽ�) IP �����ڽ�
	IPBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_IP")));
	IPBox->SetHintText(FText::FromString("IP"));

	// ���� ���� ��ư Ŭ�� �� ȣ��� ��������Ʈ�� �Լ� ���
	GameStartButton->OnClicked.AddDynamic(this, &UTitleWidget::GameStartButtonCallback);

	// ������ IP �������� ��ư Ŭ�� �� ȣ��� ��������Ʈ�� �Լ� ���
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
