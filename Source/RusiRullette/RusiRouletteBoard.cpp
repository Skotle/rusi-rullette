#include "RusiRouletteBoard.h"

#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	const FLinearColor FutureColor(0.16f, 0.20f, 0.24f, 1.0f);
	const FLinearColor CurrentColor(1.00f, 0.72f, 0.18f, 1.0f);
	const FLinearColor EmptySpentColor(0.05f, 0.07f, 0.09f, 1.0f);
	const FLinearColor FiredColor(0.95f, 0.08f, 0.05f, 1.0f);
	const FLinearColor AliveColor(0.12f, 0.78f, 0.95f, 1.0f);
	const FLinearColor ActivePlayerColor(1.00f, 0.94f, 0.10f, 1.0f);
	const FLinearColor OutColor(0.13f, 0.13f, 0.13f, 1.0f);
}

ARusiRouletteBoard::ARusiRouletteBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	UStaticMesh* CylinderAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	SphereAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	CubeAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	CylinderMesh->SetupAttachment(SceneRoot);
	if (CylinderAsset)
	{
		CylinderMesh->SetStaticMesh(CylinderAsset);
	}
	CylinderMesh->SetRelativeScale3D(FVector(5.2f, 5.2f, 0.30f));
	CylinderMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 42.0f));
	CylinderMesh->SetMaterial(0, MakeMaterial(TEXT("CylinderMaterial"), FLinearColor(0.07f, 0.02f, 0.09f, 1.0f)));

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	FloorMesh->SetupAttachment(SceneRoot);
	if (CubeAsset)
	{
		FloorMesh->SetStaticMesh(CubeAsset);
	}
	FloorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -34.0f));
	FloorMesh->SetRelativeScale3D(FVector(18.0f, 18.0f, 0.08f));
	FloorMesh->SetMaterial(0, MakeMaterial(TEXT("FloorMaterial"), FLinearColor(0.025f, 0.02f, 0.05f, 1.0f)));

	BackdropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Backdrop"));
	BackdropMesh->SetupAttachment(SceneRoot);
	if (CubeAsset)
	{
		BackdropMesh->SetStaticMesh(CubeAsset);
	}
	BackdropMesh->SetRelativeLocation(FVector(0.0f, 520.0f, 270.0f));
	BackdropMesh->SetRelativeScale3D(FVector(12.0f, 0.12f, 5.8f));
	BackdropMesh->SetMaterial(0, MakeMaterial(TEXT("BackdropMaterial"), FLinearColor(0.08f, 0.0f, 0.11f, 1.0f)));

	PointerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pointer"));
	PointerMesh->SetupAttachment(SceneRoot);
	if (CubeAsset)
	{
		PointerMesh->SetStaticMesh(CubeAsset);
	}
	PointerMesh->SetRelativeLocation(FVector(0.0f, -360.0f, 85.0f));
	PointerMesh->SetRelativeScale3D(FVector(0.20f, 1.30f, 0.08f));
	PointerMesh->SetMaterial(0, MakeMaterial(TEXT("PointerMaterial"), FLinearColor(0.15f, 1.0f, 0.32f, 1.0f)));

	KeyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("KeyLight"));
	KeyLight->SetupAttachment(SceneRoot);
	KeyLight->SetRelativeLocation(FVector(-280.0f, -420.0f, 520.0f));
	KeyLight->SetLightColor(FLinearColor(0.20f, 0.95f, 1.0f));
	KeyLight->SetIntensity(7800.0f);
	KeyLight->SetAttenuationRadius(1600.0f);

	FillLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("FillLight"));
	FillLight->SetupAttachment(SceneRoot);
	FillLight->SetRelativeLocation(FVector(420.0f, -260.0f, 320.0f));
	FillLight->SetLightColor(FLinearColor(1.0f, 0.08f, 0.50f));
	FillLight->SetIntensity(5200.0f);
	FillLight->SetAttenuationRadius(1400.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SceneRoot);
	Camera->SetRelativeLocation(FVector(0.0f, -880.0f, 620.0f));
	Camera->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));
	Camera->SetFieldOfView(48.0f);
	Camera->bAutoActivate = true;

	TitleText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitleText"));
	TitleText->SetupAttachment(SceneRoot);
	TitleText->SetRelativeLocation(FVector(-450.0f, 455.0f, 520.0f));
	TitleText->SetRelativeRotation(FRotator(70.0f, 0.0f, 0.0f));
	TitleText->SetHorizontalAlignment(EHTA_Center);
	TitleText->SetTextRenderColor(FColor(255, 230, 0));
	TitleText->SetWorldSize(72.0f);
	TitleText->SetText(NSLOCTEXT("Rusi", "WorldTitle", "RUSI RULLETTE"));

	PriceText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PriceText"));
	PriceText->SetupAttachment(SceneRoot);
	PriceText->SetRelativeLocation(FVector(110.0f, 455.0f, 410.0f));
	PriceText->SetRelativeRotation(FRotator(70.0f, 0.0f, 0.0f));
	PriceText->SetHorizontalAlignment(EHTA_Center);
	PriceText->SetTextRenderColor(FColor(0, 255, 160));
	PriceText->SetWorldSize(46.0f);
	PriceText->SetText(NSLOCTEXT("Rusi", "WorldPrice", "1000 WON EDITION"));

	PromptText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptText"));
	PromptText->SetupAttachment(SceneRoot);
	PromptText->SetRelativeLocation(FVector(-230.0f, -350.0f, 155.0f));
	PromptText->SetRelativeRotation(FRotator(62.0f, 0.0f, 0.0f));
	PromptText->SetHorizontalAlignment(EHTA_Center);
	PromptText->SetTextRenderColor(FColor(255, 90, 210));
	PromptText->SetWorldSize(34.0f);
	PromptText->SetText(NSLOCTEXT("Rusi", "WorldPrompt", "PRESS START TO WASTE VALUE"));
}

