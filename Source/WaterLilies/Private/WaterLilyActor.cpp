#include "WaterLilyActor.h"

AWaterLilyActor::AWaterLilyActor()
{
 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set a random height for this actor
	MaxScale = FMath::FRandRange(0.7f, 1.3f);
	FVector Scale = FVector(1, 1, MaxScale);
}

void AWaterLilyActor::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* Material = PetalMeshComponent->GetStaticMesh()->GetMaterial(0);
	MaterialInstance = PetalMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Material);
}

// Called every frame
void AWaterLilyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Scale = GetActorScale3D();
	float ScaleFactor = Scale.Z;
	// Shrink the actor
	if (b_IsShrinking)
	{
		ScaleFactor -= DeltaTime;
	}
	// Grow while the actor has not reached its maximum scale
	else
	{
		if (ScaleFactor < MaxScale)
		{
			ScaleFactor += DeltaTime * 0.5f;
		}
		else
		{
			b_ShrinkingByClick = false;
		}
	}
	ScaleFactor = FMath::Clamp(ScaleFactor, MaxScale / 10, MaxScale);
	Scale.Z = ScaleFactor;
	SetActorScale3D(Scale);
}

void AWaterLilyActor::BeginHover(InteractionMode CurrentMode)
{
	if (CurrentMode == Scaling)
	{
		if (!b_IsShrinking)
		{
			b_IsShrinking = true;
		}
	}
}

void AWaterLilyActor::EndHover()
{
	if (b_IsShrinking)
	{
		b_IsShrinking = false;
	}
}

void AWaterLilyActor::WasClicked(InteractionMode CurrentMode, TArray<AWaterLilyActor*> Actors)
{
	if (CurrentMode == Scaling)
	{
		ShrinkConnected(Actors);
	}
	else if (CurrentMode == Coloring)
	{
		ChangeColor();
	}
}

void AWaterLilyActor::ResetColor()
{
	FLinearColor NewColor = FColor::White;
	
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue(TEXT("Color"), NewColor);
		PetalMeshComponent->GetStaticMesh()->SetMaterial(0, MaterialInstance);

	}
}

void AWaterLilyActor::ShrinkConnected(TArray<AWaterLilyActor*> Actors)
{
	// Do not shrink the same actors multiple times 
	if (b_ShrinkingByClick) return;

	// Instantly shrink the actor to its minimum scale
	FVector Scale = GetActorScale3D();
	Scale.Z = MaxScale / 10;
	SetActorScale3D(Scale);
	b_IsShrinking = false;
	b_ShrinkingByClick = true;
	
	// Shrink all actors in ShrinkRange
	FVector ActorLocation = GetActorLocation();
	for (AWaterLilyActor* Actor : Actors)
	{
		FVector NeighborLocation = Actor->GetActorLocation();
		float Distance = FVector::Dist(ActorLocation, NeighborLocation);
		if (Distance <= ShrinkRange && this != Actor)
		{
			// Use a timer to delay next shrinking wave
			FTimerDelegate TimerDelegate;
			FTimerHandle TimerHandle;
			TArray<AWaterLilyActor*> RemainingActors = Actors;
			RemainingActors.Remove(Actor);
			TimerDelegate.BindUObject(Actor, &AWaterLilyActor::ShrinkConnected, RemainingActors);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.4f, false);
		}
	}
}

void AWaterLilyActor::ChangeColor()
{
	FLinearColor NewColor;
	// Randomize the color
	NewColor.R = FMath::RandRange(0.0f, 1.0f);
	NewColor.G = FMath::RandRange(0.0f, 1.0f);
	NewColor.B = FMath::RandRange(0.0f, 1.0f);
	NewColor.A = 1.0f;
	// Apply it to the mesh
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue(TEXT("Color"), NewColor);
		PetalMeshComponent->GetStaticMesh()->SetMaterial(0, MaterialInstance);

	}
}