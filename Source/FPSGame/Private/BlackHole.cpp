// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlackHoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackHoleMesh"));
	RootComponent = BlackHoleMesh;
	//BlackHoleMesh->SetWorldScale3D(FVector(0.7f));

	DestroyingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyingSphere"));
	DestroyingSphere->SetupAttachment(BlackHoleMesh);
	DestroyingSphere->SetSphereRadius(160.f);


	AttractingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractingSphere"));
	AttractingSphere->SetupAttachment(AttractingSphere);
	AttractingSphere->SetSphereRadius(5000.f);


}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> AttractingOverlaps;
	AttractingSphere->GetOverlappingComponents(AttractingOverlaps);

	for (UPrimitiveComponent* OverlappedComp : AttractingOverlaps)
	{
			AActor* OwningActor = OverlappedComp->GetOwner();
			if (OwningActor)
			{
				if (OwningActor->GetName().Contains("Cube"))
				{
					//UE_LOG(LogTemp, Warning, TEXT("GRRRR"));
					FVector ForceVector = ((UKismetMathLibrary::Conv_RotatorToVector(UKismetMathLibrary::FindLookAtRotation(OwningActor->GetActorLocation(), GetActorLocation()))) * ((GetActorLocation() - OwningActor->GetActorLocation()).Size()))* AttractionForce;
					OverlappedComp->AddForce(ForceVector, NAME_None, true);
				}
			}
	}

	TArray<UPrimitiveComponent*> DestroyingOverlaps;
	DestroyingSphere->GetOverlappingComponents(DestroyingOverlaps);
	for (UPrimitiveComponent* OverlappedComp : DestroyingOverlaps)
	{

			AActor* OwningActor = OverlappedComp->GetOwner();
			if (OwningActor)
			{
				if (OwningActor->GetName().Contains("Cube"))
				{
					GetWorld()->DestroyActor(OwningActor);
				}
			}
	}


}

