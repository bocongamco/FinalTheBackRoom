// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheBackRoomCharacter.h"
#include "TheBackRoomProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// ATheBackRoomCharacter

ATheBackRoomCharacter::ATheBackRoomCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	
	SprintSpeed = 3.0f;
	isSprinting = false;
	outOfStamina = false;
	currentStamina = 1.0f;
	maxStamina = 1.0f;
	staminaUsage = 0.2f;
	staminaRecoverRate = 0.2f;
	outCounter = 0;


}

void ATheBackRoomCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	playerDead = false;

}

//////////////////////////////////////////////////////////////////////////// Input

void ATheBackRoomCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ATheBackRoomCharacter::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATheBackRoomCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATheBackRoomCharacter::MoveRight);


	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATheBackRoomCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATheBackRoomCharacter::StopSprinting);



	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATheBackRoomCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATheBackRoomCharacter::LookUpAtRate);


	


}




void ATheBackRoomCharacter::Sprint() {
			

		if (!outOfStamina) {
			GetCharacterMovement()->MaxWalkSpeed *= SprintSpeed;
			isSprinting = true;
			UE_LOG(LogTemp, Warning, TEXT("The character is sprinting"));
		}
}


void ATheBackRoomCharacter::StopSprinting() {
	if(outOfStamina){
		GetCharacterMovement()->MaxWalkSpeed /= SprintSpeed;
		isSprinting = false;
		UE_LOG(LogTemp, Warning, TEXT("The character is stop sprinting"));
	}
	else if (isSprinting) {

		GetCharacterMovement()->MaxWalkSpeed /= SprintSpeed;
		isSprinting = false;
	}

}



void ATheBackRoomCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnItemUsed.Broadcast();
}

void ATheBackRoomCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ATheBackRoomCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ATheBackRoomCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * TurnRateGamepad;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * TurnRateGamepad;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ATheBackRoomCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);

	}
}

void ATheBackRoomCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATheBackRoomCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATheBackRoomCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool ATheBackRoomCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATheBackRoomCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ATheBackRoomCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATheBackRoomCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void ATheBackRoomCharacter::Tick(float DeltaTime) {
	
	if (isSprinting) {
		currentStamina = FMath::FInterpConstantTo(currentStamina, 0.0f, DeltaTime, staminaUsage);
		
		if (currentStamina == 0) {
			outOfStamina=true;
			UE_LOG(LogTemp, Warning, TEXT("out OF stamina : TRUE"));
			StopSprinting();
			
			outCounter++;
			if (outCounter == 2) {
				playerDead = true;
				//Destroy();
				UE_LOG(LogTemp, Warning, TEXT("playerDead= TRUE"));

			}

		}
	}else {
		
		 if  (currentStamina < maxStamina) {
			currentStamina = FMath::FInterpConstantTo(currentStamina, maxStamina, DeltaTime, staminaRecoverRate);
			if (currentStamina != 0) {
				UE_LOG(LogTemp, Warning, TEXT("out OF stamina : false"));
				outOfStamina = false;
			}
			if (currentStamina == maxStamina) {
				outCounter = 0;
			}
		}

	}

	

}
void ATheBackRoomCharacter::DealDamage(bool checkCollide) {
	
	if (checkCollide) {
		playerDead = true;
		Destroy();
	}
}

