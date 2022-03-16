// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "ReturnoftheGorm/ActorMovement.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Object"));
	RootComponent = SpawnObjectMesh;

	ActorMovement = CreateDefaultSubobject<UActorMovement>(TEXT("Spawner Movement"));
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	FindPointer();
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnerLogic();
}

void ASpawner::SpawnerLogic()
{
	// if player press the trigger volume, spawner will spawn one actor.
	if (PressPlate && PressPlate->IsOverlappingActor(ActorThatOpen))
	{
		if(Logic)
		{
			// Spawning healthbox with delay.
			GetWorldTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ASpawner::Spawner,
			SpawnSpeed,
			false,
			SpawnRepeat);
			Logic = false;
		}
	}
}

void ASpawner::Spawner()
{
	SpawnerCalculate();
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnerLocation, SpawnerRotation);
	FindPointer();
	UGameplayStatics::SpawnEmitterAtLocation(this, SpawnBoxParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, SpawnBoxSound, GetActorLocation());
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

// MinZ-MaxZ between it will spawn one actor.
void ASpawner::SpawnerCalculate()
{
	SpawnerLocation = GetActorLocation();
	SpawnerRotation = GetActorRotation();
	SpawnerLocation.Z = GetActorLocation().Z - FMath::RandRange(SpawnMinZ, SpawnMaxZ);
}

void ASpawner::FindPointer()
{
	if (!SpawnBoxParticle) { return; }
	if (!SpawnBoxSound) { return; }
	if (!PressPlate) { return; }
}
