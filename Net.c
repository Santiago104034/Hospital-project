#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Net.h"

// Definimos max follows
#define MAX_FOLLOWS 4

struct strUsuario {
    char username [30];
    int edad;
    char genero;
    struct strUsuario * global_next;
    struct strUsuario * gender_next;
    struct strUsuario * gender_prev;
    int followCount;
    struct strUsuario * follows[4];
};//user

struct str_net{
    user list;
    user list_male;
    user list_female;
    int user_count;
    int male_count;
    int female_count;
};// net

struct strqnode {
    user profile;
    struct strqnode *next;
};

struct strqueue {
    qnode front;
    qnode rear;
};


net net_create(){
    net n = (net) malloc(sizeof(struct str_net));
    n->female_count=0;
    n->male_count=0;
    n->user_count=0;
    n->list= NULL;
    n->list_female=NULL;
    n->list_male=NULL;
    return n;
}

user new_profile(char username[],int edad, char genero){

    user new_user =(user) malloc(sizeof(struct strUsuario));
    // Inicia valores
    new_user->edad=edad;
    new_user->genero=genero;
    strcpy(new_user->username, username);

    // Inica campos que usaremos más adelante
    new_user->global_next=NULL;
    new_user->gender_next=NULL;
    new_user->gender_prev=NULL;
    new_user->followCount=0;

    for (int i = 0; i < MAX_FOLLOWS; i++){
        new_user->follows[i] = NULL;        
    }
    return new_user;
}

Bool net_AddUser(net twitter,char username[],int edad, char genero){

    // Validación si ya existe
    if (findUser(twitter,username) != NULL)
    {
        printf("El usuario %s ya esta logeado\n",username);
        return FALSE;
    }
    
    user new_user = new_profile(username, edad, genero);

// Lista global //
    if (twitter->list==NULL){

        printf("Primer usuario que entra\n");
        twitter->list=new_user;
    }else{
        user temp = twitter->list;
        while (temp->global_next != NULL)
        {
            temp = temp->global_next;
        }
        temp->global_next=new_user;
    }

// Lista por genero //
    user *gender_list = NULL;

    if (genero == 'H') {
        gender_list = &twitter->list_male;
    } else {
        gender_list = &twitter->list_female;
    }
// lista de genero vacia //
    if(*gender_list == NULL) {
        *gender_list = new_user;
    } else {
        user temp = *gender_list;
        user prev = NULL;

        // Posicion por edad //
        while (temp != NULL && temp->edad < edad) {
            prev = temp;
            temp = temp->gender_next;
        }

        // Insertar al inicio //
        if (prev == NULL) {
            new_user->gender_next = *gender_list;
            (*gender_list)->gender_prev = new_user;
            *gender_list = new_user;
        }
        // Insertar en medio o al final //
        else {
            new_user->gender_next = temp;
            new_user->gender_prev = prev;
            prev->gender_next = new_user;
            if (temp != NULL) {
                temp->gender_prev = new_user;
        }
    }
}
// Actualizar contadores //
    twitter->user_count++;

    if (new_user->genero == 'H')
    {
        twitter->male_count++;
    }else{
        twitter->female_count++;
    }

    return TRUE;
}


user findUser(net n,char username[]){
    user current = n->list;
    while (current != NULL)
    {   // Si el name de current == name (dado)
        if (strcmp(current->username, username) == 0) 
        {
            return current;
        }
        current=current->global_next;
    }
    return NULL;
    
}

Bool net_follow(net n, char username1[], char username2[]) {
    int i;

    // Los nombres no son identicos
    int iguales = 1;
    for(i = 0; username1[i] != '\0' || username2[i] != '\0'; i++) {
        if(username1[i] != username2[i]) {
            iguales = 0;
            break;
        }
    }
    if (iguales) {
        return FALSE; // El usuario no se puede seguir a si mismo
    }

    // Buscar usuarios
    user u1 = findUser(n, username1);
    user u2 = findUser(n, username2);
    if (u1 == NULL || u2 == NULL) {
        return FALSE; // Alguno de los usuarios no existe
    }

    // Limite de follows
    if(u1->followCount >= MAX_FOLLOWS) {
        printf("No puedes seguir mas usuario\n");
        return FALSE; // No puede seguir a mas usuarios (alcanzo el maximo)
    }

    // Revisar si ya sigue al usuario
    for (i = 0; i < u1->followCount; i++) {
        if(u1->follows[i] == u2) {
            return FALSE; // Ya sigue a este usuario
        }
    }

    // Seguir al usuario
    u1->follows[u1->followCount++] = u2;
    return TRUE;
}



