/*
 *  server.c - server file for the nuggets program
 * 
 *  Usage:
 *   The server accepts two command-line arguments:
 *      - <map.txt>: the pathname of the map file to be read.
 *      - [seed]: an optional parameter that is used for the randomness of the program
 * 
 *  Exit codes:
 *   0  - Success (Server ran successfully)
 *   1  - Incorrect number of command-line arguments
 *   2  - Failed to open the map file
 *   3  - Invalid seed value provided
 *   4  - Failure to initialize message handling
 *   5  - Game initialization failure (unable to create game status)
 * 
 *  Team Big D Nuggies
 *  Rana Moeez Hassan, Fall 2024
 *  Jacob Fleming, Fall 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "file.h"
#include "log.h"
#include "message.h"
#include "player.h"
#include "spectator.h"
#include "grid.h"
#include "gold.h"
#include "gamestatus.h"

/**************** global constants (defined by REQUIREMENTS) ****************/
static const int MaxNameLength = 50;   // maximum number of chars in playerName

/**************** helper functions definitions ****************/

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


/**************** core functions definitions ****************/

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

/**************** main() ****************/
/* Controls the flow of the program and execution */
int
main(const int argc, const char* argv[])
{
    log_init(stderr);
    int seed = getpid();
    parseArgs(argc, argv, &seed);
    srand(seed);

    int port = message_init(stderr);
    if (port == 0){
        log_v("Could not initialize message in server main()...\n");
        exit(4);
    }
    printf("Ready to play, waiting at port '%d'\n", port);

    // int randomNumGoldPile = randomInt(GoldMinNumPiles, GoldMaxNumPiles);
    gamestatus_t* game = gamestatus_new(argv[1], seed);
    if (game == NULL) {
        log_v("Server could not initialize a new gamestatus_t...\n");
        exit(5);
    }

    message_loop(game, 0, NULL, NULL, handleMessage);

    message_done();

    gamestatus_delete(game);

    return 0;
}

/**************** parseArgs() ****************/
/* See top of the file for the description */
void 
parseArgs(const int argc, const char* argv[], int* seed)
{
    // Check for the correct number of arguments
    if (argc < 2 || argc > 3) {
        log_v("Wrong number of inputs provided...\n");
        printf("Usage: ./server map.txt <seed>\n");
        exit(1);
    }

    FILE* fp;
    // Attempt to open the map file
    if ((fp = fopen(argv[1], "r")) == NULL) {
        log_v("Could not open the map file provided...\n");
        exit(2);
    }
    fclose(fp);

    // Validate the seed if it's provided
    if (argc == 3) {
        if (sscanf(argv[2], "%d", seed) != 1) {
            log_s("Invalid seed number provided: %s\n", argv[2]);
            exit(3);
        }
    }
}


/**************** handleMessage() ****************/
/* See top of the file for the description */
bool 
handleMessage(void *arg, const addr_t from, const char *message) 
{
    /* if any pointer arguments are NULL, quit the game */
    if (arg == NULL || message == NULL) {
        log_v("Entered message loop without a gamestatus...\n");
        return true;
    }

    gamestatus_t* game = (gamestatus_t*) arg;

    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0 || strncmp(message, "play ", strlen("PLAY ")) == 0) {
        const char* playerName = message + strlen("PLAY ");
        handlePlayMessage(game, from, playerName);
    } else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0 || strncmp(message, "spectate", strlen("SPECTATE")) == 0) {
        const char* spectatorName = message + strlen("SPECTATE");
        handleSpectateMessage(game, from, spectatorName);
    } else if (strncmp(message, "KEY ", strlen("KEY ")) == 0 || strncmp(message, "key ", strlen("KEY ")) == 0) {
        const char* keyPressed = message + strlen("KEY "); // Should be just a single character, error checking done later
        handleKeyMessage(game, from, keyPressed);
    } else {
        log_v("A valid message was not provided to the server...\n");
        log_s("The invalid message was: %s\n", message);
        message_send(from, "ERROR malformed message was received by server...\n");
        return false;
    }

    sendUpdatedDisplays(game);
    sendUpdatedGold(game);

    // Check if the game is ended
    if (!isGameDone(game)) {
        return false;
    } else {
	    endGame(game);
        return true;
  }
}

