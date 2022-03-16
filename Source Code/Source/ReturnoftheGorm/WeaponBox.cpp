// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBox.h"
#include "Components/StaticMeshComponent.h"
#include "ReturnoftheGorm/ContraCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeaponBox::AWeaponBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBox Mesh"));
	WeaponBoxMesh->OnComponentHit.AddDynamic(this, &AWeaponBox::OnHit);
	RootComponent = WeaponBoxMesh;
}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBox::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }

	if(ContraCharacter && OtherActor != this)
	{
		// when player taked weaponbox, it will swap boolean option setting true and destroying himelf.
		FindPointer();
		ContraCharacter->IsCanChange = true;
		UGameplayStatics::SpawnEmitterAtLocation(this,GiveWeaponParticle,GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, GiveWeaponSound, GetActorLocation());
		Destroy();
	}
}

void AWeaponBox::FindPointer()
{
	if (!GiveWeaponParticle) { return; }
	if (!GiveWeaponSound) { return; }
}


