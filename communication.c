#include <stdlib.h>
#include <stdbool.h>

bool handleTimeout(void *arg);
bool handleInput  (void *arg),
bool handleMessage(void *arg,const addr_t from, const char *message);


bool handleTimeout(void *arg){

}

bool handleInput  (void *arg){

    return true;
}

// TODO: update move to indclude the from
//       work out handleTimeout
//       work onn handleInput

bool handleMessage(void *arg, const addr_t from, const char *message){
    switch(message){
        case 'Q':
            // quit game
            return quit(arg);
        // singular move 
        case 'h':
            // move left
            return move(arg, -1, 0); 
        case 'l':
            // move right
            return move(arg, 1, 0); 
        case 'j':
            // move down
            return move(arg, 0, -1); 
        case 'k':
            // move up
            return move(arg, 0, 1); 
        case 'y':
            // move up and left
            return move(arg, -1, 1);
        case 'u':
            // move up and right
            return move(arg, 1, 1);
        case 'b':
            // move down and left
            return move(arg, -1, -1);
        case 'n':
            // move down and right
            return move(arg, 1, -1);

        // continuous movement
        case 'h':
            // move left
            return continuousMove(arg, -1, 0);
        case 'L':
            // move right
            return continuousMove(arg, 1, 0);
        case 'J':
            // move down
            return continuousMove(arg, 0, -1);
        case 'K':
            // move up
            return continuousMove(arg, 0, 1);
        case 'Y':
            // move up and left
            return continuousMove(arg, -1, 1);
        case 'U':
            // move up and right
            return continuousMove(arg, 1, 1);
        case 'B':
            // move down and left
            return continuousMove(arg, -1, -1);
        case 'N':
            // move down and right
            return continuousMove(arg, 1, -1);

    }
    return false;
}

/*
 * move will move the player to the right by dx
 * and to upwards by dy
 */
bool move(void *arg, int dx, int dy){
    // check if is spectator or player
    // return false if spectator

}

bool continuousMove(void *arg, int dx, int dy){
    if(move(arg, dx, dy)){
        while(move(*arg, dx, dy)){}
        return true;
    }else{
        return false;
    }
}
