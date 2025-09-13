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
	if (!Owner)  // Owner�� ������ �ٷ� ����
	{
		return;
	}

	if (Owner->HasAuthority())  // Owner�� ������ ������ ������ ����
	{
		return;
	}


	UWorld* World = Owner->GetWorld();
	if (!World) return;


	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(Owner);
	if (!Character) return; // ���̽� ĳ���� �ƴϸ� ���� �� ��
	
	if (!AttackProjectileClass) {
		UE_LOG(LogTemp, Warning, TEXT("AttackProjectileClass is nullptr"));
		return;
	}



	




   // (1) ���� ��ġ: �ִԳ�Ƽ���� ���� ������ "������ ��" ���� ��ġ
   FVector StartLocation = MeshComp->GetSocketLocation(SpawnSocket);

   // (2) ��ǥ ��ġ: ������ ����� Ÿ�� ��ġ�� �����´ٰ� ����
   FVector StoredTargetLocation = GetCrosshairTraceHit(Character);

   // (3) ������ ������ StoredTargetLocation�� ���� ���� Ʈ���̽� ���� (�߰� Ʈ���̽��� ���� ��ǥ ��ġ ����)
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

   // ���� ��ǥ ��ġ: Ʈ���̽��� �����ϸ� ImpactPoint, �����ϸ� StoredTargetLocation ���
   FVector FinalTargetLocation = bHit ? HitResult.ImpactPoint : StoredTargetLocation;

   // ����� ���� ǥ�� (�ɼ�)
   //DrawDebugLine(World, StartLocation, FinalTargetLocation, FColor::Red, false, 2.0f, 0, 2.0f);

   // (4) ����ü �߻縦 ���� ���� ���
   FVector Dir = (FinalTargetLocation - StartLocation).GetSafeNormal();
   FRotator SpawnRot = Dir.Rotation();

   // (5) ����ü ����: ProjectileClass�� �����Ǿ� �־�� ��
 
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
	
	// Ʈ���̽�
	FHitResult Hit;
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceDirection = Camera->GetForwardVector();
	FVector TraceEnd = TraceStart + TraceDirection * 100000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character); // �ڱ� �ڽ� ����

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
		// �浹ü ������ �� �Ÿ���
		CameraHitLocation = TraceEnd;
	}

	 //����� ���� ǥ��(�ʿ� ��)
	//DrawDebugLine(GetWorld(), TraceStart, CameraHitLocation, FColor::Green, false, 2.0f);


	return CameraHitLocation;
}




