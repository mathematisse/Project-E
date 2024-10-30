# Render

The `Render` module is responsible for drawing sprites and sprite sheets to the screen. It also provides tools for animating sprites.

## Components

- **Sprite** (*textureId: size_t*)
- **AnimatedSprite** (*textureId: size_t, nbFrame: uint8_t, animationSpeed: float*)

## Systems

- **DebugDrawSprite** (*Position2D, Color, Size2D*)
- **DrawSprite** (*Position2D, Size2D, Rotation, Sprite*)
- **DrawAnimatedSprite** (*Position2D, Size2D, Rotation, AnimatedSprite*)
- **SpriteAnimation** (*AnimatedSprite, Timer*)

## Aspects

- **SpriteEntity** (*Sprite, Size*)
- **AnimatedSpriteEntity** (*AnimatedSprite, Timer, Size*)
