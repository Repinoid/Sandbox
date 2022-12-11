// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Sandbox/Public/BaseGeometryActor.h>
#include "Engine/World.h"
#include "GeometryHubActor.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub, All, All);

USTRUCT(BlueprintType)
struct FGeometryPayload {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	UPROPERTY(EditAnywhere)
	FGeometryData Data;

	UPROPERTY(EditAnywhere)
	FTransform InitialTransform;
};

UCLASS()
class SANDBOX_API AGeometryHubActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeometryHubActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FGeometryPayload> GeometryPayloads;

	void OnColorChanged(const FLinearColor& Color, const FString& Name);
	void OnTimerFinished(AActor* Actor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
