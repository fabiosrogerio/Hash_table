#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define t_chave 17
#define t_file 60
#define TAM 255

struct registro{

    struct registro* below;
    int* ocorrencia;
    char keyword[t_chave];
    int num_ocorrencia;

};
struct index{

    int t_hash;
    Registro** HASH;
    char name_file[t_file];
    int num_elementos;

};

static int verifica_num_primo(int numero_qualquer){

    int valor, i, j;


    while (numero_qualquer > 1){
        valor = numero_qualquer;
        i = 2;
        j = 0;

        while (j != 1){


                if ((valor % i == 0) && (valor / i == 1)){


                    return 0;
                    j = 1;

                }else if ((valor % i == 0) && (valor / i != 1)){

                    return 1;
                    j = 1;

                }else{
                    i++;
                }

        }


    }


}



int index_createfrom(const char *key_file, const char *text_file, Index **idx ){

    if ((fopen(key_file, "rt")) && (fopen(text_file, "rt"))){

        /*Inicio da criação do tamanho da lista HASH, sendo este tamanho o de um
        número inteiro equidistante a dois números elevados a potência 2*/

        FILE* fkey = fopen(key_file, "rt");

        int j;
        int n_elementos = 0;


        char chave[TAM];

        while(fgets(chave, TAM, fkey)!=NULL){


            if(chave[strlen(chave)-1] == '\n') chave[strlen(chave)-1] = '\0';

            if((strlen(chave)<= t_chave)&&(strlen(chave)>0)){

                n_elementos++;


            }


        }

        int t_estimado = n_elementos*3, signal = 0, aux1, aux2;
        float acumula, div;
        acumula = t_estimado;


        int a, conta1 = 0, conta2 = 0, equidistante, novo1, novo2, M;


        while(signal == 0){

            div = acumula;

            while (div >= 1){

                div = div / 2;

                if (div == 1){

                    signal = 1;

                    aux1 = acumula;

                }

            }

            acumula = acumula + 1;
        }

        signal = 0;

        acumula = t_estimado;

        while(signal == 0){

            div = acumula;

            while (div >= 2){

                div = div / 2;

                if (div == 1){

                    signal = 1;

                    aux2 = acumula;

                }

            }

            acumula = acumula - 1;
        }

        equidistante = aux1 - ((aux1 - aux2)/2);


        aux1 = equidistante;

        if(verifica_num_primo(equidistante)){

            while (verifica_num_primo(aux1)){

                aux1 = aux1 + 1;

                if (!verifica_num_primo(aux1)){

                    novo1 = aux1;
                }
                conta1++;
            }
        }

        aux1 = equidistante;

        if(verifica_num_primo(equidistante)){

            while (verifica_num_primo(aux1)){

                aux1 = aux1 - 1;

                if (!verifica_num_primo(aux1)){

                    novo2 = aux1;
                }
                conta2++;
            }
        }


        if (conta2<conta1){

            M = novo2;

        } else{

            M = novo1;

        }


        fclose(fkey);

        /*Fim da criação do tamanho da lista HASH, sendo este tamanho o de um
        número inteiro equidistante a dois números elevados a potência 2*/


        *idx = (Index*)malloc(sizeof(Index));
        Index* tabela = (*idx);
        strcpy(tabela->name_file,text_file);
        tabela->t_hash = M;
        tabela->num_elementos = n_elementos;
        tabela->HASH = (Registro**)malloc(sizeof(Registro)*tabela->t_hash);

        for (j = 0; j < tabela->t_hash; j++){
            tabela->HASH[j]= NULL;


        }

        FILE* f_key = fopen(key_file, "rt");

        while(fgets(chave, TAM, f_key)!=NULL){

            int i;
            int soma = 0;
            int indice;

            if(chave[strlen(chave)-1] == '\n') chave[strlen(chave)-1] = '\0';

            if((strlen(chave)<= t_chave)&&(strlen(chave)>0)){


                for (i = 0; i < strlen(chave); i++){


                    soma = soma+ chave[i];

                }

                Registro* novo = (Registro*)malloc(sizeof(Registro));
                strcpy(novo->keyword, chave);


                FILE* ftext = fopen(text_file, "rt");

                int linha = 1;
                j = 0;

                int* inteiros;

                inteiros = (int*)malloc(sizeof(int)*TAM);

                while(fgets(chave, TAM, ftext)!=NULL){


                    if(strstr(chave, novo->keyword)){
                        inteiros[j] = linha;
                        j++;


                    }
                    linha++;

                }
                fclose(ftext);
                novo->num_ocorrencia = j;
                novo->ocorrencia = inteiros;


                if (tabela->HASH[soma%tabela->t_hash] != NULL){

                    Registro* p = tabela->HASH[soma%tabela->t_hash];

                    do{

                        if(p->below == NULL){
                            p->below = novo;
                            novo->below = NULL;

                        }
                        p = p->below;

                    }while (p!= NULL);


                }else{

                    tabela->HASH[soma%tabela->t_hash] = novo;
                    novo->below = NULL;
                }



            }


        }

        fclose(f_key);




        return 0;

    }else{

        return 1;
    }


}
int index_get(const Index *idx, const char *key, int **occurrences, int *num_occurrences ){

    int i;
    int soma = 0;
    int tamanho;
    int signal = 0;
    for (i = 0; i < strlen(key); i++){

        soma = soma+ key[i];

    }

    const Index* p = idx;
    i = soma%p->t_hash;
    const Registro* q = p->HASH[i];


    while (q != NULL){



        if (strstr(key, q->keyword)){

            *occurrences = q->ocorrencia;
            *num_occurrences = q->num_ocorrencia;
            signal = 1;
        }

        q = q->below;

    }


    if(signal == 1){

        return 0;

    }else{

        return 1;
    }




}


