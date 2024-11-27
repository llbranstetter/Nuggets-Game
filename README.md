# Nuggets
## Team Big D Nuggies, Fall 2024
### Rana Moeez Hassan, Jake Fleming, Leah Branstetter, Kellen Seeley

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## Usage

The *server* and *client* are the two executables for this game. The *server* module takes two parameters `map.txt [seed]` which reference the file path name to a map and an optional seed for the randomization. The *client* module takes three parameters `hostname port [playername]` which reference the hostname and port you want to connect on and the optional playername. If you don't enter a playername you will join as a spectator. Running these must occur on separate terminals or devices, and if you are in the main directory, may look something like this, routing the logs to new files:

```
./server/server 2>server.log ./maps/map.txt
./client/client 2>client.log plank 12345 player1
```

## Implementation

See the implementation spec for more details here.

## Assumptions

- Assume the given file is a valid mapfile, and example of a valid mapfile we wrote and tested is ./maps/bigDmap.txt
- For our DISPLAY message, we use a buffer string that has space for 10,000 characters, so when sending these messages, we must not have a map with an area greater than 9,992 characters (because the message begins with "DISPLAY\n). 

## Files and Directories

- `client` - Directory containing client module and test logs/findings
- `clienttypes` - Directory containing player and spectator modules and tests
- `gamestatus` - Directory containing gamestatus module
- `gold` - Directory containing gold module and test
- `grid` - Directory containing grid module and test
- `maps` - Directory containing many valid mapfiles
- `server` - Directory containing server module and test logs/findings
- `support` - Directory containing provided modules such as message, file, and log
- `DESIGN.md` - Design spec
- `IMPLEMENATION.md` - Implementation spec
- `REQUIREMENTS.md` - Requirements spec
- `Makefile` - compilation procedure

## Compilation

Simply run make in the main directory, and all the necessary executables, .o files, and libraries will compile.

## Testing

See results in client and server directories written up in testing.md, clienttesting.md, and servertesting.md. The implementation spec also lays out the testing procedures.
