// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstances/EndlessSpireGameInstance.h"
#include <Net/UnrealNetwork.h>

UEndlessSpireGameInstance::UEndlessSpireGameInstance()
{
	Nickname = "Name";
	Class = EClass::NONE;
	Stage = 0;
	PlayerMoney = 300;
}