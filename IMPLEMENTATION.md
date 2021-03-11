# Nuggets Implementation Spec
### Team Chicken

## Pseudo Code for Major Components

#### map_loader

This function loads the map into an array of pointers to chars.

1. Open the file passed in the function parameters
1. IF the file is null return an error
1. Else use freadlinep() to read lines of the file into strings
1. While there are still strings to be read, insert the strings into an  array
1. After looping, insert the array of strings into the hashtable passed as a parameter with a key ‘main’ which will be used as the main/spectator map.

#### gold_generator

This function drops a random number of gold piles between min and max with a random number of gold in each pile.

Note: srand() is called in main, behavior is already seeded. This implementation is also rather inefficient (the function may have to iterate over the map multiple times, which could be thousands of characters), but does so in order to introduce as much random behavior into gold pile generation as possible.
1. Create a counters struct with position in map as the key and the number of gold in the pile as the counters
2. Choose a random number of piles between the min and the max to create
2. Create a variable to track number of gold to left to be dropped
2. While there is still gold to be dropped
2. Choose a random row and column. If that position is a room spot (‘.’)
2. If number of piles is 1, drop the rest of the gold in that last pile
2. If there are more than 1 piles left, drop between 1 and `maxGoldInPile`, where `maxGoldInPile` is the maximum gold that can be in the pile in order to have at least one gold per remaining pile
2. Change the character at the row and column to an asterisk
2. Record number of gold in pile in gold counts map
2. Subtract number of gold in pile from the gold to be dropped
2. Decrement pile by one
2. Return the counters struct

#### updateVisibility
Updates the visibility map of player
1. Create a map tracking visited locations. All “slots” set to false initially
2. Perform Depth First Search on the players location:
	2.1. Mark location as visited
	2.2. Loop through all 8 neighboring points
	2.3 If point is in bounds and is visible
		2.3.1 Mark location as visible in player visibility map
		2.3.2 Call dfs on that location
3. Change the character at player’s location in visibility map to ‘@’

#### isVisible
Returns whether a point is visible from player's perspective
1. If point is on same row, loop through the columns and return if any character in row is not roomspot or gold
2. If point is on same column, loop through the rows and return if any character in that column is not roomspot or gold
3. Else
	3.1 Calculate the slope of the line between player and point (Δcolumn/Δrow)
	3.2 Set to ints j1 and j2 to the column with lower row number. These variables track the columns between the two points of interest
	3.3 Loop through rows between the player and point
	3.3.1 Increment j1 by floor of slope and j2 by ceiling of slope
	3.3.2 Return false if point not in bounds or character at the two locations are not room spot or gold
	3.3.3 Repeat 3.1-3.3.2, but this time switching rows and columns (ie, Looping through columns and incrementing rows by Δrow/Δcolumn)
4. return point is visible 


#### player_new

This function adds a new player struct into the hashtable of clients and drops the player in a random room spot.

1. Create new player struct and initialize variables
2. Initalize a visibility map for player. Characters at all slots initially space character
3. Set the players letter to 'A'
4. Find a random room spot and update player's location to that position in game map and player's visibility map
5. Add player to hashtable of players
6. Return the player

### handle_message

This function processes messages passed between server and client.

1. Server is updated by calling message_send (sends message to the server)

Handle Message – part of the message_loop
The following messages are sent from the server to clients:
1. “GRID “

	a. Followed by the columns and rows of the map, which describe the size

	b. Validate that the user’s screen can fit the map

2. “GOLD “

	a. Followed by a string that displays the amount of gold that was just collected by a client, the amount of gold that is in their inventory, and the remaining gold

	b. Update display message

3. “DISPLAY “

	a. Followed by a string that represents the map, update maps

4. “QUIT “

	a. Followed by the message or error

		i. Too many players

		ii. A player quits

		iii. A spectator quits

		iv. A spectator is replaced

		v. Game over (all the gold is collected) printed with game summary


The following messages are sent to the servers

1. “PLAY “

	a. Followed by a player name

	b. If there are spots left

		i. Save player name, send received message

		ii. Send grid and gold messages

		iii. Update other players
	c. Otherwise game is full

		i. Send QUIT message

2. SPECTATE

	a. If there isn’t another spectator

		i. Add spectator

	b. Otherwise, replace spectator

	c. Send grid and gold message

3. “KEY “

	a. Followed by a key

	b. Verify player that sent message

	c. Handle keystroke

		i. Move player

		ii. Collect and update gold

		iii. Quit game

		iv. Error message if invalid key

