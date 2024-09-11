#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "glib.h"
#include "users.h"
#include "m_structs_aux.h"
#include "dates.h"
#include "verifica_input.h"

#define MAXU 100000

//Estrutura do catálogo de utilizadores.
struct user
{
    char* username;
    char* name;
    char* gender;
    int age;
    char* acc_creation;
    char* pay_meth;         
    char* acc_status;
    char* date_ride;
    double med_score;
    int n_rides;
    double tot_losses;
    int distance;
    int is_valid;
};

//Estrutura array de users.
struct suarray {
    int size;
    int max;
    struct user** Uarray;
};

//Estrutura de hash_table de utilizadores.
struct uh
{
    GHashTable* hash;
};

//Estrutura principal do módulo de utilizadores, armazena todas as outras.
struct users
{
    struct uh* uh;
    struct suarray* suarray;
};

//Inicializa uma struct user.
struct user* init_user(char* line_m)
{
    struct user* user = malloc(sizeof(struct user));
    char* line = strdup(line_m);
    char* linef = line;
    char* ptr;
    int index=0;
    user->is_valid=1;

    while((ptr=strsep(&line, ";\n")) != NULL && user->is_valid==1) 
    {
        switch (index)
        {
            case 0:
            {
                char* username_ = strdup(ptr);
                if(verificaTamanho(username_))
                {
                    user->username = username_;
                }
                else
                {
                    free(username_);
                    user->is_valid=0;
                }
                break;
            }
            case 1:
            { 
                char* name_ = strdup(ptr);
                if(verificaTamanho(name_))
                {
                    user->name = name_;
                }
                else
                {
                    free(user->username);
                    free(name_);
                    user->is_valid=0;
                }
                break;
            }
            case 2:
            {
                char* gender_ = strdup(ptr);
                if(verificaTamanho(gender_))
                {
                    user->gender = gender_; 
                }
                else
                {
                    free(user->username);
                    free(user->name);
                    free(gender_);
                    user->is_valid=0;
                }
                break;
            }
            case 3:
            {
                char* date = strdup(ptr);
                if(!verificaData(date))
                {
                    user->age = calcula_idade(create_Date(date));
                }
                else
                {
                    free(user->username);
                    free(user->name);
                    free(user->gender);
                    user->is_valid=0;
                }
                free(date);
                break;
            }
            case 4:
            {
                char* date = strdup(ptr);
                if(!verificaData(date))
                {
                    user->acc_creation = date;
                }
                else
                {
                    free(user->username);
                    free(user->name);
                    free(user->gender);
                    free(date);
                    user->is_valid=0;
                }
                break;
            }
            case 5:
            {
                char* pay_meth_ = strdup(ptr);
                if(verificaTamanho(pay_meth_))
                {
                    user->pay_meth = pay_meth_; 
                }
                else
                {
                    free(user->username);
                    free(user->name);
                    free(user->gender);
                    free(user->acc_creation);
                    free(pay_meth_);
                    user->is_valid=0;
                }
                break;
            }
            case 6:
            {
                char* acc_status_ = strdup(ptr); 
                if(verificaAccStatus(acc_status_))
                {
                    user->acc_status = acc_status_; 
                }
                else
                {
                    free(user->username);
                    free(user->name);
                    free(user->gender);
                    free(user->acc_creation);
                    free(user->pay_meth);
                    free(acc_status_);
                    user->is_valid=0;
                }
                break;
            }
        }
        index++;
    }

    if(user->is_valid == 1)
    {
        char* date = "00/00/0000";
        user->date_ride = strdup (date);
        user->med_score = 0.000;
        user->n_rides = 0;
        user->tot_losses = 0.000;
        user->distance =0;
    }

    free(linef);
    
    return user;
}

//Inicializa uma struct uh.
struct uh* init_uh()
{
    struct uh* uh = malloc(sizeof(struct uh));
    GHashTable* hash = g_hash_table_new_full(g_str_hash, g_str_equal,NULL,free_uhash);
    uh->hash = hash;

    return uh;
}

//Inicializa uma struct suarray.
struct suarray* init_suarray()
{
    struct suarray* suarray = malloc(sizeof(struct suarray));

    suarray->size = 0;
    suarray->max = MAXU;
    suarray->Uarray= malloc(sizeof(struct user*)*suarray->max);

    return suarray;
}

