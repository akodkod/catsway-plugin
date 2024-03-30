// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SimpleInventoryItem.h"
#include "SimpleInventoryComponent.generated.h"

// Create a struct for inventory item
USTRUCT(BlueprintType)
struct FInventoryItem {
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  USimpleInventoryItem* Item;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 Quantity;

  // Implement == operator for this struct
  bool operator==(const FInventoryItem& Other) const {
    return Item == Other.Item;
  }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CATSWAY_API USimpleInventoryComponent : public UActorComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  USimpleInventoryComponent();

  // Inventory items
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  TArray<FInventoryItem> Items;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void AddItem(USimpleInventoryItem* Item);

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void AddItemQuantity(USimpleInventoryItem* Item, int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void RemoveItem(USimpleInventoryItem* Item);

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void RemoveItemQuantity(USimpleInventoryItem* Item, int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  bool HasItem(USimpleInventoryItem* Item) const;

  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  int32 GetItemQuantity(USimpleInventoryItem* Item) const;
};
