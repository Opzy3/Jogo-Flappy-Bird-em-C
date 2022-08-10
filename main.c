#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <poll.h> 
#include <fcntl.h> 
#include <errno.h>

////////////////////////////////////////////////////////
int kbhit(void){
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
////////////////////////////////////////////////////////
int getch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
///////////////////////////////////////////////////////
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
///////////////////////////////////////////////////////
char Mapa[20][20];
int x,y;
int obs;
char salto;
int px = 10, py = 4;
int pxa = 9;
int contsalto = 0;
int cont;
int game = 1;
int score = 0;
///////////////////////////////////////////////////////
void mapa(){
	for(x=0;x<20;x++){
		for(y=0;y<20;y++){
				if(x==0||x==19||y==0||y==19){
					Mapa[x][y] ='x'; 
				}
				else{
					Mapa[x][y] =' '; 
				}
		}
	}
	Mapa[px][py] ='O';
}
///////////////////////////////////////////////////////
void printmapa(){
	for(x=0;x<20;x++){
		for(y=0;y<20;y++){
				printf("%c",Mapa[x][y]);
				printf(" ");
		}
		printf("\n");
	}
	printf("\n SCORE: %d\n\n",score);
}
///////////////////////////////////////////////////////
void obstaculos(){
	srand(time(NULL));
	obs = rand()%16;
  while (obs == 0){
    obs = rand()%16;}

	for(x=0;x<20; x++){
		Mapa[x][17] ='x';
		Mapa[x][16] ='x';

		if(x == obs||x == obs + 1||x == obs + 2||x == obs + 3){
    
			Mapa[x][17] =' ';
			Mapa[x][16] =' ';
		}
	}
}
///////////////////////////////////////////////////////
void rodar(){
	
	for(x=18;x>0; x--){
		for( y=2; y<19;y++){
			Mapa[x][y-1]=Mapa[x][y];
		}
	}
	Mapa[px][py-1] = ' ';	
	Mapa[px][py] = 'O';
}
///////////////////////////////////////////////////////
void pulo(){
	salto = getch();
	if(salto == 'z' && Mapa[px-2][py] == ' '&& contsalto ==0){
		pxa = px;
		Mapa[px][py] = ' ';
		Mapa[px-2][py] = 'O';
		px = px-2;
		contsalto = 1;
	}
	else if(salto == 'z' && Mapa[px-1][py] == ' '){
		pxa = px;
		Mapa[px][py] = ' ';
		Mapa[px-1][py] = 'O';
		px = px-1;
	}
}
///////////////////////////////////////////////////////
void queda(){
	if( Mapa[px+1][py] == ' '){
		pxa = px;
		Mapa[px][py] = ' ';
		Mapa[px+1][py] = 'O';
		px = px+1;	
		contsalto = 0;
	}
}
///////////////////////////////////////////////////////
int fim(){
		if(Mapa[px][py+1] =='x'){
			game = 0;
			return 1;
		}
		return 0;
}
///////////////////////////////////////////////////////
int main(void) {
  system("clear");
	mapa();
	printmapa();
	while(game == 1){
		while(!kbhit()){
			fim();
			if(fim() == 1){
				break;
			}
			
			system("clear");
			rodar();
			printmapa();
			msleep(200);
			
			queda();
			cont += 1;
			if(cont == 15){
			obstaculos();
			cont = 0;	
			score += 1;
			}
		
		}
		if(game ==1){
				pulo();
		}
	
	}
	
	system("clear");
	printf("\n\n\n\t\t GAME OVER");
}