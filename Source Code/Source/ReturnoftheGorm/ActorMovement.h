// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "ActorMovement.generated.h"

UENUM()
enum EPlatformDirection
{
	DirectionX UMETA(Display = "X direction"),
	DirectionY UMETA(Display = "Y direction"),
	DirectionZ UMETA(Display = "Z direction")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RETURNOFTHEGORM_API UActorMovement : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent = nullptr;
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen;
	float DirectionValue;
	FVector Backward;
	FVector Forward;

	float TransporterForwardLast = 0.0f;
	UPROPERTY(EditAnywhere)
	float TransportDelay = 5.0f;
	float TransporterInitial;
	float TransporterCurrent;
	UPROPERTY(EditAnywhere)
	float TransporterTarget = 3000;

public:
	// Sets default values for this component's properties
	UActorMovement();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Function
	void FindPressPlate();
	void FindAudioComponent();
	void DirectionChoice();
	void ForwardTransporter(float DeltaTime);
	void BackwardTransporter(float DeltaTime);
	void TransporterLogic(float DeltaTime);

	// Trigger Volume
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressPlate = nullptr;
	// Direction choice
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Direction", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPlatformDirection> Direction;

	// Variables
	bool ForwardSound = false;
	bool BackwardSound = true;
	UPROPERTY(EditAnywhere)
	float TransporterForwardSpeed = 200.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
