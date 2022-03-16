// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

class AContraCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RETURNOFTHEGORM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void DoorLogic(float DeltaTime);
	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	AContraCharacter* ContraCharacter = nullptr;
	// Door Calculation
	float InitialYaw;
	float CurrentYaw;
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;
	float DoorLastOpen = 0.0f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.05f;
	// Door Settings
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 60.0f;
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 60.0f;
	UPROPERTY(EditAnywhere)
	float OpenDistance = 300.0f;
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
