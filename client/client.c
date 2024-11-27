/*****
 * client.c
 * 
 * Leah Branstetter
 * Big D Nuggets group
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h> 
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "../support/log.h"
#include "../support/message.h"

/************* function declarations **********/
static bool handleMessage(void* arg, const addr_t from, const char* message);
static void handleOK(const char* message);
static void handleGrid(const char* message);
static void handleGold(const char* message);
static void handleDisplay(const char* message);
static bool handleQuit(const char* message);
static void handleError(const char* message);
static void handleDisplay(const char* message);
static void constantHeader(bool isPlayer);
static bool handleInput(void* arg);
int main(const int argc, char* argv[]);

/***************** global vars ********/
char character_letter;
int current_gold;
int unclaimed_gold;
bool isPlayer;


/***************** main *******************************/
/* Much of this code comes from the miniclient.c file provided in the lab starter kit
* written by David Kotz - May 2021
* 
* modified to fit the specs of the nuggets assignment
* now checks for a playername argument and sends a message to the server to initialize the game
* it also 
 */
int
main(const int argc, char* argv[])
{
  log_init(stderr);
  // initialize the message module (without logging)
  if (message_init(stderr) == 0) {
    exit(2); // failure to initialize message module
  }

  // check arguments
  const char* program = argv[0];
  // check if the arguments are 3 or 4: 
  // invalid commandline:
  if (argc < 3 || argc > 4) {
    log_s("usage: %s hostname port", program);
    exit(3); // bad commandline
  }

  // commandline provides address for server
  const char* serverHost = argv[1];
  const char* serverPort = argv[2];
  addr_t server; // address of the server
  if (!message_setAddr(serverHost, serverPort, &server)) {
    log_s("can't form address to %s", serverPort);
    exit(4); // bad hostname/port
  }

  // if they are three, send a message to the server saying this is a spectator:
  if (argc == 3) {
    // send message to server with "SPECTATOR"
    message_send(server, "SPECTATE");
    isPlayer = false;
  }
  // if it's 4 arguments, send a message that it's a player character:
  if (argc == 4) {
    // send message to server with "PLAYER arg[4]"
    char tosend[58];  // 58 is for max player name length
    snprintf(tosend, sizeof(tosend), "PLAY %s", argv[3]); 
    message_send(server, tosend);
    isPlayer = true;
  }
  
 // if all arguments are good: initialize the window for ncurses:
  initscr();

  // Turn off line buffering and special character handling
  cbreak();
  noecho();
  // Get user input as raw characters
  keypad(stdscr, TRUE);

  // we will display on the terminal in the handle messages functions

  // Loop, waiting for input or for messages; provide callback functions.
  // We use the 'arg' parameter to carry a pointer to 'server'.
  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
  

  // shut down the message module
  message_done();

  
  return ok? 0 : 1; // status code depends on result of message_loop
}

/**************** handleInput ****************/
/* This code comes from the miniclient.c file provided in the lab starter kit
* written by David Kotz - May 2021
* see miniclient.c for further documentation
* modified to fit the specs of the nuggets assignment
* now input is sent in the format outlined in requirements
 */
static bool
handleInput(void* arg)
{
  // We use 'arg' to receive an addr_t referring to the 'server' correspondent.
  // Defensive checks ensure it is not NULL pointer or non-address value.
  addr_t* serverp = arg;
  if (serverp == NULL) {
    log_s("%s", "handleInput called with arg=NULL");
    return true;
  }
  if (!message_isAddr(*serverp)) {
    log_s("%s", "handleInput called without a correspondent.");
    return true;
  }
  
  char key = getch();
  
  // Allocate space for the final string
  char character[2]; 
  character[0] = key;
  character[1] = '\0';

  // Format the string and store it in 'tosend'
  char* tosend = malloc(6*sizeof(char));
  strcpy(tosend, "KEY ");
  strcat(tosend, character);

  // send as message to server
  message_send(*serverp, tosend);
  // normal case: keep looping
  return false;

}

