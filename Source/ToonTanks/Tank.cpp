// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"





ATank::ATank() {

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::HandleDestruction() {

	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	bIsAlive = false;
	SetActorTickEnabled(false);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay() {

	Super::BeginPlay();

	PlayerControllerPTR = Cast<APlayerController>(GetController());
}
// Called every frame
void ATank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (PlayerControllerPTR) {
		FHitResult HitResult;
		PlayerControllerPTR->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		FVector ImpactPoint = HitResult.ImpactPoint;

		RotateTurret(ImpactPoint);
	}
}

void ATank::Move(float Value) {

	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	FVector DeltaLocation(0, 0, 0);
	DeltaLocation.X = Value * Speed * DeltaTime;
	AddActorLocalOffset(DeltaLocation, true);
	
}
void ATank::Turn(float Value) {

	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = DeltaTime * Value * TurnRate;
	AddActorLocalRotation(DeltaRotation, true);

}

