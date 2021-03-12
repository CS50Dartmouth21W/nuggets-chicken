# Nuggets Testing
### Team Chicken

## Testing

We completed our testing as a team to ensure that our program ran accordingly and implemented some of our testing in the testing.sh file. 

We tested our program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

These included testing valid parameters such as:
```bash
./server maps/main.txt
```
```bash
./server maps/narrow.txt 10000
```
```bash
./server maps/big.txt 2
```

As well as invalid parameters such as:

Too many arguments
```bash
./server maps/main.txt 3 hello
```
Too few arguments
```bash
./server
```
Invalid seeds
```bash
./server maps/main.txt %___
```

### Additional server testing
1. Tested empty and non-existent map files
2. Tested files with missing permissions
3. Tested files in a different directory
4. Tested various map and seed sizes

### Game testing
1. Tested the server with different keystrokes and inputs
    1. verified that players stay in map boundaries
    2. tested special movement characters function properly
2. Tested the server with multiple spectators, and when spectator quits
3. Tested the server with various player names, including: 
    1. different lengths
    2. many spaces
    3. special or non-alpha characters
    4. null
4. Tested and verified that players that quit cannot rejoin or be replaced
5. Tested the server with more than Max Players
6. Tested handling of malformatted messages
7. Tested the visibility of our program
