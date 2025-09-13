// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "HUDS/LobbyHUD.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"



void USpawnProjectileAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)  // Owner가 없으면 바로 리턴
	{
		return;
	}

	if (Owner->HasAuthority())  // Owner가 권한을 가지고 있으면 리턴
	{
		return;
	}


	UWorld* World = Owner->GetWorld();
	if (!World) return;


	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(Owner);
	if (!Character) return; // 베이스 캐릭터 아니면 실행 안 함
	
	if (!AttackProjectileClass) {
		UE_LOG(LogTemp, Warning, TEXT("AttackProjectileClass is nullptr"));
		return;
	}



	




   // (1) 시작 위치: 애님노티파이 실행 시점의 "오른손 끝" 소켓 위치
   FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);

   // (2) 목표 위치: 이전에 저장된 타겟 위치를 가져온다고 가정
   FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

   // (3) 오른손 끝에서 StoredTargetLocation을 향해 라인 트레이스 실행 (추가 트레이스로 최종 목표 위치 결정)
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

   // 최종 목표 위치: 트레이스에 성공하면 ImpactPoint, 실패하면 StoredTargetLocation 사용
   FVector FinalTargetLocation = bHit ? HitResult.ImpactPoint : StoredTargetLocation;

   // 디버그 라인 표시 (옵션)
   //DrawDebugLine(World, StartLocation, FinalTargetLocation, FColor::Red, false, 2.0f, 0, 2.0f);

   // (4) 투사체 발사를 위한 방향 계산
   FVector Dir = (FinalTargetLocation - StartLocation).GetSafeNormal();
   FRotator SpawnRot = Dir.Rotation();

   // (5) 투사체 스폰: ProjectileClass가 지정되어 있어야 함
 
   Character->Server_SpawnProjectile(StartLocation, SpawnRot, Dir, AttackProjectileClass);


}


FVector USpawnProjectileAnimNotify::GetCrosshairTraceHit(ABaseCombatCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp,Warning,TEXT("Character : Fail"))
		return FVector::ZeroVector;
	}

	UCameraComponent* Camera = Character->GetFollowCamera();
	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera is nullptr"));
		return FVector::ZeroVector;
	}
	
	// 트레이스
	FHitResult Hit;
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceDirection = Camera->GetForwardVector();
	FVector TraceEnd = TraceStart + TraceDirection * 100000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character); // 자기 자신 무시

	bool bHit = Character->GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		CameraHitLocation = Hit.ImpactPoint;
	}
	else
	{
		// 충돌체 없으면 먼 거리로
		CameraHitLocation = TraceEnd;
	}

	 //디버그 라인 표시(필요 시)
	//DrawDebugLine(GetWorld(), TraceStart, CameraHitLocation, FColor::Green, false, 2.0f);


	return CameraHitLocation;
}




