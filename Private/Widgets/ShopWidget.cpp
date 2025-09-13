// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ShopWidget.h"
#include "HUDs/LobbyHUD.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "ActorComponents/MMInventoryComponent.h"
#include "UObjects/ItemBase.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Widgets/ShopItemSlot.h"


void UShopWidget::NativeOnInitialized()
{
	LobbyHUD = Cast<ALobbyHUD>(GetOwningPlayer()->GetHUD());
	if (LobbyHUD)
	{
		LobbyHUD->PlayerUI->InventoryUI->SetVisibility(ESlateVisibility::Visible);
	}

	Player = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryReference = Player->GetInventory();
	}

	Button_Buy->OnClicked.AddDynamic(this, &UShopWidget::BuyButtonCallback);
	Button_Sell->OnClicked.AddDynamic(this, &UShopWidget::SellButtonCallback);
	Button_Exit->OnClicked.AddDynamic(this, &UShopWidget::ExitButtonCallback);

	VerticalBox_Buttons->SetVisibility(ESlateVisibility::Hidden);

	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}


void UShopWidget::SetOwner(AWanderingTraderNonPlayer* Owner)
{
	WanderingTrader = Owner;
}

void UShopWidget::UpdateItems(TArray<FItemData>& Items)
{
	WrapBox_ShopWrapBox->ClearChildren();

	for (const FItemData& Element : Items)
	{
		UShopItemSlot* ItemSlot = CreateWidget<UShopItemSlot>(this, ShopItemSlotClass);

		ItemSlot->SetItemInfo(Element, this);
		WrapBox_ShopWrapBox->AddChildToWrapBox(ItemSlot);
	}
}

void UShopWidget::BuyButtonCallback()
{
	if (InventoryReference->Money >= SelectItem.ItemStatistics.SellValue * 1.5)
	{
		InventoryReference->ServerNewData(SelectItem);
		InventoryReference->ServerModifyMoney(-SelectItem.ItemStatistics.SellValue * 1.5);
	}
}

void UShopWidget::SellButtonCallback()
{
	for(const UItemBase* Element : InventoryReference->GetInventoryContents())
	{
		if (Element->ID == SelectItem.ID)
		{
			InventoryReference->ServerRemoveItem(SelectItem.ID);
			InventoryReference->ServerModifyMoney(SelectItem.ItemStatistics.SellValue);
			break;
		}
	}
}

void UShopWidget::ExitButtonCallback()
{
	LobbyHUD->PlayerUI->InventoryUI->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	RemoveFromParent();
}

FText UShopWidget::GetSelectItemName()
{
	return FText(SelectItem.TextData.Name);
}
