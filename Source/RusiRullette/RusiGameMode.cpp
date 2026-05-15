#include "RusiGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RusiGameWidget.h"
#include "RusiPlayerController.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

ARusiGameMode::ARusiGameMode()
{
	PlayerControllerClass = ARusiPlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USoundBase> RevolveFinder(TEXT("/Game/Audio/revol.revol"));
	static ConstructorHelpers::FObjectFinder<USoundBase> EmptyFinder(TEXT("/Game/Audio/none.none"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireFinder(TEXT("/Game/Audio/fire.fire"));

	if (RevolveFinder.Succeeded())
	{
		RevolveSound = RevolveFinder.Object;
	}
	if (EmptyFinder.Succeeded())
	{
		EmptySound = EmptyFinder.Object;
	}
	if (FireFinder.Succeeded())
	{
		FireSound = FireFinder.Object;
	}
}

void ARusiGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameWidget = CreateWidget<URusiGameWidget>(GetWorld(), URusiGameWidget::StaticClass());
	if (GameWidget)
	{
		GameWidget->AddToViewport();
		GameWidget->BindGameMode(this);
	}

	ResetState();
	RefreshWidget();
}

void ARusiGameMode::StartGame(int32 InChamberCount, int32 InBulletCount, int32 InPlayerCount)
{
	ChamberCount = FMath::Clamp(InChamberCount, 2, 99);
	BulletCount = FMath::Clamp(InBulletCount, 1, ChamberCount);
	PlayerCount = FMath::Clamp(InPlayerCount, 1, 32);

	BulletChambers.Reset();
	AlivePlayers.Reset();
	CurrentRoundIndex = 0;
	bIsPlaying = true;

	for (int32 Player = 1; Player <= PlayerCount; ++Player)
	{
		AlivePlayers.Add(Player);
	}

	while (BulletChambers.Num() < BulletCount)
	{
		const int32 Chamber = FMath::RandRange(1, ChamberCount);
		BulletChambers.AddUnique(Chamber);
	}

	PickCurrentPlayer();
	StatusText = FText::Format(
		NSLOCTEXT("Rusi", "Started", "Cylinder loaded. Player {0}, make your guess."),
		FText::AsNumber(CurrentPlayer));
	ResultText = FText::GetEmpty();
	PlaySound(RevolveSound);
	RefreshWidget();
}

void ARusiGameMode::MakeGuess(bool bGuessFire)
{
	if (!bIsPlaying || AlivePlayers.IsEmpty())
	{
		return;
	}

	const int32 Chamber = CurrentRoundIndex + 1;
	const bool bActualFire = BulletChambers.Contains(Chamber);
	const bool bCorrect = bGuessFire == bActualFire;

	if (bActualFire)
	{
		BulletChambers.Remove(Chamber);
		PlaySound(FireSound);
	}
	else
	{
		PlaySound(EmptySound);
	}

	if (bCorrect)
	{
		ResultText = FText::Format(
			NSLOCTEXT("Rusi", "Correct", "Player {0} survives. Correct guess."),
			FText::AsNumber(CurrentPlayer));
	}
	else
	{
		ResultText = FText::Format(
			NSLOCTEXT("Rusi", "Wrong", "Player {0} is out. Wrong guess."),
			FText::AsNumber(CurrentPlayer));
		if (PlayerCount > 1)
		{
			AlivePlayers.Remove(CurrentPlayer);
		}
	}

	++CurrentRoundIndex;

	if (PlayerCount == 1 && (!bCorrect || bActualFire))
	{
		FinishGame(ResultText);
		return;
	}

	if (AlivePlayers.IsEmpty())
	{
		FinishGame(NSLOCTEXT("Rusi", "AllOut", "Everyone is out."));
		return;
	}

	if (BulletChambers.IsEmpty())
	{
		FinishGame(NSLOCTEXT("Rusi", "NoBullets", "All bullets have been spent."));
		return;
	}

	if (CurrentRoundIndex >= ChamberCount)
	{
		FinishGame(NSLOCTEXT("Rusi", "NoChambers", "No chambers remain."));
		return;
	}

	PickCurrentPlayer();
	StatusText = FText::Format(
		NSLOCTEXT("Rusi", "NextTurn", "Player {0}, make your guess."),
		FText::AsNumber(CurrentPlayer));
	RefreshWidget();
}

void ARusiGameMode::StopGame()
{
	FinishGame(NSLOCTEXT("Rusi", "Stopped", "Game stopped."));
}

void ARusiGameMode::ResetState()
{
	BulletChambers.Reset();
	AlivePlayers.Reset();
	CurrentRoundIndex = 0;
	CurrentPlayer = 1;
	bIsPlaying = false;
	StatusText = NSLOCTEXT("Rusi", "Ready", "Choose settings, then start.");
	ResultText = FText::GetEmpty();
}

void ARusiGameMode::PickCurrentPlayer()
{
	if (AlivePlayers.IsEmpty())
	{
		CurrentPlayer = 0;
		return;
	}

	CurrentPlayer = AlivePlayers[FMath::RandRange(0, AlivePlayers.Num() - 1)];
}

void ARusiGameMode::FinishGame(const FText& FinishText)
{
	bIsPlaying = false;
	StatusText = FinishText;
	if (AlivePlayers.Num() > 0 && PlayerCount > 1)
	{
		ResultText = FText::Format(
			NSLOCTEXT("Rusi", "Survivors", "Survivors: {0}"),
			FText::AsNumber(AlivePlayers.Num()));
	}
	RefreshWidget();
}

void ARusiGameMode::RefreshWidget()
{
	if (GameWidget)
	{
		GameWidget->Refresh();
	}
}

void ARusiGameMode::PlaySound(USoundBase* Sound) const
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}
