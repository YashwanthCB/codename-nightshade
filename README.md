Nightshade is a stealth game inspired by Splinter Cell.

Main Elements: Light exposure meter and Sound sensor.

The Nightshade AI System is a custom Unreal Engine 5 framework built to support dynamic stealth and combat behavior for human and robotic NPCs.
It leverages Behavior Trees, Blackboard and Unreal’s Perception System to create context-aware agents capable of patrolling, detecting, investigating, and engaging the player seamlessly.

The system is designed in C++ with Blueprint hooks for rapid designer iteration while maintaining the scalability and performance of native code.

Gameplay Video: https://youtu.be/DNXjnePVrZ0

OVERVIEW

The AI system in Nightshade is built for stealth gameplay where
enemies dynamically sense, investigate, and react to player actions.

The system uses:
  - C++ Base AI Controller
  - C++ AI Character
  - Behavior Trees
  - Blackboards
  - Custome AI Perception (Touch, Sight and Hearing)
  - Custom C++ Tasks and Services
  - Animation Notifies for reactions and sync

+-----------------------------------------------------------+
|                   AICharacter (C++)                       |
|-----------------------------------------------------------|
| - Inherits from ACharacter                                |
| - Contains movement logic, combat interface, and state     |
| - Syncs animation and gameplay state                       |
| - Communicates with AIController                           |
+-----------------------------------------------------------+
                           |
                           v
+-----------------------------------------------------------+
|                   AIController (C++)                      |
|-----------------------------------------------------------|
| - Possesses the AICharacter                               |
| - Manages blackboard and behavior tree runtime             |
| - Handles sight/hearing events via perception system       |
| - Updates blackboard values dynamically                    |
+-----------------------------------------------------------+
                           |
                           v
+-----------------------------------------------------------+
|               Blackboard + Behavior Tree                  |
|-----------------------------------------------------------|
| - Blackboard: Stores runtime values like                  |
|     * Player reference                                    |
|     * LastKnownLocation                                   |
|     * IsAlerted                                           |
| - Behavior Tree: Drives state transitions                 |
|     * Patrol                                              |
|     * Investigate noise                                   |
|     * Chase player                                        |
|     * Return to patrol                                    |
+-----------------------------------------------------------+

World and Gameplay:
  The level consists of lights, NPC AIs and movable objects that makes noise.
  The objective is to gain access a government facility and extract important data without getting detected.
  Player will have a grapple hook to move rapidly between different elevations.

Grapple Hook:
  A custom cable component and resolver is used for grappling. The player can grapple to any ledge in the level.
  The level doesn't have to add any kind of logic for edge detection.
  Everything is done efficiently in the grapple hook component of the player itself.
  
AI Perception:

Custom Vision System:
  AI in Nightshade doesn't just simply use the default perception system which acts like binary system -> either visible or invisible.
  The custome vision system calculates the percentage of the player's body visible to NPC and the percentage of body exposed to light.
  A number of probes are placed on the player mesh and ray casted to the AI perception sensor.
  The percentage of visibility is calculated by dividing the number of ray casts reaching the AI and total number of ray casts.
  Light visibility is calculated using light fall off formula. 
  And based on how aware the AI is, the detection flag is triggered.
  
Custom Hearing System:
  Default hearing system in unreal engine doesn't take into account, the inclusion of walls between the source of noise and hearing sensor.
  The Custom hearing code takes walls, ceilings and few other objects into account and calculates the sound fall off accurately. 
  The detection flag is triggerred based on the volume of noise calculated using the above constriants.
  
Touch Sensitivity System:
  AI will detect the player if he goes very close to the NPC and touches. 

Investigation Logic:
  AI in Nightshade are really smart. They can detect if someone has entered the room by using the clues that is left in the room.
  Examples: A door that is forgotten to be closed, a moved chair, Lights or electrical equipments that are turned on or off.
  The AIs initially try to revert the state (like closing the door) and develop a minor suspicion.
  If these things happen frequently, they will become highly alerted and start to investigate the whole room eventually finding the player.
  Only a very games use this kind of detection logic

All these components are added to the NPC AIs in the level and placed in strategic positions to challenges the player's skills. 

