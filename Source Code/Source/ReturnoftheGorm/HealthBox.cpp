// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "ContraCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHealthBox::AHealthBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBox Mesh"));
	HealthBoxMesh->OnComponentHit.AddDynamic(this, &AHealthBox::OnHit);
	RootComponent = HealthBoxMesh;

}

// Called when the game starts or when spawned
void AHealthBox::BeginPlay()
{
	Super::BeginPlay();
}

// When hit the player on healthbox, it will give 10.0f health value. After taking health, it will destroy own.
void AHealthBox::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			FVector NormalImpulse, const FHitResult& Hit)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }

	if(ContraCharacter && OtherActor != this)
	{
		ContraCharacter->Health += GiveHealthValue;
		FindPointer();
		UGameplayStatics::SpawnEmitterAtLocation(this,GiveHealthParticle,GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, GiveHealthSound, GetActorLocation());
		Destroy();
	}
}

void AHealthBox::FindPointer()
{
	if (!GiveHealthSound) { return; }
	if (!GiveHealthParticle) { return; }
}





