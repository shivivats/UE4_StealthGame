// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"


UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FTimerHandle TimerHandle_ResetOrientation;

	FRotator OriginalRotation;

	UFUNCTION()
	void ResetOrientation();

	EAIState GuardState;

	UPROPERTY(EditInstanceOnly, Category="Patrol")
	bool bPatrolling;

	UPROPERTY(EditInstanceOnly, Category="Patrol", meta = (EditCondition = bPatrolling))
	TArray<AActor*> PatrolPoints;

	AActor* CurrentPatrolPoint;

	int CurrentPatrolIndex;

	void MoveToNewPatrolPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);



};
