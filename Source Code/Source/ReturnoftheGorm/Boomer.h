// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "Boomer.generated.h"

UCLASS()
class RETURNOFTHEGORM_API ABoomer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoomer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	FVector SpawnerLocation;
	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	FRotator SpawnerRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Function
	void Spawner();
	void SpawnerCalculate();

	// Creating Random time
	FTimerHandle SpawnTimer;
	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	float SpawnSpeed = 1.0f;
	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	float SpawnRepeat = 1.0f;

	// Spawner Settings
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SpawnObjectMesh = nullptr;
	UPROPERTY(EditAnywhere, Category= "Spawner Settings")
	TSubclassOf<AActor> ActorToSpawn;
	// Effect System
	UPROPERTY(EditDefaultsOnly,Category= "Effects")
	UParticleSystem* SpawnBoxParticle = nullptr;
	UPROPERTY(EditDefaultsOnly,Category= "Effects")
	USoundBase* SpawnBoxSound = nullptr;

	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	int32 SpawnMinZ = 1;
	UPROPERTY(EditAnywhere,Category= "Spawner Settings")
	int32 SpawnMaxZ = 5;

	// Movement
	float TransporterInitial;
	float TransporterTarget = 2000;
	float TransporterCurrent;
	float TransporterForwardSpeed = 200.0f;
	FVector Forward;
	void Movement(float DeltaTime);
};
