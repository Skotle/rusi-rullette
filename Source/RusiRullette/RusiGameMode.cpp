#include "RusiGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RusiGameWidget.h"
#include "RusiPlayerController.h"
#include "RusiRouletteBoard.h"
#include "Sound/SoundBase.h"

ARusiGameMode::ARusiGameMode()
{
	PlayerControllerClass = ARusiPlayerController::StaticClass();

	RevolveSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/revol.revol"));
	EmptySound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/none.none"));
	FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/fire.fire"));
}

void ARusiGameMode::BeginPlay()
{
	Super::BeginPlay();

	RouletteBoard = GetWorld()->SpawnActor<ARusiRouletteBoard>(ARusiRouletteBoard::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	if (RouletteBoard)
	{
		RouletteBoard->Configure(ChamberCount, PlayerCount);
		if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
		{
			Controller->SetViewTargetWithBlend(RouletteBoard);
		}
	}

	GameWidget = CreateWidget<URusiGameWidget>(GetWorld(), URusiGameWidget::StaticClass());
	if (GameWidget)
	{
		GameWidget->AddToViewport();
		GameWidget->BindGameMode(this);
	}

	ResetState();
	RefreshBoard();
	RefreshWidget();
}

void ARusiGameMode::StartGame(int32 InChamberCount, int32 InBulletCount, int32 InPlayerCount)
{
	ChamberCount = FMath::Clamp(InChamberCount, 2, 99);
	BulletCount = FMath::Clamp(InBulletCount, 1, ChamberCount);
	PlayerCount = FMath::Clamp(InPlayerCount, 1, 32);

	BulletChambers.Reset();
	FiredChambers.Reset();
	AlivePlayers.Reset();
	CurrentRoundIndex = 0;
	bIsPlaying = true;

	for (int32 Player = 1; Player <= PlayerCount; ++Player)
	{
		AlivePlayers.Add(Player);
	}

	while (BulletChambers.Num() < BulletCount)
	{
		BulletChambers.AddUnique(FMath::RandRange(1, ChamberCount));
	}

	if (RouletteBoard)
	{
		RouletteBoard->Configure(ChamberCount, PlayerCount);
	}

	PickCurrentPlayer();
	StatusText = FText::Format(NSLOCTEXT("Rusi", "Started", "The bargain cylinder screams. Player {0}, guess like you paid 1000 won."), FText::AsNumber(CurrentPlayer));
	ResultText = FText::GetEmpty();
	PlaySound(RevolveSound);
	RefreshBoard();
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
		FiredChambers.AddUnique(Chamber);
		PlaySound(FireSound);
	}
	else
	{
		PlaySound(EmptySound);
	}

	if (bCorrect)
	{
		ResultText = FText::Format(NSLOCTEXT("Rusi", "Correct", "Player {0} survives. Somehow, that was skill."), FText::AsNumber(CurrentPlayer));
	}
	else
	{
		ResultText = FText::Format(NSLOCTEXT("Rusi", "Wrong", "Player {0} gets folded into discount history."), FText::AsNumber(CurrentPlayer));
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
		FinishGame(NSLOCTEXT("Rusi", "AllOut", "Everyone is out. Refund window remains open."));
		return;
	}

	if (BulletChambers.IsEmpty())
	{
		FinishGame(NSLOCTEXT("Rusi", "NoBullets", "All bullets spent. The budget has vanished."));
		return;
	}

	if (CurrentRoundIndex >= ChamberCount)
	{
		FinishGame(NSLOCTEXT("Rusi", "NoChambers", "No chambers remain. This is legally an ending."));
		return;
	}

	PickCurrentPlayer();
	StatusText = FText::Format(NSLOCTEXT("Rusi", "NextTurn", "Player {0}, the cube crowd demands a guess."), FText::AsNumber(CurrentPlayer));
	RefreshBoard();
	RefreshWidget();
}

void ARusiGameMode::StopGame()
{
	FinishGame(NSLOCTEXT("Rusi", "Stopped", "Game stopped. Production value preserved."));
}

void ARusiGameMode::ResetState()
{
	BulletChambers.Reset();
	FiredChambers.Reset();
	AlivePlayers.Reset();
	CurrentRoundIndex = 0;
	CurrentPlayer = 1;
	bIsPlaying = false;
	StatusText = NSLOCTEXT("Rusi", "Ready", "Choose settings. Then unleash the 1000 won spectacle.");
	ResultText = FText::GetEmpty();
}

void ARusiGameMode::PickCurrentPlayer()
{
	CurrentPlayer = AlivePlayers.IsEmpty() ? 0 : AlivePlayers[FMath::RandRange(0, AlivePlayers.Num() - 1)];
}

void ARusiGameMode::FinishGame(const FText& FinishText)
{
	bIsPlaying = false;
	StatusText = FinishText;
	if (AlivePlayers.Num() > 0 && PlayerCount > 1)
	{
		ResultText = FText::Format(NSLOCTEXT("Rusi", "Survivors", "Survivors: {0}"), FText::AsNumber(AlivePlayers.Num()));
	}
	RefreshBoard();
	RefreshWidget();
}

void ARusiGameMode::RefreshWidget()
{
	if (GameWidget)
	{
		GameWidget->Refresh();
	}
}

void ARusiGameMode::RefreshBoard()
{
	if (RouletteBoard)
	{
		RouletteBoard->RefreshBoard(CurrentRoundIndex, FiredChambers, AlivePlayers, CurrentPlayer, bIsPlaying);
	}
}

void ARusiGameMode::PlaySound(USoundBase* Sound) const
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}
