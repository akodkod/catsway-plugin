// Fill out your copyright notice in the Description page of Project Settings.


#include "SI/SimpleInteractionCharacterComponent.h"

#include "SI/SimpleInteraction.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USimpleInteractionCharacterComponent::USimpleInteractionCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USimpleInteractionCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimpleInteractionCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInteractionCharacterComponent::TraceAndFocusOnInteractableActor()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return;
	
	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	const FVector SphereCenter = OwningPawn->GetActorLocation() + TraceCenterOffset;
	const float SphereRadius = InteractionDistance + NearbyDistance;
	
	// Draw debug sphere
	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), SphereCenter, InteractionDistance, 8, FColor::Green, false, GetWorld()->GetDeltaSeconds() * 2.0f);
		DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 8, FColor::Yellow, false, GetWorld()->GetDeltaSeconds() * 2.0f);
	}

	// Define the types of objects to look for
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	
	// Define the actors to ignore
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwningPawn);
	
	// Define the array to store the results
	TArray<FOverlapResult> OverlapResults;
	
	// Define the query parameters
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);
	
	// Perform the overlap
	const bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		SphereCenter,
		FQuat::Identity,
		FCollisionObjectQueryParams(ObjectTypes),
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionQueryParams
	);
	
	if (!bOverlap)
	{
		UnfocusCurrentActor();
		UnmarkAllNearbyActors();
		return;
	}
	
	TArray<AActor*> VisibleToCameraActorsInSphere;
	AActor* NearestActor = nullptr;
	float NearestDistance = InteractionDistance;

	// TODO: Sort the OverlapResults by distance to the camera and then perform other calculations it might save a few line traces
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		AActor* Actor = OverlapResult.GetActor();

		// Skip if not implements the USimpleInteraction interface
		if (!Actor->GetClass()->ImplementsInterface(USimpleInteraction::StaticClass())) continue;		

		// Skip if not visible to camera
		if (!IsActorVisibleToCamera(Actor)) continue;

		// Add actor to the list of visible to camera actors and mark it as nearby
		VisibleToCameraActorsInSphere.Add(Actor);
		MarkActorAsNearby(Actor);
		
		// Calculate the distance to the actor
		const float Distance = FVector::Dist(SphereCenter, Actor->GetActorLocation());

		// If actor is closer than the nearest actor and visible to character, set it as the nearest actor
		if (Distance < NearestDistance && IsActorVisibleToCharacter(Actor))
		{
			NearestActor = Actor;
			NearestDistance = Distance;
		}
	}

	if (NearestActor)
	{
		FocusActor(NearestActor);
	}
	else
	{
		UnfocusCurrentActor();
	}

	UnmarkActorsAsNearbyThatAreNotFromList(VisibleToCameraActorsInSphere);
}

bool USimpleInteractionCharacterComponent::IsActorVisibleToCharacter(AActor* Actor)
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return false;
	
	return IsActorTraceable(Actor, OwningPawn->GetActorLocation() + TraceCenterOffset, OwningPawn);
}

bool USimpleInteractionCharacterComponent::IsActorVisibleToCamera(AActor* Actor)
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return false;

	const UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return false;
	
	return IsActorTraceable(Actor, Camera->GetComponentLocation(), OwningPawn);
}

bool USimpleInteractionCharacterComponent::IsActorTraceable(AActor* Actor, FVector Start, AActor* IgnoreActor)
{
	FVector End = Actor->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(IgnoreActor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionQueryParams
	);

	if (bHit)
	{
		bool bHitActor = HitResult.GetActor() == Actor;

		// Check if we hit the parent actor
		// TODO: Should I limit the hierarchy depth?
		if (!bHitActor)
		{
			AActor* ParentActor = Actor->GetAttachParentActor();
			while (ParentActor)
			{
				bHitActor = HitResult.GetActor() == ParentActor;
				if (bHitActor) break;
				ParentActor = ParentActor->GetAttachParentActor();
			}
		}
		
		if (bDebug) 
		{
		    DrawDebugLine(GetWorld(), Start, End, bHitActor ? FColor::Green : FColor::Red, false, GetWorld()->GetDeltaSeconds() * 2.0f);
		}
		
		return bHitActor;
	}
	
	return false;
}

// Mark actor as nearby if not already marked as nearby
void USimpleInteractionCharacterComponent::MarkActorAsNearby(AActor* Actor)
{
	if (NearbyActors.Contains(Actor)) return; // Actor is already marked as nearby

	NearbyActors.Add(Actor);
	ISimpleInteraction::Execute_PlayerEntersNearbyArea(Actor);
}

// Unmark actor as nearby if marked as nearby
void USimpleInteractionCharacterComponent::UnmarkActorAsNearby(AActor* Actor)
{
	if (!NearbyActors.Contains(Actor)) return; // Actor is not marked as nearby

	NearbyActors.Remove(Actor);
	ISimpleInteraction::Execute_PlayerLeavesNearbyArea(Actor);
}

void USimpleInteractionCharacterComponent::UnmarkActorsAsNearbyThatAreNotFromList(TArray<AActor*> Actors)
{
	// Create duplicate of the nearby actors list
	TArray<AActor*> NearbyActorsCopy = NearbyActors;

	for (AActor* Actor : NearbyActorsCopy)
	{
		if (!Actors.Contains(Actor)) UnmarkActorAsNearby(Actor);
	}
}

// Focus on the actor if it is not already focused, and unfocus the current actor
void USimpleInteractionCharacterComponent::FocusActor(AActor* Actor)
{
	if (FocusedActor == Actor) return; // Actor is already focused
	if (FocusedActor) UnfocusActor(FocusedActor); // Unfocus the current actor

	FocusedActor = Actor;
	ISimpleInteraction::Execute_PlayerFocuses(FocusedActor);
}

// Unfocus the actor if it is focused
void USimpleInteractionCharacterComponent::UnfocusActor(AActor* Actor)
{
	if (!FocusedActor) return; // No actor is focused
	if (FocusedActor != Actor) return; // The actor is not focused

	ISimpleInteraction::Execute_PlayerUnfocuses(FocusedActor);
	FocusedActor = nullptr;
}

// Interact with the focused actor if exists
void USimpleInteractionCharacterComponent::InteractWithFocusedActor()
{
	if (!FocusedActor) return;

	ISimpleInteraction::Execute_PlayerInteracts(FocusedActor);
}

// Unfocus the current actor if exists
void USimpleInteractionCharacterComponent::UnfocusCurrentActor()
{
	if (!FocusedActor) return;

	UnfocusActor(FocusedActor);
}

// Unmark all nearby actors
void USimpleInteractionCharacterComponent::UnmarkAllNearbyActors()
{
	for (AActor* Actor : NearbyActors)
	{
		UnmarkActorAsNearby(Actor);
	}
}
