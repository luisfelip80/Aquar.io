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

    // matriz marcao é a matriz onde é salva a posição de todos os jogadores.
    // map é o mapa que é copiado para a matriz tela para mostrar no terminal.
    char marcao [20] [40] ,map [20] [40], tela [20] [40];
    data move;
    // posição inicial dos outros playes, pode ser qualquer valor pois eles vão ser enviados para matriz marcão e serão
    // apagados como xAnterior e yAnterior.
    int id,xA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1},yA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1};
    int x;
    int y;
    // os personagens são colocados de arcordo com o seu id.
    char pers[13] = {'X','Y','Z','J','D','L','U','I','S','F','E','P','M'};
    // monta as matrizes de mapa que é copiada para tela. A marcação recebe espaços vazios.
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
    // recebe a posição anterior para apagar e, a próxima "px" e "py" para marcar o personagem.  
    void marcaPosicao(int xx, int yy, int px,int py, char personagem){
        marcao[py] [px] = personagem;
        marcao[yy] [xx] = 32;
    }
    // Gera posicao aleatória para começo do jogo.
    void GeraPosicao(){

        do{
            
                srand((unsigned)time(NULL));
                x=rand()%41-1;  
                y=rand()%21-1;
            
        } while ( map [y] [x] == 'l');
        
    }
    // faz uma cópia da matriz map na matriz tela e depois verifica em que posições na matriz macação estão
    // os jogadores. 
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
        // limpa terminal linux
        system("clear");

        for(i=0;i<20;i++){
            printf(" |");
            for(j=0;j<40;j++){
                printf("%c",tela[i] [j]);
            }
            printf("|\n");
        }
    }

    // tenta se conectar ao servidor.
    void assertConnection() {
        enum conn_ret_t ans = tryConnect();
        // ans recebe retorno da função tryCone..
       
        // enquanto o server não se conectar
        while (ans != SERVER_UP) {
            //mensagens possíveis
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
        // se conectar, pede nome, salva nome na struct de mensagem e manda para o server.
        if(ans == SERVER_UP){
            printf("Nome\n");
            scanf(" %[^\n]",move.nome);
            sendMsgToServer((void *)&move,sizeof(data));

            // espera mensagem do servidor. O servidor é quem vai definir o id desse client, ele precisa desse ip para se identificar.
            recvMsgFromServer(&move,WAIT_FOR_IT);
            // salva id recebido.
            id=move.id;
            printf("%d\n",id );      
        }
    }

    void runGame() {
        char ch;
        ch='l';
        marcaPosicao(0,0,x,y,pers[id]);
        mostraTela();
        while (1) {
            // se alguma tecla for pressionada e for diferente de 'l', envia posição, id e tecla.
            ch=getch();
            if(ch!='l'){
                move.tecla = ch;
                move.id=id;
                move.X=x;
                move.Y=y;
                sendMsgToServer((void *)&move,sizeof(data));
                ch='l';
            }

            // receber mensagem do servidor
        
            int ret = recvMsgFromServer(&move,DONT_WAIT);
            // se receber server diconect, acaba com a função
            if (ret == SERVER_DISCONNECTED) {
              return;
            } 
            // se houver mensagem, ler os dados:
            else if (ret != NO_MESSAGE) {
                printf("per %d id %d \n", move.permissao,move.id);
                 // se a permição para o pedido enviado for igual a 1, o client executa o que pediu para fazer.
                 if(move.permissao == 1 && id == move.id){
                     marcaPosicao(x,y,move.X,move.Y,pers[id]);
                     mostraTela();
                     x=move.X;
                     y=move.Y;
                }
                // se receber mensagem do server, mas o id não for o seu, mostra essa posição como sendo outro jogador.
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

        GeraPosicao();
        monta();
        
        move.X=x;
        move.Y=y;
        assertConnection();
        while(1) {
            // roda game.
            runGame();
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