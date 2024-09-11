#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "unistd.h" 
#include "time.h"
#include "drivers.h"
#include "users.h"
#include "rides.h"
#include "m_structs_aux.h"
#include "query_aux.h"
#include "testes_aux.h"
#include "main_parse.h"

//Armazena todas as informações do programa, sendo mais fácil passá-las aos módulos correspondentes.
struct data
{
    struct drivers* drivers;
    struct users* users;
    struct rides* rides;
};

//Invoca as funções que libertam a memória alocada pelo programa.
void free_mem(struct data* data)
{
    free_mem_d(data->drivers);
    free_mem_u(data->users);
    free_mem_r(data->rides);
}

//Inicializa uma struct data.
struct data* init_data(struct drivers* drivers, struct users* users, struct rides* rides)
{
    struct data* data = malloc(sizeof(struct data));
    data->drivers = drivers;
    data->users = users;
    data->rides = rides;

    return data;
}

//Invoca as funções que fazem a leitura dos .csv e tratam os dados lidos.
struct data* load_data(char* file_path)
{
    DRIVERS drivers = load_drivers(file_path);
    USERS users = load_users(file_path);
    RIDES rides = load_rides(file_path, drivers, users);
    sort_arrays(drivers, users, rides);

    struct data* data = init_data(drivers, users, rides);

    return data;
}

//Imprime as instruções para inserção de path no modo interativo.
void print_path_intstructions()
{
    printf("  -> Para que o caminho inserido seja válido, deve respeitar o seguinte formato: /.../.../.../\n");
    printf("  -> A execução coninua de queries utilizará os mesmos ficheiros. Para alterar o caminho, deverá reiniciar o programa, selecionando a opção correspondente.\n\n");
}

//Imprime as instruções para inserção de comando no modo interativo.
void print_query_instructions()
{
    printf("  -> Para que o comando inserido seja válido deve seguir a estrutura indicada entre '' na seguinte lista para a referida query:\n\n");
    printf("  Query 1, retorna o perfil dado como parâmetro: '1 <username>' ou '1 <id_driver>'\n");
    printf("  Query 2, retorna os N condutores com maior avaliação: '2 <N>'\n");
    printf("  Query 3, retorna os N utilizadores com maior distância viajada: '3 <N>'\n");
    printf("  Query 4, retorna o preço médio das viagens (sem considerar gorjetas) numa determinada cidade: '4 <city>'\n");
    printf("  Query 5, retorna preço médio das viagens (sem considerar gorjetas) num dado intervalo de tempo: '5 <dataA> <dataB>'\n");
    printf("  Query 6, retorna a distância percorrida numa cidade num dado intrevalo de tempo: '6 <city> <dataA> <dataB>'\n");
    printf("  Query 7, retorna os N condutores com maior avaliação numa determinada cidade: '7 <N> <city>'\n");
    printf("  Query 8, retorna todas as viagens onde os condutores e os utilizadores são do género passado e tem perfis com X ou mais anos: '8 <gender> <X>'\n");
    printf("  Query 9, retorna as viagens nas quais o passageiro deu gorjeta, no intervalo de tempo: '9 <dataA> <dataB>'\n\n");
    printf("  Nota: <username> e <id_driver> é o perfil que se pretende procurar, <N> é sempre um valor inteiro que se pretende apresentar, <city> é o nome da cidade desejada, <gender> deve ser a inicial maiúscula do género desejado 'M' ou 'F', e <dataA> e <dataB> são datas que devem seguir o formato dd/mm/aaaa.\n\n");
}

//Imprime as instruções de utilização do modo interativo.
void print_instructions()
{
    printf(" --------------\n | INSTRUÇÕES |\n -------------- \n\n");
    printf("Nº1: Caminho para os ficheiros de input:\n");
    print_path_intstructions();
    printf("Nº2: Comando para execução de query:\n");
    print_query_instructions();
    printf("\n");
}