#### move
Moves a player by dx and dy
1. initiate int variables new row and new column
2. Check if new position is movable spot (coordinates in bounds and character is room spot or hallway spot
	2.1 Replace current position character to original character (should be kept track of in player struct)
	2.2 Record character at new location
	2.3 Move to that new position if not occupied.
	2.4 If occupied, switch location with player
	2.5 Update status if gold is collected

#### quit
Builds and sends a string message to quit the game
1. Build a character string containing quit message
2. Call message_send to send message to player

#### quitGame
Handle a player and spectator that quit the game
1. if address is from spectator, call quit function on spectator and set address to NULL
2. else handle quit for player, decrement number of players by one

#### sendGridInfo
Send grid message with rows and columns information
1. calculate the number of digits in row and columns
2. concatenate GRID message with rows and columns and call `message_send`

#### sendGoldInfo
Send gold message with amount of gold found
1. if player is null, set purse to zero, else set purse to the players gold value
2. Calculate the number of digits in gold info
3. Concatenate gold message and call message_send

#### sendDisplay
Send display message with map information
1. get number of rows and columns
2. Call hashtable_iterate for players hashtable, passing in a helper method that copies player's visibility map into a string and calls `message_send`
3. If spectator exists, copy game map into a string and call `message_send`

### sendGameOver
Send game over message
1. allocate memory and concatenate GAME OVER message
2. call `hashtable_iterate` on players hashtable, passing in a helper method that creates string with player letter and gold count
3. if spectator exists, send message to spectator


## APIs and Function Prototypes

### communication module

```c
void quit(const addr_t addr, const char *reason);
void quitGame(game_t *game, addr_t addr);
void sendOK(player_t *player);
void sendGridInfo(game_t *game, addr_t addr); 
void sendGoldInfo(game_t *game, player_t *player, addr_t addr, int n);
void sendDisplay(game_t *game, const addr_t addr);
void sendGameOver(game_t *game, addr_t addr); 
int getNumDigits(int a); player_t *getPlayerByAddr(game_t *game, const addr_t addr); 
player_t *getPlayerByChar(game_t *game, char c); 
void find_player(void *arg, const char *key, void *item); 
void find_player2(void *arg, const char *key, void *item); 
void create_message(void *arg, const char *key, void *item); 
void broadcast(void *arg, const char *key, void *item);
```

### game module
```c
game_t *game_new(char *map[], int rows, int cols, int MaxPlayers, int TotalGold); 
void game_delete(game_t *game);
void addSpectator(game_t *game, addr_t addr);
```

### player module
```c
player_t *player_new(char *name, game_t *game, const addr_t addr);
void player_delete(void* item);

```


```c
map_loader(File* fp, hashtable_t *map_hash);
gold_generator(hashtable_t *map_hash, int min, int max, int total, int ncols);
get_visibile(hashtable_t *map_hash, char key, int position);
update_maps(hashtable_t *map_hash);
handle_message(void *arg, const addr_t from, const char *message);
create_player(char *name, game_t* game);
main(int argc, char* argv[]);
```

## Data Structures
* hashtable of clients:
Has name as key and player struct as item
Keeps track of all players and their information
* spectator: 
Stored as an item in the maps hashtable
* maps:
hashtable of maps for individual players and for the spectator/main map. The player name is key and array is item.
* message:
leverages the message data type that will be sent from the server to the players and spectators
* counters:
Used to keep track of number of gold in gold piles

## Resource Management

Dynamically allocated memory will be avoided if possible. For example, the maps will not be malloc’d but will instead be allocated by using char* map[rows]. Structs will be the main way in which memory is allocated, and structs will all be freed at the end of the game besides messages, which are not needed to store information past when the character is processed. 

## Error Handling and Recovery
If command line invalid, provide error statement and return with non-zero status
Handle malloc failures gracefully, including printing an error statement
Any message not following the above protocol, exactly, is malformatted. Print a client to the user in the form `ERROR explanation` explaining what error occurred. The server should ignore the malformatted message

## Persistent Storage

We will save information such as messages the server receives from players and spectators as well as error messages to a log file. Log files will be used for debugging purposes. The server should log useful information that can be saved in a logfile; a typical approach would be to log to stderr and thus usage could be:

./server 2>server.log map.txt

A Map file will be provided to server.c that will be read in in order to initialize the game. Map files are provided to us and we are solely responsible for reading the file and not writing to it.




 
