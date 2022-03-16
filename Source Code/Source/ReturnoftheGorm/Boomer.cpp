// Fill out your copyright notice in the Description page of Project Settings.


#include "Boomer.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoomer::ABoomer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Object"));
	RootComponent = SpawnObjectMesh;
}

// Called when the game starts or when spawned
void ABoomer::BeginPlay()
{
	Super::BeginPlay();

	// Spawning grenade with delay.
	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&ABoomer::Spawner,
		SpawnSpeed,
		true,
		SpawnRepeat);
	//movement
	TransporterInitial = GetActorLocation().Y;
	TransporterCurrent = TransporterInitial;
	TransporterTarget = TransporterInitial + TransporterTarget;
	Forward = GetActorLocation();
}

// Called every frame
void ABoomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Movement(DeltaTime);

	if (TransporterCurrent == TransporterTarget)
	{
		Destroy();
	}
}

void ABoomer::Movement(float DeltaTime)
{
	TransporterCurrent = FMath::FInterpConstantTo(TransporterCurrent, TransporterTarget, DeltaTime,
	                                              TransporterForwardSpeed);
	Forward.Y = TransporterCurrent;
	SetActorLocation(Forward);
}


void ABoomer::Spawner()
{
	SpawnerCalculate();
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnerLocation, SpawnerRotation);
	if (!SpawnBoxParticle) { return; }
	if (!SpawnBoxSound) { return; }
	UGameplayStatics::SpawnEmitterAtLocation(this, SpawnBoxParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, SpawnBoxSound, GetActorLocation());
}

// MinZ-MaxZ between it will spawn one actor.
void ABoomer::SpawnerCalculate()
{
	SpawnerLocation = GetActorLocation();
	SpawnerRotation = GetActorRotation();
	SpawnerLocation.Z = GetActorLocation().Z - FMath::RandRange(SpawnMinZ, SpawnMaxZ);
}
