// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"





// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(BaseMesh);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(Turret);

}

void ABasePawn::HandleDestruction() {
	//Visual effects and sounds etc.
	if (DeathParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());
	}
	if (DestructionSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	}
	if (DeathCameraShakeClass) {
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}
void ABasePawn::RotateTurret(FVector TargetPoint) {

	FVector StartingPoint = Turret->GetComponentLocation();
	FVector ToTarget = TargetPoint - StartingPoint;
	FRotator Rotation = ToTarget.Rotation();
	Rotation.Roll = 0;
	Rotation.Pitch = 0;

	FRotator InterpolatedRotation = FMath::RInterpTo(Turret->GetComponentRotation(), 
										Rotation, UGameplayStatics::GetWorldDeltaSeconds(this), 10);

	Turret->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::Fire() {

	 AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
	 Projectile->SetOwner(this);
}