//Inicializa uma struct users.
struct users* init_users(struct uh* uh, struct suarray* suarray)
{
    struct users* users = malloc(sizeof(struct users));
    users->uh =uh;
    users->suarray = suarray;

    return users;
}

//Redimensiona um suarray.
int resize_suarray(struct suarray* suarray)
{ 
    if (!suarray->Uarray) 
    {
        return 0;
    } 
    suarray->max *= 2; 
    suarray->Uarray = realloc(suarray->Uarray, suarray->max*sizeof(struct user*));
    if (!suarray->Uarray ) 
    {
        return 0;
    } 
    return 1; 
}

//Insere um utilizador num suarray.
int insert_user_su(struct user* user, struct suarray* suarray)
{
    if (!suarray) 
    {
        return 0;
    }
    if (suarray->size == suarray->max) 
    {
        if (resize_suarray(suarray)) 
        {
            suarray->Uarray[suarray->size] = user; 
            suarray->size++;
            return 1; 
        }
        return 0; 
    }
    suarray->Uarray[suarray->size++] = user;
    return 1;
}

//Insere um utilizador nas estruturas necessárias.
void insert_user(struct uh* uh, struct suarray* suarray, char* line)
{
    struct user* user = init_user(line);
    GHashTable* hash = uh->hash;
    
    if (user->is_valid)
    {
        g_hash_table_insert(hash, user->username , user);

        char* status = strdup(user->acc_status);
        if (verificaActive(status))
        {
            insert_user_su(user,suarray);
        }

        free(status);
    }
    else
    {
        free(user);
    }
}

//Lê o ficheiro .csv dos utilizadores e trata os dados lidos.
struct users* load_users(char* arg)
{
    struct uh* uh = init_uh();
    struct suarray* suarray = init_suarray();

    char* argU  = g_build_path("/", arg, "users.csv", NULL);

    FILE* fptrU = fopen(argU, "r");

    char* line=NULL;
    ssize_t read;
    size_t len;

    getline(&line, &len, fptrU);
    while ((read = getline(&line, &len, fptrU)) != -1)
    {   
        insert_user (uh, suarray, line);
    }
    fclose(fptrU);
    free(argU);
    free(line);

    struct users* users = init_users(uh, suarray);

    return users;
}

//Atualiza as informações de um utilizador, face à leitura de uma viagem válida.
void update_u(char* date_user, char* user_username, double ride_cost, int distance, double score_user, double tip, struct users* users)
{
    struct user* user;
    user = g_hash_table_lookup(users->uh->hash, user_username);
        
    user->n_rides += 1;
    user->med_score = (user->med_score*(user->n_rides-1) + score_user)/user->n_rides;
    user->tot_losses += ride_cost + tip;
    user->distance += distance;

    if(compara_datas(user->date_ride, date_user) < 0)
    {
        free(user->date_ride);
        user->date_ride = date_user;
    } 
    else
    {
        free(date_user);
    }
}

/////SORTING/////

//Função de swap de utilizadores dos algoritmos de ordenação.
void swap_user(struct user** uarray, int i, int j)
{
    struct user* aux = uarray[i];
    uarray[i] = uarray[j];
    uarray[j] = aux;
}

//Função auxiliar do algoritmo de ordenação.
int partition_uarray(struct user** uarray, int left, int right)
{
    struct user* pivot = uarray[right];
    int pivot_dist = pivot->distance;

    for(int j = left; j < right; j++)
    {
        struct user* k = uarray[j];
        int k_dist = k->distance;

        if(k_dist < pivot_dist)
        {
            swap_user(uarray, left, j);
            left++;
        }
        if(k_dist == pivot_dist)
        {
            char* k_date = strdup(k->date_ride);
            char* p_date = strdup(pivot->date_ride);
            int date_cmp = compara_datas(k_date, p_date);

            if(date_cmp < 0)
            {
                swap_user(uarray, left, j);
                left++;
            }
            if(date_cmp == 0)
            {
                if(strcmp(k->username,pivot->username) > 0)
                { 
                    swap_user(uarray, left, j);
                    left++;
                }
            } 
            free(k_date);
            free(p_date);                                                                                                       
        }                                                                                                                 
    }                                                                                                                           
    swap_user(uarray, left, right);
    return left;
}

