# Testing Documentation for client
Most of the testing for client was done in the terminal using one terminal to run client and one terminal to run miniclient

### test1: send a spectator message to the server
This test actually worked the first time. Client sends miniserver a message "SPECTATOR"

### test2: send a player message to the server
#### 1. test failure: 
prints out:
START OF LOG
Segmentation fault (core dumped)
nothing was logged in player.log

#### 2. Changed how I use strcat, new implementation fixed it
Now it sends a message to server with "PLAYER playername" 
Only concerning thing is that no player.log file was created (possibly because there was nothing to log in this simple case)
Also! the server only printed out the initial message and did not continue to print out further messages. (for example it did not print KEY messages)
I think this is part of the limitation of miniserver?

### test3: test with more complex server that sends messages back to client