// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlitzCharacter.h"

ABlitzCharacter::ABlitzCharacter ()
{
  PrimaryActorTick.bCanEverTick = true;
}

void
ABlitzCharacter::Tick (float DeltaTime)
{
  Super::Tick (DeltaTime);
}

void
ABlitzCharacter::SetupPlayerInputComponent (
    UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent (PlayerInputComponent);

}

void
ABlitzCharacter::BeginPlay ()
{
  Super::BeginPlay ();
}