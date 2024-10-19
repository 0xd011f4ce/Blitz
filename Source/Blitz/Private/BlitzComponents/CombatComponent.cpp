// Fill out your copyright notice in the Description page of Project Settings.


#include "BlitzComponents/CombatComponent.h"

#include "Character/BlitzCharacter.h"

#include "Components/SphereComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Weapon/Weapon.h"

UCombatComponent::UCombatComponent ()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void
UCombatComponent::BeginPlay ()
{
  Super::BeginPlay ();
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
    }
}