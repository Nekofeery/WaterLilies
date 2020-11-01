#include "MyPawn.h"

AMyPawn::AMyPawn()
{
 	// No need for this to tick
	PrimaryActorTick.bCanEverTick = false;

	// Set this pawn to be controlled by the first player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a root component
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create a configuration for the pawn
	// For the moment, this only deals with the interaction mode, but other custom settings might go there
	MyConfiguration = Configuration();
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<APlayerController>(GetController());
	if (MyPlayerController)
	{
		// Enable mouse control
		MyPlayerController->bShowMouseCursor = true;
		MyPlayerController->bEnableClickEvents = true;
		MyPlayerController->bEnableMouseOverEvents = true;
	}
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind function calls to the keys defined in the project settings
	InputComponent->BindAction("ChangeMode", IE_Pressed, this, &AMyPawn::ChangeMode); // Spacebar 
	InputComponent->BindAction("LMouse", IE_Pressed, this, &AMyPawn::OnMouseClick); // Left mouse button
	InputComponent->BindAction("Quit", IE_Pressed, this, &AMyPawn::QuitApplication); // Escape
	InputComponent->BindAction("Reset", IE_Pressed, this, &AMyPawn::ResetFlowers);
}

InteractionMode AMyPawn::GetActiveMode()
{
	return MyConfiguration.GetMode();
}

void AMyPawn::ChangeMode()
{
	MyConfiguration.SetMode();
}

void AMyPawn::OnMouseClick()
{
	InteractionMode CurrentMode = MyConfiguration.GetMode();
	if (CurrentMode == Creation)
	{
		// Spawn a new water lily at the mouse cursor position
		FVector Location;
		FVector Direction;
		FVector PlaneOrigin(0.0f, 0.0f, WaterLevel);
		MyPlayerController->DeprojectMousePositionToWorld(Location, Direction);
		Location = FMath::LinePlaneIntersection(Location, Location + Direction, PlaneOrigin, FVector::UpVector);

		AWaterLilyActor* NewWaterLily = SpawnActor(Location);
		if (NewWaterLily)
		{
			SpawnedActors.Add(NewWaterLily);
		}
	}
}
void AMyPawn::QuitApplication()
{
	FGenericPlatformMisc::RequestExit(false);
}

AWaterLilyActor* AMyPawn::SpawnActor(FVector Location)
{
	FActorSpawnParameters SpawnParameters;
	FRotator Rotation = FRotator::ZeroRotator;
	UWorld* World = GetWorld();
	AWaterLilyActor* Actor = nullptr;
	if (World)
	{
		Actor = GetWorld()->SpawnActor<AWaterLilyActor>(BlueprintActor, Location, Rotation, SpawnParameters);
	}
	return Actor;
}

void AMyPawn::ResetFlowers()
{
	SpawnedActors[0]->ResetColor();
	for (AWaterLilyActor* Actor : SpawnedActors)
	{
		Actor->Destroy();
	}
	SpawnedActors.Empty();
}