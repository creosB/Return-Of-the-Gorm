// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBridge.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ContraCharacter.h"
#include "Engine/Public/TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFireBridge::AFireBridge()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge Mesh"));
	BridgeMesh->SetupAttachment(SceneComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireBridge::BeginOverlap);
	BoxCollision->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AFireBridge::BeginPlay()
{
	Super::BeginPlay();
	// Getting bridge scale defaul values.
	BridgeScale = GetActorScale();
}

// if you press box collision, it will start explosive function.
void AFireBridge::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }

	if (ContraCharacter && OtherActor != this)
	{
		ExplosiveLogic = true;
		FirstExplosive = true;
		Explosive();
	}
}

void AFireBridge::Explosive()
{
	// while bridge scale lessing, it will create timer and spawn sound,particle.
	// First explosive is smaller than other reaction.
	if (ExplosiveLogic && BridgeScale.X > 0.1f)
	{
		GetWorldTimerManager().SetTimer(ExplosiveTimer, this, &AFireBridge::Explosive, 6.0f, true, 1.0f);
		if (FirstExplosive)
		{
			BridgeScale.X = FMath::FInterpTo(BridgeScale.X, 0.0f, GetWorld()->GetDeltaSeconds(), 0.2f);
			if (!ExplosiveParticle) { return; }
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplosiveParticle,
			                                         GetActorLocation() + FVector(0.0f, ParticleLocation, 0.0f),
			                                         GetActorRotation(), FVector(2.0f));
			if (!ExplosiveSound) { return; }
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosiveSound, GetActorLocation());
		}
		else
		{
			BridgeScale.X = FMath::FInterpTo(BridgeScale.X, 0.0f, GetWorld()->GetDeltaSeconds(), ExplosiveSpeed);
			if (!ExplosiveParticle) { return; }
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplosiveParticle,
			                                         GetActorLocation() + FVector(
				                                         0.0f, ParticleLocation - UpdateParticle, 0.0f));
			if (!ExplosiveSound) { return; }
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosiveSound, GetActorLocation() + FVector(
				                                       0.0f, ParticleLocation - UpdateParticle, 0.0f));
			UpdateParticle += 200.0f;
		}
		SetActorRelativeScale3D(BridgeScale);
		FirstExplosive = false;
	}
	else
	{
		// when Reaction stopped, it will clear time and destroy object.
		GetWorldTimerManager().ClearTimer(ExplosiveTimer);
		ExplosiveLogic = false;
		Destroy();
	}
}