//Algoritmo QSort de ordenação de array suarray por avaliação média.
void qSort_uarray(struct user** uarray, int left, int right)
{
    if(left < right)
    {
        int index_pivot = partition_uarray(uarray, left, right); 
        qSort_uarray(uarray, left, index_pivot-1);
        qSort_uarray(uarray, index_pivot+1, right);
    }
}

//Invoca a função de sort do suarray.
void qSort_users(struct users* users)
{
    struct user** uarray = users->suarray->Uarray;
    int left = 0;
    int right = users->suarray->size - 1;

    qSort_uarray(uarray, left, right);
}

/////PRINTERS/////

//Imprime o resultado da query 1 para utilizadores.
char* print_query1_u(char* user_username, struct users* users)
{
    GHashTable* hash = users->uh->hash;
    struct user* user = g_hash_table_lookup(hash, user_username);

    char* name = Uget_name(user);
    char* gender = Uget_gender(user);
    
    char buff [100];
    sprintf (buff, "%s;%s;%d;%.3f;%d;%.3f", name, gender, user->age, user->med_score, user->n_rides, user->tot_losses);
    char* print_2_file = strdup(buff);
    
    free(name);
    free(gender);
    return print_2_file;
}

//Imprime o resultado da query 3.
int print_query3(int user_number, struct users* users, char* filename, int x, int max, int page_size, int interativo)
{
    FILE* arq = fopen(filename, "wt");
    struct user** Uarray = users->suarray->Uarray;
    int size = users->suarray->size;
    int i;

    if( interativo == 1)
    {
        i = max - (page_size-1);
    }
    else
    {
        i = 1;
        max = size;
    }

    while(i <= user_number && i <= size && i <= max)
    {
        struct user* user = Uarray[size-i];
        char* username = Uget_username(user); 
        char* name = Uget_name(user);
        if (x != 0)
        {
            fprintf(arq,"%s;%s;%.d\n", username, name, user->distance);
        }
        else
        {
            printf("%s;%s;%.d\n", username, name, user->distance);
        }
        i++;
        free(username);
        free(name);
    }

    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
    return i;
}

/////MEMORY FREES/////

//Liberta a memória alocada pela hash_table.
void free_uhash(gpointer usr)
{
    struct user* user = (struct user*)usr;

    free(user->username);
    free(user->name);
    free(user->gender);
    free(user->acc_creation);
    free(user->pay_meth);
    free(user->acc_status);
    free(user->date_ride);
    free(user);
}

//Liberta toda a memória alocada para dados de utilizadores.
void free_mem_u(struct users* users)
{
    g_hash_table_destroy (users->uh->hash);
    free(users->suarray->Uarray);
    free(users->suarray);
    free(users->uh);
    free(users);
}

/////GETTERS/////

//Getter do acc_status a partir do username de um utilizador.
int U_UNcheck_acc_status(char* user_username, struct users* users)
{
    GHashTable* h = users->uh->hash;
    struct user* user;
    if((user = g_hash_table_lookup(h, user_username)) == NULL)
    {
        return 0;
    }

    char* acc_status = strdup(user->acc_status);
    int user_status = verificaActive(acc_status);

    free(acc_status);
    return user_status;
}

//Getter do género a partir do username de um utilizador.
int U_UNcheck_gender(char* user_username, struct users* users)
{
    GHashTable* h = users->uh->hash;
    struct user* user = g_hash_table_lookup(h, user_username);

    if (strcmp(user->gender, "M") == 0)
    {
        return 1;
    }
    return 0;  
}

//Getter da acc_status a partir do username de um utilizador.
char* U_UNget_acc_creation(char* user_username, struct users* users)
{
    char* ret;
    GHashTable* h = users->uh->hash;
    struct user* user = g_hash_table_lookup(h, user_username);

    ret = strdup(user->acc_creation);
    return ret;
}

//Getter do nome a partir do username de um utilizador.
char* U_UNget_name(char* user_username, struct users* users)
{
    GHashTable* h = users->uh->hash;
    struct user* user = g_hash_table_lookup(h, user_username);

    char* ret = strdup(user->name);
    return ret;
}

//Getter do nome a partir de um utilizador.
char* Uget_name(struct user* user)
{
    char* ret = strdup(user->name);
    return ret;
}

//Getter do género a partir de um utilizador.
char* Uget_gender(struct user* user)
{
    char* ret = strdup(user->gender);
    return ret;
}

//Getter do username a partir de um utilizador.
char* Uget_username(struct user* user)
{
    char* ret = strdup(user->username);
    return ret;
}