/**************** handleMessage ****************/
/* Message received, send it to the appropriate helper function for printing
 * We ignore 'arg' here.
 * Return true if any fatal error.
 * This function assumes that the message will be sent in the correct format!
 */
static bool
handleMessage(void* arg, const addr_t from, const char* message)
{   
 
  // validate commandline
  if (message == NULL){
    log_s("%s", "Blank message received."); 
    return false;
    // we will not return true here because even if the server sends an invalid message, the client ignores it
  }
  
  if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) { // if GOLD
    handleGold(message);
  }

  else if (strncmp(message, "OK ", strlen("OK ")) == 0) { // if OK
    handleOK(message);
  }

  else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) { // if grid
    handleGrid(message);
  }

  else if (strncmp(message, "DISPLAY", strlen("DISPLAY")) == 0) { // if display
    handleDisplay(message);
  }

  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) { // if quit
    return handleQuit(message);
  }

  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) { // if error
    handleError(message);
  }
  
  else{ // if the message is not one of the server message types or is in incorrect format
    log_s("%s", "Invalid message sent to client.");
    // we will not return true here because even if the server sends an invalid message, the client ignores it
  }

  // keep looping:
  return false;
}

/********* helper functions for handleMessage **********/

/**************** handleOK function 
 * takes in the message. Prints out a message informing the client that they have successfully joined the game. 
 * prints out message in the middle of the screen.
*/
static void handleOK(const char* message){
  // print the message that will be displayed attop the screen

  character_letter = message[3]; // assume that the message is in format "OK X" where x is the letter

  mvprintw(0, 0, "Player letter %c has joined the game.", character_letter);
  // refresh the screen with new content:
  refresh();

  // Wait for 2 seconds (2000 milliseconds)
  napms(2000);
  
  // move the cursor to the start of the line:
  move(0, 0);
  // Erase the printed message
  clrtoeol();  // Clears from the current cursor position to the end of the line
  // Refresh the screen to show the cleared content
  refresh();
}

/*************** handleGrid function ***********
 * takes in a message with visible part of the grid to be displayed to the client
 *
 * scans and parses the message, then checks if the terminal is an appropriate size
 * if the terminal is not an appropriate size it prints a message to the client asking the client to resize the window or press y
 * if the client presses y, we will resize the window for them.
 * otherwise, we will assume that the client resizes the window on their own.
 * 
 * send logs to stderr if the client presses a non-y key
 * does not return NULL if anything is invalid and simply sends logs if it encounters an error.
*/
static void handleGrid(const char* message){
int nrows, ncols;
    // Parse the GRID message
    if (sscanf(message, "GRID %d %d", &nrows, &ncols) != 2) {
        log_s("%s", "Message format is incorrect for GRID.");
    }

    // Get the current terminal dimensions
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Check if the current window is too small
    if (nrows > rows || ncols > cols) {
        // Clear the screen before displaying the prompt
        clear();

        // Print message telling the user the window is too small
        mvprintw(0, 0, "Window is too small to display the grid (%d x %d).", nrows, ncols);
        mvprintw(1, 0, "Press 'a' to resize the window automatically or resize the terminal manually.");
        refresh();  // Update the screen with the prompt

        // Loop until the user either resizes manually or presses 'a' to auto-resize
        while (nrows > rows || ncols > cols) {
            // Wait for user input
            int ch = getch();
            if (ch == 'a' || ch == 'A') {
                // If the user presses 'a', automatically resize the window
                if (resize_term(nrows, ncols) == OK) {
                    // If resizing is successful, break and proceed
                    break;
                } else {
                    // If resizing fails, display a failure message
                    mvprintw(3, 0, "Failed to resize the terminal.");
                    refresh();
                    napms(2000);  // Wait for 2 seconds before continuing
                }
            } else if (ch == ERR) {
                // Handle if there's an error reading input (e.g., user closes terminal)
                log_s("%s", "Error reading input.");
                break;
            } else {
                // If any other key is pressed, show the prompt again
                clear();  // Clear the screen to refresh the prompt
                mvprintw(0, 0, "Window is too small to display the grid (%d x %d).", nrows, ncols);
                mvprintw(1, 0, "Press 'a' to resize the window automatically or resize the terminal manually.");
                refresh();  // Redraw the prompt
                constantHeader(isPlayer);
            }
        }
    } else {
        // If the window is large enough, proceed without needing to resize
        constantHeader(isPlayer);
        refresh();
    }
}

