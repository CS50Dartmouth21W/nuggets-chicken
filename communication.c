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

bool handleMessage(void *arg, const addr_t from, const char *message){
    switch(message){
        case 'Q':
            // quit game
        case 'h':
            // move left
        case 'l':
            // move right
        case 'j':
            // move down
        case 'k':
            // move up
        case 'y':
            // move up and left
        case 'u':
            // move up and right
        case 'b':
            // move down and left
        case 'n':
            // move down and right
    }
    return true;
}
