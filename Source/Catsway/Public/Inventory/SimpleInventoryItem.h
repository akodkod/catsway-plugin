// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class CATSWAY_API USimpleInventoryItem : public UDataAsset {
  GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  FString Description;
  
  // Implement == operator for this class
  bool operator==(const USimpleInventoryItem& Other) const {
    return this == &Other;
  }
};
