// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UActorMovement;

UCLASS()
class RETURNOFTHEGORM_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector SpawnerLocation;
	FRotator SpawnerRotation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Function
	void Spawner();
	void SpawnerLogic();
	void SpawnerCalculate();
	void FindPointer();

	// Creating Random time
	FTimerHandle SpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Spawner Settings", meta = (AllowPrivateAccess = true))
	float SpawnSpeed = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Spawner Settings", meta = (AllowPrivateAccess = true))
	float SpawnRepeat = 1.0f;

	// Press Plate
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressPlate = nullptr;
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen;
	// Spawner Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* SpawnObjectMesh = nullptr;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	TSubclassOf<AActor> ActorToSpawn;
	// Calling ActorMovement.cpp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UActorMovement* ActorMovement;
	// Effect System
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	UParticleSystem* SpawnBoxParticle = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Effects", meta = (AllowPrivateAccess = true))
	USoundBase* SpawnBoxSound = nullptr;

	bool Logic = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Spawner Settings", meta = (AllowPrivateAccess = true))
	int32 SpawnMinZ = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Spawner Settings", meta = (AllowPrivateAccess = true))
	int32 SpawnMaxZ = 5;
};