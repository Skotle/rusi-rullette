#include "RusiGameWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "RusiGameMode.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

void URusiGameWidget::BindGameMode(ARusiGameMode* InGameMode)
{
	GameMode = InGameMode;
	Refresh();
}

TSharedRef<SWidget> URusiGameWidget::RebuildWidget()
{
	ChamberInput = NewObject<UEditableTextBox>(this);
	ChamberInput->SetText(FText::FromString(TEXT("6")));
	BulletInput = NewObject<UEditableTextBox>(this);
	BulletInput->SetText(FText::FromString(TEXT("1")));
	PlayerInput = NewObject<UEditableTextBox>(this);
	PlayerInput->SetText(FText::FromString(TEXT("4")));

	StatusText = NewObject<UTextBlock>(this);
	OddsText = NewObject<UTextBlock>(this);
	AliveText = NewObject<UTextBlock>(this);
	ResultText = NewObject<UTextBlock>(this);

	StartButton = NewObject<UButton>(this);
	GuessEmptyButton = NewObject<UButton>(this);
	GuessFireButton = NewObject<UButton>(this);
	StopButton = NewObject<UButton>(this);

	UTextBlock* StartLabel = NewObject<UTextBlock>(this);
	StartLabel->SetText(NSLOCTEXT("Rusi", "Start", "START THE BAD IDEA"));
	StartLabel->SetJustification(ETextJustify::Center);
	StartButton->AddChild(StartLabel);

	UTextBlock* StopLabel = NewObject<UTextBlock>(this);
	StopLabel->SetText(NSLOCTEXT("Rusi", "Stop", "STOP"));
	StopLabel->SetJustification(ETextJustify::Center);
	StopButton->AddChild(StopLabel);

	UTextBlock* EmptyLabel = NewObject<UTextBlock>(this);
	EmptyLabel->SetText(NSLOCTEXT("Rusi", "GuessEmpty", "0: EMPTY, TRUST ME"));
	EmptyLabel->SetJustification(ETextJustify::Center);
	GuessEmptyButton->AddChild(EmptyLabel);

	UTextBlock* FireLabel = NewObject<UTextBlock>(this);
	FireLabel->SetText(NSLOCTEXT("Rusi", "GuessFire", "1: FIRE, OBVIOUSLY"));
	FireLabel->SetJustification(ETextJustify::Center);
	GuessFireButton->AddChild(FireLabel);

	StartButton->OnClicked.AddDynamic(this, &URusiGameWidget::HandleStartClicked);
	GuessEmptyButton->OnClicked.AddDynamic(this, &URusiGameWidget::HandleGuessEmptyClicked);
	GuessFireButton->OnClicked.AddDynamic(this, &URusiGameWidget::HandleGuessFireClicked);
	StopButton->OnClicked.AddDynamic(this, &URusiGameWidget::HandleStopClicked);

	TSharedRef<SVerticalBox> Panel = SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
		[
			SNew(STextBlock).Text(NSLOCTEXT("Rusi", "Title", "Rusi Rullette: 1000 Won Edition")).Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 28))
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
		[
			SNew(STextBlock).Text(NSLOCTEXT("Rusi", "SteamLine", "Main menu included. Value doubled."))
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
		[
			SNew(SUniformGridPanel).SlotPadding(FMargin(5))
			+ SUniformGridPanel::Slot(0, 0)[SNew(STextBlock).Text(NSLOCTEXT("Rusi", "Chambers", "Chambers"))]
			+ SUniformGridPanel::Slot(1, 0)[ChamberInput->TakeWidget()]
			+ SUniformGridPanel::Slot(0, 1)[SNew(STextBlock).Text(NSLOCTEXT("Rusi", "Bullets", "Bullets"))]
			+ SUniformGridPanel::Slot(1, 1)[BulletInput->TakeWidget()]
			+ SUniformGridPanel::Slot(0, 2)[SNew(STextBlock).Text(NSLOCTEXT("Rusi", "Players", "Players"))]
			+ SUniformGridPanel::Slot(1, 2)[PlayerInput->TakeWidget()]
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(0, 0, 8, 0)[StartButton->TakeWidget()]
			+ SHorizontalBox::Slot().AutoWidth()[StopButton->TakeWidget()]
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)[StatusText->TakeWidget()]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)[OddsText->TakeWidget()]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)[AliveText->TakeWidget()]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(0, 0, 8, 0)[GuessEmptyButton->TakeWidget()]
			+ SHorizontalBox::Slot().AutoWidth()[GuessFireButton->TakeWidget()]
		]
		+ SVerticalBox::Slot().AutoHeight()[ResultText->TakeWidget()];

	return SNew(SBorder)
		.Padding(24)
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		[
			SNew(SBox).WidthOverride(460)[Panel]
		];
}

void URusiGameWidget::Refresh()
{
	if (!GameMode || !StatusText || !OddsText || !AliveText || !ResultText)
	{
		return;
	}

	StatusText->SetText(GameMode->GetStatusText());
	ResultText->SetText(GameMode->GetResultText());
	AliveText->SetText(JoinAlivePlayers());

	const int32 RemainingChambers = FMath::Max(1, GameMode->GetChamberCount() - GameMode->GetCurrentRoundIndex());
	const float FireOdds = static_cast<float>(GameMode->GetBulletChambers().Num()) / static_cast<float>(RemainingChambers) * 100.0f;
	OddsText->SetText(FText::Format(
		NSLOCTEXT("Rusi", "Odds", "Remaining danger: {0} / Remaining excuses: {1} / Bad idea odds: {2}%"),
		FText::AsNumber(GameMode->GetBulletChambers().Num()),
		FText::AsNumber(RemainingChambers),
		FText::AsNumber(FireOdds)));

	const bool bPlaying = GameMode->IsPlaying();
	GuessEmptyButton->SetIsEnabled(bPlaying);
	GuessFireButton->SetIsEnabled(bPlaying);
	StopButton->SetIsEnabled(bPlaying);
	StartButton->SetIsEnabled(!bPlaying);
}

void URusiGameWidget::HandleStartClicked()
{
	if (GameMode)
	{
		GameMode->StartGame(ParseInt(ChamberInput, 6), ParseInt(BulletInput, 1), ParseInt(PlayerInput, 2));
	}
}

void URusiGameWidget::HandleGuessEmptyClicked()
{
	if (GameMode)
	{
		GameMode->MakeGuess(false);
	}
}

void URusiGameWidget::HandleGuessFireClicked()
{
	if (GameMode)
	{
		GameMode->MakeGuess(true);
	}
}

void URusiGameWidget::HandleStopClicked()
{
	if (GameMode)
	{
		GameMode->StopGame();
	}
}

int32 URusiGameWidget::ParseInt(UEditableTextBox* TextBox, int32 Fallback) const
{
	if (!TextBox)
	{
		return Fallback;
	}

	const FString Text = TextBox->GetText().ToString();
	return Text.IsNumeric() ? FCString::Atoi(*Text) : Fallback;
}

FText URusiGameWidget::JoinAlivePlayers() const
{
	if (!GameMode || GameMode->GetAlivePlayers().IsEmpty())
	{
		return NSLOCTEXT("Rusi", "NoAlive", "Survivors: none");
	}

	TArray<FString> Parts;
	for (int32 Player : GameMode->GetAlivePlayers())
	{
		Parts.Add(FString::Printf(TEXT("Player %d"), Player));
	}

	return FText::FromString(FString::Printf(TEXT("Survivors: %s"), *FString::Join(Parts, TEXT(", "))));
}
