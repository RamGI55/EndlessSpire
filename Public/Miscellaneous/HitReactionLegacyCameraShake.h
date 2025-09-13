// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Camera/CameraShakeBase.h"
#include "HitReactionLegacyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UHitReactionShakePattern : public UCameraShakeBase
{
	GENERATED_BODY()
	
public:
	UHitReactionShakePattern(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	


protected:
	virtual void GetShakePatternInfoImpl(FCameraShakeInfo& OutInfo) const ;
	virtual void StartShakePatternImpl(const FCameraShakePatternStartParams& Params) ;
	virtual void UpdateShakePatternImpl(const FCameraShakePatternUpdateParams& Params, FCameraShakePatternUpdateResult& OutResult);
	virtual bool IsFinishedImpl() const ;
private:
	// Legacy-style oscillation parameters
	float Duration;
	float BlendInTime;
	float BlendOutTime;
    
	// Oscillation state
	float ElapsedTime;
	bool bIsActive;
    
	// Oscillation parameters (matching your legacy settings)
	struct FOscillationParams
	{
		float Amplitude;
		float Frequency;
		float Phase; // For random offset
	};
    
	FOscillationParams XOscillation;
	FOscillationParams YOscillation;
	FOscillationParams ZOscillation;
};

/**
 * Camera shake that uses the custom oscillation pattern
 */
UCLASS()
class ENDLESSSPIRE_API UHitReactionLegacyCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
    
public:
	UHitReactionLegacyCameraShake(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};