// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlitzAnimInstance.h"
#include "Character/BlitzCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void
UBlitzAnimInstance::NativeInitializeAnimation ()
{
  Super::NativeInitializeAnimation ();

  BlitzCharacter = Cast<ABlitzCharacter> (TryGetPawnOwner ());
}

void
UBlitzAnimInstance::NativeUpdateAnimation (float DeltaSeconds)
{
  Super::NativeUpdateAnimation (DeltaSeconds);

  if (BlitzCharacter == nullptr)
    {
      return;
    }

  FVector Velocity = BlitzCharacter->GetVelocity ();
  Velocity.Z = 0.f;
  Speed = Velocity.Size ();

  bIsInAir = BlitzCharacter->GetCharacterMovement ()->IsFalling ();
  bIsAccelerating = BlitzCharacter->GetCharacterMovement ()->
                                    GetCurrentAcceleration ().Size () > 0.f;
  bWeaponEquipped = BlitzCharacter->IsWeaponEquipped ();
  bIsCrouched = BlitzCharacter->bIsCrouched;
}