#include <stdio.h>
//---------------------Do Not Change This!---------------------
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
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
void mymain(){
  //this code is for your testing:
  while(1){
    printf("%d -th input was %c\n",input_count,c);
    sleep(1); //hint for your code ;)
  }

}


