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

    // �޺� ��Ÿ�� ����
    PlayComboMontageWithServer(SkillData.Montage[0]);

    //UWorld* World = GetWorld();
    //if (!World)
    //{
    //    return;
    //}

    //// ���� ��ġ ����: ���� Buff�� � ĳ���Ϳ� �پ� �ִٸ� �� ĳ������ ��ġ�� �������� ��.
    //// GetTypedOuter<AActor>()�� ���� Buff�� �ܺο� �ִ� AActor�� �����´ٰ� �����մϴ�.
    //AArcherPlayerCharacter* BuffOwner = GetTypedOuter<AArcherPlayerCharacter>();
    //FVector Origin = BuffOwner ? BuffOwner->GetActorLocation() : FVector::ZeroVector;

    //// 30���� (Unreal Unit�� cm �����̹Ƿ� 30 * 100 = 3000)
    //const float SearchRadius = 3000.0f;

    //// ���� ���� ��� ABasePlayerCharacter ���͵��� ã���ϴ�.
    //TArray<AActor*> FoundPlayers;
    //UGameplayStatics::GetAllActorsOfClass(World, ABasePlayerCharacter::StaticClass(), FoundPlayers);

    //// �� �÷��̾ ��ȸ�ϸ� Origin���κ����� �Ÿ��� ����ϰ�, �ݰ� ���� �ִ� �÷��̾���� ���� ������ �����ɴϴ�.
    //for (AActor* Actor : FoundPlayers)
    //{
    //    if (ABasePlayerCharacter* OtherPlayer = Cast<ABasePlayerCharacter>(Actor))
    //    {
    //        const float Distance = FVector::Dist(Origin, OtherPlayer->GetActorLocation());
    //        if (Distance <= SearchRadius)
    //        {
    //            // �÷��̾��� ���� ������ ������
    //            FPlayerCharacterStat CharacterStats = OtherPlayer->GetPlayerStatComponent()->GetPlayerStats();

    //            // ������ ���� ������ ������� �߰� ���� �ο�
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
