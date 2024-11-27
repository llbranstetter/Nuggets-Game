# CS50 Nuggets
## Implementation Spec
##### Team Big D Nuggies, Fall 2024

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes x, y, z modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

#### Plan for division of labor

*Leah*: Client module, Player module (do this first, so that we can work on gamestatus), spectator module (this can happen last)

*Jake*: Grid module (visibility), then help with server

*Kellen*: Gold module first, then Gamestatus module, and help Rana with server

*Rana*: Server module, ended up rewriting gamestatus and gold modules

Unit testing is being done by Jake, Kellen, and Leah for their respecitive modules, and system/integration testing will be done all together

### Client

#### Data structures

This module will use the data structures `player_t` and `spectator_t` as written in the player and spectator modules. 
`ncurses.h` - library that provides data structures for initializing the screen and printing out the grid. This also handles resizing (for resizing the screen)

#### Definition of function prototypes

The functions defined in `miniclient.c` provide the main structure for the setup and interaction between the server and client. They allow the client to handle messages from the server and send input to the server. As such, the main functions in `client.c` will be very similar or the same as the functions in `miniclinent.c`, but we will implement helper functions to better specify the client functions for the purposes of the game and to handle the specific messages that we will be receiving from the server.

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module. 
A function to handle messages by calling helper functions depending on which type of message it receives from the server. For example if it receives a GOLD message, it will call the `handleGold` function. It uses strcmp to identify which message it will call.
A function to handle input from the client and send it to the server. It will send the server 3 message types: PLAY, SPECTATE, and KEY.
A main function that runs parseArgs and initializes other functions to construct a working client that connects and interacts with the server.

Helper functions (called by handleMessage) to handle each type of message that you can receive from the server (these will each have print functions that print out the data sent by the server): OK, GRID, GOLD, DISPLAY, QUIT, and ERROR.

```c
static int parseArgs(const int argc, char* argv[]);
static bool handleMessages(void* arg, const addr_t from, const char* message);
static bool handleOK(const char* message);
static bool handleGrid(const char* message);
static bool handleGold(const char* message);
static bool handleDisplay(const char* message);
static bool handleQuit(const char* message);
static bool handleError(const char* message);
static bool handleDisplay(const char* message);
static bool handleInput(void* arg);
int main(const int argc, char* argv[]);
```

#### Detailed pseudo code

> For each function write pseudocode indented by a tab, which in Markdown will cause it to be rendered in literal form (like a code block).
> Much easier than writing as a bulleted list!
> For example:

##### `parseArgs`: (this follows the code in `miniclient.c`)

	validate commandline
	initialize message module
	print assigned port number
	decide whether spectator or player

##### `handleMessages`

	validate commandline
	If Gold
		handleGold
	If OK
		handleOK
	If Grid
		handleGrid
	If Display
		handleDisplay
	If Quit
		handleQuit
	If Error
		handleError

	Return false

##### `handleInput`  (this follows the code in `miniclient.c`)
	Arg = addr_t
	Validate commandline
	Allocate buffer to read input
	Read input from stdin
	Check strcmp to decide which type of input it is
	Create a message and send it to the server
	(it is the server’s job to check the message type and validate the message)

---

### Server

#### Data structures

The server will only be maintaining to references to the data structures that we build up in the other modules, namely gameStatus_t (which in turn will likely use player_t, spectator_t, gold_t, grid_t)

