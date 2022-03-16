// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBoss.h"

#include "ReturnoftheGormGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "NiagaraFunctionLibrary.h"
#include "ReturnoftheGorm/ContraCharacter.h"

// Sets default values
ABallBoss::ABallBoss()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	BigSphere = CreateDefaultSubobject<UStaticMeshComponent>("Big Sphere");
	BigSphere->SetupAttachment(Root);
	MiddleSphere = CreateDefaultSubobject<UStaticMeshComponent>("Middle Sphere");
	MiddleSphere->SetupAttachment(Root);
	SmallSphere = CreateDefaultSubobject<UStaticMeshComponent>("Small Sphere");
	SmallSphere->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABallBoss::BeginPlay()
{
	Super::BeginPlay();
	// it is answering to "who can start this movement ?"
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	// Mapping to health information. If you want to change, check details panel.
	BigSphereHealth = BigMaxHealth;
	MiddleSphereHealth = MiddleMaxHealth;
	SmallSphereHealth = SmallMaxHealth;
}

// Called every frame
void ABallBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Calculate();
}

// checking switch logic
void ABallBoss::ChangeLocation()
{
	// returning switch logic with opposite
	CanSwitch = !CanSwitch;
}

// checking default position or middle
void ABallBoss::Calculate()
{
	// when player went to trigger area, it will begin.
	if (ShamanStart && ContraCharacter && ShamanStart->IsOverlappingActor(ContraCharacter))
	{
		if (CanSwitch)
		{
			// if balls default position, can move 360 degrees
			if (CanCircularMove)
			{
				CircularMovement(CircularLocation());
			}
			else
			{
				BackToInitial();
			}
			// For delay
			CircularMovementLast = GetWorld()->GetTimeSeconds();
			// giving 8 sec delay and after waiting, going middle
			if (GetWorld()->GetTimeSeconds() - MiddleMovementLast > 8.0f)
			{
				ChangeLocation();
			}
		}
		else
		{
			MiddleMovement();
			MiddleMovementLast = GetWorld()->GetTimeSeconds();
			// giving 8 sec delay and after waiting, going default position
			if (GetWorld()->GetTimeSeconds() - CircularMovementLast > 8.0f)
			{
				ChangeLocation();
			}
		}
	}
}

// calculating 360 degrees movement
FVector ABallBoss::CircularLocation()
{
	// declare arbitrary vector point in the world to circle around
	FVector NewLocation = FVector(0, 0, 400);

	// declare size of radius to move around
	FVector Radius = FVector(200, 0, 0);

	// angle increases by 1 every frame
	AngleAxis++;

	// prevent number from growind indefinitely
	if (AngleAxis > 360.0f)
	{
		AngleAxis = 1;
	}

	FVector RotateValue = Radius.RotateAngleAxis(AngleAxis, FVector(0, 1, 0));

	NewLocation.X += RotateValue.X;
	NewLocation.Y += RotateValue.Y;
	NewLocation.Z += RotateValue.Z;

	return NewLocation;
}

// balls are moving around 360 degrees.
void ABallBoss::CircularMovement(FVector Location) const
{
	/* Setting sphere location with different height and
	 "/6" because we want to little slow circular movement.
	 if you want to speed or more slow movement, you can change this value.
	 */
	// if sphere alive!
	if (BigSphere) { BigSphere->SetRelativeLocation(Location / 6); }
	if (MiddleSphere) { MiddleSphere->SetRelativeLocation(Location / 6 - FVector(0, 0, 125)); }
	if (SmallSphere) { SmallSphere->SetRelativeLocation(Location / 6 + FVector(0, 0, 150)); }
}

