#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "query_aux.h"

//Verifica se a string dada como argumento corresponde a um possível id de driver.
int is_driver(char* line)
{
    int i = 0;
    while(line[i])
    {
        if ((line[i]-'0') >= 0 && (line[i]-'0') < 10)
        { 
            i++;
        }
        else 
        {
            return 0;
        }
    }
    return 1;
}

//Retorna uma string com o nome do ficheiro de output a ser criado para a determinada query.
char* write_filename(int x)
{
    char* init = "./Resultados/command";
    char* end = "_output.txt";
    double x2 = x;
    double y;
    int j = 1;

    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    char buff[j+1];
    sprintf (buff, "%d", x); 
    
    char filename[21+j];
    sprintf (filename,  "%s%s%s", init, buff, end);
    char* ret = strdup(filename);


    return ret;
}

//Retorna uma string com o nome do ficheiro de output dos testes a ser criado.
char* write_tst_result(int x)
{
    char* init = "./Testes/teste";
    char* end = "_output.txt";
    double x2 = x;
    double y;
    int j = 1;

    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    char buff[j+1];
    sprintf (buff, "%d", x); 
    
    char filename[21+j];
    sprintf (filename,  "%s%s%s", init, buff, end);
    char* ret = strdup(filename);


    return ret;
}

//Retorna uma string com o nome do ficheiro onde se encontra o output que aquele comando deveria gerar.
char* write_rtfilename(int x, int testes)
{
    char* abs = "/home/rod/Documents/Testes";
    char* init = "/command";
    char* end = "_output.txt";
    double x2 = x;
    double y;
    int j = 1;

    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    char buff[j+1];
    sprintf (buff, "%d", x); 
    
    char filename[21+j];
    sprintf (filename,  "%s%s%s", init, buff, end);

    x2 = testes;
    j=1;
    
    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    char aux [j+1];
    sprintf (aux, "%d", testes);
    
    char aux2 [32+j];
    sprintf (aux2,  "%s%s%s", abs, aux, filename);

    char* ret = strdup(aux2);


    return ret;
}

//Retorna uma string correspondente ao argv que inicia o modo batch de testes para cada teste/dataset sobre o qual se deseja fazer testes.
void write_argv(char* argv[], int x)
{
    char* abs = "/home/rod";
    char* init = "/Documents/Testes";
    char* end = "/Dataset";
    double x2 = x;
    double y;
    int j = 1;
    

    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    char buff[j+1];
    sprintf (buff, "%d", x); 
    
    char filename[21+j];
    sprintf (filename,  "%s%s%s", init, buff, end);
    char aux[42+j];
    sprintf(aux, "%s%s", abs, filename);
    argv[1] = strdup(aux);

    end = "/Dataset/input.txt";
    x2 = x;
    j = 1;

    while ((y= x2/10) >= 1)
    {
        x2 = y;
        j++;
    }

    sprintf (buff, "%d", x); 
    
    sprintf (filename,  "%s%s%s", init, buff, end);
    sprintf(aux, "%s%s", abs, filename);
    argv[2] = strdup(aux);

}

/**
 * @brief Função que compara o conteúdo de dois ficheiros
 * 
 * @param exp_file Pathing relativo para o ficheiro com o output esperado
 * @param res_file Pathing relativo para o ficheiro com o output produzido
 * @return int 0 -> Os dois ficheiros não têm o mesmo conteúdo / 1 -> Os dois ficheiros têm o mesmo conteúdo
 */
int compare(char* exp_file, char* res_file)
{

    // Abrimos os dois ficheiros em modo de leitura!
    FILE* exp = fopen (exp_file, "r");
    FILE* res = fopen (res_file, "r");

    // Inicialização das strings que vão ser usadas para armazenar as linhas de cada um dos ficheiros
    int size = 4096;
    char* exp_line = malloc(size*sizeof(char));
    char* res_line = malloc(size*sizeof(char));
    int result = 1; // Variável que armazena o resultado da comparação dos ficheiros

    // Ciclo while que vai ler uma linha de cada
    while (fgets(exp_line, size, exp) && fgets(res_line, size, res))
    {
        if (strcmp(exp_line, res_line)) {
            result = 0;
            break;
        }
    }

    free(exp_line);
    free(res_line);
    fclose(exp);
    fclose(res);

    return result;
}