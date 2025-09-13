// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MultiSpawnProjectile.h"
#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMultiSpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// �θ��� ���� ����ü ���� ���� ���, ��ä�� ������� ���� ����ü�� �����ϵ��� ����
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	// ���� Ȯ��: �� RPC�� Ŭ���̾�Ʈ���� ����Ǿ�� �� (�Ǵ� �����ϰ� ó��)
	if (Owner->HasAuthority())
	{
		return;
	}

	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(Owner);
	if (!Character || !MeshComp)
	{
		return;
	}

	UWorld* World = Character->GetWorld();
	if (!World)
	{
		return;
	}

	// (1) ���� ��ġ: �ִ� ��Ƽ���̰� ����� �� ������ ��ġ�� ����
	FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);


	// (2) Ÿ�� ��ġ: ���÷� GetCrosshairTraceHit() �Լ��� ����� Ÿ�� ��ġ ��� (�θ��� ���� ����)
	FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

	// (3) ���� Ʈ���̽��� ���� ���� Ÿ�� ��ġ ����
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		StoredTargetLocation,
		ECC_Visibility,
		QueryParams
	);
	FVector FinalTargetLocation = bHit ? HitResult.ImpactPoint : StoredTargetLocation;

	// (4) �⺻ ����ü �߻� ���� ��� (���� ���)
	FVector Dir = (FinalTargetLocation - StartLocation).GetSafeNormal();
	FRotator BaseSpawnRot = Dir.Rotation();
	BaseSpawnRot.Pitch = 0.0f; // ���� ���⸸ ���

	// (5) ��ä�� ������� ����ü ����
	int32 Level = Character->TryGetStatComponent()->GetStat("Level");
	int32 ExtraArrowCount = Level / 3;
	int32 Count = (NumProjectiles > 0) ? NumProjectiles + ExtraArrowCount : 1;
	float HalfSpread = SpreadAngleDegrees * 0.5f;



	if (Count == 1)
	{
		// ���� ����ü ����
		Character->Server_SpawnProjectile(StartLocation, BaseSpawnRot, Dir, AttackProjectileClass);
	}
	else
	{
		// �� ����ü ���� ���� ���� ���
		float AngleStep = SpreadAngleDegrees / (Count - 1);
		for (int32 i = 0; i < Count; i++)
		{
			// -HalfSpread ���� +HalfSpread ���� ���� ������ ���
			float AngleOffset = -HalfSpread + AngleStep * i;
			FRotator SpawnRot = BaseSpawnRot;
			SpawnRot.Yaw += AngleOffset;

			float SpawnOffsetDistance = 20.0f;
			FVector AdjustedSpawnLocation = StartLocation + SpawnRot.Vector() * SpawnOffsetDistance;

			// ������ ȸ�����κ��� ���� ���� ���
			FVector NewDir = SpawnRot.Vector();
			Character->Server_SpawnProjectile(StartLocation, SpawnRot, NewDir, AttackProjectileClass);
		}
	}
}
