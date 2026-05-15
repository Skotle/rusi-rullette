#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RusiGameMode.generated.h"

class URusiGameWidget;
class USoundBase;

UCLASS()
class RUSIRULLETTE_API ARusiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARusiGameMode();

	virtual void BeginPlay() override;

	UFUNCTION()
	void StartGame(int32 InChamberCount, int32 InBulletCount, int32 InPlayerCount);

	UFUNCTION()
	void MakeGuess(bool bGuessFire);

	UFUNCTION()
	void StopGame();

	const TArray<int32>& GetAlivePlayers() const { return AlivePlayers; }
	const TArray<int32>& GetBulletChambers() const { return BulletChambers; }
	int32 GetCurrentRoundIndex() const { return CurrentRoundIndex; }
	int32 GetChamberCount() const { return ChamberCount; }
	int32 GetCurrentPlayer() const { return CurrentPlayer; }
	bool IsPlaying() const { return bIsPlaying; }
	FText GetStatusText() const { return StatusText; }
	FText GetResultText() const { return ResultText; }

private:
	UPROPERTY()
	TObjectPtr<URusiGameWidget> GameWidget;

	UPROPERTY()
	TObjectPtr<USoundBase> RevolveSound;

	UPROPERTY()
	TObjectPtr<USoundBase> EmptySound;

	UPROPERTY()
	TObjectPtr<USoundBase> FireSound;

	TArray<int32> BulletChambers;
	TArray<int32> AlivePlayers;
	int32 ChamberCount = 6;
	int32 BulletCount = 1;
	int32 PlayerCount = 1;
	int32 CurrentRoundIndex = 0;
	int32 CurrentPlayer = 1;
	bool bIsPlaying = false;
	FText StatusText;
	FText ResultText;

	void ResetState();
	void PickCurrentPlayer();
	void FinishGame(const FText& FinishText);
	void RefreshWidget();
	void PlaySound(USoundBase* Sound) const;
};
