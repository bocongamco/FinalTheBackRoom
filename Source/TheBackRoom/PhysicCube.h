// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PhysicCube.generated.h"



UCLASS()

class THEBACKROOM_API APhysicCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicCube();


	

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PPMesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* PPHitBox;
	
	
	UPROPERTY(EditAnywhere)
	float ForceStrength;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHitboxCollide(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	bool PhysicContact;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
