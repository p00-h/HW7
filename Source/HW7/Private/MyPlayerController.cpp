#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
	}
}
