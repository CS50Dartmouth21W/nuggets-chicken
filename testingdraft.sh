./server 2>server.log maps/main.txt
port=$(tail -n 1 server.log | grep -o '[0-9]*')
./player 2>player.log localhost $port jason
./player 2>spectator.log localhost $port