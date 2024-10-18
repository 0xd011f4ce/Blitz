// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"

#include "Components/SphereComponent.h"

AWeapon::AWeapon ()
{
  PrimaryActorTick.bCanEverTick = false;

  // replicate
  bReplicates = true;

  WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent> (
      TEXT ("Weapon Mesh"));
  WeaponMesh->SetCollisionResponseToAllChannels (ECR_Block);
  WeaponMesh->SetCollisionResponseToChannel (ECC_Pawn, ECR_Ignore);
  WeaponMesh->SetCollisionEnabled (ECollisionEnabled::NoCollision);
  SetRootComponent (WeaponMesh);

  AreaSphere = CreateDefaultSubobject<USphereComponent> (TEXT ("AreaSphere"));
  AreaSphere->SetupAttachment (GetRootComponent ());
  AreaSphere->SetCollisionResponseToAllChannels (ECR_Ignore);
  AreaSphere->SetCollisionEnabled (ECollisionEnabled::NoCollision);
}

void
AWeapon::Tick (float DeltaTime)
{
  Super::Tick (DeltaTime);
}

void
AWeapon::BeginPlay ()
{
  Super::BeginPlay ();

  // if we're the server, enable collision
  if (HasAuthority ())
    {
      AreaSphere->SetCollisionEnabled (ECollisionEnabled::QueryAndPhysics);
      AreaSphere->SetCollisionResponseToChannel (ECC_Pawn, ECR_Overlap);
    }
}