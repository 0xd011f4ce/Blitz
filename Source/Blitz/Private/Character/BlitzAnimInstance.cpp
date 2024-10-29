// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlitzAnimInstance.h"
#include "Character/BlitzCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
  bAiming = BlitzCharacter->IsAiming ();

  // Offset yaw for strafing
  /// all these variables are already replicated
  FRotator AimRotation = BlitzCharacter->GetBaseAimRotation ();
  FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX (
      BlitzCharacter->GetVelocity ());
  FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator (
      MovementRotation, AimRotation);
  DeltaRotation = FMath::RInterpTo (DeltaRotation, DeltaRot, DeltaSeconds,
                                    6.f);
  YawOffset = DeltaRotation.Yaw;

  CharacterRotationLastFrame = CharacterRotation;
  CharacterRotation = BlitzCharacter->GetActorRotation ();
  const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator (
      CharacterRotation, CharacterRotationLastFrame);
  const float Target = Delta.Yaw / DeltaSeconds;
  const float Interp = FMath::FInterpTo (Lean, Target, DeltaSeconds, 6.f);
  Lean = FMath::Clamp (Interp, -90, 90);

  AO_Yaw = BlitzCharacter->GetAO_Yaw ();
  AO_Pitch = BlitzCharacter->GetAO_Pitch ();
}