void ARusiRouletteBoard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RunningTime += DeltaSeconds;
	const float Wobble = FMath::Sin(RunningTime * 4.7f) * 1.4f;
	Camera->SetRelativeRotation(FRotator(-35.0f + Wobble, FMath::Sin(RunningTime * 1.9f) * 2.0f, 0.0f));
	Camera->SetFieldOfView(bAttractMode ? 54.0f + FMath::Sin(RunningTime * 1.2f) * 2.0f : 48.0f);
	CylinderMesh->AddRelativeRotation(FRotator(0.0f, (bAttractMode ? 42.0f : 12.0f) * DeltaSeconds, 0.0f));
	PointerMesh->SetRelativeScale3D(FVector(0.20f + FMath::Abs(FMath::Sin(RunningTime * 5.0f)) * 0.08f, 1.30f, 0.08f));

	if (TitleText)
	{
		TitleText->SetRelativeLocation(FVector(-450.0f, 455.0f, 520.0f + FMath::Sin(RunningTime * 2.0f) * 16.0f));
	}
	if (PromptText)
	{
		PromptText->SetVisibility(bAttractMode);
		PromptText->SetWorldSize(34.0f + FMath::Abs(FMath::Sin(RunningTime * 4.0f)) * 8.0f);
	}

	for (int32 Index = 0; Index < PlayerMeshes.Num(); ++Index)
	{
		if (!PlayerMeshes[Index])
		{
			continue;
		}

		const bool bIsActive = (Index + 1) == HighlightPlayer;
		const float Bounce = bIsActive ? FMath::Abs(FMath::Sin(RunningTime * 9.0f)) * 42.0f : FMath::Sin(RunningTime * 2.0f + Index) * 6.0f;
		FVector Location = PlayerMeshes[Index]->GetRelativeLocation();
		Location.Z = (Location.Z < -20.0f) ? Location.Z : 92.0f + Bounce;
		PlayerMeshes[Index]->SetRelativeLocation(Location);
		PlayerMeshes[Index]->AddRelativeRotation(FRotator(0.0f, bIsActive ? 260.0f * DeltaSeconds : 35.0f * DeltaSeconds, 0.0f));
	}

	for (int32 Index = 0; Index < JunkMeshes.Num(); ++Index)
	{
		if (JunkMeshes[Index])
		{
			JunkMeshes[Index]->AddRelativeRotation(FRotator(11.0f * DeltaSeconds, 23.0f * DeltaSeconds, 17.0f * DeltaSeconds));
		}
	}
}

