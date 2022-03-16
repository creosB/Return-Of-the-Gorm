// Fill out your copyright notice in the Description page of Project Settings.


#include "FireObstacle.h"
#include "Components/SceneComponent.h"
#include "ReturnoftheGorm/ContraCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ReturnoftheGorm/ActorMovement.h"
#include "ReturnoftheGorm/FireObstacleSpawner.h"
#include "Engine/Public/TimerManager.h"


// Sets default values
AFireObstacle::AFireObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<UStaticMeshComponent>("Obstacle Mesh");
	RootComponent = SceneComponent;

	EffectStart = CreateDefaultSubobject<UStaticMeshComponent>("Effect Start");
	EffectStart->SetupAttachment(SceneComponent);
	EffectEnd = CreateDefaultSubobject<UStaticMeshComponent>("Effect End");
	EffectEnd->SetupAttachment(SceneComponent);

	FireEffect = CreateDefaultSubobject<UNiagaraComponent>("Fire Effect");
	FireEffect->SetupAttachment(EffectEnd);
	// Actor Movement settings on details page.
	ActorMovement = CreateDefaultSubobject<UActorMovement>("FireObstacle Movement");
}

// Called when the game starts or when spawned
void AFireObstacle::BeginPlay()
{
	Super::BeginPlay();
	// Calling main player.
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FireEffect->SetVectorParameter(EffectName, EffectStart->GetComponentLocation());
	GetWorldTimerManager().SetTimer(DeadTimer, this, &AFireObstacle::ObstacleDestroy, 1.0f, false,
	                                DeadDelay);
}

void AFireObstacle::Tick(float DeltaSeconds)
{
	EffectHitStart();
}

// It is creating LineTrace
void AFireObstacle::EffectHitStart()
{
	// Setting effect location every frame because actor is moving.
	FireEffect->SetVectorParameter(EffectName, EffectStart->GetComponentLocation());

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	// sending line trace between end-start
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		EffectStart->GetComponentLocation(),
		EffectEnd->GetComponentLocation(),
		ECC_Pawn,
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	// when hit the pawn and if pawn tag is player, returning true.
	if (ActorHit && ActorHit->ActorHasTag("Player"))
	{
		ObstacleHit();
	}
}

// When hit the obstacle.
void AFireObstacle::ObstacleHit()
{
	// if It called this function, decreasing health value.
	if (ContraCharacter)
	{
		ContraCharacter->Health -= 0.5f;
	}
}

void AFireObstacle::ObstacleDestroy()
{
	GetWorldTimerManager().ClearTimer(DeadTimer);
	Destroy();
}
