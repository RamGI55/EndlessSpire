// Fill out your copyright notice in the Description page of Project Settings.


#include "UStructs/FSkillTraceData.h"


FSkillTraceData::FSkillTraceData()
{
	BoxExtent = FVector(100.0f, 100.0f, 100.0f);
	TraceDirection = FVector(1.0f, 0.0f, 0.0f);
}