void ARusiRouletteBoard::Configure(int32 InChamberCount, int32 InPlayerCount)
{
	ChamberCount = FMath::Clamp(InChamberCount, 2, 99);
	PlayerCount = FMath::Clamp(InPlayerCount, 1, 32);

	for (UStaticMeshComponent* Mesh : ChamberMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	ChamberMeshes.Reset();
	ChamberMaterials.Reset();

	for (UStaticMeshComponent* Mesh : PlayerMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	PlayerMeshes.Reset();
	PlayerMaterials.Reset();

	for (UStaticMeshComponent* Mesh : JunkMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	JunkMeshes.Reset();

	BuildChambers();
	BuildPlayers();
	BuildJunk();
	BuildStageTrim();
}

void ARusiRouletteBoard::BuildChambers()
{
	const float Radius = 250.0f;
	for (int32 Index = 0; Index < ChamberCount; ++Index)
	{
		const float Angle = 2.0f * PI * static_cast<float>(Index) / static_cast<float>(ChamberCount);
		const FVector Location(FMath::Sin(Angle) * Radius, -FMath::Cos(Angle) * Radius, 60.0f);

		UStaticMeshComponent* ChamberMesh = NewObject<UStaticMeshComponent>(this);
		ChamberMesh->RegisterComponent();
		ChamberMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		ChamberMesh->SetRelativeLocation(Location);
		ChamberMesh->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.42f));
		if (SphereAsset)
		{
			ChamberMesh->SetStaticMesh(SphereAsset);
		}

		UMaterialInstanceDynamic* Material = MakeMaterial(*FString::Printf(TEXT("Chamber_%d"), Index), FutureColor);
		ChamberMesh->SetMaterial(0, Material);
		ChamberMeshes.Add(ChamberMesh);
		ChamberMaterials.Add(Material);
	}
}

void ARusiRouletteBoard::BuildPlayers()
{
	const float Radius = 430.0f;
	for (int32 Index = 0; Index < PlayerCount; ++Index)
	{
		const float Angle = 2.0f * PI * static_cast<float>(Index) / static_cast<float>(PlayerCount);
		const FVector Location(FMath::Sin(Angle) * Radius, -FMath::Cos(Angle) * Radius, 92.0f);

		UStaticMeshComponent* PlayerMesh = NewObject<UStaticMeshComponent>(this);
		PlayerMesh->RegisterComponent();
		PlayerMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		PlayerMesh->SetRelativeLocation(Location);
		PlayerMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 1.10f));
		if (CubeAsset)
		{
			PlayerMesh->SetStaticMesh(CubeAsset);
		}

		UMaterialInstanceDynamic* Material = MakeMaterial(*FString::Printf(TEXT("Player_%d"), Index), AliveColor);
		PlayerMesh->SetMaterial(0, Material);
		PlayerMeshes.Add(PlayerMesh);
		PlayerMaterials.Add(Material);
	}
}

void ARusiRouletteBoard::BuildJunk()
{
	for (int32 Index = 0; Index < 14; ++Index)
	{
		const float Angle = 2.0f * PI * static_cast<float>(Index) / 14.0f;
		const float Radius = 650.0f + static_cast<float>((Index % 3) * 45);
		UStaticMeshComponent* JunkMesh = NewObject<UStaticMeshComponent>(this);
		JunkMesh->RegisterComponent();
		JunkMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		JunkMesh->SetRelativeLocation(FVector(FMath::Sin(Angle) * Radius, -FMath::Cos(Angle) * Radius, 20.0f + Index * 4.0f));
		JunkMesh->SetRelativeRotation(FRotator(Index * 13.0f, Index * 41.0f, Index * 7.0f));
		JunkMesh->SetRelativeScale3D(FVector(0.18f + (Index % 4) * 0.05f, 0.18f, 0.90f + (Index % 5) * 0.25f));
		if (CubeAsset)
		{
			JunkMesh->SetStaticMesh(CubeAsset);
		}
		JunkMesh->SetMaterial(0, MakeMaterial(*FString::Printf(TEXT("Junk_%d"), Index), FLinearColor::MakeRandomColor()));
		JunkMeshes.Add(JunkMesh);
	}
}

