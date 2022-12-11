// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World) {
		for (const FGeometryPayload Payload : GeometryPayloads) {
			ABaseGeometryActor* Geometry = World->SpawnActorDeferred<ABaseGeometryActor>(Payload.GeometryClass, Payload.InitialTransform);

			if (Geometry) {
				Geometry->SetGeometryData(Payload.Data);
				Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
				Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);
				Geometry->FinishSpawning(Payload.InitialTransform);
			}
		}
	}
}

void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor name: %s Color: %s"), *Name, *Color.ToString());
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if(!Actor) return;
	UE_LOG(LogGeometryHub, Error, TEXT("Timer is finished: %s"), *Actor->GetName());

	ABaseGeometryActor* Geometry = Cast<ABaseGeometryActor>(Actor);

	if(!Geometry) return;

	UE_LOG(LogGeometryHub, Display, TEXT("Cast was succed, amplitude %f"), Geometry->GetGeometryData().Amplitude);

	Geometry->Destroy();
	//Geometry->SetLifeSpan(2.0f)
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



