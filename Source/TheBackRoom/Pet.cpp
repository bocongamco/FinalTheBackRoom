// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet.h"

// Sets default values
APet::APet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	RootComponent = BottomMesh;
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(BottomMesh);

}

// Called when the game starts or when spawned
void APet::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<ATheBackRoomCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (material) {
		matInstance = UMaterialInstanceDynamic::Create(material, this);
		TopMesh->SetMaterial(0, matInstance);

	}
}

// Called every frame
void APet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(player && matInstance)
	{
		float distanceToPlayer = FVector::Distance(player->GetActorLocation(), GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Dist: %f"), distanceToPlayer);

		matInstance->SetVectorParameterValue("Colour", FLinearColor(0, 1 - distanceToPlayer / 1000, 0));
		matInstance->SetScalarParameterValue("Emissive", 1 - distanceToPlayer / 400);
		
		float clampedDistance = FMath::Clamp(distanceToPlayer / 1000, 0, 1);
		matInstance->SetScalarParameterValue("Vibration", 1 - clampedDistance);
	}

}

