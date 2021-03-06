#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "client.h" 
#include <ctype.h>
#include <time.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200
#define MAX_LOG_SIZE 18

#define LARGURA_TELA 640
#define ALTURA_TELA 480

ALLEGRO_BITMAP *lobby = NULL;
ALLEGRO_BITMAP *end = NULL;

ALLEGRO_BITMAP *instrucoes = NULL;
ALLEGRO_BITMAP *creditos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *imagem1 = NULL;
ALLEGRO_BITMAP *imagem2 = NULL;
ALLEGRO_BITMAP *peixe1 = NULL;
ALLEGRO_BITMAP *peixe2 = NULL;
ALLEGRO_BITMAP *peixe3 = NULL;
ALLEGRO_BITMAP *peixe4 = NULL;
ALLEGRO_BITMAP *peixe5 = NULL;
ALLEGRO_BITMAP *peixe6 = NULL;
ALLEGRO_BITMAP *peixe7 = NULL;

ALLEGRO_BITMAP *racao = NULL;
ALLEGRO_BITMAP *isca = NULL;
ALLEGRO_BITMAP *isca_a = NULL;
ALLEGRO_BITMAP *isca_t = NULL;
ALLEGRO_BITMAP *armario = NULL;
ALLEGRO_BITMAP *sacos = NULL;
ALLEGRO_BITMAP *personagem_1 = NULL;
ALLEGRO_BITMAP *personagem_2 = NULL;
ALLEGRO_BITMAP *personagem_3 = NULL;
ALLEGRO_BITMAP *personagem_4 = NULL;
ALLEGRO_BITMAP *personagem_5 = NULL;
ALLEGRO_BITMAP *personagem_6 = NULL;
ALLEGRO_BITMAP *personagem_7 = NULL;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonteChat = NULL;
ALLEGRO_FONT *fonteM = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos_lobby = NULL;
ALLEGRO_EVENT_QUEUE *Eventos = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;



typedef struct {
  int X,Y,permissao,id,tecla,pers,tamanho,xb,yb,fome,direcao,x_aux,y_aux,mens;
  char nome[30],time[10],texto[50];
}data;


char str_buffer [MAX_LOG_SIZE][50],str[30] = {"192.168.15.166"};
bool sair = false;
bool concluido = false;
bool f_choice = false;
bool lobby_flag=false;
bool RunGame=false;
int bx=1,by=0;
int vivos[6]= {1,1,1,1,1,1}; 
int fome=0,tamanho=0;
int ant_dir [6] ={0,0,0,0,0,0};
int matriz_tamanho [ALTURA_TELA] [LARGURA_TELA];
// matriz marcacao é a matriz onde é salva a posição de todos os jogadores.
// map é o mapa que é copiado para a matriz tela para mostrar no terminal.
char marcacao [ALTURA_TELA] [LARGURA_TELA] ,map [ALTURA_TELA] [LARGURA_TELA];
data dados,dados_aux;
// posição inicial dos outros playes, pode ser qualquer valor pois eles vão ser enviados para matriz marcão e serão
// apagados como xAnterior e yAnterior.
int id,Peixe_eu=0;
int xA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1},yA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1};
int x,y,indicador_X_Bit=0,indicador_Y_Bit=0;
int correcaoTamanho_X[5] = {7,24,38,50,62};
int correcaoTamanho_Y[5] = {10,20,30,40,50};
int x_bit[2][5] = {{0,160,320,480,640},{1440,1280,1120,960,800}},y_bit[5] = {0,129,260,391,522};
int x_bit_isca[10]={0,77,154,231,308,385,462,539,616,693},xi=0;
char res[2]={32,'\0'};
char tempo[10]="0:0",meu_nome[30];
bool flagBoladona=false;
bool pedir_nome = false,right=true,left=false;
// os personagens são colocados de arcordo com o seu id.
char fom [5],pers [2] [2] [7] = { { {'L','U','I','S','F','M','C'},{'P','E','R','A','V','D','H'} } , { {'l','u','i','s','f','m','c'},{'p','e','r','a','v','d','h'} } };
// monta as matrizes de mapa que é copiada para tela. A marcação recebe espaços vazios.
void monta ();
void GeraPosicao();
void mostraTela();
void marcaPosicao(int xx, int yy, int px,int py, int personagem,int dir,int camada,int tamanho_peixe);
enum conn_ret_t tryConnect();
bool assertConnection();
void runGame();
int maximo();
bool telaInicial();
char manipular_char(ALLEGRO_EVENT evento);
int conectar();
bool inicializar();
bool carregar_arquivos();
void finalizar();
void manipular_entrada();
void exibir_texto_centralizado();
bool Choice();
bool lobbyMenu();

int main() {
    if(!inicializar()){
        finalizar();
        return 0;
    }
    carregar_arquivos();

    GeraPosicao();
    monta();
    
    dados.X=x;
    dados.Y=y;
     
    if (!telaInicial()){
        return 0;
    }
    
    if (!Choice()){
        return 0;
    }
    if (!lobbyMenu()){
        return 0;
    }
    //while(!sair) {
        // roda game.
        runGame();
        al_draw_bitmap(end,0,0,0);
        al_flip_display();
        //printf("Server disconnected! Want to try again? [Y/N] ");
        int ress;
        
    //}
    finalizar();

    return 0;
}

