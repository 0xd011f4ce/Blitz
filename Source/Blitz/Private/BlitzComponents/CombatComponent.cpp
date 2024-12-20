// Fill out your copyright notice in the Description page of Project Settings.

#include "BlitzComponents/CombatComponent.h"

#include "Character/BlitzCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "Weapon/Weapon.h"

UCombatComponent::UCombatComponent ()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void
UCombatComponent::BeginPlay ()
{
  Super::BeginPlay ();

  if (Character)
    {
      Character->GetCharacterMovement ()->MaxWalkSpeed = BaseWalkSpeed;
    }
}

void
UCombatComponent::SetAiming (bool bIsAiming)
{
  bAiming = bIsAiming;
  ServerSetAiming (bIsAiming);

  if (Character)
    {
      Character->GetCharacterMovement ()->MaxWalkSpeed = bAiming
          ? AimWalkSpeed
          : BaseWalkSpeed;
    }
}

void
UCombatComponent::OnRep_EquippedWeapon ()
{
  if (EquippedWeapon && Character)
    {
      Character->GetCharacterMovement ()->bOrientRotationToMovement = false;
      Character->bUseControllerRotationYaw = true;
    }
}

void
UCombatComponent::ServerSetAiming_Implementation (bool bIsAiming)
{
  bAiming = bIsAiming;

  if (Character)
    {
      Character->GetCharacterMovement ()->MaxWalkSpeed = bAiming
          ? AimWalkSpeed
          : BaseWalkSpeed;
    }
}

void
UCombatComponent::TickComponent (float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction *ThisTickFunction)
{
  Super::TickComponent (DeltaTime, TickType, ThisTickFunction);
}

void
UCombatComponent::EquipWeapon (AWeapon *WeaponToEquip)
{
  if (Character && WeaponToEquip)
    {
      EquippedWeapon = WeaponToEquip;
      EquippedWeapon->SetWeaponState (EWeaponState::EWS_Equipped);

      const USkeletalMeshSocket *HandSocket = Character->GetMesh ()->
          GetSocketByName (FName ("RightHandSocket"));
      if (HandSocket)
        {
          HandSocket->AttachActor (EquippedWeapon, Character->GetMesh ());
        }
      EquippedWeapon->SetOwner (Character);

      Character->GetCharacterMovement ()->bOrientRotationToMovement = false;
      Character->bUseControllerRotationYaw = true;
    }
}

void
UCombatComponent::GetLifetimeReplicatedProps (
    TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps (OutLifetimeProps);

  DOREPLIFETIME (UCombatComponent, EquippedWeapon);
  DOREPLIFETIME (UCombatComponent, bAiming);
}