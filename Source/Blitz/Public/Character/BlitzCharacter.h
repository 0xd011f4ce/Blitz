// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BlitzCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;

UCLASS ()
class BLITZ_API ABlitzCharacter : public ACharacter
{
  GENERATED_BODY ()

public:
  ABlitzCharacter ();

  virtual void Tick (float DeltaTime) override;

  virtual void SetupPlayerInputComponent (
      UInputComponent *PlayerInputComponent) override;

  virtual void Jump () override;

protected:
  virtual void BeginPlay () override;

  /**
   * Input
   */
  UPROPERTY (EditAnywhere, Category = Input)
  UInputMappingContext *MappingContext;

  UPROPERTY (EditAnywhere, Category = Input)
  UInputAction *MovementAction;

  UPROPERTY (EditAnywhere, Category = Input)
  UInputAction *LookAction;

  UPROPERTY (EditAnywhere, Category = Input)
  UInputAction *JumpAction;

  void Move (const FInputActionValue &Value);
  void Look (const FInputActionValue &Value);

private:
  UPROPERTY (VisibleAnywhere, Category = Camera)
  USpringArmComponent *CameraBoom;

  UPROPERTY (VisibleAnywhere, Category = Camera)
  UCameraComponent *FollowCamera;
};