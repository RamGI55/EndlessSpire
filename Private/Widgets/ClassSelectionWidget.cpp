// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ClassSelectionWidget.h"
#include "Actors/Characters/NonPlayers/ClassGuideNonPlayerCharacter.h"
#include "PlayerControllers/LobbyPlayerController.h"
#include "Components/Button.h"
#include "Components/Spacer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/VerticalBox.h"

UClassSelectionWidget::UClassSelectionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerCharacterStatDataTable(TEXT("/Script/Engine.DataTable'/Game/Data/DataTable/DT_PlayerCharacterStat.DT_PlayerCharacterStat'"));

	ClassStatDataTable = PlayerCharacterStatDataTable.Object;
}

void UClassSelectionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	

	Button_Warrior->OnClicked.AddDynamic(this, &UClassSelectionWidget::WarriorButtonCallback);
	Button_Mage->OnClicked.AddDynamic(this, &UClassSelectionWidget::MageButtonCallback);
	Button_Archer->OnClicked.AddDynamic(this, &UClassSelectionWidget::ArcherButtonCallback);
	Button_Cleric->OnClicked.AddDynamic(this, &UClassSelectionWidget::ClericButtonCallback);
	Button_OK->OnClicked.AddDynamic(this, &UClassSelectionWidget::OkButtonCallback);
	Button_CANCLE->OnClicked.AddDynamic(this, &UClassSelectionWidget::CancleButtonCallback);
	Button_Remove->OnClicked.AddDynamic(this, &UClassSelectionWidget::RemoveButtonCallback);

	FInputModeUIOnly InputMode;
	if (!GetOwningPlayer()) {
		UE_LOG(LogTemp, Log, TEXT("saeorghseriulghlresuighresuilghilresugherlsiughiulergh"));
	}
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}

void UClassSelectionWidget::WarriorButtonCallback()
{
	SelectClass = EClass::WARRIOR;
	VerticalBox_CheckTheSelectedClass->SetVisibility(ESlateVisibility::Visible);
}

void UClassSelectionWidget::MageButtonCallback()
{
	SelectClass = EClass::MAGE;
	VerticalBox_CheckTheSelectedClass->SetVisibility(ESlateVisibility::Visible);
}

void UClassSelectionWidget::ArcherButtonCallback()
{
	SelectClass = EClass::ARCHER;
	VerticalBox_CheckTheSelectedClass->SetVisibility(ESlateVisibility::Visible);
}

void UClassSelectionWidget::ClericButtonCallback()
{
	SelectClass = EClass::CLERIC;
	Spacer->SetVisibility(ESlateVisibility::Hidden);
	VerticalBox_CheckTheSelectedClass->SetVisibility(ESlateVisibility::Visible);
}

void UClassSelectionWidget::OkButtonCallback()
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EClass"));
	ClassStat = *ClassStatDataTable->FindRow<FPlayerCharacterStat>(FName(EnumPtr->GetNameStringByValue((int32)SelectClass)), FString(""));

	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->ServerUpdateStat(ClassStat);
		PlayerController->ServerUpdateClass(SelectClass);
	}

	AClassGuideNonPlayerCharacter* ClassGuideNonPlayerCharacter = nullptr;
	for (TActorIterator<AClassGuideNonPlayerCharacter> It(GetWorld()); It; ++It)
	{
		ClassGuideNonPlayerCharacter = *It;
		break; // 첫 번째 발견된 액터를 반환
	}
	if (ClassGuideNonPlayerCharacter)
	{
		ClassGuideNonPlayerCharacter->GetMesh()->SetRenderCustomDepth(false);
	}

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	RemoveFromParent();
}

void UClassSelectionWidget::CancleButtonCallback()
{
	VerticalBox_CheckTheSelectedClass->SetVisibility(ESlateVisibility::Collapsed);
}

void UClassSelectionWidget::RemoveButtonCallback()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	RemoveFromParent();
}
