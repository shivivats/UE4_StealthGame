// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category="Force")
	float LaunchForceXY;

	UPROPERTY(EditDefaultsOnly, Category = "Force")
	float LaunchForceZ;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* LaunchEffect;

private:

	UFUNCTION()
	void BoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
