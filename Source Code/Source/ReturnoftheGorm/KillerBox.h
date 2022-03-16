// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "KillerBox.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AKillerBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKillerBox();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UBoxComponent* Box;
	AContraCharacter* Player = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
