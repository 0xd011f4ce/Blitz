// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "BlitzAnimInstance.generated.h"

class ABlitzCharacter;
/**
 * 
 */
UCLASS ()
class BLITZ_API UBlitzAnimInstance : public UAnimInstance
{
  GENERATED_BODY ()

public:
  virtual void NativeInitializeAnimation () override;
  virtual void NativeUpdateAnimation (float DeltaSeconds) override;

private:
  UPROPERTY (BlueprintReadOnly, Category = Character,
    meta = (AllowPrivateAccess = "true"))
  ABlitzCharacter *BlitzCharacter;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  float Speed;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  bool bIsInAir;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  bool bIsAccelerating;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  bool bWeaponEquipped;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  bool bIsCrouched;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  bool bAiming;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  float YawOffset;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  float Lean;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  float AO_Yaw;

  UPROPERTY (BlueprintReadOnly, Category = Movement,
    meta = (AllowPrivateAccess = "true"))
  float AO_Pitch;

  FRotator CharacterRotationLastFrame;
  FRotator CharacterRotation;
  FRotator DeltaRotation;
};