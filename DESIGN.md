## User Interface
The nuggets game requires command-line input from the user to initialize the server and players. Once the server is initialized, server status messages are displayed. Once players join the game, the command-line is replaced by a graphical user interface (GUI) in which to play the game. This process is described in detail below.
 
### Server
 
The server is initialized with the following command:
 
```bash
./server map.txt [seed]
```
 
Where map.txt is a valid path to a map file made for the nuggets game, and seed is an optional seed for producing consistent gold-pile placement across games.
 
### Player
 
To join a server that has already been setup, the following command is used:
 
   ```bash
   ./player hostname port [playername]
   ```
  
The first argument is the hostname or IP address of the server, and the second argument is the port the server expects to receive messages from the player on. The third argument is an optional playername. If the `playername` argument is provided, the user joins as a player and can interactively play the game. If no `playername` is provided, the user joins as a spectator.
 
Upon joining as either a spectator or a player, the user is presented with a GUI for playing the game, consisting of a view of the game map, player positions, and a message about relevant game information, such as the score. While a spectator may see the entire map and the positions of every player, a player user may only see the map, nuggets, and other players that are in their line of sight.
 
## Inputs and Outputs  
 
***Inputs***: The initial inputs to the game are the command-line parameters and the contents of the map file specified by the first parameter in the server command-line. See the User Interface section above.
 
Once the game is started and has players, players also provide key strokes as input to play the game. These keystroke inputs, which are typed on the command-line, are sent to the server, processed, and then instructions to update the game is received from the server.
 
***Output***: Besides the GUI, which the game constantly displays while being played or spectated, a game summary is printed to the command-line after the game ends. Errors or other useful information will be logged to stderr, and users are encouraged to redirect this output to their own log file.

## Functional decomposition into modules
A function that validates/processes the command line and initiates data structures
A `map_loader` function that loads the map
A `gold_generator` function that drops gold randomly in the map
A function that listens for, processes messages from clients, and calls appropriate functions
A function that updates the map for all players according to the messages
Module that sends messages to client (players, spectator)
A function that adds/delete players
Commands to send:
`PLAY`
`SPECTATE`
`KEY`
`QUIT`
`DISPLAY`
`GRID`
`GOLD`
 




## Pseudo Code

### Server

The server will run as follows:

1. Start from the command-line as shown in the User Interface
2. Parse the command-line, validate parameters, initialize other modules
3. Use `map_loader` to load the map
4. Use `gold_generator` to randomly place gold on the map
5. While there are still gold piles to be collected
  
   5.1 Wait for messages from clients.

   5.2 Handle messages including players and spectators joining and quitting, and players doing actions.

       5.2.1 Check to make sure MaxPlayers is not met when adding a new player

       5.2.2 If a second spectator tries to join, remove the first spectator, then add the new spectator

   5.3 Send appropriate response messages after processing a message to update player and spectator clients

6. Prepare a summary of player scores
7. Send clients a quit message with summary and print the summary
8. Clean up data structures and free memory, then terminate the server

### Player

The player will run as follows:

1. Start from the command-line as shown in the User Interface
2. Parse the command-line, validate parameters, initialize other modules
3. If a player name is provided:

   3.1 Send a message to join as a player

4. If a player name is not provided:

   4.1 Send a message to join as a spectator

5. Initialize ncurses to process keystrokes
6. Ensure the window sizes is big enough to display the grid provided in an incoming `GRID` Message
7. While not receiving a quit message from the server:

   7.1 Handle incoming messages regarding displaying the game and updating the display

   7.2 Send `KEY` messages corresponding to keystrokes to take actions in the game

8. Receive a quit message and print the game summary
9. Disconnect from the server, clean up memory and structures, and terminate the program.

