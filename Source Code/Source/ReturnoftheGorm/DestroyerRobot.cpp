// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyerRobot.h"

#include "ActorMovement.h"
#include "ContraCharacter.h"
#include "ReturnoftheGormGameMode.h"
#include "DrawDebugHelpers.h"
#include "FireObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"


#define OUT

// Sets default values
ADestroyerRobot::ADestroyerRobot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestroyerRobot = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Robot"));
	RootComponent = DestroyerRobot;

	LaserCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser"));
	LaserCube->SetVisibility(false);
	LaserCube->SetupAttachment(DestroyerRobot);

	FlameEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flame Effect"));
	FlameEffect->SetVisibility(false);
	FlameEffect->SetupAttachment(DestroyerRobot);

	SpeedSpawnPoint1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Speed SpawnPoint1"));
	SpeedSpawnPoint1->SetupAttachment(DestroyerRobot);

	SpeedSpawnPoint2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Speed SpawnPoint2"));
	SpeedSpawnPoint2->SetupAttachment(DestroyerRobot);
}

// Called when the game starts or when spawned
void ADestroyerRobot::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	SpeedSpawnPoint1->SetWorldLocation(DestroyerRobot->GetSocketLocation(TEXT("Laser_LSocket")));
	SpeedSpawnPoint2->SetWorldLocation(DestroyerRobot->GetSocketLocation(TEXT("Laser_RSocket")));
	FlameEffect->SetWorldLocation(DestroyerRobot->GetSocketLocation(TEXT("Laser_RSocket")));
	GetWorldTimerManager().SetTimer(LocationTimer, this, &ADestroyerRobot::GetLocation, 1, true, 1);
}

// Called every frame
void ADestroyerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Trigger && ContraCharacter && Trigger->IsOverlappingActor(ContraCharacter))
	{
		// if player in sight, it's getting location information.
		if (TriggerLogic)
		{
			// if you overlapping on trigger, it will set the timer
			GetWorld()->GetTimerManager().SetTimer(
				FireRateTimerHandle,
				this,
				&ADestroyerRobot::CheckFireCondition,
				FireRate,
				true
			);
			TriggerLogic = false;
		}

		if (ReturnDistanceToPlayer() > FireRange) { return; }
		if (CanRotate)
		{
			RotateRobot();
		}
		else
		{
			LaserTrace();
			RotateRobot();
		}
	}
	else
	{
		// it's deleting timer if you're go trigger box outside
		TriggerLogic = true;
		GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
	}
}

// laser setup
void ADestroyerRobot::LaserTrace() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	// sending line trace between end-start
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		DestroyerRobot->GetSocketLocation(TEXT("Laser_RSocket")),
		ActorLocation,
		ECC_Pawn,
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	// when hit the pawn and if pawn tag is player, returning true.
	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		EffectHit();
	}

	// Getting target info in child and calculating rotation value.
	FRotator RotationCalculate = UKismetMathLibrary::FindLookAtRotation(
		DestroyerRobot->GetSocketLocation(TEXT("Laser_RSocket")),
		ContraCharacter->GetActorLocation());
	// Getting calculated value and applying 2 between value.
	FRotator TurretRotation = FMath::RInterpTo(FlameEffect->GetComponentRotation(), RotationCalculate,
											GetWorld()->GetDeltaSeconds(), RotateSpeed);


	FlameEffect->SetWorldRotation(TurretRotation);
}

void ADestroyerRobot::EffectHit() const
{
	if (ContraCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"),ContraCharacter->Health);
		ContraCharacter->Health -= 0.1f;
	}
}


// decrease health
float ADestroyerRobot::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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

// slow motion
void ADestroyerRobot::BackToNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &ADestroyerRobot::BackToNormal, 2, true, 0.1f);
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

// check is player alive
bool ADestroyerRobot::IsDead() const
{
	return Health <= 0;
}

// for rotate to robot head
void ADestroyerRobot::RotateRobot() const
{
	const FVector LookAtTargetCleaned = FVector(
		ContraCharacter->GetActorLocation().X,
		ContraCharacter->GetActorLocation().Y,
		DestroyerRobot->GetBoneLocationByName("Head", EBoneSpaces::ComponentSpace).Z
	);

	const FVector StartLocation = DestroyerRobot->GetComponentLocation();

	// Baktığı yöne bakması için yönünü çeviriyoruz
	const FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
	DestroyerRobot->SetBoneRotationByName("Head", FRotator(0, TurretRotation.Yaw - 90, 90),
	                                      EBoneSpaces::ComponentSpace);

	SpeedSpawnPoint1->SetWorldRotation(FRotator(0, TurretRotation.Yaw, 90));
	SpeedSpawnPoint2->SetWorldRotation(FRotator(0, TurretRotation.Yaw, 90));
}

// projectile spawn settings
void ADestroyerRobot::SpeedWeapon()
{
	const FVector SpawnLocation = SpeedSpawnPoint1->GetComponentLocation();
	const FRotator SpawnRotation = SpeedSpawnPoint1->GetComponentRotation();

	const FVector SpawnLocation2 = SpeedSpawnPoint2->GetComponentLocation();
	const FRotator SpawnRotation2 = SpeedSpawnPoint2->GetComponentRotation();

	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	FireRateTank = 1.0f;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRateTank;

	if (ProjectTileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);


		AProjectileBase* TempProjectile2 = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectTileClass, SpawnLocation2, SpawnRotation2);
		TempProjectile->SetOwner(this);
	}
}

// calculating player between robot
float ADestroyerRobot::ReturnDistanceToPlayer() const
{
	if (!ContraCharacter)
	{
		return 0.0f;
	}
	return FVector::Dist(ContraCharacter->GetActorLocation(), GetActorLocation());
}

// find player location and set value
void ADestroyerRobot::GetLocation()
{
	if (ContraCharacter)
	{
		ActorLocation = ContraCharacter->GetActorLocation();
	}
}

// shooting settings
void ADestroyerRobot::CheckFireCondition()
{
	// if player == null || is dead
	if (!ContraCharacter || ContraCharacter->IsDead()) { return; }

	// if player is in range then fire
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		if (ReturnDistanceToPlayer() <= SecondWeaponRange)
		{
			CanRotate = false;
			FlameEffect->SetVisibility(true);
		}
		else
		{
			CanRotate = true;
			FireRate = 2.0f;
			FlameEffect->SetVisibility(false);
			SpeedWeapon();
		}
	}
}
