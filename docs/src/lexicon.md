# Lexicon

this page lists all relevant terminologies regarding the project.

## ECS


| Term      | Definition                                                                            |
| --------- | ------------------------------------------------------------------------------------- |
| ECS       | Entity-Component-System, design pattern used to manage modular objects                |
| Entity    | An object within the application. Can be a player, enemy, ui element etc...           |
| Component | Data that can be attached to an entity to give it specific properties.                |
| System    | An operation that perform actions on an entity that has a specific set of components. |
| Blob      | A ressource that can be used by multiple entities (i.e. meshes for collision shapes)  |

## Networking

| Term       | Definition                                                     |
| ---------- | -------------------------------------------------------------- |
| Networking | The process of transmitting data over the a network connection |
| Protocol   | A set of rules that define how data is transmitted             |
| TCP        | Transmission Control Protocol, slower but reliable             |
| UDP        | User Datagram Protocol, faster but unreliable                  |
| Latency    | The time it takes for a packet to travel to its destination    |

## Game Engine

| Term   | Definition                                                       |
| ------ | ---------------------------------------------------------------- |
| Module | A collection of components, systems, blobs, and premade entities |
| Scene  | A collection of entities that are loaded into the game           |