//Recebe o caminho para os ficheiros de input do modo interativo e chama a função de load dos dados.
struct data* i_parse()
{
    char line_m[100];
    printf("Por favor, insira corretamente o caminho para a pasta que contém os ficheiros de input:\n\n");
    printf("Caminho: ");
    scanf("%[^\n]", line_m);
    system("clear");
    printf("Carregando os ficheiros...\n");
    struct data* data = load_data(line_m);
    return data;
}

//Imprime o menu principal do projeto e retorna a opção escolhida pelo utilizador.
int MainMenu()
{    
    int opt;
    printf("Por favor, insira uma das seguintes opções:\n\n");
    printf("1) Iniciar programa\n");
    printf("2) Instruções de utilização\n");
    printf("0) Sair\n\n");
    printf("Opção: ");
    scanf("%d", &opt);
    getchar();
    return opt;
}

//Imprime o primeiro submenu do projeto e retorna a opção escolhida pelo utilizador.
int submenu_1()
{    
    int opt;
    printf("Por favor, insira uma das seguintes opções:\n\n");
    printf("1) Realizar uma query\n");
    printf("2) Reiniciar o programa\n");
    printf("0) Sair\n\n");
    printf("Opção: ");
    scanf("%d", &opt);
    printf("\n");
    getchar();
    return opt;
}

//Imprime o segundo submenu do projeto e retorna a opção escolhida pelo utilizador.
int submenu_2()
{    
    int opt;
    printf("Por favor, insira uma das seguintes opções:\n\n");
    printf("1) Voltar ao menu principal\n");
    printf("0) Sair\n\n");
    printf("Opção: ");
    scanf("%d", &opt);
    printf("\n");
    getchar();
    return opt;
}

//Imprime um subsubmenu e retorna a opção escolhida pelo utilizador.
int submenu_1_1()
{    
    int opt;
    printf("Por favor, insira uma das seguintes opções:\n\n");
    printf("1) Voltar a realizar uma query\n");
    printf("2) Reiniciar o programa\n");
    printf("0) Sair\n\n");
    printf("Opção: ");
    scanf("%d", &opt);
    printf("\n");
    getchar();
    return opt;
}

//Imprime o menu de paginação e retorna a opção escolhida pelo utilizador.
int menu_paginacao()
{    
    int opt;
    printf("Por favor, insira uma das seguintes opções:\n\n");
    printf("1) Avançar página\n");
    printf("2) Retroceder página\n");
    printf("3) Alterar o número de linhas por página\n");
    printf("0) Voltar\n\n");
    printf("Opção: ");
    scanf("%d", &opt);
    printf("\n");
    getchar();
    return opt;
}

//Função que, recebendo os comandos dados pelo usuário no interativo, os fornece à função dist_query, que por sua vez realizará as queries. Esta função tem também presente um menu de paginação de forma a apresentar os resultados obtidos pelas queries.
int realiza_query(struct data* data)
{

    char line_m[50];
    int opcao = 10;
    int opt = 30;
    int linha = 0;
    print_query_instructions();
    printf("Por favor, insira o comando de acordo com as instruções dadas:\n\n");
    printf("Comando: ");
    scanf("%[^\n]", line_m);
    getchar();
    system("clear");
    struct args* args = init_args(line_m);
    int q = get_query(args);

    if(q == 1 || q == 4 || q == 5 || q == 6)
        {
            opt = 1;
            printf("Não há mais páginas\n"); 
        }

    int page_size = opt;
    int max = page_size;

    printf("Página %d", max/page_size);
    printf("\n----------------------------------------\n");
    linha = dist_query (args, data->drivers, data->users, data->rides, 0, max, page_size, 1);
    printf("----------------------------------------\n");

    if(q!=1 && q!=4 && q!=5 && q!=6)
    {
        while (opcao != 0)
        {
            switch (opcao)
            {
                case 10 :
                {
                    opcao = menu_paginacao();
                    break;
                }

                case 1 :
                {
                    char* x = get_arg1(args);
                    if(q != 8 && q != 9 && max < atoi(x))
                    {
                        max += page_size;
                        system("clear");
                    }
                    else 
                    {
                        if((q == 9 || q == 8) && (linha == page_size))
                        {
                            max += page_size;
                            system("clear");
                        }
                        else
                        {
                            system("clear");
                            printf("Não há mais páginas\n");  
                        }
                    }

                    printf("\nPágina %d", max/page_size);
                    printf("\n----------------------------------------\n");
                    linha = dist_query (args, data->drivers, data->users, data->rides, 0, max, page_size, 1);
                    printf("----------------------------------------\n");
                    opcao = 10;
                    break;
                }

                case 2:
                {
                    if(max > page_size)
                    {
                        max-=page_size;
                        system("clear");
                    }
                    else
                    {
                        system("clear");
                        printf("Não há mais páginas\n");
                    }

                    printf("\nPágina %d", max/page_size);
                    printf("\n----------------------------------------\n");
                    linha = dist_query (args, data->drivers, data->users, data->rides, 0, max, page_size, 1);
                    printf("----------------------------------------\n");
                    opcao = 10;
                    break;
                }
                case 3:
                {
                    system ("clear");
                    int opt;
                    printf("Insira o número de linhas que deseja apresentar: ");
                    scanf("%d", &opt);
                    printf("\n");
                    getchar();
                    system ("clear");

                    page_size=opt;
                    max = page_size;

                    printf("\nPágina %d", max/page_size);
                    printf("\n----------------------------------------\n");
                    linha = dist_query (args, data->drivers, data->users, data->rides, 0, max, page_size, 1);
                    printf("----------------------------------------\n");
                    opcao = 10;
                    break;
                }


                default:
                {
                    system("clear");
                    opcao = 10;
                }
            }
        } 
        system("clear");
    }
    return 0;
}

