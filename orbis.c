#include <stdio.h>
//---------------------Do Not Change This!---------------------
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>
char c;//the input character
int input_count = 0;//the input count
void mymain(); //your main() method will be implemented later
void *keypress_callback(void *arg){
  while(1) {
    c = getchar();
    if(c == 'q'){
      exit(0);
    }
    input_count ++;
  }
  return 0;
}
void starter(){
  struct termios info;
  tcgetattr(0, &info);      
  info.c_lflag &= ~ (ICANON | ECHO) ;  
  info.c_cc[VMIN] = 1;      
  info.c_cc[VTIME] = 0;     
  tcsetattr(0, TCSANOW, &info); 
  pthread_t tid1;
  pthread_create(&tid1, NULL, &keypress_callback, NULL);
}
int main() {
  starter();
  mymain();
  return 0;
}
//---------------------Do Not Change This!---------------------

//Assume that the latest input character is in variable c 
//You can check the input_count to check if there was any key press.
void menu(void);
void help(void);
char randomdisk(int,int*);
void move(int(*)[6],int,int);
void scorecal(char,int,int*,int*,int(*)[6]);
int ratio2,ratio4,ratio2_count,ratio4_count,stop=0;
bool catchy=0;
void mymain(){

  printf("\033c"); // clear screen command
  while (1){ // for coming back to menu

    menu();
    int disk[11][6]={{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'},{'*',' ',' ',' ',' ','#'}};
    int score=0,blood=3,menukey='s',menukeynum=116,base=input_count,tdn,extime,cycle=0;
    char click;
    menukey=getchar();

    if (menukey=='h') // help page
    {
      help();
      printf("\033c");
      continue;
    }
    else if (menukey=='t') // help page
    {
      menukey='u';
      printf("\033c");
      printf("\n _____________________________ \n");
      printf("|                             |\n");
      printf("|     choose cycle number     |\n");
      printf("|      then press enter       |\n");
      printf("|_____________________________|\n");
      scanf("%d",&cycle);
    }

    printf("\033c");
    while((menukey=='u' && cycle>-1) || (menukey=='s' && 1)){
      first:

      if (disk[10][2]==' ') // when the stack is empty
        {
          char a;
          a=randomdisk(menukey,&menukeynum);
          disk[10][2]==' ';
          disk[10][3]==' ';
          disk[9][2]==' ';
          disk[9][3]==' ';
        }

      for (tdn=1 ; disk[tdn][2]==' ' ; tdn++); //finding tdn: top disk number

      if (disk[tdn][2]=='?') //for ?? disk
      {
        char a;
        if (time(NULL)%2==0)
          a='*';
        else
          a='#';
        disk[tdn][2]=a;
        disk[tdn][3]=a;
      }

      if (disk[0][2]!=' ') //when stack over flows
      {
        if (blood==0)
          break;
        else
        {
          blood--;
          for (int i=1 ; i<11 ; i++)
          {
            disk[i][2]=' ';
            disk[i][4]=disk[i][3]; 
            move(disk,score,blood);
            usleep(200000);
            disk[i][3]=' ';
            move(disk,score,blood);
            usleep(200000);
            disk[i][4]=' ';
            move(disk,score,blood);
            usleep(50000);
            printf("\033c");
          }
        }
      }

      if (blood==-1) // when blood is -1
      {
        printf("\033c");
        break;
      }

      if (catchy==1) //for ~~ disk
        input_count++;

      //for each click, destroy disks & computing score
      if (input_count>base) // if there was a new click
      {
        base=input_count;
        click=c;
        if (catchy==1) //for ~~ disk
        {
          if (disk[tdn][2]=='*' || disk[tdn][2]=='2' || disk[tdn][2]=='4') //going left
            click='a';
          else //going right
            click='d';
        }
        if (click=='d') //click left
        {
          scorecal(click,disk[tdn][2],&score,&blood,disk); //computing score in score calculator function
            disk[tdn][2]=' ';
            disk[tdn][4]=disk[tdn][3]; 
            move(disk,score,blood);
            usleep(200000);
            disk[tdn][3]=' ';
            move(disk,score,blood);
            usleep(200000);
            disk[tdn][4]=' ';
            move(disk,score,blood);
        }
        else if (click=='a') //click right
        {
          scorecal(click,disk[tdn][2],&score,&blood,disk); //computing score in score calculator function
          disk[tdn][3]=' ';
          disk[tdn][1]=disk[tdn][2];  
          move(disk,score,blood);
          usleep(200000);
          disk[tdn][2]=' ';
          move(disk,score,blood);
          usleep(200000);
          disk[tdn][1]=' ';
          move(disk,score,blood);
        }
      }
      

      if (stop>0) //for || disk
      {
        stop--;
        sleep(1);
        goto first;
      }

      printf("\033c"); // the move function prints still printed on the screen so it have to clean

      //printing game screen
      printf("\n                          score:%d\n",score);
      printf("                          blood:%d\n",blood);
      for (int i=1 ; i<11 ; i++) //print a 10long stack
      {
        for (int j=0 ; j<6 ; j++) //print a 6long string
        {
          printf("%c",disk[i][j]);
          disk[i-1][j]=disk[i][j];
        }
        printf("\n");
      }
      printf("______\n");
      

      //full the arrays
      char a = randomdisk(menukey,&menukeynum);
      disk[10][2]=a;
      disk[10][3]=a;
      
      if (catchy==1) //for ~~ disk
      {
        usleep(100000);
        goto first;
      }

      sleep (1);
      printf("\033c");

      if (menukey=='u') //test mode cycle count
        cycle--;
      if (cycle==-1)
      {
        sleep(4); //sleep 4 sec for watching final screen
        menukey='0';
        cycle=0;
      }
      if (click=='m') //back to menu
        break;
    }

    printf("\033c");
    if (blood==0)
    {
    printf("\n _____________________________ \n");
    printf("|                             |\n");
    printf("|         GAME OVER !         |\n");
    printf("|_____________________________|\n");
    }
  }
}

void scorecal(char click,int tdc,int* score,int* blood,int disk[][6])
{
    if (ratio2_count<=0) //for end of 22 disk
    {
        ratio2=1;
        ratio2_count=0;
    }
    if (ratio4_count<=0) //for end of 44 disk
    {
        ratio4=1;
        ratio4_count=0;
    }
    switch(tdc) //tdc: top disc k character
    {
      case '*':
        if (click=='a')
        {
          (*score)+=ratio2*ratio4;
          ratio2_count--;
          ratio4_count--;
          break;
        }
          (*blood)--;
          ratio2_count--;
          ratio4_count--;
          break;
      case '#':
        if (click=='a')
        {
          (*blood)--;
          ratio2_count--;
          ratio4_count--;
          break;
        }
          (*score)+=ratio2*ratio4;
          ratio2_count--;
          ratio4_count--;
          break;
      case '$':
        (*score)+=ratio2*ratio4;
        ratio2_count--;
        ratio4_count--;
        for (int i=1 ; i<11 ; i++)
        {
          if (disk[i][2]!=' ')
          {
            if (click=='a')
            {
              disk[i][2]='*';
              disk[i][3]='*';
            }
            else if (click=='d')
            {
              disk[i][2]='#';
              disk[i][3]='#';
            }
          }
        }
        break;
      case '+':
        if (*blood<3)
          (*blood)++;
        break;
      case '2':
        ratio2*=2;
        ratio2_count+=3;
        break;
      case '4':
        ratio4*=4;
        ratio4_count++;
        break;
      case '|':
        stop+=3;
        break;
      case '~':
        catchy=1;
        break;
      case '=':
        catchy=0;
        break;
    }
}

void move(int disk[][6],int score,int blood)
{
  printf("\033c");
  printf("\n                          score:%d\n",score);
  printf("                          blood:%d\n",blood);
  for (int i=1 ; i<11 ; i++) //print a 10long stack
    {
      for (int j=0 ; j<6 ; j++) //print a 6long string
        printf("%c",disk[i][j]);
      printf("\n");
    }
  printf("______\n");
}

char randomdisk(int menukey,int *menukeynum) //making a random disk
{
    if (menukey=='u') //if you press t for test mode
    {
      switch(*menukeynum)
      {
      case 116:
      (*menukeynum)++;
      return '?';
      case 117:
      (*menukeynum)++;
      return '?';
      case 118:
      (*menukeynum)++;
      return '=';
      case 119:
      (*menukeynum)++;
      return '+';
      case 120:
      (*menukeynum)++;
      return '?';
      case 121:
      (*menukeynum)++;
      return '|';
      case 122:
      (*menukeynum)++;
      return '2';
      case 123:
      (*menukeynum)++;
      return '$';
      case 124:
      (*menukeynum)++;
      return '*';
      case 125:
      (*menukeynum)=116;
      return '#';
      }
    }
    else if (menukey=='s') //if you press s for normal mode
    {
      srand(time(NULL));
      switch(rand()%18)
      {
      case 0:
      return '2';
      case 1:
      return '4';
      case 2:
      return '$';
      case 3:
      return '+';
      case 4:
      return '?';
      case 5:
      return '|';
      case 6:
      return '~';
      case 7:
      return '=';
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      return '*';
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      return '#';
      }
    }
}

void menu(void)
{
    printf("\n _____________________________ \n");
    printf("|                             |\n");
    printf("|         << ORBIS >>         |\n");
    printf("|                             |\n");
    printf("|                             |\n");
    printf("|                             |\n");
    printf("|       START : press s       |\n");
    printf("|                             |\n");
    printf("|       TEST  : press t       |\n");
    printf("|                             |\n");
    printf("|       HELP  : press h       |\n");
    printf("|                             |\n");
    printf("|       MENU  : press m       |\n");
    printf("|                             |\n");
    printf("|       EXIT  : press q       |\n");
    printf("|                             |\n");
    printf("|                             |\n");
    printf("|_____________________________|\n");
}

void help(void)
{
    printf("\033c");
    printf("\n ______________________________________________________\n");
    printf("|                                                      |\n");
    printf("|                      << HELP >>                      |\n");
    printf("|                                                      |\n");
    printf("|                                                      |\n");
    printf("| > press 'a' for left, 'd' for right                  |\n");
    printf("|                                                      |\n");
    printf("| > go to following link to see disks performance:     |\n");
    printf("| https://quera.ir/course/assignments/12062/problems   |\n");
    printf("|                                                      |\n");
    printf("|                                                      |\n");
    printf("|                   MENU  : press m                    |\n");
    printf("|                                                      |\n");
    printf("|______________________________________________________|\n");
    char back=getchar();
    if (back=='m')
    return;
}