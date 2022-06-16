// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PressurePad.h"
#include "ChasingGhost.generated.h"

UCLASS()
class THEBACKROOM_API AChasingGhost : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChasingGhost();

	UPROPERTY(EditAnywhere)
		APressurePad* linksPad;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
		void OnSensed(const TArray<AActor*>& UpdatedActors);

	void SetNewRotation(FVector TargetPosition, FVector CurrentPosition);

	UPROPERTY(VisibleDefaultsOnly, Category = Ghost)
		class UAIPerceptionComponent* AIPerComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Ghost)
		class UAISenseConfig_Sight* SightConfig;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		FRotator EnemyRotation;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FVector BaseLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		float MovementSpeed;




	

	bool BackToBaseLocation;
	FVector NewLocation;
	float DistanceSquared;

	
	UPROPERTY(EditAnywhere)
		bool isCollide = false;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;
	


public: 

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	
};
