// Fill out your copyright notice in the Description page of Project Settings.


#include "ElecticShock.h"
#include "ContraCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ReturnoftheGorm/ReturnoftheGormGameMode.h"
#include "TimerManager.h"

#define OUT

// Sets default values
AElecticShock::AElecticShock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
	RootComponent = SceneComponent;
	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boss"));
	BossMesh->SetupAttachment(SceneComponent);
	End = CreateDefaultSubobject<UStaticMeshComponent>("End");
	End->SetupAttachment(SceneComponent);
	Start = CreateDefaultSubobject<UStaticMeshComponent>("Start");
	Start->SetupAttachment(SceneComponent);
	Right = CreateDefaultSubobject<UStaticMeshComponent>("Right");
	Right->SetupAttachment(SceneComponent);
	Left = CreateDefaultSubobject<UStaticMeshComponent>("Left");
	Left->SetupAttachment(SceneComponent);

	ElectricEffect = CreateDefaultSubobject<UNiagaraComponent>("Electric Effect");
	//ElectricEffect->OnComponentHit.AddDynamic(this, &AElecticShock::OnHit);
	ElectricEffect->SetupAttachment(Start);
	ElectricEffect2 = CreateDefaultSubobject<UNiagaraComponent>("Electric Effect2");
	//ElectricEffect2->OnComponentHit.AddDynamic(this, &AElecticShock::OnHit);
	ElectricEffect2->SetupAttachment(Left);
	ElectricEffect3 = CreateDefaultSubobject<UNiagaraComponent>("Electric Effect3");
	//ElectricEffect3->OnComponentHit.AddDynamic(this, &AElecticShock::OnHit);
	ElectricEffect3->SetupAttachment(Right);
}

// Called when the game starts or when spawned
void AElecticShock::BeginPlay()
{
	Super::BeginPlay();
	// When it started, it setting effect visibility off and getting location information.
	EffectLocation();
	EffectVisibility();
	Health = MaxHealth;
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void AElecticShock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectLogic(DeltaTime);
}

// Effect Logic
void AElecticShock::EffectLogic(float DeltaTime)
{
	// if logic true-false, calling effect and updating last effect worked time.
	if (EffectSwitch && PressPlate && PressPlate->IsOverlappingActor(ContraCharacter))
	{
		EffectOn(DeltaTime);
		EffectOnLast = GetWorld()->GetTimeSeconds();
	}
	else
	{
		EffectOff(DeltaTime);
		EffectOffLast = GetWorld()->GetTimeSeconds();
	}
}

// Effect Start location backup
void AElecticShock::EffectLocation()
{
	BossMesh->SetWorldLocation(FVector(1590.000000, -18580.000000, 206.000000));
	EndLocation = End->GetComponentLocation();
	LeftLocation = Left->GetComponentLocation();
	RightLocation = Right->GetComponentLocation();

	EndUpdateLocation = EndLocation;
	LeftUpdateLocation = LeftLocation;
	RightUpdateLocation = RightLocation;
}

// Effect target location
void AElecticShock::EffectOn(float DeltaTime)
{
	// It is calculating smooth movement end updating location.
	EndUpdateLocation = FMath::VInterpConstantTo(EndUpdateLocation,
	                                             EndTargetLocation,
	                                             DeltaTime, 500.0f);

	LeftUpdateLocation = FMath::VInterpConstantTo(LeftUpdateLocation,
	                                              LeftTargetLocation,
	                                              DeltaTime, 700.0f);
	RightUpdateLocation = FMath::VInterpConstantTo(RightUpdateLocation,
	                                               RightTargetLocation,
	                                               DeltaTime, 700.0f);
	End->SetWorldLocation(EndUpdateLocation);
	Left->SetWorldLocation(LeftUpdateLocation);
	Right->SetWorldLocation(RightUpdateLocation);
	// if 3 balls reached targed, effect visibility turning on. After waiting delay, turning effect switch false.
	if (End->GetComponentLocation().Y == EndTargetLocation.Y && Left->GetComponentLocation().Y == LeftTargetLocation.Y
		&&
		Right->GetComponentLocation().Y == RightTargetLocation.Y)
	{
		// Setting effect visibility on.
		EffectVisibilityLogic = false;
		EffectVisibility();
		// All effect connecting with end sphere.
		if (!ElectricEffect && !ElectricEffect2 && !ElectricEffect3) { return; }
		ElectricEffect->SetVectorParameter(EffectName, End->GetComponentLocation());
		ElectricEffect2->SetVectorParameter(EffectName, End->GetComponentLocation());
		ElectricEffect3->SetVectorParameter(EffectName, End->GetComponentLocation());
		ElectricHit();
		if (GetWorld()->GetTimeSeconds() - EffectOffLast > EffectDelay)
		{
			EffectSwitch = false;
		}
	}
}

