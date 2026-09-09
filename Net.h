#ifndef NET_H
#define NET_H

typedef int Bool;
#define TRUE 1
#define FALSE 0

typedef struct strUsuario *user; // Definimos el perfil

typedef struct str_net *net; // Red en general
// estructuras de los queues
typedef struct strqnode *qnode;
typedef struct strqueue *queue;

// Crea la red social
net net_create();

// Agrega un usuario a nuestra red social
Bool net_AddUser(net twitter,char username[],int edad, char genero);
// Encuentra su existe el usuario que le pides
user findUser(net n,char username[]);
// Imprime la lista: 0 = todo, 1 = hombres, 2 = mujeres
void net_print(net n,int dato);
//Hace que un usuario siga o otro
Bool net_follow(net n, char username1[], char username2[]);
// Elimina un usuario de la red
Bool net_delUser(net n, char username[]);
// Sube prefiles desde un archivo
void net_load_files(net n, char user_file[], char friends_file[]);
// Funciones del queue
void queue_init(queue q);
int queuein(queue q, user profile);
user queueout(queue q);
// Creamos una nueva cola ya inicializada
queue queue_create();
// Getter para obtener el nombre, edad y genero del usuario
char* get_username(user u);
int get_user_age(user u);
char get_user_gender(user u);
// Para hacer frees
void net_destroy(net n);
// Archivo tipo exel
void net_toTextoFile (net twitter, char * filename);

#endif