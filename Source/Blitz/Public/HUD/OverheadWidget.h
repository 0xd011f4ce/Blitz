// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "OverheadWidget.generated.h"

class UTextBlock;

UCLASS ()
class BLITZ_API UOverheadWidget : public UUserWidget
{
  GENERATED_BODY ()

public:
  UPROPERTY (meta=(BindWidget))
  UTextBlock *DisplayText;

  UPROPERTY (meta=(BindWidget))
  UTextBlock *PlayerName;

  void SetDisplayText (FString TextToDisplay);

  UFUNCTION (BlueprintCallable)
  void ShowPlayerNetRole (APawn *InPawn);

  UFUNCTION (BlueprintCallable)
  void ShowPlayerName (AController *InController);
};