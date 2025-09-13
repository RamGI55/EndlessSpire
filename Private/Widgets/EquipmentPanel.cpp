// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/EquipmentPanel.h"
#include "Widgets/EquipmentSlot.h"
#include "Components/TextBlock.h"
#include "GameStates/LobbyGameState.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "Blueprint/WidgetTree.h"
#include "UStructs/FPlayerCharacterStat.h"

UEquipmentPanel::UEquipmentPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
    if (WidgetTree)
    {
        TArray<UWidget*> AllWidgets;
        WidgetTree->GetAllWidgets(AllWidgets);  // WidgetTree의 모든 위젯을 가져옵니다.

        // 가져온 위젯들 중에서 UEquipmentSlot 타입인 위젯들을 EquipmentSlots 배열에 추가합니다.
        for (UWidget* Widget : AllWidgets)
        {
            if (UEquipmentSlot* EquipmentSlot = Cast<UEquipmentSlot>(Widget))
            {
                EquipmentSlots.Add(EquipmentSlot);
            }
        }
    }

	ABasePlayerCharacter* Owner = Cast<ABasePlayerCharacter>(GetOwningPlayerPawn());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is nullptr in UEquipmentPanel::NativeOnInitialized"));
		return;
	}
	ALobbyGameState* GS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (!GS)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStatComponent is nullptr in UEquipmentPanel::NativeOnInitialized"));
		return;
	}
	
	
	AttackRateValue->SetText(FText::AsNumber(Owner->GetPlayerStatComponent()->GetStat("Atk")));
	DefenseRateValue->SetText(FText::AsNumber(Owner->GetPlayerStatComponent()->GetStat("Def")));
	CriticalValue->SetText(FText::AsNumber(Owner->GetPlayerStatComponent()->GetStat("CriticalChance")));
	CriticalDamageValue->SetText(FText::AsNumber(Owner->GetPlayerStatComponent()->GetStat("CriticalHitDamage")));

	//UEquipmentSlot* HeadEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_HeadEquipmentSlot")));
	//UEquipmentSlot* ArmorEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_ArmorEquipmentSlot")));
	//UEquipmentSlot* ShoesEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_ShoesEquipmentSlot")));

	//UEquipmentSlot* MainWeaponEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_MainWeaponEquipmentSlot")));
	//UEquipmentSlot* AmuletEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_Amulet1EquipmentSlot")));
	//UEquipmentSlot* RingEquipment = Cast<UEquipmentSlot>(this->GetWidgetFromName(TEXT("WBP_RingEquipmentSlot")));
	
	
}

void UEquipmentPanel::UpdateWidget(FPlayerCharacterStat newStatus)
{


	if (AttackRateValue)
	{
		AttackRateValue->SetText(FText::AsNumber(newStatus.Atk));
	}
	if (DefenseRateValue)
	{
		DefenseRateValue->SetText(FText::AsNumber(newStatus.Def));
	}
	if (CriticalValue)
	{
		CriticalValue->SetText(FText::AsNumber(newStatus.CriticalChance));
	}
	if (CriticalDamageValue)
	{
		CriticalDamageValue->SetText(FText::AsNumber(newStatus.CriticalHitDamage));
	}
	
}