void monta () {
    int i=0,j=0;

    for( i = 0 ; i < ALTURA_TELA  ; i++ ){

        for( j = 0 ; j < LARGURA_TELA ; j++ ){
            map[i] [j] =32;
            marcacao [i] [j] =32;
            matriz_tamanho [i] [j] = 0;
        }
    }
}

// recebe a posição anterior para apagar e, a próxima "px" e "py" para marcar o personagem.  
void marcaPosicao(int xx, int yy, int px,int py, int personagem, int dir,int camada,int tamanho_peixe){
    
    marcacao[py] [px] = pers[camada][dir][personagem];
    marcacao[yy] [xx] = 32;
    matriz_tamanho [py] [px] = tamanho_peixe;
    matriz_tamanho [yy] [xx] = 0;
}
void Morte(int xx, int yy){
    marcacao[yy] [xx] = 32;
}
void peixesVivos(int Fome_peixe, int ID){
    if(Fome_peixe >= 0){
        vivos[ID]=1;
    }
    else {
        vivos[ID]=0;
    }
}
// Gera posicao aleatória para começo do jogo.
void GeraPosicao(){

    do{
            srand((unsigned)time(NULL));
            y=rand()%470;
            x=rand()%630;      
    }while (( x < 10 || y <10) && x%10==0 && y%10==0);
    

}
// faz uma cópia da matriz map na matriz tela e depois verifica em que posições na matriz macação estão
// os jogadores. 
void mostraTela(int primeiro_X){
    int i,j;
    
    if(flagBoladona){
                al_draw_text(fonte, al_map_rgb(255,255,255), 310,
                (20 - al_get_font_ascent(fonteM)) / 2,
                ALLEGRO_ALIGN_CENTRE + al_get_font_ascent(fonteM), tempo);
                
                al_draw_text(fonteChat, al_map_rgb(255,255,255), 10,
        		460 ,al_get_font_ascent(fonteChat), fom);
            
    }
    for(i=0;i<ALTURA_TELA;i++){
        for(j=0;j<LARGURA_TELA;j++){

            if(marcacao[i] [j] == 'g'){
                isca_a = al_create_sub_bitmap(isca, x_bit_isca [xi],0, 77,76);
                al_draw_bitmap(isca_a, j-38,i-37,0);
            }
            else if(marcacao[i] [j] == 't'){
                isca_t = al_create_sub_bitmap(isca, x_bit_isca [xi],0, 77,76);
                al_draw_bitmap(isca_t, j-38,i-37,0);
            }
            else if (marcacao[i] [j] == 'w'){
                al_draw_bitmap(racao, j-2,i-2,0);
            }
            
            if(marcacao[i] [j] == 'L'){
                personagem_1 = al_create_sub_bitmap(peixe1, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_1, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'l'){
                personagem_1 = al_create_sub_bitmap(peixe1, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_1, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'P'){
                personagem_1 = al_create_sub_bitmap(peixe1, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_1, j-70,i-65,0);
                //al_draw_rectangle(j-correcaoTamanho_X[matriz_tamanho[i][j]], i+correcaoTamanho_Y[matriz_tamanho[i][j]], j-correcaoTamanho_X[matriz_tamanho[i][j]]-10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);
            }
            else if(marcacao[i] [j] == 'p'){
                personagem_1 = al_create_sub_bitmap(peixe1, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_1, j-70,i-65,0);
            }
            else if(marcacao[i] [j] == 'U'){
                personagem_2 = al_create_sub_bitmap(peixe2, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_2, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'u'){
                personagem_2 = al_create_sub_bitmap(peixe2, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_2, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'E'){
                personagem_2 = al_create_sub_bitmap(peixe2, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_2, j-65,i-65,0);
            }
            else if(marcacao[i] [j] == 'e'){
                personagem_2 = al_create_sub_bitmap(peixe2, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_2, j-65,i-65,0);
            }
            else if(marcacao[i] [j] == 'I'){
                personagem_3 = al_create_sub_bitmap(peixe3, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_3,j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'i'){
                personagem_3 = al_create_sub_bitmap(peixe3, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_3, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'R'){
                personagem_3 = al_create_sub_bitmap(peixe3, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_3, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'r'){
                personagem_3 = al_create_sub_bitmap(peixe3, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_3, j-89,i-65,0);
            }
            if(marcacao[i] [j] == 'S'){
                personagem_4 = al_create_sub_bitmap(peixe4, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_4, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 's'){
                personagem_4 = al_create_sub_bitmap(peixe4, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_4, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'A'){
                personagem_4 = al_create_sub_bitmap(peixe4, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_4, j-70,i-65,0);
                //al_draw_rectangle(j-correcaoTamanho_X[matriz_tamanho[i][j]], i+correcaoTamanho_Y[matriz_tamanho[i][j]], j-correcaoTamanho_X[matriz_tamanho[i][j]]-10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);
            }
            else if(marcacao[i] [j] == 'a'){
                personagem_4 = al_create_sub_bitmap(peixe4, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_4, j-70,i-65,0);
            }
            if(marcacao[i] [j] == 'F'){
                personagem_5 = al_create_sub_bitmap(peixe5, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_5, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'f'){
                personagem_5 = al_create_sub_bitmap(peixe5, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_5, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'V'){
                personagem_5 = al_create_sub_bitmap(peixe5, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_5, j-70,i-65,0);
                //al_draw_rectangle(j-correcaoTamanho_X[matriz_tamanho[i][j]], i+correcaoTamanho_Y[matriz_tamanho[i][j]], j-correcaoTamanho_X[matriz_tamanho[i][j]]-10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);
            }
            else if(marcacao[i] [j] == 'v'){
                personagem_5 = al_create_sub_bitmap(peixe5, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_5, j-70,i-65,0);
            }
            else if(marcacao[i] [j] == 'M'){
                personagem_6 = al_create_sub_bitmap(peixe6, x_bit [0][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_6, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'm'){
                personagem_6 = al_create_sub_bitmap(peixe6, x_bit [1][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_6, j-89,i-65,0);
            }
            if(marcacao[i] [j] == 'D'){
                personagem_6 = al_create_sub_bitmap(peixe6, x_bit [1][primeiro_X], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_6, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'd'){
                personagem_6 = al_create_sub_bitmap(peixe6, x_bit [0][5], y_bit[matriz_tamanho[i][j]], 160,131);
                al_draw_bitmap(personagem_6, j-89,i-65,0);
            }
            else if(marcacao[i] [j] == 'C'){
                personagem_7 = al_create_sub_bitmap(peixe7, x_bit [0][primeiro_X], y_bit[0], 160,131);
                al_draw_bitmap(personagem_7, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'c'){
                personagem_7 = al_create_sub_bitmap(peixe7, x_bit [1][5], y_bit[0], 160,131);
                al_draw_bitmap(personagem_7, j-89,i-65,0);
            }
            if(marcacao[i] [j] == 'H'){
                personagem_7 = al_create_sub_bitmap(peixe7, x_bit [1][primeiro_X], y_bit[0], 160,131);
                al_draw_bitmap(personagem_7, j-89,i-65,0);
                //al_draw_rectangle(j+correcaoTamanho_X[matriz_tamanho[i][j]], i +correcaoTamanho_Y[matriz_tamanho[i][j]], j+correcaoTamanho_X[matriz_tamanho[i][j]]+10, i-correcaoTamanho_Y[matriz_tamanho[i][j]],al_map_rgb(255,0,0),0);            
            }
            else if(marcacao[i] [j] == 'h'){
                personagem_7 = al_create_sub_bitmap(peixe7, x_bit [0][5], y_bit[0], 160,131);
                al_draw_bitmap(personagem_7, j-89,i-65,0);
            }
        }
    }
    if(!f_choice){
        al_draw_bitmap(sacos,0,0,0);
    }
    al_flip_display();
}

// tenta se conectar ao servidor.

void runGame() {
    al_draw_bitmap(fundo,0,0,0);
    char ch;
    int i,j,k;
    ch='l';
    char minutosC,segundosC;
    RunGame=true;
    printf("%d %d\n",x,y );
    marcaPosicao(0,0,x,y,Peixe_eu,0,0,0);
    mostraTela(0);
    if(id==0){
        printf("ok\n");
        dados.id=0;
        dados.permissao=2;
        sendMsgToServer((void *)&dados,sizeof(data));
    }
    flagBoladona=true;
    while (1) {
    	al_draw_bitmap(fundo,0,0,0);
    	sprintf(fom,"Score: %d\n", fome);
    	                            
        // se alguma tecla for pressionada e for diferente de 'l', envia posição, id e tecla.
        while (!al_is_event_queue_empty(fila_eventos)){
            
            ALLEGRO_EVENT evento;
            al_get_next_event(fila_eventos, &evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                dados.tecla = evento.keyboard.keycode;
                //printf("%d\n",dados.tecla );
                dados.id=id;
                dados.X=x;
                dados.Y=y;
                dados.fome=fome;
                dados.tamanho=tamanho;
                dados.pers=Peixe_eu;
                dados.direcao=ant_dir[id];
                sendMsgToServer((void *)&dados,sizeof(data));
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = true; 
            return;
            }   
            //mostraTela(x_bit[bx],y_bit[by]);
        }
        // receber mensagem do servidor
        int ret = recvMsgFromServer(&dados,DONT_WAIT);
        // se receber server diconect, acaba com a função
        if (ret == SERVER_DISCONNECTED) {
          return;
        } 
        // se houver mensagem, ler os dados:
        else if (ret != NO_MESSAGE) {
            //printf("per %d id %d \n", dados.permissao,dados.id);
             // se a permissao para o pedido enviado for igual a 1, o client executa o que pediu para fazer.
             if(dados.permissao==2){
                return;
             }
             strcpy(tempo,dados.time);
             if(dados.permissao == 1 && id == dados.id && vivos[id] == 1){
                //printf("%s\n", tempo);
                if(dados.tecla==83){//left
                    marcaPosicao(x,y,dados.X,dados.Y,Peixe_eu,0,0, dados.tamanho);
                    ant_dir[dados.id]=0;
                }
                else if(dados.tecla==82){//right
                    marcaPosicao(x,y,dados.X,dados.Y,Peixe_eu,1,0,dados.tamanho);
                    ant_dir[dados.id]=1;
                }
                else if(dados.tecla==23){
                    //printf("y %d x %d \n",dados.y_aux,dados.x_aux );
                    marcaPosicao(x,y,dados.X,dados.Y, Peixe_eu,dados.direcao,1,dados.tamanho);
                    //printf("antes - %c\n", marcacao[dados.y_aux] [dados.x_aux]);
                    if(dados.mens==3){
                        marcacao[dados.y_aux] [dados.x_aux]=32;
                    }
                    //printf("depos - %c\n", marcacao[dados.y_aux] [dados.x_aux]);
                    fome=dados.fome;
                    tamanho = dados.tamanho;
                    //printf("fome %d\n", fome );
                }
                else {
                    marcaPosicao(x,y,dados.X,dados.Y,Peixe_eu,ant_dir[dados.id],0,dados.tamanho);

                }
                 x=dados.X;
                 y=dados.Y;
                 //printf("x %d\n",x );
                 
            }
            // se receber mensagem do server, mas o id não for o seu, mostra essa posição como sendo outro jogador.
            if(dados.permissao == 1 && id != dados.id && dados.id != 6 && vivos[dados.id] == 1){
                if(vivos[dados.id] == 1){

                    if(dados.tecla==83 && dados.tecla > 0){
                        marcaPosicao(xA[dados.id],yA[dados.id],dados.X,dados.Y,dados.pers,0,0,dados.tamanho);
                        ant_dir[dados.id]=0;
                    }
                    //pritar peixe na direção que ele andar
                    else if(dados.tecla==82 && dados.tecla > 0){
                        marcaPosicao(xA[dados.id],yA[dados.id],dados.X,dados.Y,dados.pers,1,0,dados.tamanho);
                        ant_dir[dados.id]=1;
                    }
                    else if(dados.tecla==23 && dados.tecla > 0){

                        marcaPosicao(xA[dados.id],yA[dados.id],dados.X,dados.Y,dados.pers,dados.direcao,1,dados.tamanho);
                        //printf("antes - %c\n", marcacao[dados.y_aux] [dados.x_aux]);
                        if(dados.mens==3){
                            marcacao[dados.y_aux] [dados.x_aux]=32;
                        }
                        //printf("depos - %c\n", marcacao[dados.y_aux] [dados.x_aux]);
                    }

                    else if(dados.tecla < 0){
                        vivos[dados.id]=0;
                        Morte(dados.X,dados.Y);
                    }
                    else {
                        marcaPosicao(xA[dados.id],yA[dados.id],dados.X,dados.Y,dados.pers,ant_dir[dados.id],0,dados.tamanho);
                    }

                    xA[dados.id]=dados.X;
                    yA[dados.id]=dados.Y;
                    }

                }
                if(dados.fome==-1){
                    Morte(dados.X,dados.Y);
                    vivos[dados.id] = 0;
                    return;
                }
                if(dados.id==6 && dados.mens==5){
                    xi++;

                    bx=dados.xb;           	
	               marcacao[dados.y_aux] [dados.x_aux] = 'w';
		        
        
                    if(xi>=6)
                        xi=0;
                }
                else if(dados.id==6){

                    bx=dados.xb;
                }

                 if(dados.id==7 && dados.mens == 5){
                    
		              marcacao[dados.y_aux] [dados.x_aux] = 'g';
                }


                 if(dados.id==8 && dados.mens == 5){
		            marcacao[dados.y_aux] [dados.x_aux] = 't'; 
                }
        }
        mostraTela(bx);
    }
}
int maximo(int h){
    if(h<=0){
        return 0;
    }
    else if(h>=255){
        return 255;
    }
    return h;

}
//enum conn_ret_t tryConnect() {
  //printf("Please enter the server IP: ");
//  return connectToServer(str);
//}
/*
int conecta(){
  

  if(ans==SERVER_UP)
    return 1;
  else if(ans == SERVER_DOWN)
    return 2;
  else if(ans==SERVER_FULL)
    return 3;
  else if(ans == SERVER_CLOSED)
    return 4;
  else 
    return 0;s
}
*/
bool telaInicial(){
   
    int i;
    // enquanto o server não se conectar
    
    while (!concluido) {
        
        while (!al_is_event_queue_empty(fila_eventos)){
            

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (!concluido){
                manipular_entrada(evento);
                 
                al_flip_display();
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    
                    enum conn_ret_t ans = connectToServer(str);

                    if(ans == SERVER_UP){
                            strcpy(str,"");

                            while(!pedir_nome){
                                while (!al_is_event_queue_empty(fila_eventos)){
                                
                                            ALLEGRO_EVENT evento;
                                            al_wait_for_event(fila_eventos, &evento);
                                 
                                            if (!pedir_nome){
                                                manipular_entrada(evento);
                                                
                                                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                                                    strcpy(dados.nome,str);
                                                    sendMsgToServer((void *)&dados,sizeof(data));
                                                    // espera mensagem do servidor. O servidor é quem vai definir o id desse client, ele precisa desse ip para se identificar.
                                                    recvMsgFromServer(&dados,WAIT_FOR_IT);
                                                    // salva id recebido.
                                                    id=dados.id;
                                                    strcpy(meu_nome,dados.nome);
                                                    if(strcmp(meu_nome,"pisa menos") == 0){
                                                        Peixe_eu = 6;
                                                        f_choice=true;
                                                    }
                                                    pedir_nome = true;
                                                    dados.tecla=83;
                                                    dados.id=id;
                                                    dados.X=x;
                                                    dados.Y=y;
                                                    dados.fome=fome;
                                                    dados.tamanho=tamanho;
                                                    dados.pers=Peixe_eu;
                                                    dados.direcao=ant_dir[id];
                                                    sendMsgToServer((void *)&dados,sizeof(data));
                                                }
                                                

                                            }
                                                                                          
                                            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                                                concluido = true;
                                                pedir_nome - true;
                                                sair = true; 
                                            }
                                }

                                if(!pedir_nome){
                                    al_draw_bitmap(imagem1, 0, 0, 0);
                                    al_draw_text(fonte, al_map_rgb(0,0,0), 180- al_get_font_ascent(fonte),
                                    (580 - al_get_font_ascent(fonte)) / 2,
                                    ALLEGRO_ALIGN_CENTRE + al_get_font_ascent(fonte), str);
                                    al_draw_bitmap(imagem2, 0, 0, 0);
                                    exibir_texto_centralizado("Nome",290,450);
                                    al_flip_display();
                                }
                            }
                        
                      concluido=true;
                      //pede nome.
                    }
                    else if (ans == SERVER_DOWN) {
                        al_draw_bitmap(imagem1, 0, 0, 0);
                        al_draw_bitmap(imagem2, 0, 0, 0);
                        exibir_texto_centralizado("Server is down!",180 ,450);
                        al_flip_display();
                        strcpy(str,"");
                        for(i=0;i<100000000;i++);    
                    } 
                    else if (ans==SERVER_FULL) {
                      al_draw_bitmap(imagem1, 0, 0, 0);
                      al_draw_bitmap(imagem2, 0, 0, 0);
                      exibir_texto_centralizado("Server is full!",290,450);
                      al_flip_display();
                      strcpy(str,"");
                      for(i=0;i<100000000;i++);  
                    } 
                    else if (ans==SERVER_CLOSED) {
                      al_draw_bitmap(imagem1, 0, 0, 0);
                      al_draw_bitmap(imagem2, 0, 0, 0);
                      exibir_texto_centralizado("Server is closed for new connections!", 290,450);
                      al_flip_display();
                      strcpy(str,"");
                      for(i=0;i<100000000;i++);  
                    } 
                    else {
                      al_draw_bitmap(imagem1, 0, 0, 0);
                      al_draw_bitmap(imagem2, 0, 0, 0);
                      exibir_texto_centralizado("Server didn't respond to connection!",290,450);
                      al_flip_display();
                      strcpy(str,"");
                      for(i=0;i<100000000;i++); 
                    }
                    if(ans!=SERVER_UP){
                      strcpy(str,"");
                    }
                }
            }
            
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                concluido = true;
                sair = true;
                return false; 
            }
        }
        if(!concluido){
          al_draw_bitmap(imagem1, 0, 0, 0);
          al_draw_text(fonte, al_map_rgb(0,0,0), 180- al_get_font_ascent(fonte),
          (580 - al_get_font_ascent(fonte)) / 2,
          ALLEGRO_ALIGN_CENTRE + al_get_font_ascent(fonte), str);
          al_draw_bitmap(imagem2, 0, 0, 0);
          exibir_texto_centralizado("IP",290,450);
          al_flip_display();
        }
    }
    return true;
}
bool lobbyMenu(){
   	
   	int i;
    bool lobby_flag=false;
    bool instru=false,cred=false;

    // enquanto o server não se conectar
    strcpy(str,"");
    while (!lobby_flag) {
        //printf("oi\n");    
        instru=false;
        cred=false;
        while (!al_is_event_queue_empty(fila_eventos_lobby)){
        //printf("opa\n");
                    ALLEGRO_EVENT evento_lobby;
                    al_wait_for_event(fila_eventos_lobby, &evento_lobby);
         
                    if (!lobby_flag){
                        manipular_entrada(evento_lobby);
                        
                        if (evento_lobby.type == ALLEGRO_EVENT_KEY_DOWN && evento_lobby.keyboard.keycode == ALLEGRO_KEY_ENTER){
                            if(strlen(str)>0){
                                strcpy(dados.texto,str);
                                strcpy(dados.nome,meu_nome);
                                dados.id=id;
                                dados.tecla=0;
                                dados.mens=1;
                                //printf("aj %s\n", dados.texto);
                                sendMsgToServer((void *)&dados,sizeof(data));
                                strcpy(str,"");
                                dados.mens=0;
                            }
                        }
                        
                    }
            if (evento_lobby.type == ALLEGRO_EVENT_MOUSE_AXES){
                        	//printf("x %d  y %d\n", evento_lobby.mouse.x,evento_lobby.mouse.y);
            }
                        
        	if (evento_lobby.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                
                if(evento_lobby.mouse.x > 492 && evento_lobby.mouse.x < 625 && evento_lobby.mouse.y > 103 && evento_lobby.mouse.y <160){
                //instruções
                	while(!instru){
                		al_draw_bitmap(instrucoes,0,0,0);
                		al_flip_display();
                		while (!al_is_event_queue_empty(fila_eventos_lobby)){
        				//printf("opa\n");
                 	    ALLEGRO_EVENT evento_lobby;
                 	    al_wait_for_event(fila_eventos_lobby, &evento_lobby);
         				
	         				if(evento_lobby.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP){
	         					
	         					if(evento_lobby.mouse.x > 70 && evento_lobby.mouse.x < 210 && evento_lobby.mouse.y > 245 && evento_lobby.mouse.y <305){
         							instru=true;       					
             				    }					
	         				}
                		}
                		int mens = recvMsgFromServer(&dados_aux,DONT_WAIT);
				        // se receber server diconect, acaba com a função
				        if (mens == SERVER_DISCONNECTED) {
				          return false;
				        }
				        // se houver mensagem, ler os dados:
				        else if (mens != NO_MESSAGE) {
							if(dados_aux.mens==2){
								instru=true;
								lobby_flag=true;
							}	        	
				        }
                	}
                }
                else if(evento_lobby.mouse.x > 496 && evento_lobby.mouse.x < 616 && evento_lobby.mouse.y > 175 && evento_lobby.mouse.y <231){
                // créditos
                	while(!cred){
                		al_draw_bitmap(creditos,0,0,0);
                		al_flip_display();
                		while (!al_is_event_queue_empty(fila_eventos_lobby)){
        				//printf("opa\n");
                 	    ALLEGRO_EVENT evento_lobby;
                 	    al_wait_for_event(fila_eventos_lobby, &evento_lobby);
         				
	         				if(evento_lobby.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP){
	         					
	         				cred=true;       					
             				    					
	         				}
                		}
                		int mens = recvMsgFromServer(&dados_aux,DONT_WAIT);
				        // se receber server diconect, acaba com a função
				        if (mens == SERVER_DISCONNECTED) {
				          return false;
				        }
				        // se houver mensagem, ler os dados:
				        else if (mens != NO_MESSAGE) {
							if(dados_aux.mens==2){
								cred=true;
								lobby_flag=true;
							}	        	
				        }
                	}
                }
                else if(evento_lobby.mouse.x > 488 && evento_lobby.mouse.x < 630 && evento_lobby.mouse.y > 245 && evento_lobby.mouse.y <322){
                	if(id==0){
                		dados.id=id;
                		dados.permissao=10;
                		sendMsgToServer((void *)&dados,sizeof(data));
                		int mens = recvMsgFromServer(&dados_aux,WAIT_FOR_IT);
				        // se receber server diconect, acaba com a função
				        if (mens == SERVER_DISCONNECTED) {
				          return false;
				        }
				        // se houver mensagem, ler os dados:
				        else if (mens != NO_MESSAGE) {
							if(dados_aux.mens==2){
								lobby_flag=true;
							}	        	
				        }
                	}
                    else {
                        
                        while(dados_aux.mens!=2){

                            int mens = recvMsgFromServer(&dados_aux,WAIT_FOR_IT);
                            // se receber server diconect, acaba com a função
                            if (mens == SERVER_DISCONNECTED) {
                              return false;
                            }
                            // se houver mensagem, ler os dados:
                            else if (mens != NO_MESSAGE) {
                                if(dados_aux.mens==2){
                                    lobby_flag=true;
                                }               
                            }
                        }         
                    }

                }   
        	}                                         
            if (evento_lobby.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                concluido = true;
                pedir_nome - true;
                sair = true; 
                lobby_flag=true;
            }
        }

        int mens = recvMsgFromServer(&dados_aux,DONT_WAIT);
        // se receber server diconect, acaba com a função
        if (mens == SERVER_DISCONNECTED) {
          return false;
        }
        // se houver mensagem, ler os dados:
        else if (mens != NO_MESSAGE) {
            // se receber mensagem do server, mas o id não for o seu, mostra essa posição como sendo outro jogador.
            if(dados_aux.mens==1){
                for(i = MAX_LOG_SIZE; i>=1; i--){
                        strcpy(str_buffer[i],str_buffer[i-1]);
                }
                strcpy(str_buffer[0],dados_aux.texto);
                strcpy(dados_aux.texto,"");
                //printf("opa\n");
                printf("%s\n",str_buffer[0]);
                dados_aux.mens=0;
            }
        }
        if(!lobby_flag){
            al_draw_bitmap(lobby, 0, 0, 0);        
            al_draw_text(fonteChat, al_map_rgb(0,0,0),130,210,ALLEGRO_ALIGN_CENTRE +al_get_font_ascent(fonteChat), str);
            for(i = 0; i < MAX_LOG_SIZE; ++i){
                al_draw_text(fonteChat, al_map_rgb(0, 0, 0), 63, 200 - (i*al_get_font_ascent(fonteChat)), ALLEGRO_ALIGN_CENTRE +al_get_font_ascent(fonteChat), str_buffer[i]);
            }
            al_draw_text(fonteChat, al_map_rgb(0,0,0),63,210,ALLEGRO_ALIGN_CENTRE +al_get_font_ascent(fonteChat), "Mensagem:");
            al_flip_display();
        }            
    }  
    return true;
}
bool Choice(){
   al_draw_bitmap(armario,0,0,0);
    // enquanto o server não se conectar
    // Flag indicando se o mouse está sobre o retângulo central
    marcaPosicao(0,0,209,172,0,0,0,0);
    marcaPosicao(0,0,328,172,1,0,0,0);
    marcaPosicao(0,0,458,172,2,0,0,0);
    marcaPosicao(0,0,209,279,3,0,0,0);
    marcaPosicao(0,0,328,279,4,0,0,0);
    marcaPosicao(0,0,458,279,5,0,0,0);
    //marcaPosicao(0,0,193,279,6,0,0,0);
    //marcaPosicao(0,0,191,392,7,0,0,0);
    //marcaPosicao(0,0,317,392,8,0,0,0);
    //marcaPosicao(0,0,454,392,9,0,0,0);
    

    //al_draw_bitmap(armario,0,0,0);
    
    //al_flip_display();
    while (!f_choice){
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(Eventos)){
            ALLEGRO_EVENT eventoss;
            al_wait_for_event(Eventos, &eventoss);
 
            // Se o evento foi de movimentação do mouse
            if (eventoss.type == ALLEGRO_EVENT_MOUSE_AXES){
                if(eventoss.mouse.x > 171 && eventoss.mouse.x < 252 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    marcaPosicao(0,0,209,172,0,0,1,0);
                }
                else if(eventoss.mouse.x > 291 && eventoss.mouse.x < 367 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    marcaPosicao(0,0,328,172,1,0,1,0);   
                }
                else if(eventoss.mouse.x > 426 && eventoss.mouse.x < 497 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    marcaPosicao(0,0,458,172,2,0,1,0);
                }
                
                else if(eventoss.mouse.x > 156 && eventoss.mouse.x < 237 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    marcaPosicao(0,0,209,279,3,0,1,0);

                }
                else if(eventoss.mouse.x > 289 && eventoss.mouse.x < 362 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    marcaPosicao(0,0,328,279,4,0,1,0);

                }
                else if(eventoss.mouse.x > 423 && eventoss.mouse.x < 500 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    marcaPosicao(0,0,458,279,5,0,1,0);
                }/*
                else if(eventoss.mouse.x > 154 && eventoss.mouse.x < 232 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }
                else if(eventoss.mouse.x > 280 && eventoss.mouse.x < 360 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }
                else if(eventoss.mouse.x > 415 && eventoss.mouse.x < 497 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }*/
                else {
                    marcaPosicao(0,0,209,172,0,0,0,0);
                    marcaPosicao(0,0,328,172,1,0,0,0);
                    marcaPosicao(0,0,458,172,2,0,0,0);
                    marcaPosicao(0,0,209,279,3,0,0,0);
                    marcaPosicao(0,0,328,279,4,0,0,0);
                    marcaPosicao(0,0,458,279,5,0,0,0);

                }
            }
            else if (eventoss.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if(eventoss.mouse.x > 171 && eventoss.mouse.x < 252 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    Peixe_eu=0;
                    f_choice=true;
                }
                else if(eventoss.mouse.x > 291 && eventoss.mouse.x < 367 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    Peixe_eu=1;
                    f_choice=true;   
                }
                else if(eventoss.mouse.x > 426 && eventoss.mouse.x < 497 && eventoss.mouse.y > 135 && eventoss.mouse.y <199){
                    Peixe_eu=2;
                    f_choice=true;
                }
                
                else if(eventoss.mouse.x > 156 && eventoss.mouse.x < 237 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    Peixe_eu=3;
                    f_choice=true;
                }
                else if(eventoss.mouse.x > 289 && eventoss.mouse.x < 362 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    Peixe_eu=4;
                    f_choice=true;
                }
                else if(eventoss.mouse.x > 423 && eventoss.mouse.x < 500 && eventoss.mouse.y > 248 && eventoss.mouse.y <308){
                    Peixe_eu=5;
                    f_choice=true;
                }/*
                else if(eventoss.mouse.x > 154 && eventoss.mouse.x < 232 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }
                else if(eventoss.mouse.x > 280 && eventoss.mouse.x < 360 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }
                else if(eventoss.mouse.x > 415 && eventoss.mouse.x < 497 && eventoss.mouse.y > 361 && eventoss.mouse.y <424){
                    
                }*/
                
            }
            else if (eventoss.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                concluido = true;
                sair = true;
                f_choice= true;
                Morte(209,172);
                Morte(328,172);
                Morte(458,172);
                Morte(209,279);
                Morte(328,279);
                Morte(458,279);
                //Morte(193,279);
                //Morte(191,392);
                //Morte(317,392);
                //Morte(454,392); 
                return false; 
            }
        }
    int mens = recvMsgFromServer(&dados,DONT_WAIT);
            // se receber server diconect, acaba com a função
            if (mens == SERVER_DISCONNECTED) {
              return false;
            } 
            // se houver mensagem, ler os dados:
            else if (mens != NO_MESSAGE) {
                // se receber mensagem do server, mas o id não for o seu, mostra essa posição como sendo outro jogador.
                     
                if(dados.id==6){
                    bx=dados.xb;
                }
            }    
    mostraTela(bx);
    }
Morte(209,172);
Morte(328,172);
Morte(458,172);
Morte(209,279);
Morte(328,279);
Morte(458,279);
//Morte(193,279);
//Morte(191,392);
//Morte(317,392);
//Morte(454,392);  
return true;    
}
 
bool inicializar(){

    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }
    
    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar primitives\n");
        return false;
    }
 
    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_set_window_title(janela, "Aquar.io");
    

    fila_eventos = al_create_event_queue();
    fila_eventos_lobby = al_create_event_queue();
    Eventos = al_create_event_queue();
    if (!fila_eventos){ 
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
 
    // Dizemos que vamos tratar os eventos vindos do mouse
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());  
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos_lobby, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos_lobby, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos_lobby, al_get_mouse_event_source());
    al_register_event_source(Eventos, al_get_mouse_event_source());
    al_register_event_source(Eventos, al_get_display_event_source(janela));
 
    return true;
}

bool carregar_arquivos(){
    
    lobby = al_load_bitmap("AllegroAquar/Resources/Wallpaper/lobby.png");
    if(!lobby){
        fprintf(stderr, "Falha ao abrir \"lobby.png\". \n");
        return false;
    }
    end = al_load_bitmap("AllegroAquar/Resources/Wallpaper/end.png");
    if(!end){
        fprintf(stderr, "Falha ao abrir \"end.png\". \n");
        return false;
    }
    creditos = al_load_bitmap("AllegroAquar/Resources/Wallpaper/creditos.png");
    
    if (!creditos){
        fprintf(stderr, "Falha ao carregar \"creditos.png\".\n");
        return false;
    }
    instrucoes = al_load_bitmap("AllegroAquar/Resources/Wallpaper/instrucoes.png");
    
    if (!instrucoes){
        fprintf(stderr, "Falha ao carregar \"instrucoes.png\".\n");
        return false;
    }
    armario = al_load_bitmap("AllegroAquar/Resources/Wallpaper/armarioFundo.png");
    if(!armario){
        fprintf(stderr, "Falha ao abrir \"armario.png\". \n");
        return false;
    }
    sacos = al_load_bitmap("AllegroAquar/Resources/Wallpaper/sacos.png");
    if(!sacos){
        fprintf(stderr, "Falha ao abrir \"sacos.png\". \n");
        return false;
    }
    fundo = al_load_bitmap("AllegroAquar/Resources/Wallpaper/fundo.png");
    
    if (!fundo){
        fprintf(stderr, "Falha ao carregar \"fundo.jpg\".\n");
        return false;
    }
    imagem1 = al_load_bitmap("AllegroAquar/Resources/Wallpaper/tuba.png");
    
    if (!imagem1){
        fprintf(stderr, "Falha ao carregar \"tuba.jpg\".\n");
        return false;
    }
    imagem2 = al_load_bitmap("AllegroAquar/Resources/Wallpaper/dentes.png");
    
    if (!imagem2){
        fprintf(stderr, "Falha ao carregar \"dente.png\".\n");
        return false;
    }
    peixe1 = al_load_bitmap ("AllegroAquar/Resources/Players/peixe1.png");
    if(!peixe1){
        fprintf(stderr, "Falha ao carregar \"peixe1.png\". \n");
        return false;
    }
    peixe2 = al_load_bitmap ("AllegroAquar/Resources/Players/peixe2.png");
    if(!peixe1){
        fprintf(stderr, "Falha ao carregar \"peixe2.png\". \n");
        return false;
    }
   
    peixe3 = al_load_bitmap ("AllegroAquar/Resources/Players/peixe3.png");
    if(!peixe3){
        fprintf(stderr, "Falha ao carregar \"peixe3.png\". \n");
        return false;
    }
     
    peixe4 = al_load_bitmap ("AllegroAquar/Resources/Players/peixe4.png");
    if(!peixe4){
        fprintf(stderr, "Falha ao carregar \"peixe4.png\". \n");
        return false;
    }
    peixe5 = al_load_bitmap ("AllegroAquar/Resources/Players/peixe5.png");
    if(!peixe5){
        fprintf(stderr, "Falha ao carregar \"peixe5.png\". \n");
        return false;
    }
    peixe6 = al_load_bitmap ("AllegroAquar/Resources/Players/acm.png");
    if(!peixe6){
        fprintf(stderr, "Falha ao carregar \"acm.png\". \n");
        return false;
    }
    peixe7 = al_load_bitmap ("AllegroAquar/Resources/Players/cap.png");
    if(!peixe7){
        fprintf(stderr, "Falha ao carregar \"cap.png\". \n");
        return false;
    }
    
    isca = al_load_bitmap ("AllegroAquar/Resources/Players/isca.png");
    if(!isca){
        fprintf(stderr, "Falha ao carregar \"isca.png\". \n");
        return false;
    }
    racao = al_load_bitmap ("AllegroAquar/Resources/Players/racao.png");
    if(!racao){
        fprintf(stderr, "Falha ao carregar \"racao.png\". \n");
        return false;
    }
    fonte = al_load_font("AllegroAquar/Resources/Fonts/Ubuntu-R.ttf", 42, 0);
    
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"Ubuntu-R.ttf\".\n");
        return false;
    }
    fonteChat = al_load_font("AllegroAquar/Resources/Fonts/VCR_OSD_MONO_1.001.ttf", 13, 2);
    
    if (!fonteChat){
        fprintf(stderr, "Falha ao carregar \"Ubuntu-R.ttf\".\n");
        return false;
    }
    fonteM = al_load_font("AllegroAquar/Resources/Fonts/Ubuntu-R.ttf", 18, 0);
    
    if (!fonteM){
        fprintf(stderr, "Falha ao carregar \"Ubuntu-R.ttf\".\n");
        return false;
    }
 
    return true;
}
 
void finalizar()
{
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(imagem1);
    al_destroy_bitmap(imagem2);
    al_destroy_bitmap(armario);
    al_destroy_bitmap(sacos);
    al_destroy_bitmap(peixe1);
    al_destroy_bitmap(peixe2);
    al_destroy_bitmap(peixe3);
    al_destroy_bitmap(peixe4);
    al_destroy_bitmap(peixe5);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_event_queue(Eventos);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}
 
void manipular_entrada(ALLEGRO_EVENT evento){

    if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
        
        if (strlen(str) <= 29){
            char temp[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' '){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= '0' &&
                     evento.keyboard.unichar <= '9'){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar == '.'){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z'){
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z'){
                strcat(str, temp);
            }
        }
        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0){
            str[strlen(str) - 1] = '\0';
        }
    }
}
char manipular_char(ALLEGRO_EVENT evento){

    if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
        
            if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z'){
                res [0]= evento.keyboard.unichar;
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z'){
                res[0]= evento.keyboard.unichar;
            }
            else 
              res[0]= '0';
        }
}
 
void exibir_texto_centralizado(char texto [30], int linha, int coluna ){
    
    if (strlen(texto) > 0){
        al_draw_text(fonte, al_map_rgb(255,255,255), linha,
        (coluna - al_get_font_ascent(fonte)) / 2,
        ALLEGRO_ALIGN_CENTRE + al_get_font_ascent(fonte), texto);
    }
}
