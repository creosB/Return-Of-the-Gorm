// Fill out your copyright notice in the Description page of Project Settings.


#include "FireObstacleSpawner.h"
#include "ReturnoftheGorm/ContraCharacter.h"
#include "FireObstacle.h"
#include "ActorMovement.h"
#include "ReturnoftheGormGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFireObstacleSpawner::AFireObstacleSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>("Spawner Mesh");
	RootComponent = SpawnerMesh;
}

// Called when the game starts or when spawned
void AFireObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();
	// Calling main player.
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Health = MaxHealth;
}

void AFireObstacleSpawner::Tick(float DeltaSeconds)
{
	// if player goes out of the trigger, timer will stop (spawnner).
	if (Trigger && Trigger->IsOverlappingActor(ContraCharacter) && IsWork)
	{
		SetSpawnTimer();
		IsWork = false;
	}
	else if (Trigger && !Trigger->IsOverlappingActor(ContraCharacter))
	{
		GetWorldTimerManager().PauseTimer(SpawnTimer);
		IsWork = true;
	}
}

// Call Function with Timer
void AFireObstacleSpawner::SetSpawnTimer()
{
	GetWorldTimerManager().UnPauseTimer(SpawnTimer);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AFireObstacleSpawner::ObstacleSpawn, SpawnRepeat, true,
	                                SpawnSpeed);
}

// Spawn Actor
void AFireObstacleSpawner::ObstacleSpawn()
{
	// Repeating array 0 to max and restarting.
	if (i < SpawnSetting.Num())
	{
		SpawnChoice();
		i++;
	}
	else
	{
		i = 0;
		SpawnChoice();
	}
	// It's giving information to child (SpawnedActor)
	AFireObstacle* SpawnedActor = GetWorld()->SpawnActor<AFireObstacle>(ActorToSpawn, SpawnerLocation, SpawnerRotation);
	if (SpawnedActor && SpawnedActor->ActorMovement && Trigger)
	{
		SpawnedActor->DeadDelay = SpawnedDeadTime;
		SpawnedActor->ActorMovement->Direction = DirectionX;
		SpawnedActor->ActorMovement->PressPlate = Trigger;
		SpawnedActor->ActorMovement->TransporterForwardSpeed = SpawnedMovementSpeed;
		SpawnedActor->FireObstacleSpawner = this;
	}
}

// Set Properties
void AFireObstacleSpawner::SpawnChoice()
{
	// It's giving random value to each array elements and repeats every time we call this function.
	switch (SpawnSetting[i].SpawnDirection)
	{
	case SpawnDirectionZ:
		{
			SpawnerLocation = GetActorLocation();
			SpawnerLocation.Z = GetActorLocation().Z - FMath::RandRange(
				SpawnSetting[i].MinSpawnLocation,
				SpawnSetting[i].MaxSpawnLocation);
			SpawnerRotation = FRotator(0, 0, SpawnSetting[i].SpawnRotation);
			break;
		}
	case SpawnDirectionY:
		{
			SpawnerLocation = GetActorLocation();
			SpawnerLocation.Y = GetActorLocation().Y - FMath::RandRange(
				SpawnSetting[i].MinSpawnLocation,
				SpawnSetting[i].MaxSpawnLocation);
			SpawnerRotation = FRotator(0, 0, SpawnSetting[i].SpawnRotation);
			break;
		}
	case SpawnDirectionX:
		{
			SpawnerLocation = GetActorLocation();
			SpawnerLocation.X = GetActorLocation().X - FMath::RandRange(
				SpawnSetting[i].MinSpawnLocation,
				SpawnSetting[i].MaxSpawnLocation);
			SpawnerRotation = FRotator(0, 0, SpawnSetting[i].SpawnRotation);
			break;
		}
	}
}

float AFireObstacleSpawner::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                       AController* EventInstigator, AActor* DamageCauser)
{
	// pawn taking damage and applying health.
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;

	// when health was less than 10, it will set start slow motion value and call function.
	if (Health < 10)
	{
		SlowLogic = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimerValue);
		BacktoNormal();
	}
	// when player was dead, it will spawn particle and sound.
	if (IsDead())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
		// pawn is giving information to gamemode.
		AReturnoftheGormGameMode* GameMode = GetWorld()->GetAuthGameMode<AReturnoftheGormGameMode>();
		if (GameMode != nullptr && ReturnGameModeDead)
		{
			GameMode->PawnKilled(this);
			// returning game mode one! It's protecting sum more score.
			ReturnGameModeDead = false;
		}
		if (Controller != nullptr && Controller->GetPawn() == this)
		{
			Controller->PawnPendingDestroy(this);
			if (Controller != nullptr)
			{
				Controller->UnPossess();
				Controller = nullptr;
			}
		}

		//Destroy();
		//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}
// Check Health less than 0
bool AFireObstacleSpawner::IsDead() const
{
	return Health <= 0;
}
// Slow Motion with Destroy
void AFireObstacleSpawner::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &AFireObstacleSpawner::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
			GetWorldTimerManager().ClearTimer(SpawnTimer);
			// Detect attached actors or static meshes and it will destroy with herself
			if (DestroyWAttachment)
			{
				GetAttachedActors(Attached);
				for(int a = 0; a < Attached.Num(); a++)
				{
					Attached[a]->Destroy();
				}
			}
			Destroy();
		}
		else
		{
			// if player doesn't dead and slow motion enough, slow time returning normal but slowly.
			TimerValue += 0.1;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
		}
	}
}