#### Definition of function prototypes
The implementation went through several iterations and this is what we decided on for the server.
```c
int main(const int argc, const char* argv[]);
/* 
 * parseArgs - Parse command-line arguments for the game settings.
 *
 * Checks the number of arguments, opens the specified map file, 
 * and validates an optional seed value. Exits with an error 
 * message if the map file cannot be opened or if the seed 
 * is invalid.
 *
 * Parameters:
 *   argc - Number of command-line arguments.
 *   argv - Array of command-line argument strings.
 *   seed - Pointer to store the parsed seed value.
 */
void parseArgs(const int argc, const char* argv[], int* seed);

/* 
 * handleMessage - Process incoming messages for the game.
 *
 * This function handles messages received from players or 
 * other sources. It interprets the message content and takes 
 * appropriate actions based on the game game and the sender's 
 * address. The function returns a boolean indicating the 
 * success or failure of message processing.
 *
 * Parameters:
 *   arg     - Additional data passed to the function 
 *   from    - The address of the sender from which the message originated.
 *   message - The content of the message to be processed.
 * 
 * Returns:
 *   true to terminate the infinite looping by message module.
 */

bool handleMessage(void *arg, const addr_t from, const char *message);

/* 
 * handlePlayMessage - Process a play message from the client.
 */
void handlePlayMessage(gamestatus_t* game, const addr_t from, const char* playerName);

/* 
 * handleSpectateMessage - Process a spectate message from the client.
 */
void handleSpectateMessage(gamestatus_t* game, const addr_t from, const char* spectatorName);

/* 
 * handleKeyMessage - Process a key press from the client.
 */
void handleKeyMessage(gamestatus_t* game, const addr_t from, const char* pressedKey);

/* 
 * randomInt - Generate a random integer within a specified range.
 */
int randomInt(int lowerBound, int upperBound);

/* 
 * getPlayerLetter - Get the letter corresponding to a player's ID.
 */
char getPlayerLetter(int ID);

/* 
 * movePlayer - Move a player to a new position on the game grid.
 */
void movePlayer(gamestatus_t* game, player_t* player, int r, int c);

/* 
 * handlePlayerQuit - Marks a player as no longer active and sends a quit message.
 */
void handlePlayerQuit(gamestatus_t* game, const addr_t from);

/* 
 * handleSpectatorQuit - Marks a spectator as no longer active and sends a quit message.
 */
void handleSpectatorQuit(gamestatus_t* game, const addr_t from);

/* 
 * sendPlayersGoldMessage - Sends updated gold information to all players in the game.
 */
void sendPlayersGoldMessage(gamestatus_t* game);

/* 
 * sendSpectatorGoldMessage - Sends updated gold information to the spectator.
 */
void sendSpectatorGoldMessage(gamestatus_t* game);

/* 
 * sendPlayerDisplayMessage - Delivers a visual representation of the game state to a specific player.
 */
void sendPlayerDisplayMessage(gamestatus_t* game, player_t* player);

/* 
 * sendSpectatorDisplayMessage - Delivers a visual representation of the game state to a specific spectator.
 */
void sendSpectatorDisplayMessage(gamestatus_t* game, spectator_t* spectator);

/* 
 * sendInitOKMessage - Sends an initialization confirmation message to a specified player.
 */
void sendInitOKMessage(gamestatus_t* game, const addr_t to);

/* 
 * sendInitGridMessage - Sends the grid initialization message to a specified player or spectator.
 */
void sendInitGridMessage(gamestatus_t* game, const addr_t to, bool isPlayer);

/* 
 * extractRowFromPosition - Converts a linear position to a row index based on the number of columns.
 */
int extractRowFromPosition(int position, int ncol);

/* 
 * extractColumnFromPosition - Converts a linear position to a column index based on the number of columns.
 */
int extractColumnFromPosition(int position, int ncol);

/* 
 * convertCoordinatesToPosition - Converts 2D grid coordinates to a linear position index.
 */
int convertCoordinatesToPosition(int r, int c, int ncol);

/* 
 * isGameDone - Checks if the game has ended based on the remaining gold status.
 */
bool isGameDone(gamestatus_t* game);

/* 
 * endGame - Finalizes game state and sends messages to all players and spectators when the game is over.
 */
void endGame(gamestatus_t* game);

/* 
 * sendUpdatedDisplays - Updates and sends the current game display to players and spectators.
 */
void sendUpdatedDisplays(gamestatus_t* game);

/* 
 * sendUpdatedGold - Sends updated gold status to all players and the spectator.
 */
void sendUpdatedGold(gamestatus_t* game);

/* 
 * goldPickedUp - Handles gold collection by a player and updates their score and the game state.
 */
void goldPickedUp(gamestatus_t* game, player_t* player, int goldPileValue);
```

#### Detailed pseudo code
##### `main`:
	call parseArgs, initialize gamestatus module, message module and return 0 if boolean returned by message_loop is true else return 1;

##### `parseArgs`:
	Parse command-line arguments for the game settings.
 	Checks the number of arguments, opens the specified map file, and validates an optional seed value. Exits with an error message if the map file cannot be opened or if the seed 
 	is invalid.

##### `handleMessage`
	Process incoming messages for the game.
 	Handle messages received from players or other sources.
	Interpret the message content and takes  appropriate actions based on the game game and the sender's address. 
	Returns a boolean indicating the success or failure of message processing.

##### `handlePlayMessage`  
	Processes a play message from the client.  
	Validate `playerName` and `from` address.  
	Check if the player is already in the game.  
	If valid, initialize the player in the game.  
	Return a response message to the client.


