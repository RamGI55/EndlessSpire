// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/VoteWidget.h"
#include "GameStates/LobbyGameState.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UVoteWidget::NativeOnInitialized()
{
	Button_Yes->OnClicked.AddDynamic(this, &UVoteWidget::YesButtonCallback);
	Button_No->OnClicked.AddDynamic(this, &UVoteWidget::NoButtonCallback);

	TextBlock_VoteTitle->SetText(FText::FromString("Next Stage ?"));

	// 투표 인원
	LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (LobbyGS)
	{
		switch (LobbyGS->PlayerInformations.Num())
		{
		case 1:
			SizeBox_Vote1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		case 2:
			SizeBox_Vote1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		case 3:
			SizeBox_Vote1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		case 4:
			SizeBox_Vote1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SizeBox_Vote4->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		default:
			break;
		}
	}

	// UI 세팅모드
	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVoteWidget::RemoveWidget, 3.0f, false);
}

void UVoteWidget::YesButtonCallback()
{
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (LobbyPC)
	{
		LobbyPC->SetVote(1);
	}

	HorizontalBox_ButtonBox->SetVisibility(ESlateVisibility::Hidden);
}

void UVoteWidget::NoButtonCallback()
{
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (LobbyPC)
	{
		LobbyPC->SetVote(0);
	}

	HorizontalBox_ButtonBox->SetVisibility(ESlateVisibility::Hidden);
}

void UVoteWidget::RemoveWidget()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (LobbyPC)
	{
		LobbyPC->ClearVote();
	}

	RemoveFromParent();
}

int32 UVoteWidget::GetVoteArray(int32 index)
{
	if (LobbyGS->VoteArray.Num() > index)
	{
		return int32(LobbyGS->VoteArray[index]);
	}
	return int32(2);
}
