// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MinimapWidget.h"
#include "GameStates/LobbyGameState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"

void UMinimapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_Minimap = Cast<UImage>(GetWidgetFromName(TEXT("Image_Minimap")));

}

void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		TextBlock_Stage->SetText(FText::AsNumber(GameState->Stage));
	}
}


