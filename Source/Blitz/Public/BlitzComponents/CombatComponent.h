// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"

class ABlitzCharacter;
class AWeapon;

UCLASS (ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLITZ_API UCombatComponent : public UActorComponent
{
  GENERATED_BODY ()

public:
  UCombatComponent ();
  virtual void TickComponent (float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction *
                              ThisTickFunction) override;

  friend class ABlitzCharacter;

  void EquipWeapon (AWeapon *WeaponToEquip);
  virtual void GetLifetimeReplicatedProps (
      TArray<class FLifetimeProperty> &OutLifetimeProps) const override;

protected:
  virtual void BeginPlay () override;

  void SetAiming (bool bIsAiming);

  UFUNCTION (Server, Reliable)
  void ServerSetAiming (bool bIsAiming);

private:
  ABlitzCharacter *Character;

  UPROPERTY (Replicated)
  AWeapon *EquippedWeapon;

  UPROPERTY (Replicated)
  bool bAiming;
};