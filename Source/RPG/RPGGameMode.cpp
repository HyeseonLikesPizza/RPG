// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGGameMode.h"
#include "RPGCharacter.h"
#include "RPGPlayerController.h"

ARPGGameMode::ARPGGameMode()
{
	DefaultPawnClass = ARPGCharacter::StaticClass();
	PlayerControllerClass = ARPGPlayerController::StaticClass();
}
