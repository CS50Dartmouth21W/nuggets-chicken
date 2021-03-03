# Nuggets Implementation Spec
### Team Chicken

## Pseudo Code for Major Components

#### map_loader
1. Open the file passed in the function parameters
1. IF the file is null return an error
1. Else use freadlinep() to read lines of the file into strings
1. While there are still strings to be read, insert the strings into an  array
1. After looping, insert the array of strings into the hashtable passed as a parameter with a key ‘main’ which will be used as the main/spectator map.

#### gold_generator
Note: srand() is called in main, behavior is already seeded. This implementation is also rather inefficient (the function may have to iterate over the map multiple times, which could be thousands of characters), but does so in order to introduce as much random behavior into gold pile generation as possible.
1. Create a counters struct with position in map as the key and the number of gold in the pile as the counters
2. Choose a random number of piles between the min and the max to create
2. Create a variable to track number of gold to left to be dropped
2. While there is still gold to be dropped
2. Choose a random row and column. If that position is a room spot (‘.’)
2. If number of piles is 1, drop the rest of the gold in that last pile
2. If there are more than 1 piles left, drop between 1 and `maxGoldInPile`, where `maxGoldInPile` is the maximum gold that can be in the pile in order to have at least one gold per remaining pile
2. Create a new counters object with the position as key and number of gold in pile as counters
2. Change the character at the row and column to an asterisk
2. Subtract number of gold in pile from the gold to be dropped
2. Decrement pile by one
2. Return the counters struct

#### get_visible
1. Use the provided key parameter to access the correct map in the hashtable.
1. Create a new empty map based on the dimensions of the accessed map.
1. Perform logic to get visible characters from provided position parameter
1. For every character that is visible, add it to the empty map.
1. Return the new map

#### update_maps
1. Loop through every map except the main map
1. Iterate through the map incrementing a position integer until the character corresponding to the map is found
1. Call get_visible passing it the current key, its map, and the position of that key.
1. Add every character that is not blank in the returned map to the keys current map

#### create_Player
1. While true:
1. Set int row to a random number between 0 and width of map - 1
1. Set int column to a random number between 0 and height of map - 1
1. If that position in map is a room spot
1. Create a new player struct with the name, row, and column
1. Change the character at that row and column to a ‘@’
1. Add the player to a data structure of players
1. Break out of loop
1. Return the player

### handle_message
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


## APIs and Function Prototypes

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
* hashtable of clients
Has name as key and player struct as item
Keeps track of all players and their information
* spectator 
Stored as a key in the maps hashtable
* maps
hashtable of maps for individual players and for the spectator/main map
* message
leverages the message data type that will be sent from the server to the players and spectators
* counters
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

