#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Configuration.h"
#include "WaterLilyActor.generated.h"

UCLASS()
class WATERLILIES_API AWaterLilyActor : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWaterLilyActor();
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* PetalMeshComponent;
private:
	// The maximum scale to return to after shrinking
	float MaxScale;
	// A dynamic material instance derived from the base petal material
	UMaterialInstanceDynamic* MaterialInstance;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	/** 
	* Start shrinking the flower when the mouse hovers over it 
	* @param CurrentMode - The current interaction mode
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Begin Hover", Keywords = "Begin Start Hover Mouseover"), Category = "Water Lily Actor")
		void BeginHover(InteractionMode CurrentMode);
	/** Stop shrinking the flower when the mouse leaves its mesh */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "End Hover", Keywords = "End Hover Mouseover"), Category = "Water Lily Actor")
		void EndHover();
	/** Register a mouseclick */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Was Clicked", Keywords = "Was Actor Clicked OnClicked"), Category = "Water Lily Actor")
		void WasClicked(InteractionMode CurrentMode, TArray<AWaterLilyActor*> Actors);
	/** Reset petal color to white */
	void ResetColor();
private:
	// Is the mouse hovering over this actor?
	bool b_IsShrinking = false;
	// Was this actor shrunk via mouse click?
	bool b_ShrinkingByClick = false;
	// The maximum range in which actors shrink on click
	const float ShrinkRange = 150.0f;
	// Set this actor's scale to its minimum and do the same for all actors in ShrinkRange
	void ShrinkConnected(TArray<AWaterLilyActor*> Actors);
	// Set a new random color for the actor's petals
	void ChangeColor();
};