void net_print(net n,int dato){
    if (n == NULL) {
        return;
    }
    
    if (dato == 0) {
        user temp = n->list;
        while (temp != NULL) {
            if (temp->followCount == 0) {
                printf("El usuario %s no sigue a nadie todavia\n", temp->username);
            } 
            else {
                printf("El usuario %s sigue a %d usuarios:\n", temp->username, temp->followCount);
                for (int i = 0; i < temp->followCount; i++) {
                    printf("%s\n", temp->follows[i]->username);
                }
            }
            temp = temp->global_next;
        }
    }
    else if (dato == 1) {
        printf("Lista de hombres\n");
        user temp = n->list_male;
        while(temp != NULL) {
            printf("%s %d años\n", temp->username, temp->edad);
            temp = temp->gender_next;
        }
    }
    else if(dato == 2) {
        printf("Lista de Mujeres\n");
        user temp = n->list_female;
        while(temp != NULL) {
            printf("%s %d años\n", temp->username, temp->edad);
            temp = temp->gender_next;
        }
    }
    else {
        printf("Opcion no valida");
    }
}

// Faltantes
Bool net_delUser(net n, char username[]){

    user prev = NULL;
    user curr = n->list;
    user delete_user = NULL;
    
    while (curr != NULL && strcmp(curr->username, username) != 0 ){ // Avanzamos si no son iguales las cadenas
        prev = curr;
        curr = curr->global_next;   
        // Si no se encontro el usuario entonces devolver falso
        
    }
    
    // Si no se encontro el arreglo entonces se regresa falso
    if (curr == NULL){
        return FALSE;
    }

    delete_user = curr;
    // Posibilidad de que pase al principio
    if ( prev == NULL){
        n->list = delete_user->global_next;
    }
    else{
        prev->global_next = delete_user->global_next;
    }

    // Quitamos el genero en su lista (lista genero)

    if (delete_user->genero == 'H'){

        // Si se encuentra al principio
        if (delete_user->gender_prev == NULL){
            n->list_male = delete_user->gender_next;
            //  AÑADIR ESTA COMPROBACIÓN 
            if (delete_user->gender_next != NULL) {
                delete_user->gender_next->gender_prev = NULL;
            }
            
        }
        // Si se encuentra al final
        else if (delete_user->gender_next == NULL){
        
            delete_user->gender_prev->gender_next = NULL;
        }
        // Si estaba en medio
        else{
            delete_user->gender_prev->gender_next = delete_user->gender_next;  
            delete_user->gender_next->gender_prev = delete_user->gender_prev;
        }
    } 
    else {
        
        // Si se encuentra al principio
        if (delete_user->gender_prev == NULL){
            n->list_female = delete_user->gender_next;
            if (delete_user->gender_next != NULL) {
                delete_user->gender_next->gender_prev = NULL;
            }
            
        }
        // Si se encuentra al final
        else if (delete_user->gender_next == NULL){
            delete_user->gender_prev->gender_next = NULL;
        }
        // Si estaba en medio
        else{
            delete_user->gender_prev->gender_next = delete_user->gender_next;
            delete_user->gender_next->gender_prev = delete_user->gender_prev;
        }
    }

    // Quitar al usuario de los followers
    user profile = NULL;
    for ( profile = n->list; profile != NULL; profile = profile->global_next){
        for (int i = 0; i < profile->followCount; i++){
            if (profile->follows[i] == delete_user) {
                for (int j = i; j < profile->followCount - 1; j++) {
                    profile->follows[j] = profile->follows[j + 1];
                }
                profile->follows[profile->followCount - 1] = NULL;
                 profile->followCount--;
                i--;
            }
        }
    }
    //Le quitamos 1 por que quitamos al usuario 
    n->user_count--;

    //Lo mismo pero ahora por genero
    if (delete_user->genero == 'H'){
        n->male_count--;
    }
    else{ 
        n->female_count--;
    }

    free(delete_user);
    return TRUE;
    
}

