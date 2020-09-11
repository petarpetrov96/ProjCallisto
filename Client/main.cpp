#include "game.h"
int main() {
    Game Game;
    if(!Game.init()) {
        MessageBox(NULL,"Your computer doesn't support OpenGL 3.3!\nPlease check your video drivers and the minimum system requirements for the game!","Error",MB_OK);
        return 0;
    }
    Game.mainMenu();
    Game.exit();
    return 0;
}
