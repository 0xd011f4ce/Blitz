// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BlitzCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputComponent;

UCLASS ()
class BLITZ_API ABlitzCharacter : public ACharacter
{
  GENERATED_BODY ()

public:
  ABlitzCharacter ();

  virtual void Tick (float DeltaTime) override;

  virtual void SetupPlayerInputComponent (
      UInputComponent *PlayerInputComponent) override;

protected:
  virtual void BeginPlay () override;

private:
  UPROPERTY (VisibleAnywhere, Category = Camera)
  USpringArmComponent *CameraBoom;

  UPROPERTY (VisibleAnywhere, Category = Camera)
  UCameraComponent *FollowCamera;
};