/**************** sendUpdatedDisplays() ****************/
/* See top of the file for the description */
void 
sendUpdatedDisplays(gamestatus_t* game)
{
    // Sends updated display and gold for spectator
    sendSpectatorDisplayMessage(game, game->spectator);

    // Sends updated display and gold for players
    int numPlayers = game->numPlayers;
    player_t** players = game->players;
    for(int i = 0; i < numPlayers; i++){
        if (players[i] == NULL){
            continue;
        }
        sendPlayerDisplayMessage(game, players[i]);
    }
}

/**************** sendUpdatedGold() ****************/
/* See top of the file for the description */
void 
sendUpdatedGold(gamestatus_t* game)
{
    sendSpectatorGoldMessage(game);
    sendPlayersGoldMessage(game);
}

/**************** handleSpectateMessage() ****************/
/* See top of the file for the description */
void
handleSpectateMessage(gamestatus_t* game, const addr_t from, const char* spectatorName)
{
    // Check if the SPECTATE message is valid
    if (strlen(spectatorName) > 0) {
        log_v("There was an extra message passed along with SPECTATE message...\n");
        message_send(from, "QUIT Sorry - you must not provide extra messages to the SPECTATE message.\n");
        return;
    }

    // Check if there is already another spectator
    if (game->spectator != NULL) {
        spectator_sendMessage(game->spectator, "QUIT You have been replaced by a new spectator.\n");
        gamestatus_removeSpectator(game);
        // need to free spectator before changing pointer
        game->spectator = NULL;
    }
    gamestatus_addSpectator(game, from);
    sendInitGridMessage(game, from, false);
}

/**************** handlePlayMessage() ****************/
/* See top of the file for the description */
void 
handlePlayMessage(gamestatus_t* game, const addr_t from, const char* playerName)
{
    if (strlen(playerName) <= 0) {
        log_v("There was no player name provided to the server, player has been ignored...\n");
        message_send(from, "QUIT Sorry - you must provide player's name.\n");
        return;
    }

    // Buffer for the actual player name
    char actualPlayerName[MaxNameLength + 1]; 

    // Check if playerName is too long
    if (strlen(playerName) > MaxNameLength) {
        snprintf(actualPlayerName, MaxNameLength + 1, "%s", playerName);
        log_s("The player name provided to server was too long and automatically truncated to first 50 characters: %s...\n", actualPlayerName);
    } else {
        // If the name fits, just use the original
        strncpy(actualPlayerName, playerName, MaxNameLength + 1);
        actualPlayerName[MaxNameLength] = '\0';
    }

    // Replace with '_' for any character that is not graphical or blank
    // int playerNameLength = strlen(actualPlayerName);
    for (int i = 0; actualPlayerName[i] != '\0'; i++) {
        if (!isgraph(actualPlayerName[i]) && !isblank(actualPlayerName[i])) {
            actualPlayerName[i] = '_';
        }
    }

#ifdef DEBUGPRINT
    printf("Printed the name after processing: %s\n", actualPlayerName);
#endif

    player_t* player;
    // Add player to gamestatus and send initialization messages
    if ((player = gamestatus_addPlayer(game, actualPlayerName, from)) == NULL) {
        message_send(from, "QUIT Game is full: no more players can join.\n");
        return;
    }

    player->name = malloc(strlen(actualPlayerName) + 1); // +1 for the null terminator
    if (player->name == NULL) {
        log_v("Failed to allocate memory for player name.\n");
        free(player); // Free previously allocated memory for the player structure
        message_send(from, "QUIT Server error: Unable to allocate player details.\n");
        return;
    }

    strcpy(player->name, actualPlayerName); // Copy the name into the allocated space

#ifdef DEBUGPRINT
    printf("Printed the name being stored: %s\n", player->name);
#endif
    
    game->grid->gridArray[player->position] = getPlayerLetter(player->ID);
    player->grid->gridArray[player->position] = '@';

    sendInitOKMessage(game, from);
    sendInitGridMessage(game, from, true);
    // sendUpdatedDisplays(game);
    // sendInitDisplayMessage(game, from, game->grid);
}

