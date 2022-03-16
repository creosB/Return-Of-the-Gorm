// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "ContraCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetNotifyRigidBodyCollision(true);
	GrenadeMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	GrenadeMesh->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
	RootComponent = GrenadeMesh;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	SpawnedTime = GetWorld()->GetTimeSeconds();
}

void AGrenade::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->GetTimeSeconds() - SpawnedTime > ExplosiveTime)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, GrenadeParticle, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, GrenadeSound, GetActorLocation());
		Destroy();
	}
}


// When hit the player on Grenade, it will decrease health
void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                     FVector NormalImpulse, const FHitResult& Hit)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }

	if (ContraCharacter && OtherActor != this)
	{
		ContraCharacter->Health -= DamageValue;
		FindPointer();
		UGameplayStatics::SpawnEmitterAtLocation(this, GrenadeParticle, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, GrenadeSound, GetActorLocation());
		Destroy();
	}
}

void AGrenade::FindPointer()
{
	if (!GrenadeSound) { return; }
	if (!GrenadeParticle) { return; }
}
