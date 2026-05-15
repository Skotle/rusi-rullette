# Rusi Rullette

Unreal Engine C++ rebuild of the original Python roulette game.

## Original Python Flow

- Enter chamber count, bullet count, and player count.
- Bullet chamber positions are chosen randomly with no duplicates.
- Each round picks one living player at random.
- The player guesses `0` for empty or `1` for fire.
- A correct guess survives. A wrong guess removes the player in multiplayer.
- The game ends when every player is out, all bullets are spent, all chambers are used, or Stop is pressed.

## Unreal Version

Open `RusiRullette.uproject` in Unreal Engine 5.4 or newer.

Implemented files:

- `Source/RusiRullette/RusiGameMode.*`: game state and roulette rules.
- `Source/RusiRullette/RusiGameWidget.*`: runtime UI for settings, status, guesses, and results.
- `Source/RusiRullette/RusiPlayerController.*`: mouse/UI input mode.
- `Content/Audio/*.mp3`: copied from the Python version for import into Unreal.

## Audio

The C++ code looks for these imported Unreal sound assets:

- `/Game/Audio/revol`
- `/Game/Audio/none`
- `/Game/Audio/fire`

If Unreal does not automatically import the MP3 files, import the three files from `Content/Audio` in the editor and keep the asset names `revol`, `none`, and `fire`.