// balls are going to target location
void ABallBoss::MiddleMovement()
{
	// Creating shaman effect once for per function
	CreateShamanEffect();
	if (CanGiveDamage || CanGiveDamage2) // optional
	{
		DecreaseDamage();
	}
	CanCreateShamanEffect = false;
	// Turning of circular movement
	CanCircularMove = false;
	// Calculating and setting movement
	if (BigSphere)
	{
		// if big sphere didn't destroy, it's setting location.
		FVector BigLocation = FMath::VInterpConstantTo(BigSphere->GetComponentLocation(), TargetLocation,
		                                               GetWorld()->DeltaTimeSeconds, 600.0f);
		BigSphere->SetWorldLocation(BigLocation);
	}

	if (MiddleSphere)
	{
		// if middle sphere didn't destroy, it's setting location.
		FVector MiddleLocation = FMath::VInterpConstantTo(MiddleSphere->GetComponentLocation(),
		                                                  TargetLocation,
		                                                  GetWorld()->DeltaTimeSeconds, 500.0f);
		MiddleSphere->SetWorldLocation(MiddleLocation);
	}

	if (SmallSphere)
	{
		// if small sphere didn't destroy, it's setting location.
		FVector SmallLocation = FMath::VInterpConstantTo(SmallSphere->GetComponentLocation(),
		                                                 TargetLocation,
		                                                 GetWorld()->DeltaTimeSeconds, 600.0f);
		SmallSphere->SetWorldLocation(SmallLocation);
	}
}

// balls are coming back to default position
void ABallBoss::BackToInitial()
{
	// returning true because new function need to create effect again.
	CanCreateShamanEffect = true;
	CanCreateShamanEffect2 = true; // optional
	// Calculating and setting movement
	if (BigSphere)
	{
		// if big sphere didn't destroy, it's setting location.
		FVector BigLocation = FMath::VInterpConstantTo(BigSphere->GetComponentLocation(), BigInitialLocation,
		                                               GetWorld()->DeltaTimeSeconds, 600.0f);
		BigSphere->SetWorldLocation(BigLocation);
	}

	if (MiddleSphere)
	{
		// if middle sphere didn't destroy, it's setting location.
		FVector MiddleLocation = FMath::VInterpConstantTo(MiddleSphere->GetComponentLocation(),
		                                                  MiddleInitialLocation,
		                                                  GetWorld()->DeltaTimeSeconds, 500.0f);
		MiddleSphere->SetWorldLocation(MiddleLocation);
	}

	if (SmallSphere)
	{
		// if small sphere didn't destroy, it's setting location.
		FVector SmallLocation = FMath::VInterpConstantTo(SmallSphere->GetComponentLocation(),
		                                                 SmallInitialLocation,
		                                                 GetWorld()->DeltaTimeSeconds, 600.0f);
		SmallSphere->SetWorldLocation(SmallLocation);
	}
	// if balls default position, can move.
	// + if all balls live!!
	if (BigSphere && MiddleSphere && SmallSphere && (BigSphere->GetComponentLocation() == BigInitialLocation ||
		MiddleSphere->GetComponentLocation() ==
		MiddleInitialLocation || SmallSphere->GetComponentLocation() == SmallInitialLocation))
	{
		CanCircularMove = true;
	}
}

// Creating 2 Forward effect
void ABallBoss::CreateShamanEffect()
{
	if (CanCreateShamanEffect)
	{
		CanGiveDamage = true;
		// Setting timer for stop damage. ! 3.7f is anim stop time.
		GetWorldTimerManager().SetTimer(DamageTimer, this, &ABallBoss::StopDamage, 1.0f, false, 3.7f);
		// setting timer for forward effect after 5 sec. delaying.
		GetWorldTimerManager().SetTimer(EffectTimer, this, &ABallBoss::CreateShamanEffect2, 1.0f, false, 5.0);
		// Creating 2 effect to your choice position and different (+45) rotation.
		for (int a = 0; a < 2;)
		{
			float SumRotation = 0.0f;

			for (int i = 0; i < ShamanEffects[a].EffectCount; i++)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ShamanEffects[a].ShamanEffect,
				                                               ShamanEffects[a].EffectTargetLocation,
				                                               ShamanEffects[a].EffectTargetRotation + FRotator(
					                                               0.0f, SumRotation, 0.0f));
				SumRotation += 45.0f;
			}
			a++;
		}
	}
}

