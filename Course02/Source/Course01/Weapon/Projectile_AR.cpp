// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_AR.h"


// Sets default values
AProjectile_AR::AProjectile_AR()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile_AR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_AR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

