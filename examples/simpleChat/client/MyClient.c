#include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "client.h" 
    #include <string.h>
    #include <ctype.h>
    #include <time.h>


    #define MSG_MAX_SIZE 350
    #define BUFFER_SIZE (MSG_MAX_SIZE + 100)
    #define LOGIN_MAX_SIZE 13
    #define HIST_MAX_SIZE 200
    

    typedef struct {
      int X,Y,permissao,id;
      char tecla;
      char nome[30];
    }data;

    enum conn_ret_t tryConnect() {
      char server_ip[30];
      printf("Please enter the server IP: ");
      scanf(" %s", server_ip);
      getchar();
      return connectToServer(server_ip);
    }

    char marcao [20] [40] ,map [20] [40], tela [20] [40];
    data move;
    int id,xA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1},yA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1};
    int x;
    int y;
    char pers[13] = {'X','Y','Z','J','D','L','U','I','S','F','E','P','M'};
    void monta () {
        int i_des=0,j_des=0;

        for( i_des = 0 ; i_des < 20 ; i_des++ ){

            for( j_des = 0 ; j_des < 40 ; j_des++ ){
                map[i_des] [j_des] =32;
                tela[i_des] [j_des] =32;
                marcao [i_des] [j_des] =32;
            }

        }

        for( i_des = 5 ; i_des < 10 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
                tela[i_des] [j_des] ='l';
            }

        }
        for( i_des = 13 ; i_des < 18 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
                tela[i_des] [j_des] ='l';
            }

        }

        for( i_des = 8 ; i_des < 15 ; i_des++ ){

            for( j_des = 25 ; j_des < 30 ; j_des++ ){
                map[i_des] [j_des] ='l';
                tela[i_des] [j_des] ='l';
            }

        }


    }

    void marcaPosicao(int xx, int yy, int px,int py, char personagem){
        marcao[py] [px] = personagem;
        marcao[yy] [xx] = 32;
    } 
    void mostraTela(){
        int i,j;

        for(i=0;i<20;i++){
            for(j=0;j<40;j++){
                tela[i] [j] = map  [i] [j];
            }
        }
        for(i=0;i<20;i++){
            for(j=0;j<40;j++){
                if(marcao[i] [j] != 32){
                    tela[i] [j] = marcao[i] [j];
                }
            }
        }
        system("clear");

        for(i=0;i<20;i++){
            printf(" |");
            for(j=0;j<40;j++){
                printf("%c",tela[i] [j]);
            }
            printf("|\n");
        }
    }
    void assertConnection() {

        enum conn_ret_t ans = tryConnect();
        if(ans == SERVER_UP){
            printf("Nome\n");
            scanf(" %[^\n]",move.nome);
            sendMsgToServer((void *)&move,sizeof(data));
            recvMsgFromServer(&move,WAIT_FOR_IT);
            id=move.id;
            printf("%d\n",id );

               
        }
        while (ans != SERVER_UP) {
            if (ans == SERVER_DOWN) {
                puts("Server is down!");    
            } 
            else if (ans == SERVER_FULL) {
                puts("Server is full!");
            } 
            else if (ans == SERVER_CLOSED) {
                puts("Server is closed for new connections!");
            } 
            else {
                puts("Server didn't respond to connection!");
            }
            printf("Want to try again? [Y/n] ");
            int res;
            while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
                puts("anh???");
            }
            if (res == 'n') {
                exit(EXIT_SUCCESS);
            }
            ans = tryConnect();
        }
    }

    void runChat() {
      char ch,msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
      int type_pointer = 0;
      ch='l';
      marcaPosicao(0,0,x,y,pers[id]);
      mostraTela();
      while (1) {
        // LER UMA TECLA DIGITADA
        ch=getch();
        if(ch!='l'){
            move.tecla = ch;
            move.id=id;
            move.X=x;
            move.Y=y;
            sendMsgToServer((void *)&move,sizeof(data));
            ch='l';
        }

        // LER UMA MENSAGEM DO SERVIDOR
        
        int ret = recvMsgFromServer(&move,DONT_WAIT);

        if (ret == SERVER_DISCONNECTED) {
          return;
        } 
        else if (ret != NO_MESSAGE) {
            printf("per %d id %d \n", move.permissao,move.id);
            
            if(move.permissao == 1 && id == move.id){
                marcaPosicao(x,y,move.X,move.Y,pers[id]);
                mostraTela();
                x=move.X;
                y=move.Y;
            }
            if(move.permissao == 1 && id != move.id){
                marcaPosicao(xA[move.id],yA[move.id],move.X,move.Y,pers[move.id]);
                mostraTela();
                xA[move.id]=move.X;
                yA[move.id]=move.Y;

            }
        }
      }
    }

    int main() {

        srand((unsigned)time(NULL));
        x=rand()%41-1;
        y=rand()%21-1;
        monta();
        
        move.X=x;
        move.Y=y;
        assertConnection();
        while(1) {
            runChat();
            printf("Server disconnected! Want to try again? [Y/N] ");
            int res;
            while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
              puts("anh???");
            }
            if (res == 'y' || res == '\n') {
              assertConnection();
            } else {
              break;
            }
        }
        return 0;
    }