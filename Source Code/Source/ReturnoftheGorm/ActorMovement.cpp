// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorMovement.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT
// Sets default values for this component's properties
UActorMovement::UActorMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorMovement::BeginPlay()
{
	Super::BeginPlay();
	// Getting your direction choice.
	DirectionChoice();
	// Setting initial value for target.
	TransporterInitial = DirectionValue;
	TransporterCurrent = TransporterInitial;
	TransporterTarget = TransporterInitial + TransporterTarget;
	// for the null pointer
	FindPressPlate();
	FindAudioComponent();
	// it is answering to "who can start this movement ?"
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UActorMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TransporterLogic(DeltaTime);
}

void UActorMovement::DirectionChoice()
{
	// Getting your choice and setting value for target.
	switch (Direction)
	{
	case DirectionX:
		{
			DirectionValue = GetOwner()->GetActorLocation().X;
			//Forward
			Forward = GetOwner()->GetActorLocation();
			Forward.X = TransporterCurrent;
			//Backward
			Backward = GetOwner()->GetActorLocation();
			Backward.X = TransporterCurrent;
			break;
		}
	case DirectionY:
		{
			DirectionValue = GetOwner()->GetActorLocation().Y;
			//Forward
			Forward = GetOwner()->GetActorLocation();
			Forward.Y = TransporterCurrent;
			//Backward
			Backward = GetOwner()->GetActorLocation();
			Backward.Y = TransporterCurrent;
			break;
		}
	case DirectionZ:
		{
			DirectionValue = GetOwner()->GetActorLocation().Z;
			//Forward
			Forward = GetOwner()->GetActorLocation();
			Forward.Z = TransporterCurrent;
			//Backward
			Backward = GetOwner()->GetActorLocation();
			Backward.Z = TransporterCurrent;
			break;
		}
	default:
		DirectionValue = GetOwner()->GetActorLocation().Z;
		//Forward
		Forward = GetOwner()->GetActorLocation();
		Forward.Z = TransporterCurrent;
		//Backward
		Backward = GetOwner()->GetActorLocation();
		Backward.Z = TransporterCurrent;
		break;
	}
}

void UActorMovement::TransporterLogic(float DeltaTime)
{
	if (PressPlate && PressPlate->IsOverlappingActor(ActorThatOpen))
	{
		if (TransporterCurrent != TransporterTarget)
		{
			// when it was moving, it is updating location.
			ForwardTransporter(DeltaTime);
			// it is updating last value to return.
			TransporterForwardLast = GetWorld()->GetTimeSeconds();
		}
	}
	else if (PressPlate && !PressPlate->IsOverlappingActor(ActorThatOpen))
	{
		// returns after delay
		if (GetWorld()->GetTimeSeconds() - TransporterForwardLast > TransportDelay)
		{
			BackwardTransporter(DeltaTime);
		}
	}
}

void UActorMovement::ForwardTransporter(float DeltaTime)
{
	// Calculating movement speed with direction.
	TransporterCurrent = FMath::FInterpConstantTo(TransporterCurrent, TransporterTarget, DeltaTime,
	                                               TransporterForwardSpeed);
	DirectionChoice();
	GetOwner()->SetActorLocation(Forward);

	BackwardSound = false;
	// to play sound while moving
	if (!AudioComponent) { return; }
	if (!ForwardSound)
	{
		AudioComponent->Play();
		ForwardSound = true;
	}
}

void UActorMovement::BackwardTransporter(float DeltaTime)
{
	// Calculating movement speed with direction.
	TransporterCurrent = FMath::FInterpConstantTo(TransporterCurrent, TransporterInitial,
	                                               DeltaTime,
	                                               TransporterForwardSpeed);
	DirectionChoice();
	GetOwner()->SetActorLocation(Backward);
	// to play sound while moving
	ForwardSound = false;
	if (!AudioComponent) { return; }
	if (!BackwardSound)
	{
		AudioComponent->Play();
		BackwardSound = true;
	}
}

void UActorMovement::FindAudioComponent()
{
	// it is picking to audio.
	// Note: Don't forget add audio component on details.
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	// for the null pointer.
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Audio Component bulunamadı"), *GetOwner()->GetName());
	}
}

void UActorMovement::FindPressPlate()
{
	// for the null pointer.
	if (!PressPlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("PressPlate not found!!"));
	}
}
