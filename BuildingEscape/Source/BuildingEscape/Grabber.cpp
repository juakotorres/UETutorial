// Holi

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached input component.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component find"));
		/// Bind the input axis
		InputComponent->BindAction(FName("Grab"), IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName("Grab"), IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}
}

/// Look for attached Physics Handle.
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
		/// Physics handle found.
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}



void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbing"));

	/// Line trace  and see if we reach any actors with physics body collision channel set.
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = hitResult.GetComponent();
	auto ActorHit = hitResult.GetActor();

	/// If we hit something then attached a physics handle.
	if (ActorHit) {
		// attach physics handle.
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}

}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Released"));

	// release physics handle.
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * Reach;

	// physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * Reach;

	/// set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA Ray-cast) out to reach distance
	FHitResult lineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT lineTraceHit,
		playerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit 
	AActor *actorHit = lineTraceHit.GetActor();
	if (actorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace hit: %s"), *(actorHit->GetName()))
	}

	return lineTraceHit;
}