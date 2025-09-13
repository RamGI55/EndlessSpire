// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/TitleHUD.h"
#include "Blueprint/UserWidget.h"

ATitleHUD::ATitleHUD()
{
	// 블루프린트 클래스 불러오기
	static ConstructorHelpers::FClassFinder<UUserWidget>TitleWidgetAsset(TEXT("WidgetBluePrint'/Game/UI/WBP_Title.WBP_Title_C'"));
	
	// 템플릿 클래스에 블루프린트 클래스 넣기
	if (TitleWidgetAsset.Succeeded())
	{
		TitleWidgetClass = TitleWidgetAsset.Class;
	}
}

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowTitleWidget(TitleWidgetClass);
}

void ATitleHUD::ShowTitleWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

	if (NewWidgetClass != nullptr)
	{
		TitleWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (TitleWidget != nullptr)
		{
			TitleWidget->AddToViewport();
		}
	}
}

