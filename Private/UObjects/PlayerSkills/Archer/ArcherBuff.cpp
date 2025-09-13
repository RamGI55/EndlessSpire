// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Archer/ArcherBuff.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "Actors/Characters/Players/ArcherPlayerCharacter.h"
#include "UStructs/FPlayerCharacterStat.h"

#include "ActorComponents/MMPlayerStatComponent.h"

UArcherBuff::UArcherBuff()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Archer_Buff"), FString("UArcherBasicSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UArcherBuff::OnBeginExecute()
{
	Super::OnBeginExecute();

    // 콤보 몽타주 실행
    PlayComboMontageWithServer(SkillData.Montage[0]);

    //UWorld* World = GetWorld();
    //if (!World)
    //{
    //    return;
    //}

    //// 기준 위치 설정: 만약 Buff가 어떤 캐릭터에 붙어 있다면 그 캐릭터의 위치를 기준으로 함.
    //// GetTypedOuter<AActor>()를 통해 Buff의 외부에 있는 AActor를 가져온다고 가정합니다.
    //AArcherPlayerCharacter* BuffOwner = GetTypedOuter<AArcherPlayerCharacter>();
    //FVector Origin = BuffOwner ? BuffOwner->GetActorLocation() : FVector::ZeroVector;

    //// 30미터 (Unreal Unit은 cm 기준이므로 30 * 100 = 3000)
    //const float SearchRadius = 3000.0f;

    //// 월드 내의 모든 ABasePlayerCharacter 액터들을 찾습니다.
    //TArray<AActor*> FoundPlayers;
    //UGameplayStatics::GetAllActorsOfClass(World, ABasePlayerCharacter::StaticClass(), FoundPlayers);

    //// 각 플레이어를 순회하며 Origin으로부터의 거리를 계산하고, 반경 내에 있는 플레이어들의 스탯 정보를 가져옵니다.
    //for (AActor* Actor : FoundPlayers)
    //{
    //    if (ABasePlayerCharacter* OtherPlayer = Cast<ABasePlayerCharacter>(Actor))
    //    {
    //        const float Distance = FVector::Dist(Origin, OtherPlayer->GetActorLocation());
    //        if (Distance <= SearchRadius)
    //        {
    //            // 플레이어의 스탯 정보를 가져옴
    //            FPlayerCharacterStat CharacterStats = OtherPlayer->GetPlayerStatComponent()->GetPlayerStats();

    //            // 가져온 스탯 정보를 기반으로 추가 스탯 부여
    //            CharacterStats.AttackSpeed *= (CharacterStats.AttackSpeed + BuffOwner->GetPlayerStatComponent()->GetStat("Level")*0.05f);
    //            CharacterStats.Atk *= (CharacterStats.Atk + BuffOwner->GetPlayerStatComponent()->GetStat("Level") * 0.05f);

    //            UE_LOG(LogTemp, Log, TEXT("AttackSpeed : %f"), CharacterStats.AttackSpeed);
    //            UE_LOG(LogTemp, Log, TEXT("Atk : %d"), CharacterStats.Atk);
    //        }
    //    }
    //    
    //}
    //if (FoundPlayers.Num() == 0)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("No Players Found"));
    //}
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlayComboMontageWithServer(SkillData.Montage[0]);
	}
}
