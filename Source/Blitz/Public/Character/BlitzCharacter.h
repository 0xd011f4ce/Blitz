// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

#include "BlitzCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UWidgetComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class FLifetimeProperty;

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
  virtual void GetLifetimeReplicatedProps (
      TArray<FLifetimeProperty> &OutLifetimeProps) const override;
  virtual void PostInitializeComponents () override;

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

  UPROPERTY (EditAnywhere, Category = Input)
  UInputAction *EquipAction;

  UPROPERTY (EditAnywhere, Category = Input)
  UInputAction *CrouchAction;

  void Move (const FInputActionValue &Value);
  void Look (const FInputActionValue &Value);
  void Equip (const FInputActionValue &Value);
  void CrouchUncrouch (const FInputActionValue &Value);

private:
  UPROPERTY (VisibleAnywhere, Category = Camera)
  USpringArmComponent *CameraBoom;

  UPROPERTY (VisibleAnywhere, Category = Camera)
  UCameraComponent *FollowCamera;

  UPROPERTY (EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"),
    Category = UI)
  UWidgetComponent *OverheadWidget;

  UPROPERTY (ReplicatedUsing = OnRep_OverlappingWeapon)
  AWeapon *OverlappingWeapon;

  UFUNCTION ()
  void OnRep_OverlappingWeapon (AWeapon *LastWeapon);

  UPROPERTY (VisibleAnywhere)
  UCombatComponent *Combat;

  UFUNCTION (Server, Reliable)
  void ServerEquipButtonPressed ();

public:
  /**
   * Setters and Getters
   */
  void
  SetOverlappingWeapon (AWeapon *Weapon);

  bool IsWeaponEquipped () const;
};