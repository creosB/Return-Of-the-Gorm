// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "ContraCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ContraCharacter = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + TargetYaw;

	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DoorLogic(DeltaTime);
}

void UOpenDoor::DoorLogic(float DeltaTime)
{
	if (ContraCharacter && ContraCharacter->CanOpen && ContraCharacter->GetDistanceTo(GetOwner()) <= OpenDistance)
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	FRotator Open = GetOwner()->GetActorRotation();
	Open.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(Open);

	CloseDoorSound = false;
	// Kapı açılınca sesin başlaması için:
	if (!AudioComponent) { return; }
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator Close = GetOwner()->GetActorRotation();
	Close.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(Close);

	OpenDoorSound = false;
	if (!AudioComponent) { return; }
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Audio Component bulunamadı"), *GetOwner()->GetName());
	}
}

