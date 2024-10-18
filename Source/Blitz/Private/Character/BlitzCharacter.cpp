// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlitzCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABlitzCharacter::ABlitzCharacter ()
{
  PrimaryActorTick.bCanEverTick = true;

  CameraBoom = CreateDefaultSubobject<USpringArmComponent> (
      TEXT ("CameraBoom"));
  CameraBoom->TargetArmLength = 600.f;
  CameraBoom->bUsePawnControlRotation = true;
  CameraBoom->SetupAttachment (GetMesh ());

  FollowCamera = CreateDefaultSubobject<UCameraComponent> (
      TEXT ("FollowCamera"));
  FollowCamera->SetupAttachment (CameraBoom, USpringArmComponent::SocketName);
  FollowCamera->bUsePawnControlRotation = false;
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