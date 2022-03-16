// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotBoss.h"

#include "BallBoss.h"
#include "Components/CapsuleComponent.h"
#include "ProjectileBase.h"
#include "ReturnoftheGorm/ReturnoftheGormGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/Rotator.h"
#include "ReturnoftheGorm/ContraCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARobotBoss::ARobotBoss()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	RobotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	RobotMesh->SetupAttachment(CapsuleComponent);

	ElectricGunSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ElectricGunSpawnPoint->SetupAttachment(RobotMesh);

	Metal = CreateDefaultSubobject<UMaterial>(TEXT("Metal"));
	MetalInstance = CreateDefaultSubobject<UMaterialInstance>(TEXT("Metal Dissolve"));
	Body = CreateDefaultSubobject<UMaterial>(TEXT("Body"));
	BodyInstance = CreateDefaultSubobject<UMaterialInstance>(TEXT("Body Dissolve"));
}

// Called when the game starts or when spawned
void ARobotBoss::BeginPlay()
{
	Super::BeginPlay();
	// set health
	Health = MaxHealth;
	// get player information
	PlayerPawn = Cast<AContraCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void ARobotBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 ChangeMaterial();
	// if player in sight, it's getting location information.
	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) { return; }
	RotateTurret(PlayerPawn->GetActorLocation());

	if (Trigger->IsOverlappingActor(PlayerPawn) && Trigger)
	{
		// if you overlapping on trigger, it will set the timer
		if (TriggerLogic)
		{
			GetWorld()->GetTimerManager().SetTimer(
				FireRateTimerHandle,
				this,
				&ARobotBoss::CheckFireCondition,
				FireRate,
				true
			);
			TriggerLogic = false;
		}
	}
	else
	{
		// it's deleting timer if you're go trigger box outside
		TriggerLogic = true;
		GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
	}
}

// Rotation Settings
void ARobotBoss::RotateTurret(FVector LookAtTarget) const
{
	// Getting target info in child and calculating rotation value.
	FRotator RotationCalculate = UKismetMathLibrary::FindLookAtRotation(RobotMesh->GetComponentLocation(),
	                                                                    LookAtTarget);
	// Getting calculated value and applying 2 between value.
	FRotator TurretRotation = FMath::RInterpTo(RobotMesh->GetComponentRotation(), RotationCalculate,
	                                           GetWorld()->GetDeltaSeconds(), RotateSpeed);
	// Setting turret rotation value
	RobotMesh->SetWorldRotation(TurretRotation);
}

// Health - Dead system
float ARobotBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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
		BackToNormal();
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
void ARobotBoss::HandleDestruction()
{
	// Spawn effect in actor location
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
	Destroy();
}

// Check Health
bool ARobotBoss::IsDead() const
{
	return Health <= 0;
}

// Slow Motion
void ARobotBoss::BackToNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &ARobotBoss::BackToNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
			// Detect attached actors or static meshes and it will destroy with herself
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

// Gun
void ARobotBoss::ElectricGun()
{
	// Getting spawn point value
	FVector SpawnLocation = ElectricGunSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = RobotMesh->GetComponentRotation();
	// check fire condiction
	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	FireRateTank = 1.0f;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRateTank;

	// Spawn projectile
	if (ProjectTileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void ARobotBoss::CheckFireCondition()
{
	// if player == null || is dead
	if (!PlayerPawn || PlayerPawn->IsDead()) { return; }

	// if player is in range then fire
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		ElectricGun();
	}
}

float ARobotBoss::ReturnDistanceToPlayer() const
{
	if (!PlayerPawn)
	{
		return 0.0f;
	}
	// 2 vector arasındaki mesafeyi hesaplıyor ve fonksiyona döndürüyor.
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void ARobotBoss::ChangeMaterial()
{
	if(Trigger && Trigger->IsOverlappingActor(PlayerPawn))
	{
		if(CanChangeMaterial)
		{
			NormalMaterial();
			NormalMaterialLast = GetWorld()->GetTimeSeconds();
			// going to dissolve material
			if(GetWorld()->GetTimeSeconds() - DissolveMaterialLast > 8.0f)
			{
				CanChangeMaterial = false;
			}
		}else
		{
			DissolveMaterial();
			DissolveMaterialLast = GetWorld()->GetTimeSeconds();
			// going back to normal
			if(GetWorld()->GetTimeSeconds() - NormalMaterialLast > 8.0f)
			{
				CanChangeMaterial = true;
			}
		}
	}
}

void ARobotBoss::NormalMaterial()
{
	// it sets material and stopping shoot
	RobotMesh->SetMaterial(0, Metal);
	RobotMesh->SetMaterial(2, Body);
	RobotMesh->SetMaterial(5, Body);
	FireRange = 2000;
}

void ARobotBoss::DissolveMaterial()
{
	// it sets material and stopping shoot
	RobotMesh->SetMaterial(0, MetalInstance);
	RobotMesh->SetMaterial(2, BodyInstance);
	RobotMesh->SetMaterial(5, BodyInstance);
	FireRange = 0;
}