int index_put( Index *idx, const char *key ){

    /*Para a inserção de novos elementos através da função index_put,
    só consegui ler novas chaves a partir do gets() e não do scanf(),
    além disso, precisei esvaziar o buffer com o 'setbuf(stdin, 0);'
    para que a variável do teste no programa principal pudesse receber
    um valor*/

    int linha = 1, i, j = 0, soma;
    int signal = 0;
    int signal1 = 0;

    Index* p = idx;

    char chave[TAM];

    for (i = 0; i < strlen(key); i++){


            soma = soma+ key[i];

    }

    i = soma%p->t_hash;


    Registro* q = p->HASH[i];

    while (q!= NULL){

        if(strcmp(key, q->keyword) == 0){

            signal = 1;

        }
        q = q->below;

    }


    if(signal != 1){

        p->num_elementos++;
        Registro* novo = (Registro*)malloc(sizeof(Registro));
        strcpy(novo->keyword, key);

        FILE* ftext = fopen(p->name_file, "rt");

        int* inteiros;
        inteiros = (int*)malloc(sizeof(int)*TAM);

        while(fgets(chave, TAM, ftext)!= NULL){


            if(strstr(chave, key)){

                inteiros[j] = linha;
                j++;


            }
            linha++;

        }
        fclose(ftext);
        novo->ocorrencia = inteiros;
        novo->num_ocorrencia = j;

        if (p->HASH[i] != NULL){

            Registro* q = p->HASH[i];

            do{

                if(q->below == NULL){
                    q->below = novo;
                    novo->below = NULL;

                }
                q = q->below;

            }while (q!= NULL);


        }else{

            p->HASH[i] = novo;
            novo->below = NULL;
        }
        signal1 = 1;

    }else{

        FILE* ftext = fopen(p->name_file, "rt");

        int* inteiros;
        inteiros = (int*)malloc(sizeof(int)*TAM);
        while(fgets(chave, TAM, ftext)!= NULL){

            if(strstr(chave, key)){

                inteiros[j] = linha;
                j++;



            }
            linha++;

        }
        fclose(ftext);

        q = p->HASH[i];

        do{

            if(strcmp(q->keyword, key) == 0){

                free(q->ocorrencia);
                q->ocorrencia = inteiros;
                q->num_ocorrencia = j;

            }
            q = q->below;

        }while (q!= NULL);

        signal1 = 1;
    }



    if(signal1 == 1){

        return 0;

    }else{

        return 1;
    }
}
int index_print( const Index *idx ){

    char aux[t_chave];
    aux[0] = '\0';
    char aux2[t_chave];
    aux2[0] = '\0';

    int signal = 0;
    int signal1 = 0;


    int i, j, soma = 0;

    const Index* p = idx;

    for (i = 0; i < p->t_hash; i++){

        const Registro*q = p->HASH[i];

        while(q != NULL){

            if(strlen(aux) == 0){


                strcpy(aux, q->keyword);
            }else{

                if(strcmp(aux,q->keyword) == 1){


                    strcpy(aux, q->keyword);
                }
            }
            q= q->below;

        }
    }

    for (i = 0; i < strlen(aux); i++){



        soma = soma+ aux[i];


    }

    i = soma%p->t_hash;


    const Registro*q = p->HASH[i];

    while (q != NULL){



        if (strcmp(aux, q->keyword) == 0){

            printf("\n%s:", q->keyword);
            signal++;
            if (q->num_ocorrencia>1){

                for (j = 0; j < q->num_ocorrencia-1; j++){

                    printf(" %d,", q->ocorrencia[j]);
                }
                printf(" %d", q->ocorrencia[j]);
                printf("\n");
                signal1++;

            }else if (q->num_ocorrencia == 1){

                printf(" %d", q->ocorrencia[0]);
                printf("\n");
                signal1++;

            }else {

                printf(" Nenhuma ocorrencia.\n");
                signal1++;
            }

        }



        q = q->below;

    }

    for (i = 0; i < p->num_elementos; i++){

        for (j = 0; j < p->t_hash; j++){

            const Registro* q = p->HASH[j];

            while(q != NULL){



                if(strcmp(q->keyword, aux) == 1){

                    if(strlen(aux2) == 0){


                        strcpy(aux2, q->keyword);
                    }else{

                        if(strcmp(aux2,q->keyword) == 1){

                            strcpy(aux2, q->keyword);

                        }
                    }
                }


                q= q->below;

            }
        }

        strcpy(aux,aux2);
        aux2[0] = '\0';

        soma = 0;

        for (j = 0; j < strlen(aux); j++){


            soma = soma+ aux[j];


        }

        j = soma%p->t_hash;


        q = p->HASH[j];

        while (q != NULL){



            if (strcmp(aux, q->keyword) == 0){

                printf("%s:", q->keyword);
                signal++;
                if (q->num_ocorrencia>1){

                    for (j = 0; j < q->num_ocorrencia-1; j++){

                        printf(" %d,", q->ocorrencia[j]);
                    }
                    printf(" %d", q->ocorrencia[j]);
                    printf("\n");
                    signal1++;

                }else if (q->num_ocorrencia == 1){

                    printf(" %d", q->ocorrencia[0]);
                    printf("\n");
                    signal1++;

                }else {

                    printf(" Nenhuma ocorrencia.\n");
                    signal1++;
                }


            }


            q = q->below;

        }
    }


    if((signal == p->num_elementos) && (signal1 == p->num_elementos)){

        return 0;
    }else{

        return 1;
    }


}
