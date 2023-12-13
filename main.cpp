#include "snake.h"


int main() {
    signal(SIGINT, handle_sigint);
    while(1){
        menu();
        system("pause");
    }
    return 0;
}


