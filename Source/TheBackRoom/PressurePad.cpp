// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePad.h"

// Sets default values
APressurePad::APressurePad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	
	PressurePadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	PressurePadMesh->SetupAttachment(RootComponent);
	
	PressurePadHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	PressurePadHitBox->SetupAttachment(RootComponent);
	
	overLap = false;
	
}



// Called when the game starts or when spawned
void APressurePad::BeginPlay()
{
	Super::BeginPlay();
	PressurePadHitBox->OnComponentBeginOverlap.AddDynamic(this, &APressurePad::OnHitboxOverlapBegin);
	
}

// Called every frame
void APressurePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePad::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult){
	
	if (OtherActor && OtherActor != this)
	{
		
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlap Has Begun"));
	PressurePadHitBoxDelegate.Broadcast();
	
	overLap = true;

	
}

bool APressurePad::getOverlap()
{
	return overLap;
}