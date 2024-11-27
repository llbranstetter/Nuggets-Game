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
I believe it also started up the ncurses window appropriatly in terminal (not sure tho)
Also! the server only printed out the initial message and did not continue to print out further messages. (for example it did not print KEY messages)
I think this is part of the limitation of miniserver?

### test3: test with more complex server that sends messages back to client
1. Starts up and connects, but cannot print a player message with the player letter
    fix: the way I was setting global vars was wrong. I changed that so that the player letter is set correctly. 
2. The client send key messages were formatted incorrectly. I had to change how to concatinate with "KEY "
3. The client is sending and receiving all types of messages correctly. I am not seeing any bugs so far.
    I have run it with both prof Palmer's server, and with our groups server, and it looks like everything is working correctly.
    Also the extra credit worked first try without any issues. Yay!

### valgrid testing:
1. tried running with valgrind and quitting with ^C. This results in a lot of memory loss because messageloop is never ended and `endwin()` is never called.
Here is the valgrind output:
==1272858== HEAP SUMMARY:
==1272858==     in use at exit: 105,834 bytes in 416 blocks
==1272858==   total heap usage: 466 allocs, 50 frees, 141,121 bytes allocated
==1272858== 
==1272858== LEAK SUMMARY:
==1272858==    definitely lost: 0 bytes in 0 blocks
==1272858==    indirectly lost: 0 bytes in 0 blocks
==1272858==      possibly lost: 201 bytes in 3 blocks
==1272858==    still reachable: 105,633 bytes in 413 blocks
==1272858==         suppressed: 0 bytes in 0 blocks
==1272858== Rerun with --leak-check=full to see details of leaked memory
==1272858== 
==1272858== For lists of detected and suppressed errors, rerun with: -s
==1272858== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
When I ran it with leek-check=full, it let me know that the memory leaks come from the message and ncurses functions, which let me know that it was from the main function never reaching the `message_done()` function.

We're going to ignore this since we assume that the player doesn't quit using ^C... 