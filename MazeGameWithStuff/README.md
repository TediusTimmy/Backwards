BackRooms Maze Game Thing Demo
==============================

This uses everything, and is built upon all that has come before. We have a scripting language that can be used to program the player AI of this ... thing. It's not quite a game, yet. You can just wander around and get lost, and start to fear doors. Or, you can just play around with a calculator that does math with nine digits of decimal.

# New Functions
* float Down ()  # Schedules an action for the player to move down. Only one action may be scheduled in an update. Returns one.
* array DumpMachine ()  # Prints out the stack of queues of states starting from the top of the stack. Returns an array of strings, starting from the bottom of the stack.
* float Left ()  # Schedules an action for the player to move left. Only one action may be scheduled in an update. Returns one.
* float Load (string)  #  Returns one. Loads from file a Dictionary of states to add to the machine.
* float Look (float; float)  # Schedules an action to look at a specific place on the visible screen. The result can be recovered with GetInput on a later update. Returns one.
* float Right ()  # Schedules an action for the player to move right. Only one action may be scheduled in an update. Returns one.
* float SetInput (value)  # Sets the input value to be later returned by GetInput. Returns one.
* float SetDebugScript (array)  # Returns one. Sets an array of strings to be executed in the debugger, when it is called during an update.
* float Up ()  # Schedules an action for the player to move up. Only one action may be scheduled in an update. Returns one.

## Known Bugs
* User input from the console is processed before updates happen, and the update loop clears any prior action that has been commanded. As such, calling a function that performs an action has no effect.
* DumpMachine can't be called from the Debugger. Actually, no function on the state machine can be called from the Debugger (given what most of those functions do, that doesn't feel like a bug to me).
* The debug script doesn't reset between calls to the debugger (intentional) or updates (easy to fix, but marginally useful).

# Examples
* EasyMove.txt - this defines four states: "Up", "Down", "Left", "Right". When you Push() one of these states, the player will move in that direction until they hit a wall or other obstruction. And you can fight against it with the direction keys (or get the player unstuck). Hold "F1" to clear the state engine and cancel this.
* Solve.txt - this defines a wall-following maze solving algorithm. Just have the player with a wall to the right when you Push("Walk"). They will then follow the walls, not going through doors, as if their right hand were touching the wall.
* Demo.txt - this is Solve.txt, but the player will blindly go through doors.
