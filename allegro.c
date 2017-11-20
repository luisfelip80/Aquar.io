#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "client.h"
#include <string.h>
#include <ctype.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 8
#define HIST_MAX_SIZE 200

enum conn_ret_t tryConnect() {
  char server_ip[30];
  printf("Please enter the server IP: ");
  scanf(" %s", server_ip);
  getchar();
  return connectToServer(server_ip);
}

void assertConnection() {
  printHello();
  enum conn_ret_t ans = tryConnect();
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {
      puts("Server is down!");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!");
    } else {
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
 
const int LARGURA_TELA = 480;
const int ALTURA_TELA = 480;
 
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_BITMAP *Opeixe = NULL;

 
bool inicializar();
 
int main(void)
{
    bool sair = false;
    int tecla = 0;
 
    if (!inicializar())
    {
        return -1;
    }
 
    float raio = .0;
    float x = raio;
    float y = raio;
 
    while (!sair)
    {
        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
            {
                                printf(" %d \n", evento.keyboard.keycode);

                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    tecla = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    tecla = 2;
                    break;
                case ALLEGRO_KEY_LEFT:
                    tecla = 3;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    tecla = 4;
                    break;
                }
            }

            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        } 

        if (tecla)
        {
 
            switch (tecla)
            {
            case 1:
                y--;
                break;
            case 2:
                y++;
                break;
            case 3:
                x--;
                break;
            case 4:
                x++;
                break;
            }
 
            tecla = 0;
        }

        if (x >= LARGURA_TELA - raio){
            x = LARGURA_TELA - raio;
        } 
        else if (x <= raio) {
            x = raio;
        }
 
        if (y >= ALTURA_TELA - raio)
        {
            y = ALTURA_TELA - raio;
        } 
        else if (y <= raio) {
            y = raio;
        }

        al_draw_bitmap(fundo, 0, 0, 0);
        al_draw_bitmap(imagem, x, y, 0);
        al_draw_bitmap(Opeixe, 240, 240, 0);
        al_flip_display();
    }
 
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
 
    return 0;
}
 
bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }
 
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }
 
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }
 
    al_set_window_title(janela, "É nois!");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
     fundo = al_load_bitmap("aqua.png");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
     imagem = al_load_bitmap("peixe.png");
    if (!imagem)
    {
        fprintf(stderr, "Falha ao carregar imagem de imagem.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
    Opeixe = al_load_bitmap("peixe.png");
    if (!Opeixe)
    {
        fprintf(stderr, "Falha ao carregar imagem de imagem.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
    return true;
}
/* //código do client
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 8
#define HIST_MAX_SIZE 200

enum conn_ret_t tryConnect() {
  char server_ip[30];
  printf("Please enter the server IP: ");
  scanf(" %s", server_ip);
  getchar();
  return connectToServer(server_ip);
}
typedef struct {
  int X,Y;
}data;


void printHello() {
  puts("Hello! Welcome to simple chat example.");
  puts("We need some infos to start up!");
}

void assertConnection() {
  printHello();
  data move;
  enum conn_ret_t ans = tryConnect();
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {

      puts("Server is down!");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!");
    } else {
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
  char str_buffer[BUFFER_SIZE], type_buffer[MSG_MAX_SIZE] = {0};
  char msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
  int type_pointer = 0;
  
  while (1) {
    
    // mandar

    type_buffer[type_pointer++] = '\0';

    sendMsgToServer( move,sizeof(data));

    if (ret == SERVER_DISCONNECTED) {
      return;
    }

    // LER UMA MENSAGEM DO SERVIDOR
    int ret = recvMsgFromServer( , );
    if (ret == SERVER_DISCONNECTED) {
      return;
    } 

    else if (ret != NO_MESSAGE) {
      int i;
      for (i = 0; i < HIST_MAX_SIZE - 1; ++i) {
        strcpy(msg_history[i], msg_history[i + 1]);
      }
      strcpy(msg_history[HIST_MAX_SIZE - 1], str_buffer);
    }

    // PRINTAR NOVO ESTADO DO CHAT
  }
}

int main() {
  assertConnection();

  puts("Welcome to the chat example");
  puts("Just type your messages e talk to your friends");
  puts("Press [Enter] to continue");
  getchar();
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
*/