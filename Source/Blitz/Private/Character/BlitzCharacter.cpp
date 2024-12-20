// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlitzCharacter.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "BlitzComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"

#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Weapon/Weapon.h"

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

  bUseControllerRotationYaw = false;
  GetCharacterMovement ()->bOrientRotationToMovement = true;

  OverheadWidget = CreateDefaultSubobject<UWidgetComponent> (
      TEXT ("OverheadWidget"));
  OverheadWidget->SetupAttachment (RootComponent);

  Combat = CreateDefaultSubobject<UCombatComponent> (TEXT ("CombatComponent"));
  Combat->SetIsReplicated (true); // replicate this component

  GetCharacterMovement ()->NavAgentProps.bCanCrouch = true;
  GetCapsuleComponent ()->SetCollisionResponseToChannel (
      ECC_Camera, ECR_Ignore);
  GetMesh ()->SetCollisionResponseToChannel (ECC_Camera, ECR_Ignore);
}

void
ABlitzCharacter::Tick (float DeltaTime)
{
  Super::Tick (DeltaTime);

  AimOffset (DeltaTime);
}

void
ABlitzCharacter::SetupPlayerInputComponent (
    UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent (PlayerInputComponent);

  if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<
    UEnhancedInputComponent> (PlayerInputComponent))
    {
      EnhancedInputComponent->BindAction (MovementAction,
                                          ETriggerEvent::Triggered, this,
                                          &ThisClass::Move);
      EnhancedInputComponent->BindAction (LookAction, ETriggerEvent::Triggered,
                                          this, &ThisClass::Look);
      EnhancedInputComponent->BindAction (JumpAction, ETriggerEvent::Triggered,
                                          this, &ThisClass::Jump);
      EnhancedInputComponent->BindAction (EquipAction,
                                          ETriggerEvent::Triggered,
                                          this, &ThisClass::Equip);
      EnhancedInputComponent->BindAction (CrouchAction,
                                          ETriggerEvent::Started, this,
                                          &ThisClass::CrouchUncrouch);
      EnhancedInputComponent->BindAction (CrouchAction,
                                          ETriggerEvent::Completed, this,
                                          &ThisClass::CrouchUncrouch);
      EnhancedInputComponent->BindAction (AimAction,
                                          ETriggerEvent::Started, this,
                                          &ThisClass::Aim);
      EnhancedInputComponent->BindAction (AimAction,
                                          ETriggerEvent::Completed, this,
                                          &ThisClass::Aim);
    }
}

void
ABlitzCharacter::Jump ()
{
  Super::Jump ();
}

void
ABlitzCharacter::GetLifetimeReplicatedProps (
    TArray<FLifetimeProperty> &OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps (OutLifetimeProps);

  // Replicate to everyone
  DOREPLIFETIME_CONDITION (ABlitzCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void
ABlitzCharacter::PostInitializeComponents ()
{
  Super::PostInitializeComponents ();

  if (Combat)
    {
      Combat->Character = this;
    }
}

void
ABlitzCharacter::BeginPlay ()
{
  Super::BeginPlay ();

  // Setup input
  if (APlayerController *PlayerController = Cast<APlayerController> (
      Controller))
    {
      if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> (
              PlayerController->GetLocalPlayer ()))
        {
          Subsystem->AddMappingContext (MappingContext, 0);
        }
    }
}

void
ABlitzCharacter::Move (const FInputActionValue &Value)
{
  const FVector2D Direction = Value.Get<FVector2D> ();

  if (Controller != nullptr)
    {
      const FRotator YawRotation (0.f, Controller->GetControlRotation ().Yaw,
                                  0.f);
      const FVector ForwardDirection (
          FRotationMatrix (YawRotation).GetUnitAxis (EAxis::X));
      AddMovementInput (ForwardDirection, Direction.Y);

      const FVector RightDirection (
          FRotationMatrix (YawRotation).GetUnitAxis (EAxis::Y));
      AddMovementInput (RightDirection, Direction.X);
    }
}

void
ABlitzCharacter::Look (const FInputActionValue &Value)
{
  const FVector2D Direction = Value.Get<FVector2D> ();

  AddControllerYawInput (Direction.X);
  AddControllerPitchInput (Direction.Y);
}

void
ABlitzCharacter::Equip (const FInputActionValue &Value)
{
  if (Combat)
    {
      if (HasAuthority ())
        {
          // we're the server
          Combat->EquipWeapon (OverlappingWeapon);
        }
      else
        {
          // we're the client
          ServerEquipButtonPressed ();
        }
    }
}

void
ABlitzCharacter::CrouchUncrouch (const FInputActionValue &Value)
{
  if (Value.Get<bool> ())
    {
      Crouch ();
    }
  else
    {
      UnCrouch ();
    }
}

void
ABlitzCharacter::Aim (const FInputActionValue &Value)
{
  if (Combat)
    {
      Combat->SetAiming (Value.Get<bool> ());
    }
}

void
ABlitzCharacter::AimOffset (float DeltaTime)
{
  if (Combat && Combat->EquippedWeapon == nullptr)
    {
      return;
    }

  FVector Velocity = GetVelocity ();
  Velocity.Z = 0.f;
  float Speed = Velocity.Size ();

  bool bIsInAir = GetCharacterMovement ()->IsFalling ();

  if (Speed == 0.f && !bIsInAir)
    {
      // standing still, not jumping
      FRotator CurrentAimRotation = FRotator (0.f, GetBaseAimRotation ().Yaw,
                                              0.f);
      FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator (
          CurrentAimRotation, StartingAimRotation);

      AO_Yaw = DeltaAimRotation.Yaw;

      bUseControllerRotationYaw = false;
    }

  if (Speed > 0.f || bIsInAir)
    {
      // running or jumping
      if (!bUseControllerRotationYaw)
        {
          bUseControllerRotationYaw = true;
        }

      StartingAimRotation = FRotator (0.f, GetBaseAimRotation ().Yaw, 0.f);
      AO_Yaw = 0.f;
    }

  AO_Pitch = GetBaseAimRotation ().Pitch;
}

void
ABlitzCharacter::OnRep_OverlappingWeapon (AWeapon *LastWeapon)
{
  if (OverlappingWeapon)
    {
      OverlappingWeapon->ShowPickupWidget (true);
    }

  if (LastWeapon)
    {
      LastWeapon->ShowPickupWidget (false);
    }
}

void
ABlitzCharacter::ServerEquipButtonPressed_Implementation ()
{
  if (Combat)
    {
      Combat->EquipWeapon (OverlappingWeapon);
    }
}

void
ABlitzCharacter::SetOverlappingWeapon (AWeapon *Weapon)
{
  if (OverlappingWeapon)
    {
      OverlappingWeapon->ShowPickupWidget (false);
    }

  OverlappingWeapon = Weapon;
  if (IsLocallyControlled ())
    {
      if (OverlappingWeapon)
        {
          OverlappingWeapon->ShowPickupWidget (true);
        }
    }
}

bool
ABlitzCharacter::IsWeaponEquipped () const
{
  return (Combat && Combat->EquippedWeapon);
}

bool
ABlitzCharacter::IsAiming () const
{
  return (Combat && Combat->bAiming);
}