void ARusiRouletteBoard::BuildStageTrim()
{
	for (int32 Index = 0; Index < 10; ++Index)
	{
		const float X = -560.0f + static_cast<float>(Index) * 124.0f;
		UStaticMeshComponent* TrimMesh = NewObject<UStaticMeshComponent>(this);
		TrimMesh->RegisterComponent();
		TrimMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		TrimMesh->SetRelativeLocation(FVector(X, 500.0f, 56.0f + (Index % 2) * 40.0f));
		TrimMesh->SetRelativeScale3D(FVector(0.45f, 0.25f, 0.45f));
		if (CubeAsset)
		{
			TrimMesh->SetStaticMesh(CubeAsset);
		}
		TrimMesh->SetMaterial(0, MakeMaterial(*FString::Printf(TEXT("Trim_%d"), Index), Index % 2 == 0 ? FLinearColor(1.0f, 0.0f, 0.45f, 1.0f) : FLinearColor(0.0f, 0.85f, 1.0f, 1.0f)));
		JunkMeshes.Add(TrimMesh);
	}
}

void ARusiRouletteBoard::RefreshBoard(int32 CurrentRoundIndex, const TArray<int32>& FiredChambers, const TArray<int32>& AlivePlayers, int32 CurrentPlayer, bool bGameActive)
{
	bAttractMode = !bGameActive;
	HighlightPlayer = CurrentPlayer;
	if (PriceText)
	{
		PriceText->SetVisibility(bAttractMode);
	}
	if (PromptText)
	{
		PromptText->SetVisibility(bAttractMode);
	}

	for (int32 Index = 0; Index < ChamberMeshes.Num(); ++Index)
	{
		const int32 ChamberNumber = Index + 1;
		if (FiredChambers.Contains(ChamberNumber))
		{
			SetMeshColor(Index, FiredColor);
		}
		else if (Index < CurrentRoundIndex)
		{
			SetMeshColor(Index, EmptySpentColor);
		}
		else if (Index == CurrentRoundIndex)
		{
			SetMeshColor(Index, CurrentColor);
		}
		else
		{
			SetMeshColor(Index, FutureColor);
		}
	}

	for (int32 Index = 0; Index < PlayerMeshes.Num(); ++Index)
	{
		const int32 PlayerNumber = Index + 1;
		const bool bAlive = bAttractMode || AlivePlayers.Contains(PlayerNumber);
		const bool bActive = (bAttractMode && Index == 0) || (PlayerNumber == CurrentPlayer && bAlive);
		if (PlayerMaterials.IsValidIndex(Index) && PlayerMaterials[Index])
		{
			PlayerMaterials[Index]->SetVectorParameterValue(TEXT("Color"), bActive ? ActivePlayerColor : (bAlive ? AliveColor : OutColor));
		}
		if (PlayerMeshes[Index])
		{
			FVector Location = PlayerMeshes[Index]->GetRelativeLocation();
			Location.Z = bAlive ? FMath::Max(Location.Z, 92.0f) : -46.0f;
			PlayerMeshes[Index]->SetRelativeLocation(Location);
			PlayerMeshes[Index]->SetRelativeScale3D(bActive ? FVector(0.75f, 0.75f, 1.35f) : FVector(0.55f, 0.55f, bAlive ? 1.10f : 0.20f));
		}
	}

	const float Angle = 2.0f * PI * static_cast<float>(CurrentRoundIndex) / static_cast<float>(FMath::Max(1, ChamberCount));
	CylinderMesh->SetRelativeRotation(FRotator(0.0f, FMath::RadiansToDegrees(Angle), 0.0f));
}

UMaterialInstanceDynamic* ARusiRouletteBoard::MakeMaterial(FName Name, const FLinearColor& Color)
{
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, this, Name);
	if (Material)
	{
		Material->SetVectorParameterValue(TEXT("Color"), Color);
	}
	return Material;
}

void ARusiRouletteBoard::SetMeshColor(int32 Index, const FLinearColor& Color)
{
	if (ChamberMaterials.IsValidIndex(Index) && ChamberMaterials[Index])
	{
		ChamberMaterials[Index]->SetVectorParameterValue(TEXT("Color"), Color);
	}
}
