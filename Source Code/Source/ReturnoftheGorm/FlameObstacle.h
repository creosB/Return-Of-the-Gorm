// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "Engine/TriggerVolume.h"
#include "FlameObstacle.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AFlameObstacle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlameObstacle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere,Category= "Components")
	UStaticMeshComponent* FlameLauncherMesh;
	UPROPERTY(EditAnywhere,Category= "Components")
	UNiagaraComponent* FlameEffect;
	UPROPERTY(EditAnywhere, Category= "Components")
	USceneComponent* EffectSpawnPoint;
	UPROPERTY(EditAnywhere,Category= "Components")
	ATriggerVolume* Trigger;

	void EffectOn() const;
	void EffectOff() const;
	void EffectHit() const;
	void EffectLogic();
	bool EffectHitStart() const;

	FVector StartLocation;
	FVector EndLocation;

	UPROPERTY(EditAnywhere,Category= "Effect Settings")
	float EffectDelay = 3.0f;
	bool EffectSwitch = true;

	FTimerHandle EffectTimer;
	AContraCharacter* Player = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