//Função principal do modo interativo que inicia o modo interativo e trata todas as possibilidades de utilização por parte do user, com mecanismos de segurança face a inserção de dados inválidos.
int interativo()
{
    system("clear");
    struct data* data;
    int opt = 10;

    while (opt != 0)
    {
        switch (opt)
        {
            case 10:
            {
                opt = MainMenu();
                break;
            }
            case 1:
            {
                system("clear");
                data = i_parse();
                system("clear");
                opt = submenu_1();
                while (opt != 1 && opt != 2 && opt != 0)
                {
                    system("clear");
                    opt = submenu_1();
                }
                while(opt == 1)
                {
                    system("clear");
                    realiza_query(data);
                    opt = submenu_1_1();
                    while (opt != 1 && opt != 2 && opt != 0)
                    {
                        system("clear");
                        opt = submenu_1_1();
                    }
                }
                free_mem(data);
                if (opt == 2)
                {
                    opt = 10;
                    system("clear");
                }
                break;
            }
            case 2:
            {
                system("clear");
                print_instructions();
                opt = submenu_2();
                switch (opt)
                {
                    case 1:
                    {
                        opt = 10;
                        system("clear");
                        break;
                    }
                    case 0:
                    {
                        opt = 0;
                        break;
                    }
                    default:
                    {
                        opt = 2;
                    }
                }
                break;
            }
            default:
            {
                system("clear");
                opt = 10;
            }
        } 
    }
    system("clear");
    return 0;
}

//Função principal do modo batch que trata de chamar a função de load e de resolução das queries (main_parse), bem como a libertação de memória alocada.
int batch(char* argv[])
{
    struct data* data = load_data(argv[1]);
    mkdir("Resultados", S_IRWXU);
    main_parse (argv[2], data->drivers, data->users, data->rides);
    free_mem(data);
    free(data);
    return 0;
}

//Função de teste do modo batch que trata de chamar a função de load, registando o seu tempo de execução, e de resolução das queries, com registo do seu tempo de execução (main_parse_teste), bem como a libertação de memória alocada.
void batch_teste(double** time, char* argv[], int testes, int pf)
{
    for(int j=0; j<REPS; j++)
    {
    printf("Repetição %d\n", (j+1));
    float load_time;
    clock_t tStart = clock();
    struct data* data = load_data(argv[1]);
    load_time = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    printf("Load time: %.3fs.\n", load_time);
    time [j] = main_parse_teste(argv[2], data->drivers, data->users, data->rides, load_time, testes, pf);
    free_mem(data);
    free(data);
    }
}
