// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ShopItemSlot.h"
#include "Widgets/ShopWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UShopItemSlot::NativeOnInitialized()
{
	
}

FReply UShopItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		ParentWidget->VerticalBox_Buttons->SetVisibility(ESlateVisibility::Visible);
		ParentWidget->SelectItem = Item;
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	return Reply.Unhandled();
}

void UShopItemSlot::SetItemInfo(const FItemData& Element, UShopWidget* Parent)
{
	Item = Element;
	ParentWidget = Parent;

	switch (Item.ItemQuality)
	{
	case EItemQuality::Unique:
		ItemBorder->SetBrushColor(FLinearColor::Red);
		break;
	case EItemQuality::Epic:
		ItemBorder->SetBrushColor(FLinearColor(238.0f, 130.0f, 238.0f, 1.0f));
		break;
	case EItemQuality::Rare:
		ItemBorder->SetBrushColor(FLinearColor::Yellow);
		break;
	case EItemQuality::Magic:
		ItemBorder->SetBrushColor(FLinearColor::Blue);
		break;
	case EItemQuality::Common:
		ItemBorder->SetBrushColor(FLinearColor::Gray);
		break;
	default:;
	}

	ItemIcon->SetBrushFromTexture(Item.AssetData.Icon);
	ItemPrice_Buy->SetText(FText::Format(NSLOCTEXT("YourNamespace", "PriceText", "$ {0}"), Item.ItemStatistics.SellValue * 1.5));
	ItemPrice_Sell->SetText(FText::Format(NSLOCTEXT("YourNamespace", "PriceText", "$ {0}"), Item.ItemStatistics.SellValue));
}

