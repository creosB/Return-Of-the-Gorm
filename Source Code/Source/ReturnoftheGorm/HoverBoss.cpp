// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverBoss.h"

#include "ReturnoftheGormGameMode.h"
#include "HoverMini.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHoverBoss::AHoverBoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hover Mesh"));
	RootComponent = Mesh;
	HoverSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hover Spawn Point"));
	HoverSpawnPoint->SetupAttachment(Mesh);
	BombSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb Spawn Point"));
	BombSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AHoverBoss::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) { return; }
	MovementInitial = GetActorLocation();
	MovementCurrent = MovementInitial;
	MovementTarget = Player->GetActorLocation();

	Forward = GetActorLocation();

	// set health
	Health = MaxHealth;
}

// Called every frame
void AHoverBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Trigger && Player && Trigger->IsOverlappingActor(Player))
	{
		HoverMovement(DeltaTime);
		if(IsTimerSet)
		{
			SetAllTimer();
		}
		// Spawning bomb to hoverboss down
	}else
	{
		IsTimerSet = true;
		GetWorldTimerManager().ClearTimer(UpdateTimer);
		GetWorldTimerManager().ClearTimer(BombTimer);
		GetWorldTimerManager().ClearTimer(HoverTimer);
	}
}


// Movement System
void AHoverBoss::UpdateTargetLocation()
{
	if (!Player) { return; }
	MovementTarget = Player->GetActorLocation();
}

void AHoverBoss::HoverMovement(float DeltaTime)
{
	MovementCurrent = FMath::VInterpConstantTo(MovementCurrent, MovementTarget, DeltaTime,
	                                           MovementSpeed);
	Forward = MovementCurrent;
	SetActorLocation(FVector(Forward.X, Forward.Y, HoverHeight));
}


// Spawn System
void AHoverBoss::SetAllTimer()
{
	// Update player location timer
	GetWorldTimerManager().SetTimer(
		UpdateTimer,
		this,
		&AHoverBoss::UpdateTargetLocation,
		1.0f,
		true,
		5.0f);
	// Bomb spawn timer
	GetWorldTimerManager().SetTimer(
		BombTimer,
		this,
		&AHoverBoss::BombSpawn,
		BombRepeatTime,
		true,
		5.0f);
	// Hover Spawn timer
	GetWorldTimerManager().SetTimer(
		HoverTimer,
		this,
		&AHoverBoss::HoverSpawn,
		HoverRepeatTimer,
		true,
		3.0f);
	IsTimerSet = false;
}


void AHoverBoss::BombSpawn()
{
	// Spawning bomb to hoverboss down
	GetWorld()->SpawnActor<AActor>(Bomb, GetActorLocation() - FVector(0, 0, 50), FRotator(0, 0, 180));
}

void AHoverBoss::HoverSpawn()
{
	// Spawning Hover to hoverboss right side
	AHoverMini* SpawnedActor = GetWorld()->SpawnActor<AHoverMini>(Hover, GetActorLocation() - FVector(0, 500, 0),
	                                                              FRotator(0, 0, 180));
}

// Health - Death System
float AHoverBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
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

// Check health logic
bool AHoverBoss::IsDead() const
{
	return Health <= 0;
}

// Slow Motion
void AHoverBoss::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &AHoverBoss::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
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