// Creating 2 backward effect (optional)
void ABallBoss::CreateShamanEffect2()
{
	if (CanCreateShamanEffect2)
	{
		CanGiveDamage2 = true;
		// Setting timer for stop damage. ! 3.7f is anim stop time.
		GetWorldTimerManager().SetTimer(DamageTimer, this, &ABallBoss::StopDamage, 1.0f, false, 3.7f);
		// Effect deleting timer
		GetWorldTimerManager().ClearTimer(EffectTimer);
		CanCreateShamanEffect2 = false;
		// Creating 2 effect to your choice position and different (+45) rotation.
		for (int a = 2; a < ShamanEffects.Num();)
		{
			float SumRotation = 0.0f;

			for (int i = 0; i < ShamanEffects[a].EffectCount; i++)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ShamanEffects[a].ShamanEffect,
				                                               ShamanEffects[a].EffectTargetLocation,
				                                               ShamanEffects[a].EffectTargetRotation + FRotator(
					                                               0.0f, SumRotation, 0.0f));
				SumRotation += 45.0f;
			}
			a++;
		}
	}
}

// Give Damage with effects
void ABallBoss::DecreaseDamage()
{
	if (!ContraCharacter) { return; }
	if (!ShamanEffects[0].DamageArea || !ShamanEffects[1].DamageArea || !ShamanEffects[2].DamageArea || !ShamanEffects[
		3].DamageArea) { return; }
	// if player's overlapping in trigger box (1-2), decreasing player health.
	if (CanGiveDamage && (ShamanEffects[0].DamageArea->IsOverlappingActor(ContraCharacter) ||
		ShamanEffects[1].DamageArea->IsOverlappingActor(ContraCharacter)))
	{
		ContraCharacter->Health -= 0.05;
	}
		// if player's overlapping in trigger box (2-3), decreasing player health.
	else if (CanGiveDamage2 && (ShamanEffects[2].DamageArea->IsOverlappingActor(ContraCharacter) ||
		ShamanEffects[3].DamageArea->IsOverlappingActor(ContraCharacter)))
	{
		ContraCharacter->Health -= 0.05;
	}
}

// it's stopping effect damage
void ABallBoss::StopDamage()
{
	CanGiveDamage = false;
	CanGiveDamage2 = false;
}

// apply damage
float ABallBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                            AActor* DamageCauser)
{
	FHitResult hitRes;
	FVector impulseDir;
	// it's getting hit information and mapping to projectile apply point damage.
	DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);

	// pawn taking damage and applying health.
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// it's checking hit result for is this equal to component.
	if (hitRes.GetComponent())
	{
		if (hitRes.Component == BigSphere)
		{
			DamageToApply = FMath::Min(BigSphereHealth, DamageToApply);
			BigSphereHealth -= DamageToApply;
			if (BigSphereHealth <= 0)
			{
				// it's destroying sphere and spawning sound, particle.
				BigSphere->DestroyComponent();
				UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
				UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
			}
		}
		else if (hitRes.Component == MiddleSphere)
		{
			DamageToApply = FMath::Min(MiddleSphereHealth, DamageToApply);
			MiddleSphereHealth -= DamageToApply;
			if (MiddleSphereHealth <= 0)
			{
				// it's destroying sphere and spawning sound, particle.
				MiddleSphere->DestroyComponent();
				UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
				UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
			}
		}
		else if (hitRes.Component == SmallSphere)
		{
			DamageToApply = FMath::Min(SmallSphereHealth, DamageToApply);
			SmallSphereHealth -= DamageToApply;
			if (SmallSphereHealth <= 0)
			{
				// it's destroying sphere and spawning sound, particle.
				SmallSphere->DestroyComponent();
				UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
				UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
			}
		}
	}

	// when health was less than 10, it will set start slow motion value and call function.
	if (BigSphereHealth < 10 && MiddleSphereHealth < 10 && SmallSphereHealth < 10)
	{
		// when all balls died, it will destroy herself
		Health -= 100;
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
// slow motion
void ABallBoss::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &ABallBoss::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		// + when all balls died, it's calling this function.
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

// check is health less or equal zero.
bool ABallBoss::IsDead() const
{
	return Health <= 0;
}