/**************** sendInitGridMessage() ****************/
/* See top of the file for the description */
void 
sendInitGridMessage(gamestatus_t* game, const addr_t to, bool isPlayer)
{
    // Send initialization message to player/spectator
    int rows = game->grid->nrow;
    int cols = game->grid->ncol;
    char initMessage[100];
    sprintf(initMessage, "GRID %d %d", rows, cols);
    if (isPlayer){
        player_t* player = gamestatus_getPlayerByAddress(game, to);
        if (player == NULL) {
            log_v("Could not find the player to send the GRID initialization message...\n");
        }
        player_sendMessage(player, initMessage);
    } else {
        spectator_sendMessage(game->spectator, initMessage);
    }
}

/**************** sendInitOKMessage() ****************/
/* See top of the file for the description */
void 
sendInitOKMessage(gamestatus_t* game, const addr_t to)
{
    player_t* player = gamestatus_getPlayerByAddress(game, to);
    if (player == NULL) {
        log_v("Could not find the player to send the OK initialization message...\n");
        return;
    }

    char initMessage[100];
    snprintf(initMessage, sizeof(initMessage), "OK %c", getPlayerLetter(player->ID));
    player_sendMessage(player, initMessage);
}

/**************** handleKeyMessage() ****************/
/* See top of the file for the description */
void 
handleKeyMessage(gamestatus_t* game, const addr_t from, const char* pressedKey)
{
    if (game == NULL || pressedKey == NULL) {
        log_v("Invalid game, address or pressed key passed to handleKeyMessage...\n");
        return;
    }

    grid_t* grid;
    player_t* player = gamestatus_getPlayerByAddress(game, from);
    int r;
    int c;
    if (player != NULL) {
        grid = player->grid;
        r = extractRowFromPosition(player->position, grid->ncol);
        c = extractColumnFromPosition(player->position, grid->ncol);
#ifdef DEBUGPRINT
    printf("Printed the name at handleKeyMessage: %s\n", player->name);
#endif
    }
    
    char keyPressed;
    if (strlen(pressedKey) > 1) {
        log_v("Error: KEY message had more than one character for the key");
    } else {
        keyPressed = pressedKey[0];
    }

    switch (keyPressed) {
        case 'Q':
            if (game->spectator != NULL) {
                if ((message_eqAddr(from, game->spectator->IPaddress))) {
                    handleSpectatorQuit(game, from);
                } 
            } else if (player->isPlaying == false) {
                log_v("A player client that has already quit tried to play with keyboard...\n");
                return;
            } else {
                handlePlayerQuit(game, from);
            }
            break;

        // Horizontal movement
        case 'l': 
            movePlayer(game, player, r, c + 1);
            break;
        case 'h': 
            movePlayer(game, player, r, c - 1);
            break;

        // Vertical movement
        case 'k': 
            movePlayer(game, player, r - 1, c);
            break;
        case 'j': 
            movePlayer(game, player, r + 1, c);
            break;

        // Diagonal movements
        case 'u': 
            movePlayer(game, player, r - 1, c+1);
            break;
        case 'y': 
            movePlayer(game, player, r - 1, c-1);
            break;
        case 'b': 
            movePlayer(game, player, r+ 1, c-1);
            break;
        case 'n': 
            movePlayer(game, player, r+1, c+1);
            break;

        // Max horizontal movements
        case 'L': 
            while(!grid_isWall(grid, r, c + 1)){
                movePlayer(game, player, r, c + 1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        case 'H': 
            while(!grid_isWall(grid, r, c - 1)){
                movePlayer(game, player, r, c - 1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;

        // Max vertical movements
        case 'K': 
            while(!grid_isWall(grid, r - 1, c)){
                movePlayer(game, player, r - 1, c);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        case 'J': 
            while(!grid_isWall(grid, r + 1, c)){
                movePlayer(game, player, r + 1, c);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        
        // Max diagonal movements
        case 'U': 
            while(!grid_isWall(grid, r - 1, c+1)){
                movePlayer(game, player, r - 1, c+1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        case 'Y': 
            while(!grid_isWall(grid, r - 1, c-1)){
                movePlayer(game, player, r - 1, c-1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        case 'B': 
            while(!grid_isWall(grid, r+ 1, c-1)){
                movePlayer(game, player, r+ 1, c-1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;
        case 'N': 
            while(!grid_isWall(grid, r+1, c+1)){
                movePlayer(game, player, r+1, c+1);
                r = extractRowFromPosition(player->position, grid->ncol);
                c = extractColumnFromPosition(player->position, grid->ncol);
            }
            break;

        // Ignore situations where the client detects a key but it is not one of the specific keys
        default:
            break;
        }
}


/**************** randomInt() ****************/
/* See top of the file for the description */
int 
randomInt(int lowerBound, int upperBound)
{
    if (lowerBound > upperBound) {
        log_v("Server tried to create a random number with invalid ranges...\n");
        return -1;
    } else {
        return (rand() % (upperBound - lowerBound + 1)) + lowerBound;
    }
}

/**************** getPlayerLetter() ****************/
/* See top of the file for the description */
char 
getPlayerLetter(int ID) 
{
    if (ID < 0 || ID > 25) {
        log_v("Invalid player ID was provided to convert to letter...\n");
        return '?'; // Placeholder for invalid letter
    }
    return 'A' + ID; 
}

/**************** movePlayer() ****************/
/* See top of the file for the description */
void 
movePlayer(gamestatus_t *game, player_t *player, int r, int c)
{
    if (game == NULL) {
		return;
	}

    if(r < 0 || c < 0|| c >= game->grid->ncol || r >= game->grid->nrow){
        return;
    }

	grid_t* mainGrid = game->grid;
 	gold_t** gameGold = game->goldPiles;
    if (gameGold == NULL) {
        log_v("Error: Gold piles are null in move player.\n");
    }
	
  	char* playerGridArray = player->grid->gridArray;
	char* mainGridArray = mainGrid->gridArray;
    char* originalGridArray = game->originalGrid->gridArray;
	// int* gold_array = gameGold->goldCounter;

	player_t** players = (player_t**) game->players;
    int positionToMoveTo = convertCoordinatesToPosition(r, c, mainGrid->ncol);
	player_t* otherPlayer = NULL;

    // check if another player is in the position
	for(int i = 0; i < game->numPlayers; i++) {
		otherPlayer = players[i];
		if ((otherPlayer->position == positionToMoveTo) && (otherPlayer->ID != player->ID)){
			break;
		} else{
            otherPlayer = NULL;
		}
	}

	if (!grid_isWall(mainGrid, r, c)) {
        int position = player->position;
		if (grid_isGold(mainGrid, r, c)){
            // Update the current position of the player to be empty 
            playerGridArray[position] = '.';			
            playerGridArray[positionToMoveTo] = '@';

            mainGridArray[position] = '.';
            mainGridArray[positionToMoveTo] = getPlayerLetter(player->ID);

            player->position = positionToMoveTo;

            for (int i = 0; i < game->numGoldPiles; i++) {
                if (gameGold[i] == NULL) {
                    continue;
                } 
                if (gameGold[i]->placement == positionToMoveTo && gameGold[i]->isFound == false) {
                    gameGold[i]->isFound = true;
                    game->totalGold = game->totalGold - gameGold[i]->value;
                    player->score = player->score + gameGold[i]->value;
                    goldPickedUp(game, player, gameGold[i]->value);
                }
            }

		} else if (otherPlayer != NULL) {
            // If there are two players on the same spot, they shall swap places according to requirement spec
			char* otherPlayerGridArray = otherPlayer->grid->gridArray;

			playerGridArray[positionToMoveTo] = '@';
            playerGridArray[position] = getPlayerLetter(otherPlayer->ID);

			otherPlayerGridArray[position] = '@';
            otherPlayerGridArray[positionToMoveTo] = getPlayerLetter(player->ID);

            mainGridArray[position] = getPlayerLetter(otherPlayer->ID);
            mainGridArray[positionToMoveTo] = getPlayerLetter(player->ID);

            player->position = positionToMoveTo;
            otherPlayer->position = position;
			
		} else {
            // If we get here, then the spot is just an empty spot/corridor that player is moving to
            playerGridArray[position] = originalGridArray[position];
            mainGridArray[position] = originalGridArray[position];
            playerGridArray[positionToMoveTo] = '@';
            mainGridArray[positionToMoveTo] = getPlayerLetter(player->ID);
			player->position = positionToMoveTo;
		}
        // Update visibility after each move
        grid_calculateVis(mainGrid, player->grid, game->originalGrid, 
                          extractRowFromPosition(player->position, mainGrid->ncol), 
                          extractColumnFromPosition(player->position, mainGrid->ncol));
	}
}

/**************** handlePlayerQuit() ****************/
/* See top of the file for the description */
void 
handlePlayerQuit(gamestatus_t *game, const addr_t from)
{
  // Search for player with matching address in game
  player_t* player = gamestatus_getPlayerByAddress(game, from);

  // Send the quit message to player, but still keep a record of the information for leaderboard
  if (player != NULL) {
    player->isPlaying = false;
    player->grid->gridArray[player->position] = game->originalGrid->gridArray[player->position];
    game->grid->gridArray[player->position] = game->originalGrid->gridArray[player->position];
    player_sendMessage(player, "QUIT Thank you for playing!");
  } else {
    log_v("No matching player OR spectator found for an incoming QUIT keystroke\n");
  }

}

/**************** handleSpectatorQuit() ****************/
/* See top of the file for the description */
void 
handleSpectatorQuit(gamestatus_t *game, const addr_t from)
{
    spectator_t* spectator = game->spectator;

    // Send quit message to spectator and remove from gamestatus
    spectator_sendMessage(spectator, "QUIT Thank you for watching!");
    gamestatus_removeSpectator(game);
}

/**************** sendPlayersGoldMessage() ****************/
/* See top of the file for the description */
void 
sendPlayersGoldMessage(gamestatus_t* game)
{
    // Loop over every player
    player_t** allPlayers = game->players;
    int numPlayers = game->numPlayers;

    // Buffer for the gold message
    char goldMessage[100]; // Declare a fixed-size array for the message

    for (int i = 0; i < numPlayers; i++) {
        if (allPlayers[i] == NULL) {
            continue;
        }
        int currentPlayerGold = allPlayers[i]->score;
        int justCollectedGold = 0;
        int goldLeftInGame = game->totalGold;

        // Format the message
        snprintf(goldMessage, sizeof(goldMessage), "GOLD %d %d %d", justCollectedGold, currentPlayerGold, goldLeftInGame);
        player_sendMessage(allPlayers[i], goldMessage);
    }
}

/**************** sendSpectatorGoldMessage() ****************/
/* See top of the file for the description */
void 
sendSpectatorGoldMessage(gamestatus_t* game)
{
    spectator_t* spectator = game->spectator;
    if (spectator == NULL) {
        return;
    }

    int goldCollected = 0;
    int goldCollectedTotal = 0;
    int goldLeftInGame = game->totalGold;
 
    // Create the gold message and send it
    char goldMessage[100];
    sprintf(goldMessage, "GOLD %d %d %d", goldCollected, goldCollectedTotal, goldLeftInGame);
    spectator_sendMessage(spectator, goldMessage);

}

/**************** sendPlayerDisplayMessage() ****************/
/* See top of the file for the description */
void 
sendPlayerDisplayMessage(gamestatus_t* game, player_t* player)
{
    // Update player's visible grid
    grid_t* mainGrid = game->grid;
    grid_t* originalGrid = game->originalGrid;
    grid_calculateVis(mainGrid, player->grid, originalGrid, extractRowFromPosition(player->position, mainGrid->ncol), extractColumnFromPosition(player->position, mainGrid->ncol));

    player->grid->gridArray[player->position] = '@';

    // grid_toString sends a string in the format: 'DISPLAY\n[grid with rows seperated by \n]'
    char playerGridAsString[10000];
    grid_toString(player->grid, playerGridAsString);
    player_sendMessage(player, playerGridAsString);
    
}

/**************** sendSpectatorDisplayMessage() ****************/
/* See top of the file for the description */
void 
sendSpectatorDisplayMessage(gamestatus_t* game, spectator_t* spectator)
{
    // No need to update display if there is no spectator
    if (spectator == NULL) {
        return;
    }

    // grid_toString sends a string in the format: 'DISPLAY\n[grid with rows seperated by \n]'
    grid_t* mainGrid = game->grid;
    char mainGridString[message_MaxBytes];
    grid_toString(mainGrid, mainGridString);
    spectator_sendMessage(spectator, mainGridString);
}

/**************** extractRowFromPosition() ****************/
/* See top of the file for the description */
int 
extractRowFromPosition(int position, int ncol)
{
    return (int) position / (ncol + 1);
}

/**************** extractColumnFromPosition() ****************/
/* See top of the file for the description */
int 
extractColumnFromPosition(int position, int ncol)
{
    return position % (ncol + 1);
}

/**************** convertCoordinatesToPosition() ****************/
/* See top of the file for the description */
int 
convertCoordinatesToPosition(int r, int c, int ncol)
{
    return  r * (ncol + 1) + c;
}

/**************** isGameDone() ****************/
/* See top of the file for the description */
bool isGameDone(gamestatus_t *game)
{
    if (game->totalGold <= 0) {
        game->gameOver = true;
    } else {
        game->gameOver = false;
    }
    return game->gameOver;
}

/**************** endGame() ****************/
/* See top of the file for the description */
void endGame(gamestatus_t *game) {
    player_t **allPlayers = (player_t **)game->players;
    if (allPlayers == NULL) {
        log_v("The allPlayers array has not initialized properly for some reason...\n");
        return;
    }

    int numPlayers = game->numPlayers;
    if (numPlayers <= 0) {
        log_v("No players to process...\n");
        return;
    }

    // Calculate the maximum required size for the endMessage buffer
    size_t maxMessageSize = numPlayers * (MaxNameLength + 20) + 25;
    char *endMessage = calloc(maxMessageSize, sizeof(char));
    if (endMessage == NULL) {
        log_v("Memory allocation failed for endMessage...\n");
        return;
    }

    strcpy(endMessage, "QUIT GAME OVER:\n"); // Initialize with a message

    for (int i = 0; i < numPlayers; i++) {
        if (allPlayers[i] == NULL) {
            log_v("Skipping uninitialized player slot...\n");
            continue;
        }
#ifdef DEBUGPRINT
        printf("Player Name in endGame Before Formatting: %s\n", allPlayers[i]->name);
#endif 

        char tempBuffer[MaxNameLength + 20];
        snprintf(tempBuffer, sizeof(tempBuffer), "%c   %d    %s\n", 
                getPlayerLetter(allPlayers[i]->ID), allPlayers[i]->score, allPlayers[i]->name);

#ifdef DEBUGPRINT
        printf("Generated Message for Player %d: %s", i, tempBuffer);
#endif
        // Append to the message
        strncat(endMessage, tempBuffer, maxMessageSize - strlen(endMessage) - 1);
    }


    // Send the message to all players
    for (int i = 0; i < numPlayers; i++) {
        if (allPlayers[i] != NULL) {
            player_sendMessage(allPlayers[i], endMessage);
        }
    }

    // Send the message to the spectator, if present
    if (game->spectator != NULL) {
        spectator_sendMessage(game->spectator, endMessage);
    }

    // Free the dynamically allocated buffer
    free(endMessage);
}

/**************** goldPickedUp() ****************/
/* See top of the file for the description */
void 
goldPickedUp(gamestatus_t* game, player_t* player, int goldPileValue)
{
    int currentPlayerGold = player->score;
    int goldLeftInGame = game->totalGold;

    // Format individual GOLD message and send to player
    char goldCollectedMessage[50];
    sprintf(goldCollectedMessage, "GOLD %d %d %d", goldPileValue, currentPlayerGold, goldLeftInGame);
    player_sendMessage(player, goldCollectedMessage);

    sendUpdatedGold(game);
}