##### `handleSpectateMessage`  
	Processes a spectate message from the client.  
	Validate the `spectatorName` and `from` address.  
	Check if the spectator slot is available.  
	If valid, add the spectator to the game.  
	Send a confirmation message to the spectator.

##### `handleKeyMessage`  
	Processes a key press from the client.  
	Validate the `pressedKey` input.  
	Map the key to an action (e.g., movement, quit).  
	Execute the corresponding game action.  
	Update the game state and notify relevant players/spectators.

##### `randomInt`  
	Generates a random integer within a specified range.  
	Take `lowerBound` and `upperBound` as inputs.  
	Use a random number generator to produce a number between the bounds.  
	Return the generated number.


##### `getPlayerLetter`  
	Gets the letter corresponding to a player's ID.  
	Input: Player's numeric ID.  
	Map the ID to a letter (`A-Z`) using ASCII or a lookup table.  
	Return the letter.


##### `movePlayer`  
	Moves a player to a new position on the game grid.  
	Validate the new position `(r, c)` for grid boundaries and obstacles.  
	Update the player's position in the game state.  
	Trigger any events associated with the new position (e.g., gold pickup).  
	Notify relevant players/spectators.


##### `handlePlayerQuit`  
	Marks a player as inactive and sends a quit message.  
	Identify the quitting player using `from` address.  
	Update the game state to mark the player as inactive.  
	Send a quit message to all other players and spectators.

##### `handleSpectatorQuit`  
	Marks a spectator as inactive and sends a quit message.  
	Identify the quitting spectator using `from` address.  
	Update the game state to mark the spectator as inactive.  
	Notify all relevant players.


##### `sendPlayersGoldMessage`  
	Sends updated gold information to all players.  
	Gather the current gold statuses of all players.  
	Format a message containing the gold details.  
	Send the message to each player.

##### `sendSpectatorGoldMessage`  
	Sends updated gold information to the spectator.  
	Compile the current gold data for all players.  
	Format the message with relevant details.  
	Send the message to the spectator.

### Player module
#### Data structures

One structure to store all player specific information and being able to retrieve it. **player_t** 
Stores player-specific information, including: 
```c
typedef struct player {
char* name;
int ID; // unique ID used to identify a player
int position;  // player position
int score; // Amount of gold collected by the player
addr_t IPaddress;
}
```

#### Definition of function prototypes

```c
player_t* player_new(const addr_t address, char* name, int ID, grid_t* grid);
void player_setLocation(player_t* player, int position);
int player_getLocation(player_t* player);
void player_addGold(player_t* player, int value);
void player_setGold(player_t* player, int score);
int player_getGold(player_t* player);
bool player_sendMessage(player_t* player, char* message);
```

#### Detailed pseudo code

##### player_new
	Create new player struct
	Set IP address to given address
	Set ID to given ID
	Set name to given name
	Set score to zero
	Set location to random coordinates
	Return player

##### player_setLocation(int position)
	If position is greater than 0:
		Player-> location = position
	
##### player_getLocation
	Position = player->location
	Return position

##### player_addGold
	player->score = player->score + value

##### player_setGold
	player->score = value

##### player_getGold
	Gold = player->score
	return Gold

##### player_delete
	Free player

##### player_setID
	player->ID = ID

---

### Spectator module

#### Data structures
This module will have a spectator struct that stores just the communication address.

```c
typedef struct spectator {
addr_t IPaddress;
}
```

#### Definition of function prototypes

```c
spectator_new(addr_t address);
spectator_delete(spectator_t* spectator);
```

#### Detailed pseudo code:

##### spectator_new(addr_t address)
	spectator->IPaddress = address;
	return spectator_t;

##### spectator_delete(spectator_t* spectator)
	free spectator->IPaddress;
	free spectator;
---

### GameStatus module

#### Data structures
```c
typedef struct gamestatus {
    grid_t* grid;            // pointer to the game grid for the spectator that keeps track of players
	grid_t* originalGrid;     //Grid that is never changed and used for updating other grids
    gold_t** goldPiles;       // array of pointers to gold piles scattered across the grid
    player_t* players[MaxPlayers]; // array of players in the game
    spectator_t* spectator;  // pointer to spectator
    int totalGold;           // total remaining gold in the game
    int numPlayers;          // current number of players in the game
    bool gameOver;           // boolean to track if the game is over
} gamestatus_t;
```

