#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WaterLilyActor.h"
#include "Configuration.h"
#include "MyPawn.generated.h"

UCLASS()
class WATERLILIES_API AMyPawn : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	AMyPawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	APlayerController* MyPlayerController;
	Configuration MyConfiguration;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** The z-value of the water surface */
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Water Level", Keywords = "Water Level Height"), Category = "Water Lily Pawn")
		float WaterLevel;
	/** Get the current interaction mode */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Active Mode", Keywords = "Get Current Active Interaction Mode"), Category = "Water Lily Pawn")
		InteractionMode GetActiveMode();
private:
	// Cycle through the available interaction modes
	void ChangeMode();
	void OnMouseClick();
	void QuitApplication();
	void ResetFlowers();
public:
	/** A list of all spawned lilies */
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Spawned Actors", Keywords = "Spawned Active Water Lily Actors"), Category = "Water Lily Pawn")
		TArray<AWaterLilyActor*> SpawnedActors;
	/** Reference to the blueprint derived from AWaterLilyActor */
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Water Lily Blueprint", Keywords = "Water Lily Actor Blueprint BP"), Category = "Water Lily Pawn")
		TSubclassOf<AWaterLilyActor> BlueprintActor;
private:
	AWaterLilyActor* SpawnActor(FVector Location);
};
