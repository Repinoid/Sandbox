// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimerRate, true);
}

void ABaseGeometryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogBaseGeometry, Error, TEXT("Actor %s is dead"), *GetName());
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
}

void ABaseGeometryActor::PrintStringTypes() {
	FString WeaponsNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
	FString IsDeadStr = "Is dead = " + FString(IsDead ? "true" : "false");

	FString Stat = FString::Printf(TEXT("\n --- All Stats --- \n %s \n %s \n %s"), *WeaponsNumStr, *HealthStr, *IsDeadStr);

	//UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *Stat)
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Red, Stat, true, FVector2D(2.f, 2.f));
	}
}

void ABaseGeometryActor::PrintTransform() {

	FTransform Transform = GetActorTransform();

	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotation: %s"), *Rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale: %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Human transform: %s"), *Transform.ToHumanReadableString());
}

void ABaseGeometryActor::HandleMovement() {
	switch (GeometryData.MoveType) {
		case EMovementType::Sin: {
			FVector CurrentLocation = GetActorLocation();
			if (GetWorld()) {
				float Time = GetWorld()->GetTimeSeconds();
				CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
				SetActorLocation(CurrentLocation);
			}
			break;
		}
		case EMovementType::Static: {
			break;
		}
		default:
			break;
	}

}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	if (!BaseMesh) return;
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (DynMaterial) {

		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	if (++TimerCount <= GeometryData.MaxTimerCount) {
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else {
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}
