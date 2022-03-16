// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameObstacle.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFlameObstacle::AFlameObstacle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlameLauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flame Launcer Mesh"));
	RootComponent = FlameLauncherMesh;

	FlameEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flame Effect"));
	FlameEffect->SetVisibility(false);
	FlameEffect->SetupAttachment(FlameLauncherMesh);

	EffectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Effect Spawn Point"));
	EffectSpawnPoint->SetupAttachment(FlameLauncherMesh);
}

// Called when the game starts or when spawned
void AFlameObstacle::BeginPlay()
{
	Super::BeginPlay();
	// Effect location
	StartLocation = EffectSpawnPoint->GetComponentLocation();
	EndLocation = StartLocation + FVector(900, 0, 0);
	// Cast to player
	Player = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	// it's setting timer for effect on-off.
	GetWorldTimerManager().SetTimer(EffectTimer, this, &AFlameObstacle::EffectLogic, EffectDelay, true, EffectDelay);
}

// Called every frame
void AFlameObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EffectSwitch)
	{
		EffectOn();
	}
	else
	{
		EffectOff();
	}
}

void AFlameObstacle::EffectLogic()
{
	EffectSwitch = !EffectSwitch;
}

void AFlameObstacle::EffectOn() const
{
	if (Trigger && Trigger->IsOverlappingActor(Player) && EffectSwitch && Player)
	{
		FlameEffect->SetVisibility(true);
		if(EffectHitStart())
		{
			EffectHit();
		}
	}
}

void AFlameObstacle::EffectOff() const
{
	FlameEffect->SetVisibility(false);
}


bool AFlameObstacle::EffectHitStart() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	// sending line trace between end-start
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		StartLocation,
		EndLocation,
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

void AFlameObstacle::EffectHit() const
{
	if (Player)
	{
		Player->Health -= 0.1f;
	}
}
