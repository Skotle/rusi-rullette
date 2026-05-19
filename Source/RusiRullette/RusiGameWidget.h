#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RusiGameWidget.generated.h"

class ARusiGameMode;
class UButton;
class UEditableTextBox;
class UTextBlock;

UCLASS()
class RUSIRULLETTE_API URusiGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindGameMode(ARusiGameMode* InGameMode);
	void Refresh();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UPROPERTY()
	TObjectPtr<ARusiGameMode> GameMode;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> ChamberInput;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> BulletInput;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> PlayerInput;

	UPROPERTY()
	TObjectPtr<UTextBlock> StatusText;

	UPROPERTY()
	TObjectPtr<UTextBlock> OddsText;

	UPROPERTY()
	TObjectPtr<UTextBlock> AliveText;

	UPROPERTY()
	TObjectPtr<UTextBlock> ResultText;

	UPROPERTY()
	TObjectPtr<UButton> StartButton;

	UPROPERTY()
	TObjectPtr<UButton> GuessEmptyButton;

	UPROPERTY()
	TObjectPtr<UButton> GuessFireButton;

	UPROPERTY()
	TObjectPtr<UButton> StopButton;

	UFUNCTION()
	void HandleStartClicked();

	UFUNCTION()
	void HandleGuessEmptyClicked();

	UFUNCTION()
	void HandleGuessFireClicked();

	UFUNCTION()
	void HandleStopClicked();

	int32 ParseInt(UEditableTextBox* TextBox, int32 Fallback) const;
	FText JoinAlivePlayers() const;
};
