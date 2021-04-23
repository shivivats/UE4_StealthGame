// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
// 	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
// 	{
// 		APawn* Pawn = It->Get();
// 		// only disable input if pawn is being locally controlled
// 		if (Pawn && Pawn->IsLocallyControlled())
// 		{
// 			Pawn->DisableInput(nullptr);
// 		}
// 	}

	// now we wanna call the mission complete function on the local player controller
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			// disable input, we dont need to check for local controller again bc we already did that
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn)
			{
				MyPawn->DisableInput(PC);
			}
		}
	}
}
