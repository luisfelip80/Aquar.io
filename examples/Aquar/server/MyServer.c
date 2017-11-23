#include "server.h"
#include <stdio.h>
#include <string.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 13

// estrutura de mensagens
typedef struct {
      int X,Y,permissao,id;
      char tecla;
      char nome[30];
}data;
data dados;

char map [20] [40];
data dados;

// montando mapa igual ao feito no cliente.    
void monta () {
        int i_des=0,j_des=0;

        for( i_des = 0 ; i_des < 20 ; i_des++ ){

            for( j_des = 0 ; j_des < 40 ; j_des++ ){
                map[i_des] [j_des] =32;
            }

        }

        for( i_des = 5 ; i_des < 10 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }
        for( i_des = 13 ; i_des < 18 ; i_des++ ){

            for( j_des = 10 ; j_des < 15 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }

        for( i_des = 8 ; i_des < 15 ; i_des++ ){

            for( j_des = 25 ; j_des < 30 ; j_des++ ){
                map[i_des] [j_des] ='l';
            }

        }


    }

int main() {
    int t;
    char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE];
    data dados_aux;
    serverInit(MAX_CHAT_CLIENTS);
    puts("Server is running!!");
    monta();
    while (1) {
    	// iguala id ao id conectado
        int id = acceptConnection();

        // Primeira vez que conecta. Não estiva conectado
        if (id != NO_CONNECTION) {
        	//Esperar o nome.
            recvMsgFromClient(&dados, id, WAIT_FOR_IT);
            strcpy(client_names[id], dados.nome);
            strcpy(str_buffer, client_names[id]);
            strcat(str_buffer, " connected to server");
            printf("%s\n",str_buffer );
            // pegar p id gerado pelo server
            dados.id=id;
            // envia para esse cliente que acabou de se conectar o seu id.
            sendMsgToClient(&dados, sizeof(data),id);
            // informa a todos que um novo jogador entrou
            broadcast(&dados, sizeof(data));
        }

        // salva mensagens não lidas em uma estrutura auxiliar.
        struct msg_ret_t msg_ret = recvMsg(&dados_aux);
        //mensagem nova
        if (msg_ret.status == MESSAGE_OK) {
            
            // pega tecla recebida, se for 'w':
            if(dados_aux.tecla == 'w'){
                //printf("%c  x %d  y  %d \n", dados_aux.tecla, dados_aux.X, dados_aux.Y);
                // valida nova posição somando ou subtraindo o valor de movimento, essas somas e subtrações dependem
                // da tecla que foi recevida.

                if(dados_aux.Y-1>=0 && map[dados_aux.Y-1] [dados_aux.X]!='l'){
                    //printf("ok\n");
                    // Caso seja ok a validação, faz a mudança no valor recebido e reenvia ele alterado.
                    dados.Y = dados_aux.Y-1;
                    dados.X = dados_aux.X;
                    // reevia o id recebido.
                    dados.id = dados_aux.id;

                    //printf("id %d\n",dados.id);
                    // concede permissão.
                    dados.permissao=1;
                    //printf("permi %d\n",dados.permissao);
                    broadcast(&dados,sizeof(data));
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados,sizeof(data));
                }
            }

            // iguais a 'w', muda apenas a alteração no valor da posição
            if(dados_aux.tecla == 's'){
                if(dados_aux.Y+1<20 && map[dados_aux.Y+1] [dados_aux.X]!='l'){
                    dados.Y= dados_aux.Y+1;
                    dados.X = dados_aux.X;
                    dados.id = dados_aux.id;
                    dados.permissao=1;    
                    broadcast(&dados,sizeof(data));
                }
                else {
                    dados.permissao = 0;
                    broadcast(&dados,sizeof(data));
                }
            }
            if(dados_aux.tecla == 'a' ){
                if(dados_aux.X-1>=0 && map[dados_aux.Y] [dados_aux.X-1]!='l'){
                    dados.X= dados_aux.X-1;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.permissao=1;    
                    broadcast(&dados,sizeof(data));
                }
                else {
                    dados.permissao = 0; 
                    broadcast(&dados,sizeof(data));  
                }
            }
            if(dados_aux.tecla == 'd'){
                if(dados_aux.X+1<40 && map[dados_aux.Y] [dados_aux.X+1]!='l'){
                    dados.X= dados_aux.X+1;
                    dados.Y = dados_aux.Y;
                    dados.id = dados_aux.id;
                    dados.permissao=1;
                    broadcast(&dados,sizeof(data));
                }
                else {
                    dados.permissao = 0; 
                    broadcast(&dados,sizeof(data));
                }
            }
            
        } 
        // caso algum jogador saia...
        else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
            printf("%s disconnected, id = %d is free\n",client_names[msg_ret.client_id], msg_ret.client_id);
        }
    }
}
