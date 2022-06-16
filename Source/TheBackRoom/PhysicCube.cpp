// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicCube.h"

// Sets default values
APhysicCube::APhysicCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	PPMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	PPMesh->SetupAttachment(RootComponent);

	PPHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	PPHitBox->SetupAttachment(RootComponent);
	
	ForceStrength = 10000.0f;
}

// Called when the game starts or when spawned
void APhysicCube::BeginPlay()
{
	Super::BeginPlay();
	PPHitBox->OnComponentBeginOverlap.AddDynamic(this, &APhysicCube::OnHitboxCollide);
	PPHitBox->OnComponentEndOverlap.AddDynamic(this, &APhysicCube::OnOverlapEnd);

}

void APhysicCube::OnHitboxCollide(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	

	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Has Begun"));

	}

	PhysicContact = true;
	if (PhysicContact) {
		UE_LOG(LogTemp, Warning, TEXT("true"));

	}

}

void APhysicCube::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	PhysicContact = false;
	if (PhysicContact == false) {
		UE_LOG(LogTemp, Warning, TEXT("false"));

	}

}

// Called every frame
void APhysicCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PhysicContact){
		FVector MoveX = PPMesh->GetForwardVector();
		PPMesh->AddForce(MoveX * ForceStrength * PPMesh->GetMass());
	}
}

