// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "ReturnoftheGorm/ContraCharacter.h"
#include "Kismet/GameplayStatics.h"


APawnTurret::APawnTurret()
{

}


// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	/*
	GetWorldTimerManager() ile aynı
	2 saniyede bir ateş etmeyi sağlıyor.
	*/
	GetWorld()->GetTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&APawnTurret::CheckFireCondition,
		FireRate,
		true
	);

	PlayerPawn = Cast<AContraCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}


// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// eğer görüş mesafesindeyse tankın konum bilgisini alıyoruz
	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) { return; }
	RotateTurret(PlayerPawn->GetActorLocation());

}

void APawnTurret::CheckFireCondition()
{
	// if player == null || is dead
	if (!PlayerPawn || PlayerPawn->IsDead()) { return; }

	// if player is in range then fire
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		switch (GunType)
		{
			case Normal:
				NormalWeapon();
				break;
			case Slow:
				SlowWeapon();
				break;
			case Speed:
				SpeedWeapon();
				break;
			default: NormalWeapon();
		}
	}
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerPawn)
	{
		return 0.0f;
	}
	// 2 vector arasındaki mesafeyi hesaplıyor ve fonksiyona döndürüyor.
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
	// pawnbase de bulunan handle destruction fonk. çağırıyor
	Super::HandleDestruction();
	Destroy();
}
