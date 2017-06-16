#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) { 
		UE_LOG(LogTemp, Error, TEXT("%s Missing pressure plate component"), *(Owner->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) { // TODO make into a parameter.
		onOpen.Broadcast();
	} else {
		onClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float totalmass = 0;

	if (!PressurePlate) { return totalmass; }
	// Find all the overlaping actors
	TArray<AActor *> overlapingActors;
	PressurePlate->GetOverlappingActors(overlapingActors);

	// Iterate through the adding their masses
	for (const auto* actor : overlapingActors) {
		totalmass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" Actor overlapping: %s"), *actor->GetName());
	}

	return totalmass;
}