#### Definition of function prototypes
```c
gamestatus_t* gamestatus_new(const char* mapFile, int seed);
player_t* gamestatus_addPlayer(gamestatus_t* game, char* playerName, const addr_t address);
bool gamestatus_addSpectator(gamestatus_t* game, const addr_t address);
void gamestatus_distributeGold(gamestatus_t* game, int minPiles, int maxPiles);
player_t* gamestatus_getPlayerByAddress(gamestatus_t* game, const addr_t address);
void gamestatus_removePlayer(gamestatus_t* game, const addr_t address);
void gamestatus_removeSpectator(gamestatus_t* game);
bool gamestatus_checkGameOver(gamestatus_t* game);
void gamestatus_endGame(gamestatus_t* game);
void gamestatus_delete(gamestatus_t* game);
```

#### Detailed pseudo code

##### gamestatus_t* gamestatus_new(const char* mapFile, int seed)
	allocate memory for a new gameStatus_t struct
  	load grid from mapFile into game
    	initialize totalGold
    	seed random generator with given seed
    	initialize players array to NULL for MaxPlayers
    	set spectator to NULL
    	set gameOver to false
    	call gameStatus_distributeGold(game, GoldMinNumPiles, GoldMaxNumPiles)
    	return the new gameStatus_t struc

##### void gamestatus_addPlayer(gamestatus_t* game, player_t* player)
if numPlayers is less than MaxPlayers
        		find the next available player
        		add player to players array 
        		increment numPlayers
        		return true
    	else
        	return false

##### void gamestatus_addSpectator(gamestatus_t* game, spectator_t* spectator)
	if spectator is not NULL
        		send QUIT message to current spectator
        		delete current spectator
    	set spectator to new spectator

##### void gamestatus_distributeGold(gamestatus_t* game, int minPiles, int maxPiles)
	generate a random number of piles between minPiles and maxPiles
    for each pile
        randomly choose a spot on the grid that is a room spot
        create a gold object with a random value and place it at the chosen spot
        add the gold object to the goldPiles array

##### bool gamestatus_checkGameOver(gamestatus_t* game)
If totalGold is zero
        		set gameOver to true
        		call gameStatus_endGame(game)
        		return true
    	else
        		return false

##### void gamestatus_endGame(gamestatus_t* game)
for each player in game->players
        		if player is not NULL
            		send QUIT message to player with their score
   		 if spectator is not NULL
        			send QUIT message to spectator
    	print game summary with each player’s score
    	set gameOver to true

##### void gamestatus_delete(gamestatus_t* game)
	delete/free grid 
   	delete/free each gold object in goldPiles
    	delete/free each player in players
    	delete/free spectator if exists
    	free memory for game struct itself

### Gold module

#### Data structures
To represent each gold pile’s state and location.
```c
typedef struct gold{
	bool isFound; // (keep track of if the gold has been collected by the player)
	int placement; // (keep track of where the gold is)
	bool isVisible; // (keep track of if the player can see the gold)
	int value; //(keep track of how much the gold is worth if collected)
	int count; // (this is to keep track of gold in an array or counters)
} gold_t;
```

#### Definition of function prototypes

```c
gold_t*  gold_new(int value, int x, int y);
void  gold_view(gold_t* gold);
int gold_collect(gold_t* gold);
void gold_delete(gold_t* gold);
```

#### Detailed pseudo code

##### gold_t*  gold_new(int value, int x, int y) {
	makes a new gold object
	set random values for placement, and value
	set isFound and isVisible to false
	set count according to order in which it is added to array.
}

##### void gold_view(gold_t* gold) { 
	set isVisible to true
}

##### int gold_collect(gold_t* gold) { 
	set isFound to true
	remove gold from array
	return value of gold (to be added to player’s score)
}

##### void gold_delete(gold_t* gold){
	frees memory allocated for gold object
}

---

### Grid module

#### Data structures
```c
typedef struct grid {
	char* gridArray;
	int ncols;
	int nrows;
}
```

#### Definition of function prototypes

```c
grid_t* grid_load(const char* mapFile);
grid_t* grid_playerLoad(grid_t* mainGrid);
void grid_delete(grid_t* grid);
bool grid_validStart(grid_t* grid, int r, int c);
bool grid_isWall(grid_t* grid, int r, int c);
bool grid_isGold(grid_t* grid, int r, int c);
char grid_getCell(grid_t* grid, int r, int c);
int grid_getRows(grid_t* grid);
int grid_getCols(grid_t* grid);
bool is_within_bounds(grid_t* grid, int row, int col);
void grid_calculateVis(grid_t* mainGrid, grid_t* playerGrid, int player_r, int player_c);
bool line_of_sight(grid_t* mainGrid, int startRow, int startCol, int endRow, int endCol);
void grid_print(grid_t* grid);
void grid_toString(grid_t* grid, char* message);

```

