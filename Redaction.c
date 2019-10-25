/*
 *  Text redaction program
 */
#include <stdio.h>

/* Array to store the text itself */
char *text[5000];
/* Array to store to words to be redacted */
char *words[50];
char *constructor;


/* Calculates the length of a string */
int length(char* string){
    int count = 0;
    
    for (int i=0 ; string[i] != '\0' ; i++){
        count++;
    }
    return count;
}

/* Converts a string to lower case so redaction is not case sensitive */
char toLowerCase(char c){
    if ( c >= 65 && c <= 90){
        return c + 32;
    }
    else return c;
}

/* Appends a signle character to the end of a string */
void append(char* s, char c){
    int len = length(constructor);
    constructor = (char *) realloc(constructor,sizeof(char) * (len +1));
    constructor[len] = c;
    constructor[len+1] = '\0';
}

/* Reads a file using a specified delimiter and stores words in an array of choice */
int readfile(char *filename, char *storage[], char delimiter){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Can't find file. \n");
        return 1;
    }
    char c;
    int count = 0;
    constructor = (char *) malloc(sizeof(char));
    while((c = fgetc(fp)) != EOF)
    {
        if(c == delimiter){
            storage[count] = constructor;
            constructor = (char *) malloc(sizeof(char));
            count++;
        }
        else{
            append(constructor,c);
        }
    }
    storage[count] = constructor;
    fclose(fp);
    int toreturn = count;
    /* Returns the number of elements being stored */
    return toreturn;
}

/* Replaces redacted word with correct number of astrix */
void replace(int index){
    for(int i = 0; text[index][i] != NULL; i++){
        if(text[index][i] == '\n'){
            break;
        }
        /* Special cases where characters should not be redacted */
        if(text[index][i] != ' ' && text[index][i] != '.' && text[index][i] != ',' && text[index][i] != ';' && text[index][i] != '\''){
            text[index][i] = '*';
        }
    }
}

/* Scans word by word for words to be redacted */
void scan(int textcount, int wordcount){
        
    for(int i = 0; i <= wordcount; i++){
        for(int j = 0; j <= textcount; j++){
            int flag = 0;
            int stop = 0;
            int additional = 0;
            int replacePreviousFlag = 1;
            
            /* Special cases at the end of words where redaction should stop */
            if(text[j][length(text[j]-1)] == ',' || text[j][length(text[j]-1)] == '.' || text[j][length(text[j]-1)] == ';'){
                stop = length(words[i])-1;
            }
            else{
                stop = length(words[i]);
            }
            /*handles redacted words with a space, e.g. "Manchester United" */
            for(int k = 0; k < stop; k++){
                if(toLowerCase(words[i][k + additional]) != toLowerCase(text[j][k]) && k < length(text[j])){
                    flag = 1;
                }
                if(k == length(text[j]) && flag == 0 && replacePreviousFlag == 1){
                    replacePreviousFlag = 0;
                    j++;
                    additional = k + 1;
                    k = 0;
                }
            }
            /* Calls for words to be redacted */
            if (flag == 0){
                if(replacePreviousFlag == 0){
                   replace(j-1);
                }
                replace(j);
            }
        }
    }
}

/*
 *  Calls to read text from files and writes redacted version to "redactedText.txt"
 */
int main() {
    
    //reads the main text file
    int count = readfile("debate.txt",text, ' ');
    int redactedwordscount = readfile("redact.txt",words, '\n');
    
    scan(count, redactedwordscount);
    
    FILE *fp2;
    
    fp2 = fopen("redactedText.txt", "w+");
    for(int j = 0; j<= count; j++){
        fprintf(fp2,"%s ",text[j]);
    }
    fclose(fp2);
    
    return 0;
}
