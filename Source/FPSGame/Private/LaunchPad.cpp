// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetWorldScale3D(FVector(1.f, 1.f, 0.165f));

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(RootComponent);
	ArrowMesh->SetRelativeLocation(FVector(0.f,0.f,130.f));
	ArrowMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 0.f));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetRelativeLocation(FVector(0.f, 0.f, 240.f));
	CollisionBox->SetRelativeScale3D(FVector(2.5f, 2.5f, 1.5f));
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::BoxOverlapped);

}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
}

void ALaunchPad::BoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, OtherActor->GetActorLocation());

		FVector LaunchVelocity = FVector(GetActorForwardVector().X*LaunchForceXY, GetActorForwardVector().Y*LaunchForceXY, LaunchForceZ);

		AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
		}
		else if(OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		}
	}


}
