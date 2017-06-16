// Holi

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;		

private:
	// How far ahead of the player can we reach in cm.
	float Reach = 100.f;

	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *InputComponent = nullptr;

	// Ray-Cast and grab whats in reach.
	void Grab();

	// Release grabbed object.
	void Release();

	// Setup (asumed) attached input component
	void SetupInputComponent();

	// Find (asumed) attached physics handle
	void FindPhysicsHandleComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current end of reach line.
	FVector GetReachLineEnd();

	// Returns current start of reach line.
	FVector GetReachLineStart();	
};
