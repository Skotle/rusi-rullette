# Rusi Rullette: 1000 Won Edition

Unreal Engine 5.7 graphical rebuild of the original Python roulette game, now shaped like a suspicious bargain-bin Steam release.

## Play

Open `RusiRullette.uproject` with Unreal Engine 5.7.x, rebuild modules when prompted, then press Play.

The game shows a very dramatic and deeply unnecessary 3D roulette arena:

- Gold chamber: current chamber.
- Dark chamber: already spent empty chamber.
- Red chamber: fired chamber.
- Future chambers stay blue-gray.
- Cyan cubes: living players.
- Gold cube: current player.
- Flat black cubes: eliminated players.
- Random neon junk: production value.

The UI keeps the original Python flow:

1. Set chamber count, bullet count, and player count.
2. Press Start.
3. Each turn, guess `0: Empty` or `1: Fire`.
4. Wrong guesses eliminate the current player in multiplayer.

## Steam Capsule Pitch

One thousand won. One cylinder. Several cubes with no survival instinct.

Features:

- Cheap neon stage.
- Wobbling camera.
- Cubes pretending to be people.
- Original Python roulette rules.
- Sound effects from the prototype.
- Exactly the amount of dignity implied by the price.

## Audio

The original MP3 files are copied into `Content/Audio`.

If Unreal does not auto-import them, import these files manually and keep the asset names:

- `revol`
- `none`
- `fire`
