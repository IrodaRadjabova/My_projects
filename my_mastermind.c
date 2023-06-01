#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int index_compare(char* collect, char x){
    int i = 0;
    while(collect[i] != '\0'){
        if(x == collect[i]){
            return i;
        }
        i++;
    } return -1;
}

char* r_generator () {
    srand((unsigned) time(NULL));
    char* collect = malloc(5);
    int i = 0;
    
    while(i < 4){
        // collect[i] = rand() % 9 + 48;
        char x = rand() % 9 + 48;
        if(index_compare(collect, x) == -1){
            collect[i] = x;
            i++; 
        }
    }
    collect[i] = '\0';
    return collect;
}

char* parse_com_line(int ac, char** av, int* attempts){
    char* s_code = NULL;
    for(int i = 1; i < ac; i++){
        if(strcmp(av[i], "-t") == 0){
            i++;
            *attempts = atoi(av[i]);
        }
        if(strcmp(av[i], "-c") == 0){
            i++;
            s_code = av[i];
        }
    }
    if(s_code == NULL){
        s_code = r_generator();
    }
    return s_code;
}

void printing_error(){
    printf("Wrong input!\n");
}

int checking_result(char* result){
    
    int len = strlen(result);
    if(len!=4){
        printing_error();
        return 1;
    }
    for(int i = 0; i < len; i++){
        if (result[i] > '8' || result[i] < '0') {
            printing_error();
            return 1;
        }
    }
    int i = 0;
    while(i < 3){
        for(int j = i+1; j < 4; j++){ 
            if(result[i] == result[j]){
                printing_error();
                return 1;
            }
        }
    i++;
    }

    return 0;
}

void filling_null(char* str, int size){
    int i = 0;
    while(i < size){
        str[i] ='\0';
        i++;
    }
}

char* reading_lines(){
    char buffer;
    char* read_result = malloc(10);
    int i = 0;
    
    int flag = 0;

    do{
        write(1, ">", 1);
        i = 0;
        filling_null(read_result, 10);
        flag = 0;

        for(; read(0, &buffer, 1); i++){
            if(buffer=='\n'){
                flag = 1;
                break;
            }
            read_result[i] = buffer;
        }
    }
    while(checking_result(read_result) == 1 && flag == 1);
    if(flag != 0){
        return read_result;
    }
    return NULL;
}

int check_places(char* secret_code, char* user_code){
    int x = 0;  // Wellplaced numbers
    int y  = 0; // Missplaced numbers
    for(int i = 0; i < 4; i++){
        if (secret_code[i] == user_code[i]) {
            x++;
        }
        else if((index_compare(secret_code, user_code[i])) != -1){
            y++;
        }
    }

    if(x == 4){
        printf("Congratz! You did it!\n");
    }
    else{
        printf("Well placed pieces: %d\nMisplaced pieces: %d\n", x, y);
    }
    return x;

}

void game_starts(int attempt, char* secret_code){

    printf("Will you find the secret code?\nPlease enter a valid guess\n");
    char* user_code = NULL;

    for (int i =0; i < attempt; i++) {
        printf("---\nRound %d\n",i);
        if((user_code = reading_lines()) == NULL){
            return;
        }
        
        if(check_places(secret_code, user_code) == 4){
            break;
        }
    }
}

int main(int ac, char** av){
    char* secret_code = NULL;
    int attempt = 10;
    secret_code = parse_com_line(ac, av, &attempt);

    game_starts(attempt, secret_code);

    return 0;
}