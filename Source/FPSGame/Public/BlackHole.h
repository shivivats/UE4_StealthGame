// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

	UPROPERTY(EditDefaultsOnly, Category="Components")
	class UStaticMeshComponent* BlackHoleMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* DestroyingSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* AttractingSphere;

	UPROPERTY(EditAnywhere, Category="Force")
	float AttractionForce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
