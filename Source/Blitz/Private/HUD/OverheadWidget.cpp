// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void
UOverheadWidget::SetDisplayText (FString TextToDisplay)
{
  if (DisplayText)
    {
      DisplayText->SetText (FText::FromString (TextToDisplay));
    }
}

void
UOverheadWidget::ShowPlayerNetRole (APawn *InPawn)
{
  ENetRole LocalRole = InPawn->GetLocalRole ();
  FString Role;

  switch (LocalRole)
    {
    case ROLE_Authority:
      Role = FString ("Authority");
      break;

    case ROLE_AutonomousProxy:
      Role = FString ("Autonomous Proxy");
      break;

    case ROLE_SimulatedProxy:
      Role = FString ("Simulated Proxy");
      break;

    case ROLE_None:
      Role = FString ("None");
      break;

    default:
      Role = FString ("Unknown");
      break;
    }

  FString LocalRoleString = FString::Printf (TEXT ("Local Role: %s"), *Role);
  SetDisplayText (LocalRoleString);
}

void
UOverheadWidget::ShowPlayerName (APawn *InPawn)
{
  if (PlayerName)
    {
      if (InPawn)
        {
          if (InPawn->GetController ())
            {
              if (APlayerState *PlayerState = InPawn->GetController ()->
                  PlayerState)
                {
                  FString PlayerNameString = PlayerState->GetPlayerName ();
                  PlayerName->SetText (FText::FromString (PlayerNameString));
                }
              else
                {
                  UE_LOG (LogTemp, Warning, TEXT ("PlayerState is nullptr"));
                  PlayerName->SetText (
                      FText::FromString (TEXT ("No Player State")));
                }
            }
          else
            {
              UE_LOG (LogTemp, Warning, TEXT ("Controller is nullptr"));
              PlayerName->SetText (
                  FText::FromString (TEXT ("No Player State")));
            }
        }
      else
        {
          UE_LOG (LogTemp, Warning, TEXT ("InPawn is nullptr"));
          PlayerName->SetText (FText::FromString (TEXT ("No Player State")));
        }
    }
}