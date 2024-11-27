# CS50 Nuggets
## Design Spec
### Big D Nuggies, 24F

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes gameStatus, grid, gold, player and spectator modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

---

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user.

### Inputs and outputs

#### Terminal Inputs:
 map.txt file (a text file for a map that follows the format specified in requirements.) Sample maps are provided, but we will also write our own map in the specified format for a unique game experience.
Optional input: seed (a positive integer to pass to the random number generator)
	If no seed is provided we will run the generator to produce a random number

#### Terminal Outputs: (as defined in requirements)
Print a player joined message when a new player joins the game: 
```
Play [player_username]
```
Print a pressed key message when player enters input:
```
KEY [player name] [pressed key]
```
Print a player quit message when encountering an issue:
```
QUIT [player name] [explanation]
```
Print a spectator joined message:
```
SPECTATOR
```

Log output:

This will help with testing. We will send log output to stderr when encountering errors.
The log will define which error we have encountered, some example errors include:
```
Cannot connect to port.
```
```
Cannot connect to client.
```
```
Null input.
```
```
Invalid map file
```
```
Invalid map format
```
```
Cannot add spectator
```

### Functional decomposition into modules

Key Functions:

`Main`: calls parse args, loads the map, and initializes the game and messages

`Parse Args`: Parses the input from the command line and validates the parameters

`Initialize Game`: Reads the map file and loads into the map data structure in memory.

`Handle messages`: Takes messages in and calls necessary helper functions to interpret the messages.

`Send messages`: Sends a message from the server to either a specific client (their score) or all of the clients (total nuggets left).

`Cleanup`: free memory, delete extra data structures, print game over message to client

### Pseudo code for logic/algorithmic flow

The main for server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initializeGame() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients
	call gameOver() to inform all clients the game has ended
	clean up

#### parseArgs

The parseArgs method will run as follows:

	check that two additional arguments were passed
	ensure seed is a valid number 
	convert seed to an integer
	try to open map file
	exit non zero if any of these fail 

#### initializeGame

The initializeGame method will run as follows:

	Initialize a gameInfo struct with gameStatus module that handles map parsing
	If this fails, exit non zero
	Otherwise, distribute the gold onto the map
	Return the gameInfo

#### handleMessages

The handleMessages method will run as follows:

	If any arguments are null, handle error
	Tokenize the message
	If there are 0 tokens
		handle error
	If token 0 is PLAY
		initialize new player and randomly place on map
	If token 0 is SPECTATE
		initialize new spectator
	If token 0 is KEY
		handle key
	If token 0 is QUIT
		remove player from game

#### sendMessages

Most logic for message building will be in separate functions outside of the server module. This is a basic layout of what needs to be done.
The sendMessages method will run as follows:

	If any argument is null
		Handle error
	If player joins game
		Send GRID message to player
	If spectator joins game
		Send SPECTATE message to spectator
	If gold changes
		Send GOLD message to all players/spectator
	If specific player moves
		Message send DISPLAY to all players/spectator
	If game ends
		Send QUIT message to all players/spectator

#### cleanUp

The cleanUp method will run as follows:
	
	Send a message to all clients that the game is over
	For each client
	Free all their resources 
	Free any remaining memory with client
	Free the grid
	Free the data from the gold module
	Free any remaining game data?
	Close off the socket
	Print to standard output that the game has ended


### Major data structures

Possible libcs50 structures: counters, hashtable
Messages: Stores incoming and outgoing messages for clients, including type of message (join, move, quit), and recipient information
GameInfo astruct: Stores location of each gold piece, value of each piece, visibility status, collection status
Stores map layout as a 2D array, accessible positions, walls, boundaries, and obstacles
Stores connection details for each client (socket info, player/spectator mode)

---

## Client

**Extra Credit:**
 The user can optionally press 'a' to automatically resize the window instead of manually resizing it.

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

See the requirements spec for both the command-line and interactive UI.

It is the client’s responsibility to connect to the correct port for the server. The server will print out the port that the client must connect to. The client is also responsible for initializing the correct window size (although a log will print telling the client to adjust window size if it is incorrect). The client will interact with the server through keyboard commands which will be transmitted to the server to update player location and score. The client will see a window with a map printed in it. And will choose their keyboard commands as it updates. If the client chooses spectator mode, their only interaction with the server will be connecting to the correct port.

### Inputs and outputs

See the requirements spec for details on key inputs that operate the client.

If the client must adjust something on their end, a log file will print to stderr that describes the issue and the required adjustment. For example: if the client window size is incorrect, a log will print to stderr saying: “Incorrect window size, please initialize a window of 000x000 size.” Other log outputs might include:
```
Cannot connect to port.
```
```
Cannot connect to server.
```
```
Invalid map file
```
```
Invalid map format
```

---

## GameStatus module

### Functional decomposition

Some of the main functions we will need to implement are the following:

`Game_init`: call all necessary functions to initialize game

`Game_addPlayers`: add a player struct when someone joins

`Game_addSpectator`: add a spectator struct when someone joins

`Game_end`: cleanup and terminate game when called

Other helper functions will be necessary such as getters and setters, but these do not need much explanation

### Pseudo code for logic/algorithmic flow

#### Game_init:

	Initialize a new game struct
	If this fails
		Return NULL
	Initialize grid
	Initialize gold
	Make space for players to join
	Make space for spectator to join