/******************* handleGold function *************
 * 
 * this takes in 3 integers (and assumes that they are in the correct order and format)
 * it does not check for invalid integers or negative numbers. 
 * if the format is incorrect it will log an error message
 * 
 * otherwise it updates the screen with a gold received message and it updates the global variables for current_gold and unclaimed_gold
*/
static void handleGold(const char* message){
  int n, p, r;

  // Parse the message using sscanf
  if (sscanf(message, "GOLD %d %d %d", &n, &p, &r) == 3) {
    // print the message that will be displayed attop the screen as the player plays
    current_gold = p;
    unclaimed_gold = r;

    // print the GOLD received message to the right of the header
    constantHeader(isPlayer);
    mvprintw(0, strlen("Player A has 39 nuggets (211 nuggets unclaimed).") + 1, "GOLD received: %d", n);

    // Refresh the screen to show the content
    refresh();

    timeout(1000);  // Non-blocking input with 100ms timeout
    nodelay(stdscr, TRUE); // Make getch() non-blocking
  
    move(0, strlen("Player A has 39 nuggets (211 nuggets unclaimed).") + 1);  // Position at where the second message started

    // Erase the printed message
    clrtoeol();  // Clears from the current cursor position to the end of the line

    // Refresh the screen to show the cleared content
    constantHeader(isPlayer);
    refresh();

  }
  // if you can't parse the message
  else{
    log_s("%s", "Message format is incorrect for GOLD.\n");
  }
  // refresh the screen with new content:
  constantHeader(isPlayer);
  refresh();
}

/*********************** handleDisplay function ************
 * 
 * takes in a message of what to display and simply prints it all out starting right below the header
 * 
 * will log an error if the message is null 
 * otherwise prints out whatever it gets starting from the second row of the window
*/
static void handleDisplay(const char* message){

  const char* newline_pos = strchr(message, '\n');
  if (newline_pos == NULL) {
    // If no newline found
    log_s("%s", "NULL display message sent.\n");
  }

  // Move past the newline character to get the string that follows
  const char* string_to_display = newline_pos + 1;

  // Move the cursor to the second row (skipping the top line)
  move(1, 0);  // Row 1, Column 0 (first column of the second row)

  // clear everything below the text at the top of the screen.
  clrtobot();

  // Add the string to the screen
  printw("%s", string_to_display);

  // refresh the screen with new content:
  constantHeader(isPlayer);
  refresh();
}

/*********************** handleQuit **********
 * 
 * this just takes in a a quit message and closes the terminal and prints out the message to stdout
 * 
 * it makes sure the message isn't NULL before printing
 * 
*/
static bool handleQuit(const char* message){
  // close the screen
  endwin();
  // print the quit message to stdout:
  if (message != NULL){
    fprintf(stdout, "%s\n", message+5);
  }
  return true;
}

/****************** handleError function  *************
 * This just prints out any error messages to the log
 * it doesn't check anything, it just prints out whatever you send it
*/
static void handleError(const char* message){
  log_s("%s", message);
}
/************ constantHeader
 * this doesn't take in anything 
 * it just prints out the header across the top of the screen for the client
 */
static void constantHeader(bool isPlayer){
  if (isPlayer) {
    // constantly print this header: (note that other things will be printed to the right of it)
    mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed).", character_letter, current_gold, unclaimed_gold);
  } else {
    mvprintw(0, 0, "Spectator: %d nuggets unclaimed.", unclaimed_gold);
  }
  
  // refresh the screen
  refresh();
}