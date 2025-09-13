// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController/MobAIController.h"
#include "Actors/Characters/NonPlayers/MobMonsterCharacter.h"
#include "NavigationSystem.h"
#include "ActorComponents/MMPlayerStatComponent.h"

#include "Actors/Characters/Players/BasePlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceTypes.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"

#include "PlayerControllers/BasePlayerController.h"
#include "UObject/ConstructorHelpers.h"


// 정적으로 Blackboard Key Name을 정의함. 
const FName AMobAIController::TargetPlayerKey(TEXT("TargetPlayer")); // Move to BaseAIController. 
const FName AMobAIController::fDisatncePlayerKey(TEXT("fDistanceToPlayer"));
const FName AMobAIController::PatrolPosKey(TEXT("PatrolPos"));

AMobAIController::AMobAIController()
{	
	static::ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageAsset(TEXT("/Game/NonPlayers/Monster/Animations/Mobs/AMZombieAttackMob.AMZombieAttackMob"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	if (AIPerception)
	{
		SetPerceptionComponent(*AIPerception);
		SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		if (!SightConfig)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create SightConfig"));
		}
			SightConfig->SightRadius = 1250.f;
			SightConfig->LoseSightRadius = 2500.f;
			SightConfig->PeripheralVisionAngleDegrees = 75.f;
			SightConfig->SetMaxAge(5.f);
			SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			
			AIPerception->ConfigureSense(*SightConfig);
			AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); 

		HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingCongfig"));
		if (!HearingConfig)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create HearingConfig"));
		}
			HearingConfig->HearingRange = 1500.f;
			HearingConfig->SetMaxAge(3.f);

			HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
			HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
			AIPerception->ConfigureSense(*HearingConfig);

		DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
		if (!DamageConfig)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create DamageConfig"));
		}
			DamageConfig->SetMaxAge(5.0f);
		// Configure damage detection affiliations
			AIPerception->ConfigureSense(*DamageConfig);
		
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AMobAIController::PerceptionUpdated);

	}
	if (AttackMontageAsset.Succeeded())
	{
		AttackMontage = AttackMontageAsset.Object;
	}
	bReplicates = true;
	/*FTimerHandle TimerHandle; 
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::StartMoving, 0.2f, false);*/
}

void AMobAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const PlayerCharacter = Cast<ABasePlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("isSenseTarget", Stimulus.WasSuccessfullySensed()); 
		GetBlackboardComponent()->SetValueAsVector("TargetLoc", Stimulus.StimulusLocation);
	}
}

void AMobAIController:: SetStatusAsNormal()
{
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum("EState", Patrolling);
		Blackboard->SetValueAsObject(TargetPlayerKey, nullptr);
	}
}

void AMobAIController::SetStatusAsAttacking(AActor* AttackTarget)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum("EState", Attacking);
		Blackboard->SetValueAsObject(TargetPlayerKey, AttackTarget);

		// Attack Motion After. 
	}
}

bool AMobAIController::isPlayerCharacter(AActor* Actor) /// 잘 작동함!! 
{
	
	// Player Character인지, 월드상의 있는 플레이어 컨틀롤러를 모두 잡아서, 스캔 데이터와 비교.
	UWorld* World = GetWorld();
	if (!World||!World->IsNetMode(NM_Client)&& !World->IsNetMode(NM_DedicatedServer))
		return false;
	for (FConstPlayerControllerIterator Iterator = World -> GetPlayerControllerIterator(); Iterator; ++ Iterator)
	{
		APlayerController* PlayerController = Iterator->Get(); // TODO: BasedPlayerController가 안되어서, 대부모로 받음. 확인 요망 
		if (!PlayerController)
		{
			continue; 
		}
		APawn* PlayerPawn = PlayerController->GetPawn();

		if (PlayerPawn && PlayerPawn == Actor)
		{
			return true;
		}
	}
	return false; 
}

void AMobAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AMobMonsterCharacter* MobMonster = Cast<AMobMonsterCharacter>(GetPawn());
	BTAsset = MobMonster->GetBehaviorTree();
	if (MobMonster && MobMonster->GetBehaviorTree())
	{
		RunBehaviorTree(MobMonster->GetBehaviorTree());
		if (BTAsset)
		{
			UseBlackboard(BTAsset->GetBlackboardAsset(), BBAsset);
			Blackboard = BBAsset;
			RunBehaviorTree(BTAsset);
		}
	}
	SetStatusAsNormal();
}

void AMobAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMobAIController::PerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	for (AActor* UpdateActor : UpdateActors)
	{
		FAIStimulus AIStimulus;
		AIStimulus = CanSenseActor(UpdateActor, EAIPerceptionSense::EPS_Sight);
		if (AIStimulus.WasSuccessfullySensed())
		{
			PrintPersivedActors();
			//UE_LOG(LogTemp, Warning, TEXT("AI can see: %s"), *UpdateActor->GetName());
			HandleSensedSight(UpdateActor);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("AI lost sight of: %s"), *UpdateActor->GetName());
			HandleSensedSight(UpdateActor);  // Update AI state when sight is lost
		}
		AIStimulus = CanSenseActor(UpdateActor, EAIPerceptionSense::EPS_Hearing);
		if (AIStimulus.WasSuccessfullySensed())
		{
			HandleSensedHearing(UpdateActor);
		}
		AIStimulus = CanSenseActor(UpdateActor, EAIPerceptionSense::EPS_Damage);
		if (AIStimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage stimulus detected for %s"), *UpdateActor->GetName());
			HandleSensedDamage(UpdateActor);
		}
	}
}

FAIStimulus AMobAIController::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus; 

	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_None:
		break;
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Hearing:
		QuerySenseClass = UAISense_Hearing::StaticClass();
		break; 
	case EAIPerceptionSense::EPS_Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Max:
		break;
	default:
		break; 
	}
	TSubclassOf<UAISense> LastSensedStimulusClass;
	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this,AIStimulus);

		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;
			return ResultStimulus; 
		}
	}
	return ResultStimulus; 
}

void AMobAIController::HandleSensedSight(AActor* Actor)
{
	// get the signal from updated function then send the signal to the blackboard/actor
	if (isPlayerCharacter(Actor))
	{
		
		bool isCanSensePlayer = CanSenseActor(Actor, EAIPerceptionSense::EPS_Sight).WasSuccessfullySensed();
		if (isCanSensePlayer)
		{
			Blackboard->SetValueAsBool("isSenseTarget", true);
			SetStatusAsAttacking(Actor);
			GetBrainComponent()->RestartLogic();
		}
		else
		{
			Blackboard->SetValueAsBool("isSenseTarget", false);
			Blackboard->ClearValue(TargetPlayerKey);
			SetStatusAsNormal();
			Blackboard->SetValueAsBool("isTargetLocked", false);
			GetBrainComponent()->RestartLogic();
			
		}
		UE_LOG(LogTemp, Warning, TEXT("AI sight status changed: %s"), isCanSensePlayer ? TEXT("SEES TARGET") : TEXT("LOST TARGET"));
	}
}
void AMobAIController::HandleSensedHearing(AActor* Actor)
{
}

void AMobAIController::HandleSensedDamage(AActor* Actor)
{
	// Get the damage stimulus
	FAIStimulus DamageStimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Damage);
	bool DamagedFromActor = DamageStimulus.WasSuccessfullySensed();
    
	// Print detailed debug info
	UE_LOG(LogTemp, Warning, TEXT("Damage handling for actor: %s, Sensed: %s, Stimulus Age: %.2f, Stimulus Strength: %.2f"), 
		   *Actor->GetName(), 
		   DamagedFromActor ? TEXT("TRUE") : TEXT("FALSE"),
		   DamageStimulus.GetAge(),
		   DamageStimulus.Strength);
    
	if (isPlayerCharacter(Actor))
	{
		if (DamagedFromActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("AI DAMAGED BY PLAYER: Setting attack state"));
			Blackboard->SetValueAsBool("isSenseTarget", true);
			SetStatusAsAttacking(Actor);
			GetBrainComponent()->RestartLogic();
		}
	}
}


bool AMobAIController::IsStuck(float inStuck = 10.0f, float inTime = 1.0f) // AI didn't get stucked, seems like BT issue for animation. 
{
	static FVector LastPos;
	static float StuckTime = 0.0f;

	FVector CurrentPos = this->GetPawn()->GetActorLocation();
	float DistanceMoved = FVector::Dist(CurrentPos, LastPos);

	if (DistanceMoved < inStuck)
	{
		StuckTime += GetWorld()->GetDeltaSeconds();
		if (StuckTime >= inTime)
		{
			StuckTime = 0.0f; 
			// AI is stuck, take action
			UE_LOG(LogTemp, Warning, TEXT("AI is stuck!")); // didn't show up even the monster stucks. 
			return true;
		}
	}
	else
	{
		StuckTime = 0.0f;
	}
	
	return false; 
} 

void AMobAIController::PrintPersivedActors() // AI가 시각적으로 잡은 플레이어 출력. 
{
	// Get all actors currently perceived by sight
	TArray<AActor*> PerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	
	// Print the number of actors detected
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
	//								FString::Printf(TEXT("Detected %d actors"), PerceivedActors.Num()));
    
	// Print info about each actor
	int32 Index = 0;
	for (AActor* Actor : PerceivedActors)
	{
		if (isPlayerCharacter(Actor)) // 플레이어만 잡는지 확인. 
		{
			// Get actor info
			FString ActorName = Actor->GetName();
			FVector ActorLocation = Actor->GetActorLocation();
            
			// Print actor info
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
				FString::Printf(TEXT("Actor %d: %s at Location: X=%.1f Y=%.1f Z=%.1f"), 
				Index, *ActorName, ActorLocation.X, ActorLocation.Y, ActorLocation.Z));
            
			Index++;
		}
	}
}
