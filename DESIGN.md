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


## Major data structures

counters to keep track of points for all players
2D array or characters to track map
Array of client structs to track information of clients
A singular spectator struct that represents a spectator and what they can see
One overarching struct containing all game information that represents the state of the game that can easily be passed between modules. Struct will contain multiple data structures. The game struct will reference a grid object containing all information and an array of players that represent the players of the game. Each player struct will track all current information about a user including their location, amount of gold, and current visibility spectrum. 
We will also make use of a message data type that will be sent from the server to the players and spectators.

## Dataflow through modules
server.c 
This is where command line arguments are parsed
The game is initialized
Random piles of gold are dropped
Handles communication flow logic with communication.c
communication.c
This is where all communication is done with client interaction occurs with both the players and the spectators. This module is responsible for networking
Makes use of message.h module in order to receive and send information
grid.c
Lets the user track visibility based off of a point
Handles geometric logic
Uses to simplify grid logic

server.c serves (haha) as the entry point for the user. It initializes the game and runs a constant listener loop that looks for message_t structs from communication.c. when such a struct appears, it is parsed and the resulting effect takes place within the game. server.c will also use grid.c to store the 2d array grid of where everyone is, as well as handles visibility for users that is passed on through server.c with the communication.c module. The server will also keep track of all of the users in an array in a struct player_t which keeps track of all current information from the user who is playing the game.

##Testing Plan
*Unit testing*. A small test program to test each module to make sure it does what it’s supposed to do.
*Integration testing*. Assemble nuggets server and test it as a whole. In each case, test spectator, different maps, varying number of players and different gold values. Ensure that map files are not empty. Print messages from the server as it proceeds (e.g., describing the grid, client information, game status updates) so you can watch its progress as it runs.
Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
Test the server with an empty or non-existent map file, missing permissions, different directory
Test the server with various sized maps (e.g., not enough room for Max Gold, large files, etc.)
 Test the server with varying seeds (e.g., null, extremely high numbers, alpha and special characters)
 Test the server with different keystrokes and inputs
Test the server with multiple spectators, and when spectator quits
Test the server with various player names (e.g., lengths, many spaces, special or non-alpha characters, or null)
Test and verify that players that quit cannot rejoin, or be replaced
Test the server with more than Max Players
Verify handling of malformatted messages
 


