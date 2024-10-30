# Spatial 2D

The `Spatial2D` module provides a 2D spatial representation with basic movement logic. It is used to manage the position, rotation, and scale of 2D objects in the game world.

## Components

- **Position2D** (*x: float, y: float*)
- **Size2D** (*width: float, height: float*)
- **Rotation** (*angle: float*)
- **Velocity2D** (*x: float, y: float*)
- **Acceleration2D** (*x: float, y: float*)

## Systems

- **ApplyVelocity** (*Position2D, Velocity2D*)
- **ApplyAcceleration** (*Velocity2D, Acceleration2D*)

## Aspects

- **StaticEntity** (*Position2D, Rotation*)
- **DynamicEntity** (*+StaticEntity, Velocity2D, Acceleration2D*)
