#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Net.h"

#define BOYS 1
#define GIRLS 2
#define ALL 0


int main(){
    
    // Creacion de los primero usuarios
    net twitter = net_create();

    int opcion = -1;
    char username[30];
    char seguido[30];
    char archivo1[50], archivo2[50];
    int edad;
    char genero;

    while (opcion != 10)
    {
        printf("Bienvenido a nuestra red social, que quieres hacer???\n");
        printf("[1]-- Crear una nueva net \n");
        printf("[2]-- Agregar un nuevo usuario\n");
        printf("[3]-- Establecer un nuevo seguidor\n");
        printf("[4]-- Imprimir a los usuarios y seguidores\n");
        printf("[5]-- Insertar nuevos usuarios y seguidores por archivos\n");
        printf("[6]-- Crear archivo de excel con los datos\n"); 
        printf("[7]-- Encontrar un usuario\n");
        printf("[8]-- Borrar un usuario\n"); 
        printf("[9]-- Fila\n"); 
        printf("[10]-- Salir\n");
        scanf("%d",&opcion);

        switch (opcion)
        {
        case 1:
            twitter = net_create();
            break;
        case 2:
            printf("Introduce el nombre, edad y genero (si es hombre coloque H o M para mujer): ");
            scanf("%s %d %c", username, &edad, &genero);
            net_AddUser(twitter, username, edad, genero);
            break;
        case 3:
            printf("Coloque el nombre de usuario de su cuenta y luego el nombre de usuario de a quien seguira: ");
            scanf("%s %s", username, seguido);
            net_follow(twitter, username, seguido);
            break;
        case 4: {
            int tipo;
            printf("¿Que deseas imprimir?\n");
            printf("0 - Todos los usuarios\n");
            printf("1 - Solo hombres\n");
            printf("2 - Solo mujeres\n");
            scanf("%d", &tipo);
            net_print(twitter, tipo);
            break;
        }
        case 5:
            printf("Archivo de usuarios y archivo de relaciones: ");
            scanf("%s %s", archivo1, archivo2);
            net_load_files(twitter, archivo1, archivo2);
            break;
        case 6:
            printf("Nombre del archivo CSV: ");
            scanf("%s", archivo1);
            net_toTextoFile(twitter, archivo1);
            break;
        case 7:
            printf("Usuario a buscar: ");
            scanf("%s", username);
            user encontrado = findUser(twitter, username);
            if (encontrado != NULL)
            printf("Usuario encontrado: %s, %d años, genero %c\n",
            get_username(encontrado),get_user_age(encontrado),get_user_gender(encontrado));
            else
            printf("No se encontro al usuario '%s'.\n", username);
            break;
        case 8:
            printf("Usuario a eliminar: ");
            scanf("%s", username);
            net_delUser(twitter, username);
            break;
        case 9:{
            // Crear una nueva cola usando la función pública
            queue q = queue_create();
            if (!q) {
                printf("Error: no se pudo crear la cola.\n");
                break;
            }

            // Metemos algunos usuarios (si existen)
            const char* nombres[] = {"Jose", "Juan", "Ana"};
            for (int i = 0; i < 3; i++) {
                user profile = findUser(twitter, (char*)nombres[i]);
                if (!queuein(q, profile))
                    printf("%s no se pudo encolar.\n", nombres[i]);
                else
                    printf("%s fue agregado a la cola.\n", nombres[i]);
            }

            // Sacar un usuario
            user sacado = queueout(q);
            if (sacado)
                printf("Se sacó de la cola a: %s\n", get_username(sacado));
            else
                printf("La cola ya estaba vacia\n");

            // Liberar la memoria de la cola
            free(q);
        }
        break;
        default:
            break;// 4,7,8,9
        }
        printf("\n");
    }
}