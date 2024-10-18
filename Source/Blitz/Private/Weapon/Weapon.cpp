// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"

#include "Character/BlitzCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

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

  PickupWidget = CreateDefaultSubobject<UWidgetComponent> (
      TEXT ("PickupWidget"));
  PickupWidget->SetupAttachment (GetRootComponent ());
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
      AreaSphere->OnComponentBeginOverlap.AddDynamic (
          this, &ThisClass::OnSphereOverlap);
    }

  if (PickupWidget)
    {
      PickupWidget->SetVisibility (false);
    }
}

void
AWeapon::OnSphereOverlap (UPrimitiveComponent *OverlappedComponent,
                          AActor *OtherActor, UPrimitiveComponent *OtherComp,
                          int32 OtherBodyIndex,
                          bool bFromSweep, const FHitResult &SweepResult)
{
  ABlitzCharacter *BlitzCharacter = Cast<ABlitzCharacter> (OtherActor);
  if (BlitzCharacter && PickupWidget)
    {
      PickupWidget->SetVisibility (true);
    }
}