#### Game_addPlayers:
	
	If a request to join comes in from the client:
		Create new player struct
		Add that player struct to the array of players
	Print a message to client describing success of their join
#### Game_addSpectator: 

	If a request to join comes in from the client:
		Create new spectator struct
	Print a message to client describing success of their join

#### Game_end:

	Loop through the arrays calling the delete functions for each struct
	Delete the game struct
	Free any unused pointers


### Major data structures

We will use a game struct that stores the following: a grid, a spectator, a list of players, a number of players, and the gold.

```c
typedef struct game {
  grid_t* masterGrid;           
  spectator_t* spectator;       
  player_t** players;       
  int players_seen;             
  gold_t* gameGold;             
} game_t;
```

---

## Grid module

### Functional decomposition

The following functions will be implemented in our grid module:

`Grid_load`: build the grid struct based on the map input

`Grid_windowSize`: return size of window player must change to

`Grid_toString`: print the grid out for player view

`Grid_isFloor`: check if given x,y location is floor

`Grid_isWall`: check if given x,y location is a wall

`Grid_isCorridor`: check if given x,y location is a corridor

`Grid_isPlayer`: check if given x,y location is a player

`Grid_isGold`: check if given x,y location is gold

`Grid_isVisible`: calculate visibility based on location

`Grid_getGrid`: return columns and rows

### Pseudo code for logic/algorithmic flow

The following four main functions we've provided pseudocode for. The others are just return booleans that check what object is in a location.

#### Grid_load:

	Check map validity
	Create a new grid struct
	Create a new array to store map characters
	Go through each character in the map reading them into an array
	Add the array as a component of the map

#### Grid_windowSize:

	Check the size of the map stored in the Grid

#### Grid_toString

	Allocate space for #column strings of length #row
	For each row, copy the map value into the string and newline
	For each player, send the string
	Return the string

#### Grid_isVisible
	
	See requirements spec for logic on visibility


### Major data structures

Grid_t will store the arrays and the number of columns and rows. Updating these saved arrays will allow us to keep track of the locations of player structs and gold structs.

---

## Gold module

### Functional decomposition

The following functions will be implemented:

`gold_new`: instantiate the gold object

`gold_view`: check for visibility of gold by player

`gold_collect`: handle instance where player lands on gold

`gold_delete`: clean up memory when done

### Pseudo code for logic/algorithmic flow

#### gold_new

	Makes a new gold object
	Set random values for placement, and value
	Set isFound and isVisible to false
	Set count according to order in which it is added to array.

#### gold_view

	Set isVisible to true

#### gold_collect

	Set isFound to true
	Remove gold from array
	Return value of gold (to be added to player’s score)

#### gold_delete

	Frees memory allocated for gold object

### Major data structures

We will have a gold_t struct to store important information about the state of gold in the game.

```c
typedef struct gold {
	bool isFound; // keep track of if the gold has been collected by the player
	int placement; // keep track of where the gold is
	bool isVisible; // keep track of if the player can see the gold
	int value; // keep track of how much the gold is worth if collected
	Int count; // this is to keep track of gold in an array or counters)
} Gold_t;
```

---

## Player module

### Functional decomposition

We will implement the following functions for the player module:

`player_new`: Instatiate a player struct

`player_setLocation`: update location

`player_getLocation`: return location

`player_setGold`: update gold

`player_getGold`: return gold

`player_getVision`: return vision

`player_setVision`: update vision

`player_delete`: cleanup

### Pseudo code for logic/algorithmic flow

The logic for these function is fairly straightforward and does not require complex pseudocode.

### Major data structures

One structure to store all player specific information and being able to retrieve it. **player_t** 
Stores player-specific information, including: 
- `name`: Player's username 
- `id`: Unique identifier for each player
- `coordinates`: Player’s position on the grid
- `vision`: Field of view for visibility calculations 
- `goldCollected`: Amount of gold collected by the player
- `clientID`: keeps track of which client this player is communicating with
- `address`: communication address

---

## Spectator module

### Functional decomposition

We will implement the following functions:

`spectator_new`: Instantiate spectator

`spectator_delete`: cleanup

The logic is very straightforward and does not require complex pseudocode

### Major data structures

This module will have a spectator struct that stores just the communication address.

### Division of Labor

#### General job division:

*Leah*: Client module, Player module (do this first, so that we can work on gamestatus), spectator module (this can happen last)

*Jake*: Grid module (visibility), then help with server

*Kellen*: Gold module first, then Gamestatus module, and help Rana with server

*Rana*: Server module 

#### Scrum schedule (separated into sprints):

- Finalize design and implementation specs (each person works on design and implementation as it relates to the modules they are working on)
- Begin work coding the core structures that will be used in other modules. These structures include gold_t, player_t, and grid_t. (include testing to make sure these structures and functions work before implementing them in other code)
- Work on coding the beginning structure of the game: client server communications, Server startup, initializing the game
- Work on game mechanics: gold collection, keeping track of data structures, player movement, visibility, adjusting visibility for different parts of the map (this will be where the bulk of the work is done and should be allotted the most time)
- Add in further functions: allowing for spectators, allowing the player to dash, checking that the game closes and restarts appropriately as all of the gold is collected, writing our own map for the game (instead of using the provided example maps for testing)
- Final touches: check memory leaks, check coding style, check playability for different clients, do a lot of testing with various edge cases, (if there is extra time, work on extra credit)
