#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
#define LARGURA_TELA 640
#define ALTURA_TELA 480

typedef struct {
  int X,Y,permissao,id,tecla;
  char nome[30];
}data;


char str[30] = {"192.168.15.161"};
bool sair = false;
bool concluido = false;
// matriz marcao é a matriz onde é salva a posição de todos os jogadores.
// map é o mapa que é copiado para a matriz tela para mostrar no terminal.
char marcao [ALTURA_TELA] [LARGURA_TELA] ,map [ALTURA_TELA] [LARGURA_TELA];
data dados;
// posição inicial dos outros playes, pode ser qualquer valor pois eles vão ser enviados para matriz marcão e serão
// apagados como xAnterior e yAnterior.
int id,xA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1},yA[13]={1,1,1,1,1,1,1,1,1,1,1,1,1};
int x;
int y;
char res[2]={32,'\0'};
bool pedir_nome = false;


ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *imagem1 = NULL;
ALLEGRO_BITMAP *imagem2 = NULL;
ALLEGRO_BITMAP *peixe1 = NULL;
ALLEGRO_BITMAP *personagem_1 = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

// os personagens são colocados de arcordo com o seu id.
char pers[13] = {'X','Y','Z','J','D','L','U','I','S','F','E','P','M'};
// monta as matrizes de mapa que é copiada para tela. A marcação recebe espaços vazios.
void monta ();
void GeraPosicao();
void mostraTela();
void marcaPosicao(int xx, int yy, int px,int py, char personagem);
enum conn_ret_t tryConnect();
bool assertConnection();
void runGame();
int maximo();
void telaInicial();
char manipular_char(ALLEGRO_EVENT evento);
int conectar();
bool inicializar();
bool carregar_arquivos();
void finalizar();
void manipular_entrada();
void exibir_texto_centralizado();


int main() {
    inicializar();
    carregar_arquivos();

    GeraPosicao();
    monta();
    
    dados.X=x;
    dados.Y=y;
    telaInicial(); 
      
    while(!sair) {
        // roda game.
        runGame();
        
        printf("Server disconnected! Want to try again? [Y/N] ");
        int ress;
        
    }
    finalizar();
    return 0;
}

void monta () {
    int i_des=0,j_des=0;

    for( i_des = 0 ; i_des < ALTURA_TELA  ; i_des++ ){

        for( j_des = 0 ; j_des < LARGURA_TELA ; j_des++ ){
            map[i_des] [j_des] =32;
            marcao [i_des] [j_des] =32;
        }

    }
    /*
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
*/

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
            x=rand()%480;  
            y=rand()%480;
        
    } while ( x < 10 || y < 10);
    
}
// faz uma cópia da matriz map na matriz tela e depois verifica em que posições na matriz macação estão
// os jogadores. 
void mostraTela(){
    int i,j;
    al_draw_bitmap(fundo,0,0,0);
    for(i=0;i<ALTURA_TELA;i++){
        for(j=0;j<LARGURA_TELA;j++){
            if(marcao[i] [j] != 32){
                personagem_1 = al_create_sub_bitmap(peixe1, 53, 11, 35,30);
                
                al_draw_bitmap(personagem_1, j,i,0);
            }
        }
    }
    al_flip_display();
}


// tenta se conectar ao servidor.

void runGame() {
    char ch;
    ch='l';
    printf("%d %d\n",x,y );
    marcaPosicao(0,0,x,y,pers[id]);
    mostraTela();
    

    
    while (1) {
        // se alguma tecla for pressionada e for diferente de 'l', envia posição, id e tecla.
        while (!al_is_event_queue_empty(fila_eventos)){
            
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    dados.tecla = (char)evento.keyboard.keycode;
                    //printf("%d\n",dados.tecla );
                    dados.id=id;
                    dados.X=x;
                    dados.Y=y;
                    sendMsgToServer((void *)&dados,sizeof(data));
                
            }         
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = true; 
            return;
            }
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
             // se a permição para o pedido enviado for igual a 1, o client executa o que pediu para fazer.
             if(dados.permissao == 1 && id == dados.id){
                 marcaPosicao(x,y,dados.X,dados.Y,pers[id]);
                 mostraTela();
                 x=dados.X;
                 y=dados.Y;
            }
            // se receber mensagem do server, mas o id não for o seu, mostra essa posição como sendo outro jogador.
            if(dados.permissao == 1 && id != dados.id){
                marcaPosicao(xA[dados.id],yA[dados.id],dados.X,dados.Y,pers[dados.id]);
                mostraTela();
                xA[dados.id]=dados.X;
                yA[dados.id]=dados.Y;

            }
        }    
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
void telaInicial(){
   
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
                                                    pedir_nome = true;
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
                return; 
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
    al_init_primitives_addon();
    
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
    //if (!al_install_mouse())
    //{
      //  fprintf(stderr, "Falha ao inicializar o mouse.\n");
      //  al_destroy_display(janela);
      //  return -1;
    //}
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_set_window_title(janela, "Aquar.io");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){ 
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
 
    // Dizemos que vamos tratar os eventos vindos do mouse
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());  
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
    return true;
}

bool carregar_arquivos(){
    fundo = al_load_bitmap("examples/AllegroAquar/Resources/Etc/fundo.png");
    
    if (!fundo){
        fprintf(stderr, "Falha ao carregar \"fundo.jpg\".\n");
        return false;
    }
    imagem1 = al_load_bitmap("examples/AllegroAquar/Resources/Etc/tuba.png");
    
    if (!imagem1){
        fprintf(stderr, "Falha ao carregar \"tuba.jpg\".\n");
        return false;
    }
    imagem2 = al_load_bitmap("examples/AllegroAquar/Resources/Etc/dentes.png");
    
    if (!imagem2){
        fprintf(stderr, "Falha ao carregar \"dente.png\".\n");
        return false;
    }
    peixe1 = al_load_bitmap ("examples/AllegroAquar/Resources/Tilesets/peixe1.png");
    if(!peixe1){
        fprintf(stderr, "Falha ao carregar \"peixe1.png\". \n");
        return false;
    }
    fonte = al_load_font("examples/AllegroAquar/Resources/Fonts/Ubuntu-R.ttf", 42, 0);
    
    if (!fonte){
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
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
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
