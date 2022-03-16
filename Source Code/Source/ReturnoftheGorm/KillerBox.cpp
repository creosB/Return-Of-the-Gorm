// Fill out your copyright notice in the Description page of Project Settings.


#include "KillerBox.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AKillerBox::AKillerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}

// Called when the game starts or when spawned
void AKillerBox::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
}

// Called every frame
void AKillerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Player && IsOverlappingActor(Player))
	{
		Player->Health -= 200;
	}
}

