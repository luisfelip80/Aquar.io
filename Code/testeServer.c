#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

#define MAX_PLAYERS 3
#define	SCREEN 480

void startMap(int m[][SCREEN], int n){
	int i, j;
	for (i = 0; i < n; ++i){
		for (j = 0; j < n; ++j){
			m[i][j] = 0;
		}
	}
	m[30][30] = 1;
}

void changePosition(int m, int n, data *move){

}

typedef struct {
	int X, Y;
} data;

int main(){
	serverInit(MAX_PLAYERS);
	int map[SCREEN][SCREEN];
	startMap(map, SCREEN);
	int number[MAX_PLAYERS];
	data move;
	printf("Server is running!");
	while(1){
		int id = acceptConnection();
		if(id != NO_CONNECTION){
			recvMsgFromClient(number[id], id, WAIT_FOR_IT);
			printf("Numero %d conectado\n", number[id]);
		}
		struct msg_ret_t msg_ret = recvMsg(&move);
		if (msg_ret.status == MESSAGE_OK){
			changePosition(map, SCREEN, &move);
		}
	}
}