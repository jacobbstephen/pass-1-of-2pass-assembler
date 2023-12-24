#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define LEN 10
char label[LEN],opcode[LEN],operand[LEN],line[LEN];
void readLine(FILE* file_ptr){
    strcpy(label,"");
    strcpy(opcode,"");
    strcpy(operand,"");
    fgets(line,sizeof(line),file_ptr);
    printf("%s\n",line);
    int n = sscanf(line,"%s %s %s",label,opcode,operand);
    if (n != 3) {
        sscanf(line,"%s %s",opcode,operand);
        strcpy(label,"");
    }
    else sscanf(line,"%s %s %[^\n]",label,opcode,operand);
    printf("%s",line);
}

bool search_optab(){
    FILE* optab = fopen("optab.txt","r");  
    while (!feof(optab)) {
        char symbol[LEN];
        char address[LEN]; 
        fscanf(optab,"%s %s",symbol,address);
        if (strcmp(opcode, symbol) == 0) {
            return true;
        }
    }
    fclose(optab);
    return(false);
}
int main(){
    FILE* file_ptr = fopen("source.txt","r");
    FILE* inter_ptr = fopen("intermediate.txt","w");
    FILE* sym_ptr = fopen("symtab.txt","w");
    int error_flag = 0;
    readLine(file_ptr);
    int locctr;
    char start_addr[LEN];
    if(!(strcmp(opcode,"START"))){
        strcpy(start_addr,operand);
        locctr = (int) strtol(start_addr,NULL,16);
        fprintf(inter_ptr,"%s",line);
    } else locctr = 0;

    readLine(file_ptr);
    while(strcmp(opcode,"END") != 0){
        
        if(strlen(label)){
            fprintf(sym_ptr,"%s\t%x\n",label,locctr);
            
        } 
        bool found = search_optab();
        if(found) locctr += 3;
        else if (strcmp(opcode,"WORD") == 0)locctr += 3;
        else if (strcmp(opcode,"RESW") == 0) locctr += (3 * atoi(operand));
        else if (strcmp(opcode,"RESB") == 0) locctr += atoi(operand);
        else if (strcmp(opcode,"BYTE") == 0){
            if(operand[0] == 'C') locctr += strlen(operand) - 3;
            else locctr += (strlen(operand) - 3) / 2;
        }
        fprintf(inter_ptr,"%s",line);
        readLine(file_ptr);
    }
    fprintf(inter_ptr,"%s",line);
    fclose(inter_ptr);
    fclose(sym_ptr);
    fclose(file_ptr);
    printf("%d",locctr);
}