#### Detailed pseudo code

##### grid_load
grid_t* grid_load(char* map) {
	Read dimensions of the grid (nrows, ncols)
	Allocate grid_t struct
	Allocate 1D gridArray or size nrows * ncols
	For each ro
		For each column
			gridArray[cell] = char from map
	Return pointer to grid_t
}

##### grid_delete

void grid_delete(grid_t* grid) {
	Free the array within the grid
	Free the grid_t struct
}


##### grid_validPosition

bool grid_validStart(grid_t* grid, int x, int y) {
if out of bounds
	return false
if grid[cell] == ‘.’
	return true
	return false
}


##### grid_getCell

char grid_getCell(grid_t* grid, int x, int y) {
	if out of bounds
		return ‘ ‘
	return grid[cell]
} 


##### grid_isWall

bool grid_isWall(grid_t* grid, int x, int y) {
	if out of bounds
		return false
	if cell is ‘-’ or ‘|’ or ‘+’
		return true
	return false
}


##### grid_calculateVis

void grid_calculateVis(grid_t* mainGrid, grid_t* playerGrid, int x, int y) {
	for each cell (r, c)
		if line_of_sight(maingrid, x, y, r, c) 
			playerGrid[cell] = mainGrid[cell]
		else if mainGrid[cell] == ‘*’
			playerGrid[cell] = ‘.’
}

##### line_of_sight

bool line_of_sight(grid_t* grid, int startRow, int startCol, int endRow, int endCol) {
	brechman’s algorithm to get line
	for each cell (x,y) along line
		if grid_isWall 
			return false
	return true
}


##### grid_print

void grid_print(grid_t* grid, int x, int y) {
	for each row
		for each column
			print gridArray[cell]
		print newline 
}

---

### Testing plan

#### Unit testing

#### Player and Spectator
These will mainly be tested as part of our testing of the server and client modules. The basic functions will be tested in a simple main function in each file that creates a spectator and player struct and then calls the functions on each struct and prints their output. 

#### Grid
This will be tested in a main function in the typedef file that creates a grid and uses the functions on the grid to ensure they are giving the correct output. Further testing will occur during the integration and system testing.

#### Gold
This will be tested in the same manner as grid, using a main function within the typedef file. Further testing will occur during the integration and system testing.


#### Integration testing
We will add the basic server functionality and test it by checking if the server starts successfully and accepts connections.
Confirm that the server can add players and spectators correctly. Check that counts of active players and spectators are accurate.
Validate that the server can receive and process messages from players. Ensure that game status is updated appropriately based on these messages.
Finally, do a full simulation of a full game scenario where players join, send messages, and the game state evolves. Verify that players see the correct environment, and interactions (like movement and actions) update the game state correctly.

We will test client by running tests to ensure that the client can connect to various server ports using the provided server in the shared directory and the miniclient. We will also test the player and spectator classes within the client module as they will be initiated and used there. Most of the testing will occur in the integration testing and system testing.

#### System testing

We will first test the client and server separately to ensure that they are working on their own. Then we will do our final tests on the server’s ability to properly connect to the client and print out messages to each other. We will begin testing with simple cases of printing messages between the client and server (likely with the client acting as a spectator), and we will gradually upgrade the difficulty of test cases to make sure that multiple clients can connect to a server simultaneously with both a spectator and a player role.

##### Testing Results
After a lot of debugging by connecting server to shared directory client and our client to shared directory server, we were able to run multiple runs of the test. The most useful feature of these test runs was the logging module that we used. In the client folder, you shall find testrunP1, testrunP2, & testrunS. In the server folder, you shall find testrun. All of these log files correspond to a big system test we performed which involved 2 players and a spectator coming in and getting kicked out by another during the match. We then checked for valrind errors in server which was completely free of memory errors and memory leaks. The client did have memory leaks but these were all regarding the ncurses library which we were told to ignore. Obviously, we conducted a lot more testing but for the sake of not taking up too much space, we chose to commit only these log files.

### Our Map
Our map can be found in the maps directory called "bigDMap.txt". 

#### Limitations
Our assumptions are listed in the README.
- Server can only handle up to 26 clients 
- We will assume that the map file is a valid map
- Only one client can join as a spectator

