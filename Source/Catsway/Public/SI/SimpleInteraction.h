// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USimpleInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CATSWAY_API ISimpleInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	void PlayerEntersNearbyArea();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	void PlayerLeavesNearbyArea();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	void PlayerFocuses();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	void PlayerUnfocuses();

	// Create CanPlayerInteract with default return true
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	bool CanPlayerInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Simple Interaction")
	void PlayerInteracts();
};