// Effect turning back
void AElecticShock::EffectOff(float DeltaTime)
{
	// It is calculating smooth movement end updating location.
	EndUpdateLocation = FMath::VInterpConstantTo(EndUpdateLocation,
	                                             EndLocation,
	                                             DeltaTime, 700.0f);

	LeftUpdateLocation = FMath::VInterpConstantTo(LeftUpdateLocation,
	                                              LeftLocation,
	                                              DeltaTime, 500.0f);
	RightUpdateLocation = FMath::VInterpConstantTo(RightUpdateLocation,
	                                               RightLocation,
	                                               DeltaTime, 500.0f);
	// setting location
	End->SetWorldLocation(EndUpdateLocation);
	Left->SetWorldLocation(LeftUpdateLocation);
	Right->SetWorldLocation(RightUpdateLocation);

	EffectVisibilityLogic = true;
	EffectVisibility();

	// if 3 balls reached targed, effect visibility turning on. After waiting delay, turning effect switch true.
	if (End->GetComponentLocation().Y == EndLocation.Y && Left->GetComponentLocation().Y == LeftLocation.Y && Right->
		GetComponentLocation().Y == RightLocation.Y)
	{
		if (GetWorld()->GetTimeSeconds() - EffectOnLast > EffectDelay)
		{
			EffectSwitch = true;
		}
	}
}

// Lightning visibility on/off settings
void AElecticShock::EffectVisibility()
{
	// if logic true, it setting effect off.
	if (EffectVisibilityLogic)
	{
		ElectricEffect->SetVisibility(false);
		ElectricEffect2->SetVisibility(false);
		ElectricEffect3->SetVisibility(false);
	}
	else
	{
		ElectricEffect->SetVisibility(true);
		ElectricEffect2->SetVisibility(true);
		ElectricEffect3->SetVisibility(true);
	}
}

// when hit the electric
void AElecticShock::ElectricHit()
{
	// if actor waiting in electric, increasing health 0.5 per second
	if ((EffectHitStart() || EffectHitLeft() || EffectHitRight()) && ContraCharacter)
	{
		ContraCharacter->Health -= 0.5f;
	}
}

// if actor been electric end-start
bool AElecticShock::EffectHitStart()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	// sending line trace between end-start
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		End->GetComponentLocation(),
		Start->GetComponentLocation(),
		ECC_Pawn,
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	// when hit the pawn and if pawn tag is player, returning true.
	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// if actor been electric end-left
bool AElecticShock::EffectHitLeft()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		End->GetComponentLocation(),
		Left->GetComponentLocation(),
		ECC_Pawn,
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// if actor been electric end-right
bool AElecticShock::EffectHitRight()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		End->GetComponentLocation(),
		Right->GetComponentLocation(),
		ECC_Pawn,
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		return true;
	}
	else
	{
		return false;
	}
}
// apply damage
float AElecticShock::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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
// check is health less or equal zero.
bool AElecticShock::IsDead() const
{
	return Health <= 0;
}
// slow motion
void AElecticShock::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &AElecticShock::BacktoNormal, 2, true, 0.1f);
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
