// Fill out your copyright notice in the Description page of Project Settings.
#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
ATank::ATank()
{
	SpringArm = CreateDefaultSubobject< USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject< UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
}
void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);

	//OnTakeAnyDamage.AddDynamic(this, &ATank::DamageTaken);
}
// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}
}
void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}
// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	PlayerControllerRef =Cast<APlayerController>(GetController());
}

void ATank::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	//X=Value*DeltaTime*Speed
	DeltaLocation.X = Value * Speed*UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation,true);
}
void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	//Yaw(Z-axis)=Value*TurnRate*DeltaTime
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation,true);
}

