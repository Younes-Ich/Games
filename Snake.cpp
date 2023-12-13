#include "snake.h"

FILE* file = NULL;
int scoreTab[6];
Map Map1;
Snake Snake1;
Player Player1;


void handle_sigint(int sig) {
    if (file != NULL) fclose(file);
    FILE* f = fopen("save-game.bin", "wb");
    GameState* game = (GameState*) malloc(sizeof(GameState));
    game->m_map = Map1;
    game->player = Player1;
    game->snake = Snake1;
    fwrite(game, sizeof(GameState), 1, f);
    fclose(f);
}

void Shift_Right(int arr[],int size){
	for( int i=size-2;i>=0;i--){
		arr[i+1]=arr[i];
	}
}


void Draw() {
    system("cls");
    char* s = (char*) calloc((Map1.Width + 1) * Map1.Height, 1);
    *s = 10;
    for(int i = 0; i < Map1.Height; i++) {
        for (int j = 1; j < Map1.Width; j++) {
            if (i == 0 || i == Map1.Height - 1) s[i * Map1.Width + j] = '*';
            else if (j == 1 || j == Map1.Width - 1) s[i * Map1.Width + j] = '*';
            else if (i == Snake1.HeadY && j == Snake1.HeadX) s[i * Map1.Width + j] = '0';
            else if (i == Map1.FruitY && j == Map1.FruitX) s[i * Map1.Width + j] = '$';
            else {
                bool printed = false;
                for (int z = 0; z < Snake1.Tail_N; z++) {
                    if (Snake1.Tail_X[z] == j && Snake1.Tail_Y[z] == i) {
                        s[i * Map1.Width + j] = 'o';
                        printed = true;
                        break;
                    }
                }
                if (!printed) s[i * Map1.Width + j] = ' ';
            }
        }
        s[i * Map1.Width + Map1.Width] = '\n';
    }
    cout << s;
    cout << "Player Score: " << Player1.score << endl;
}

void Generate_fruit(){
    srand(time(NULL));
    Map1.FruitX = rand()%(Map1.Width-2) + 1;
    Map1.FruitY = rand()%(Map1.Height-2) + 1;
}
void Input(){
	if (_kbhit()){
		char c= _getch();
		switch(c){
			case 72 :Snake1.Dir =Top;break;
			case 80 :Snake1.Dir =Down;break;
			case 77 :Snake1.Dir =Right;break;
			case 75 :Snake1.Dir =Left;break;
			case 'x': exit(0);
		}
	}
}

void setup(){

    Map1.Width = 40;
    Map1.Height = 20;
    Generate_fruit();


    Snake1.HeadX = Map1.Width /2;
    Snake1.HeadY = Map1.Height /2;
    Snake1.Tail_N = 0;


    Player1.score = 0;
    Player1.lose = false;
    remove("save-game.bin");
}

void Move(){

    Shift_Right(Snake1.Tail_X, 50);
    Shift_Right(Snake1.Tail_Y, 50);
    Snake1.Tail_X[0] = Snake1.HeadX;
    Snake1.Tail_Y[0] = Snake1.HeadY;


    switch(Snake1.Dir){
        case Top:   Snake1.HeadY--; break;
        case Down:  Snake1.HeadY++; break;
        case Right: Snake1.HeadX++; break;
        case Left:  Snake1.HeadX--; break;
    }
    if(Snake1.HeadY >= Map1.Height || Snake1.HeadY <= 0 || Snake1.HeadX >= Map1.Width || Snake1.HeadX <= 0)
        Player1.lose = true;

    if(Snake1.HeadX == Map1.FruitX && Snake1.HeadY == Map1.FruitY){
        Generate_fruit();
        Player1.score += 1;
        Snake1.Tail_N++;

    }

}

void save(int score){
    FILE* file=fopen("file.txt","r+");
    int i, aide;
    for(i=0;i<5;i++){
        fscanf(file,"%d \n",&scoreTab[i]);
    }
    fseek(file,0,SEEK_SET);
    scoreTab[5]=score;
    for(i=5;i>0;i--){
        if(scoreTab[i]>scoreTab[i-1]){
            aide=scoreTab[i-1];
            scoreTab[i-1]=scoreTab[i];
            scoreTab[i]=aide;
        }
    }
    for(i=0;i<5;i++){
        fprintf(file,"%d \n",scoreTab[i]);
    }
    fclose(file);
}

void affiche_topS(){
    int tab[5];
    int i;
    FILE* file=fopen("file.txt","r+");
    printf("******************* Top 5 Score *******************\n");
    for(i=0;i<5;i++){
        fscanf(file,"%d \n",&tab[i]);
    }
    fclose(file);
    int j=0;
    for(i=0;i<5;i++){
        if(tab[i] != 0){
            printf("\n                      %d          \n",tab[i]);
            j=1;
        }
    }
    if(j == 0){
        printf("pas de score");
    }
    getchar();
    fflush(stdin);
}

void Game(){
    while(!Player1.lose){
            // Could decouple the input from the framerate to have less delay
        Draw();
        Input();
        Move();
        Sleep(50);
    }
    save(Player1.score);
    remove("save-game.bin");
    system("cls");
    printf("********************\n\n\n");
    printf("    Game Over       \n\n\n");
    printf("********************");
    getchar();
    fflush(stdin);
}

void continu(){
    GameState* game = (GameState*)malloc(sizeof(GameState));
    fread(game, sizeof(GameState),1, file);
    Player1 = game->player;
    Map1 = game->m_map;
    Snake1 = game->snake;
    Game();
}

void menu(){
    file = fopen("save-game.bin", "rb");
    char c;
        system("cls");
        printf("*************************************\n\n");
        printf("1- Start\n");
        printf("2- Top 5 Score\n");
        if(file != NULL)
            printf("3- continue\n");
        printf("0- Quitter\n");
        c = getchar();
        fflush(stdin);
        GameState* game = (GameState*)malloc(sizeof(GameState));
        fread(game, sizeof(GameState),1, file);
        Player1 = game->player;
        Map1 = game->m_map;
        Snake1 = game->snake;
                char option = c - '0';
        option = option == 3 ? (file != NULL) * option: option;
        fclose(file);
        file = NULL;
        system("cls");

        switch(option){
            case 1:   setup();
            case 3:   Game();    break;
            case 2:   affiche_topS();    break;
            case 0:   printf("*****Quitter le programee*****");   exit(0);    break;
            default:    printf("erreur");
        }
}




