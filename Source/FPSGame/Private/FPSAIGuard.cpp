// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	// PawnSensingComponent doesnt need to be attached to anything since its not a "visible" component

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	// here we dont call function to set the guard state bc we dont want to respond to it in blueprints yet
	GuardState = EAIState::Idle;

	CurrentPatrolIndex = 0;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	if (bPatrolling)
	{
		MoveToNewPatrolPoint();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PAWN NULL"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("PAWN SEEN"));
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);

	// if see pawn then stop movement if patrolling
	AController* CurrentController = GetController();
	if (CurrentController)
	{
		CurrentController->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Black, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f);

	SetGuardState(EAIState::Suspicious);

	// if hear noise then stop movement if patrolling
	AController* CurrentController = GetController();
	if (CurrentController)
	{
		CurrentController->StopMovement();
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	// here we've stopped being sus, so we can resume patrolling if we're patrolling AI
	if (bPatrolling)
	{
		MoveToNewPatrolPoint();
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	// on the client, update the state
	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNewPatrolPoint()
{
	// get next patrol point from the list
	if (PatrolPoints.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to patrol with no patrol targets set."));
		return;
	}

	AActor* NewPatrolPoint = PatrolPoints[CurrentPatrolIndex];

	if (CurrentPatrolPoint != NewPatrolPoint)
	{
		CurrentPatrolPoint = NewPatrolPoint;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

		UE_LOG(LogTemp, Warning, TEXT("New patrol point set with index %d"), CurrentPatrolIndex);

		CurrentPatrolIndex++;
		if (CurrentPatrolIndex > PatrolPoints.Num() - 1)
		{
			CurrentPatrolIndex = 0;
		}
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	// here we make sure we replicate the GuardState for the clients
	OnRep_GuardState();

	OnStateChanged(NewState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToPoint = Delta.Size();
		//UE_LOG(LogTemp, Warning, TEXT("Checking distance. %f"), DistanceToPoint);


		if (DistanceToPoint < 100.f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Moving to new patrol point bc of distance."));
			MoveToNewPatrolPoint();
		}
	}

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// this is like a default rule to replicate this variable to all the clients
	// this var will replicate now to all the clients connected with us
	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
