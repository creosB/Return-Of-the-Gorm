// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireObstacleSpawner.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "FireObstacle.generated.h"

class UActorMovement;
class AContraCharacter;

UCLASS()
class RETURNOFTHEGORM_API AFireObstacle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireObstacle();

	virtual void Tick(float DeltaSeconds) override;

	// Cast Spawner
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	AFireObstacleSpawner* FireObstacleSpawner = nullptr;

	// Movement
	UPROPERTY(EditAnywhere)
	UActorMovement* ActorMovement = nullptr;
	// Dead Time
	UPROPERTY(EditAnywhere)
	float DeadDelay = 10.0f;

private:
	void ObstacleHit(); // When hit the obstacle
	void EffectHitStart(); // when It spawned
	void ObstacleDestroy();

	// Main Settings
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category= "Effect")
	UStaticMeshComponent* EffectStart = nullptr; // Effect start location
	UPROPERTY(EditDefaultsOnly, Category= "Effect")
	UStaticMeshComponent* EffectEnd = nullptr; // Effect end location
	// Niagara Effect
	UPROPERTY(EditDefaultsOnly, Category= "Effect")
	UNiagaraComponent* FireEffect = nullptr; // Effect type
	FName EffectName = "User.BeamEnd"; // Effect name
	// Cast Main Player
	AContraCharacter* ContraCharacter = nullptr;

	FTimerHandle DeadTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
