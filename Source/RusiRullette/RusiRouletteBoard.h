#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RusiRouletteBoard.generated.h"

class UCameraComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UPointLightComponent;
class UStaticMesh;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class RUSIRULLETTE_API ARusiRouletteBoard : public AActor
{
	GENERATED_BODY()

public:
	ARusiRouletteBoard();

	virtual void Tick(float DeltaSeconds) override;

	void Configure(int32 InChamberCount, int32 InPlayerCount);
	void RefreshBoard(int32 CurrentRoundIndex, const TArray<int32>& FiredChambers, const TArray<int32>& AlivePlayers, int32 CurrentPlayer, bool bGameActive);
	UCameraComponent* GetCamera() const { return Camera; }

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> CylinderMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PointerMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BackdropMesh;

	UPROPERTY()
	TObjectPtr<UPointLightComponent> KeyLight;

	UPROPERTY()
	TObjectPtr<UPointLightComponent> FillLight;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<UTextRenderComponent> TitleText;

	UPROPERTY()
	TObjectPtr<UTextRenderComponent> PriceText;

	UPROPERTY()
	TObjectPtr<UTextRenderComponent> PromptText;

	UPROPERTY()
	TObjectPtr<UStaticMesh> SphereAsset;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeAsset;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> ChamberMeshes;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> ChamberMaterials;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> PlayerMeshes;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> PlayerMaterials;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> JunkMeshes;

	int32 ChamberCount = 0;
	int32 PlayerCount = 0;
	int32 HighlightPlayer = 0;
	float RunningTime = 0.0f;
	bool bAttractMode = true;

	UMaterialInstanceDynamic* MakeMaterial(FName Name, const FLinearColor& Color);
	void SetMeshColor(int32 Index, const FLinearColor& Color);
	void BuildChambers();
	void BuildPlayers();
	void BuildJunk();
	void BuildStageTrim();
};
