// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/MultiSpawnProjectile.h"
#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMPlayerStatComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMultiSpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 부모의 단일 투사체 스폰 로직 대신, 부채꼴 모양으로 여러 투사체를 스폰하도록 구현
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	// 권한 확인: 이 RPC는 클라이언트에서 실행되어야 함 (또는 적절하게 처리)
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

	// (1) 시작 위치: 애님 노티파이가 실행될 때 지정한 위치로 설정
	FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);


	// (2) 타겟 위치: 예시로 GetCrosshairTraceHit() 함수로 저장된 타겟 위치 사용 (부모의 로직 참고)
	FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

	// (3) 라인 트레이스를 통해 최종 타겟 위치 결정
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

	// (4) 기본 투사체 발사 방향 계산 (수평만 고려)
	FVector Dir = (FinalTargetLocation - StartLocation).GetSafeNormal();
	FRotator BaseSpawnRot = Dir.Rotation();
	BaseSpawnRot.Pitch = 0.0f; // 수평 방향만 사용

	// (5) 부채꼴 모양으로 투사체 스폰
	int32 Level = Character->TryGetStatComponent()->GetStat("Level");
	int32 ExtraArrowCount = Level / 3;
	int32 Count = (NumProjectiles > 0) ? NumProjectiles + ExtraArrowCount : 1;
	float HalfSpread = SpreadAngleDegrees * 0.5f;



	if (Count == 1)
	{
		// 단일 투사체 스폰
		Character->Server_SpawnProjectile(StartLocation, BaseSpawnRot, Dir, AttackProjectileClass);
	}
	else
	{
		// 각 투사체 간의 각도 간격 계산
		float AngleStep = SpreadAngleDegrees / (Count - 1);
		for (int32 i = 0; i < Count; i++)
		{
			// -HalfSpread 부터 +HalfSpread 까지 각도 오프셋 계산
			float AngleOffset = -HalfSpread + AngleStep * i;
			FRotator SpawnRot = BaseSpawnRot;
			SpawnRot.Yaw += AngleOffset;

			float SpawnOffsetDistance = 20.0f;
			FVector AdjustedSpawnLocation = StartLocation + SpawnRot.Vector() * SpawnOffsetDistance;

			// 수정된 회전으로부터 방향 벡터 계산
			FVector NewDir = SpawnRot.Vector();
			Character->Server_SpawnProjectile(StartLocation, SpawnRot, NewDir, AttackProjectileClass);
		}
	}
}
