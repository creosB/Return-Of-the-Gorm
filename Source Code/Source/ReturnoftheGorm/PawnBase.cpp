// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ProjectileBase.h"
#include "ReturnoftheGorm/ReturnoftheGormGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"

#define OUT

// Sets default values
APawnBase::APawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BP oluştuğu zaman birbirinin altına oluşacaklar
	// yani RootComponent (ana kısım) >capsule > baseMesh > TurretMesh > SpawnPoint
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	SlowSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SlowSpawnPoint"));
	SlowSpawnPoint->SetupAttachment(TurretMesh);

	SpeedSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpeedSpawnPoint"));
	SpeedSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Rotation Settings
void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Getting target info in child and calculating rotation value.
	FRotator RotationCalculate = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(),
	                                                                    LookAtTarget);
	// Getting calculated value and applying 2 between value.
	FRotator TurretRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), RotationCalculate,
	                                           GetWorld()->GetDeltaSeconds(), RotateSpeed);
	// Setting turret rotation value
	TurretMesh->SetWorldRotation(TurretRotation);
}

// Health - Dead system
float APawnBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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

// when pawn is destroyed
void APawnBase::HandleDestruction()
{
	// Actor'ün olduğu yere efekt yaratıyor
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
}
// check health
bool APawnBase::IsDead() const
{
	return Health <= 0;
}

// Slow Time
void APawnBase::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &APawnBase::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
			// Detect attached actors or static meshes and it will destroy with herself
			if(DestroyWAttachment)
			{
				GetAttachedActors(Attached);
				for(int i = 0; i < Attached.Num(); i++)
				{
					Attached[i]->Destroy();
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

// Weapons system
void APawnBase::SlowWeapon()
{
	FVector SpawnLocation = SlowSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = SlowSpawnPoint->GetComponentRotation();

	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	FireRateTank = 3.0f;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRateTank;
	if (ProjectTileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::NormalWeapon()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	FireRateTank = 2.0f;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRateTank;

	if (ProjectTileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::SpeedWeapon()
{
	FVector SpawnLocation = SpeedSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = SpeedSpawnPoint->GetComponentRotation();

	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	FireRateTank = 1.0f;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRateTank;

	if (ProjectTileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}
