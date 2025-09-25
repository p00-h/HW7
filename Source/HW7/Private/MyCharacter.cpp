#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	// Create a root component
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->InitCapsuleSize(42.f, 96.f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	// Create a mesh component
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Create a movement component
	MoveComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MoveComp"));
	MoveComp->UpdatedComponent = RootComponent;
	MoveComp->MaxSpeed = 600.f;
	MoveComp->Acceleration = 4096.f;
	MoveComp->Deceleration = 4096.f;

	// Create a camera 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

}

void AMyCharacter::Look(const FInputActionValue& value)
{
	const FVector2D Axis = value.Get<FVector2D>();
	if (Axis.IsNearlyZero()) return;

	AddActorLocalRotation(FRotator(0.f, Axis.X, 0.f));

	SpringArm->AddLocalRotation(FRotator(-Axis.Y, 0.f, 0.f));
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (MoveComp) MoveComp->MaxSpeed = 1000.f;
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (MoveComp) MoveComp->MaxSpeed = 600.f;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// Add Input Mapping Context
			if(PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this, 
					&AMyCharacter::Move
				);
			}
			if(PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this, 
					&AMyCharacter::Look
				);
			}
			if(PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Started,
					this, 
					&AMyCharacter::StartSprint
				);
				EnhancedInputComponent->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this, 
					&AMyCharacter::StopSprint
				);
			}

		}
	}
}