void net_load_files(net n, char user_file[], char friends_file[]){
    FILE * F_user;
    FILE * F_friends;

    F_user=fopen(user_file,"r");
    int N_user,edad;
    char genero;
    char name[30];
    fscanf(F_user,"%d",&N_user);

    for (int i = 0; i < N_user; i++)
    {
        fscanf(F_user,"%s %d %c",name,&edad,&genero);
        net_AddUser(n,name,edad,genero);
    }
    fclose(F_user);
    // Perfiles subidos
    printf("Perfiles subidos \n");

    // Subir los amigos
    // ----------------------------
    F_friends =fopen(friends_file,"r");
    char name_seguido[30];
    char name_seguidor[30];
    // == 2 por que siempre lee 2 por linea y lo verifica que si lo haga
    while (fscanf(F_friends,"%s %s",name_seguido,name_seguidor) == 2)
    {
        net_follow(n,name_seguido,name_seguidor);
    }
    fclose(F_friends);

}

//next to file

void net_toTextoFile (net twitter, char * filename){
    
    FILE * archivo = fopen (filename, "w");

    // Agregar BOM para que lea bien acentos y ñ
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    fwrite(bom, sizeof(bom), 1, archivo);
    
    // Verificacion de cantidad de usuarios
    int count = 0;
    user temp = twitter->list;
    while (temp != NULL) {
        count++;
        temp = temp->global_next;
    }
    fprintf(archivo, "Net tiene %d users\n", count);
    
    //Impresion de los nombres 
    temp = twitter->list;
    while (temp != NULL){
        fprintf(archivo, "%s", temp->username);  
        if (temp->global_next != NULL)
            fprintf(archivo, "\t");
        temp = temp->global_next;
    }
        fprintf(archivo, "\n");

        // Impresion de los seguidores
        temp = twitter->list; // como recorrimos el temp anteriormente lo tenemos que "reiniciar" llevarlo otra vez a la primera posición
        while( temp != NULL){
            fprintf(archivo, "%s", temp->username);  
            for (int i = 0; i < temp->followCount; i++){
                fprintf(archivo, "\t%s", temp->follows[i]->username);
            }
        fprintf(archivo, "\n");         
        temp = temp->global_next;
        }
    fclose(archivo);
}

// Inicializa la cola vacía
void queue_init(queue q) {
    q->front = NULL;
    q->rear = NULL;
}

int queuein(queue q, user profile) {
    // Si no hay un usuario entonces regresar 0 de no hubo exito
    if (!profile){
        return 0;
    }  

    // Reservar memoria para un nuevo nodo
    qnode nuevo = (qnode)malloc(sizeof(struct strqnode));
    
    // Asignar datos al nodo
    nuevo->profile = profile;
    nuevo->next = NULL;

    // Si la cola está vacía, el nuevo nodo es frente y final
    if (q->rear == NULL) {
        q->front = nuevo;
        q->rear = nuevo;
    } 
    // Si ya hay elementos, se agrega al final
    else {
        q->rear->next = nuevo;
        q->rear = nuevo;
    }

    return 1;
}

user queueout(queue q) {
    // Checar si la cola vacía
    if (q->front == NULL){
        return NULL; 
    } 

    qnode temp = q->front;
    user profile = temp->profile; // Usuario a devolver

    // Avanzar el frente
    q->front = temp->next;

    // Si la cola esta vacia despues de sacar el nodo
    if (q->front == NULL)
        q->rear = NULL;

    // Liberar el nodo
    free(temp);

    return profile; 
}

// Crea una nueva cola inicializada
queue queue_create() {
    queue q = (queue) malloc(sizeof(struct strqueue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

// Devuelve el nombre del usuario (getter)
char* get_username(user u) {
    return u->username;
}
int get_user_age(user u) {
    if (u) return u->edad;
    return -1;
}

char get_user_gender(user u) {
    if (u) return u->genero;
    return '?';
}

// Frees de todo
void net_destroy(net n) {
    if (!n) return;
    user temp = n->list;
    while (temp) {
        user next = temp->global_next;
        free(temp);
        temp = next;
    }
    free(n);
}