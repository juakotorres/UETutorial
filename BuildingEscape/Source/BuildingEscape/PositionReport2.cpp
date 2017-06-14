// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "PositionReport2.h"


// Sets default values for this component's properties
UPositionReport2::UPositionReport2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UPositionReport2::BeginPlay()
{
	Super::BeginPlay();

	FString ownerName = GetOwner()->GetName();
	FVector position = GetOwner()->GetTransform().GetLocation();
	FString objectPos = "";
	objectPos.Append("X = ");
	objectPos.AppendInt(position.X);
	objectPos.Append(" Y = ");
	objectPos.AppendInt(position.Y);
	objectPos.Append(" Z = ");
	objectPos.AppendInt(position.Z);


	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ownerName, *objectPos);
	
}


// Called every frame
void UPositionReport2::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

