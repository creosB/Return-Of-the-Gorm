// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/TriggerVolume.h"
#include "FireObstacleSpawner.generated.h"

// Direction Choice
UENUM(BlueprintType)
enum ESpawnDirection
{
	SpawnDirectionX UMETA(Display = "X direction"),
	SpawnDirectionY UMETA(Display = "Y direction"),
	SpawnDirectionZ UMETA(Display = "Z direction")
};

// Choice for every array elements.
USTRUCT(BlueprintType)
struct RETURNOFTHEGORM_API FSpawnSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESpawnDirection> SpawnDirection;
	// Min-Max value for Spawn Location and Rotation
	UPROPERTY(EditAnywhere)
	float MinSpawnLocation;
	UPROPERTY(EditAnywhere)
	float MaxSpawnLocation;
	// - or + Roll value
	UPROPERTY(EditAnywhere)
	float SpawnRotation;
};

class AContraCharacter;

UCLASS()
class RETURNOFTHEGORM_API AFireObstacleSpawner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireObstacleSpawner();

	virtual void Tick(float DeltaSeconds) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

private:
	// Function
	void ObstacleSpawn(); // creating actor
	void SetSpawnTimer(); // calling timer
	void SpawnChoice(); // defines values.
	void BacktoNormal(); // Slow motion
	bool IsDead() const;

	// Spawner Main Settings
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SpawnerMesh = nullptr;
	// Cast Main Player
	AContraCharacter* ContraCharacter = nullptr;
	// Spawn Object
	UPROPERTY(EditAnywhere, Category = "Main Settings")
	TSubclassOf<AActor> ActorToSpawn;
	FTimerHandle SpawnTimer;
	// Info from struct
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Spawner Settings"))
	TArray<struct FSpawnSettings> SpawnSetting;
	// Spawner temp value for set the actor.
	FVector SpawnerLocation;
	FRotator SpawnerRotation;
	// Trigger Volume for the ActorMovement
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	ATriggerVolume* Trigger = nullptr;
	// Spawner Settings
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnSpeed = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnRepeat = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnedMovementSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnedDeadTime = 10.0f; // Spawned obstacle destroy time
	int i = 0; // array start value
	bool IsWork = true;

	// Slow Motion
	bool SlowLogic = false;
	FTimerHandle BacktoNormalTimer;
	float TimerValue = 0.01;
	// Health
	bool ReturnGameModeDead = true;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float Health;
	// Effect Choice
	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle = nullptr;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;
	// for detect attached actor or static meshes
	TArray<AActor*> Attached;
	UPROPERTY(EditAnywhere)
	bool DestroyWAttachment = false;
};
