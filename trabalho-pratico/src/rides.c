#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "glib.h"
#include "rides.h"
#include "drivers.h"
#include "users.h"
#include "m_structs_aux.h"
#include "dates.h"
#include "verifica_input.h"

#define MAXR 1000000

//Estrutura do catálogo de viagens.
struct ride
{
    char* id;
    char* date;
    char* driver_id;
    char* user_username;
    char* city;
    int distance;
    double score_user;
    double score_driver;
    double tip;
    double ride_cost;
    int is_valid;
}; 

//Estrutura array de viagens.
struct srarray 
{
    int size;
    int max;
    struct ride** Rarray;
};

//Estrutura de preço médio de viagens por cidade.
struct mpreco
{
    double price;
    int n_rides;
};

//Estrutura de hash_table de viagens.
struct rh
{
    GHashTable* hash;
};

//Estrutura principal do módulo de viagens, armazena todas as outras.
struct rides
{
    struct rh* rh;
    struct rh* ch;
    struct srarray* sr_male;
    struct srarray* sr_female;
    struct srarray* sr_tip;
};

//Estrutura utilizada para atualização de dados com auxilio de g_hash_table_foreach.
struct foreach
{
    struct drivers* drivers;
    struct users* users;
    struct rh* ch;
};

//Estrutura utilizada para resolução de query 5 e 6 com auxilio de g_hash_table_foreach.
struct foreach_5_6
{
    char* date_1;
    char* date_2;
    char* city;
    double cost;
    int distance;
    double n_rides;
};

//Inicializa uma struct ride.
struct ride* init_ride(char* line_m)
{
    struct ride* ride = malloc(sizeof(struct ride));
    char* line = strdup(line_m);
    char* linef = line;
    char* ptr;
    int index=0;
    ride->is_valid=1;

    while((ptr=strsep(&line, ";\n")) != NULL && ride->is_valid) 
    {
        switch (index)
        {
            case 0:
            {
                char* id_ = strdup(ptr);
                if(verificaTamanho(id_))
                {
                    ride->id = id_; 
                }
                else
                {
                    free(id_);
                    ride->is_valid=0;
                }                    
                break;
            }
            case 1:
            { 
                char* date = strdup(ptr);
                if(!verificaData(date))
                {
                    ride->date = date;
                }
                else
                {
                    free(ride->id);
                    free(date);
                    ride->is_valid=0;
                }
                break;
            }
            case 2:
            { 
                char* driver_id_ = strdup(ptr);
                if(verificaTamanho(driver_id_))
                {
                    ride->driver_id = driver_id_; 
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(driver_id_);
                    ride->is_valid=0;
                }                    
                break;
            }
            case 3:
            { 
                char* user_username_ = strdup(ptr); 
                if(verificaTamanho(user_username_))
                {
                    ride->user_username = user_username_; 
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(user_username_);
                    ride->is_valid=0;
                }                    
                break;
            }
            case 4:
            { 
                char* city_ = strdup(ptr);
                if(verificaTamanho(city_))
                {
                    ride->city = city_; 
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(ride->user_username);
                    free(city_);
                    ride->is_valid=0;
                }
                break;
            }
            case 5:
            {
                char* distance_ = strdup(ptr);
                int x = atoi(distance_);
                if(verificaInt(distance_) && x>0)
                {
                    ride->distance = x;
                }   
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(ride->user_username);
                    free(ride->city);
                    ride->is_valid=0;
                }     
                free(distance_);          
                break;
            }
            case 6:
            { 
                char* score_user_ = strdup(ptr); 
                int x = atoi(score_user_);
                if(verificaInt(score_user_) && x >0)
                {
                    ride->score_user = x;
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(ride->user_username);
                    free(ride->city);
                    ride->is_valid=0;
                }
                free(score_user_);               
                break;
            }
            case 7:
            {
                char* score_driver_ = strdup(ptr); 
                int x = atoi(score_driver_);
                if(verificaInt(score_driver_) && x >0)
                {
                    ride->score_driver = x; 
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(ride->user_username);
                    free(ride->city);
                    ride->is_valid=0;
                }      
                free(score_driver_);         
                break;
            }
            case 8:
            {
                char* tip_ = strdup(ptr); 
                if(!verificaDouble(tip_))
                {
                    ride->tip = arredonda(tip_); 
                }
                else
                {
                    free(ride->id);
                    free(ride->date);
                    free(ride->driver_id);
                    free(ride->user_username);
                    free(ride->city);
                    ride->is_valid=0;
                }     
                free(tip_);          
                break;
            }       
        }
        index++;
    }

    if (ride->is_valid == 1)
    {    
        ride->ride_cost = 0;
    }

    free(linef);
    
    return ride;
}

//Inicializa uma struct mpreco.
struct mpreco* init_mpreco()
{
    struct mpreco* mpreco = malloc(sizeof(struct mpreco));
    mpreco->price = 0.000;
    mpreco->n_rides = 0;

    return mpreco;
}

//Inicializa uma struct rh.
struct rh* init_rh()
{
    struct rh* rh = malloc(sizeof(struct rh));
    GHashTable* hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free_rhash);
    rh->hash = hash;

    return rh;
}

//Inicializa uma struct rh para cidades.
struct rh* init_ch()
{
    struct rh* ch = malloc(sizeof(struct rh));
    GHashTable* hash = g_hash_table_new_full(g_str_hash, g_str_equal, free_chash_k, free_chash_v);
    ch->hash = hash;

    return ch;
}

//Inicializa uma struct srarray.
struct srarray* init_srarray()
{
    struct srarray* srarray = malloc(sizeof(struct srarray));

    srarray->size = 0;
    srarray->max = MAXR;
    srarray->Rarray= malloc(sizeof(struct ride*)*srarray->max);

    return srarray;
}

//Inicializa uma struct foreach.
struct foreach* init_foreach(struct drivers* drivers, struct users* users,struct rh* ch)
{
    struct foreach* foreach = malloc(sizeof(struct foreach));
    foreach->drivers = drivers;
    foreach->users = users;
    foreach->ch = ch;

    return foreach;
}

//Inicializa uma struct foreach_5_6.
struct foreach_5_6* init_foreach_5_6(char* date_1, char* date_2, char* city)
{
    struct foreach_5_6* foreach_5_6 = malloc(sizeof(struct foreach_5_6));
    foreach_5_6->date_1 = date_1;
    foreach_5_6->date_2 = date_2;
    foreach_5_6->city = city;
    foreach_5_6->cost = 0;
    foreach_5_6->distance = 0;
    foreach_5_6->n_rides = 0;

    return foreach_5_6;
}

//Inicializa uma struct rides.
struct rides* init_rides(struct rh* rh, struct rh* ch, struct srarray* sr_male, struct srarray* sr_female, struct srarray* sr_tip)
{
    struct rides* rides = malloc(sizeof(struct rides));
    rides->rh = rh;
    rides->ch = ch;
    rides->sr_male = sr_male;
    rides->sr_female = sr_female;
    rides->sr_tip = sr_tip;

    return rides;
}

//Redimensiona um srarray.
int resize_srarray(struct srarray* srarray)
{ 
    if (!srarray->Rarray) 
    {
        return 0; 
    }
    srarray->max *= 2; 
    srarray->Rarray = realloc(srarray->Rarray, srarray->max*sizeof(struct ride*));
    if (!srarray->Rarray ) 
    {
        return 0;
    } 
    return 1; 
}

//Insere uma viagem num srarray.
int insert_ride_sr(struct ride* ride, struct srarray* srarray)
{
    if (!srarray) 
    {
        return 0;
    }
    if (srarray->size == srarray->max) 
    {
        if (resize_srarray(srarray)) 
        {
            srarray->Rarray[srarray->size] = ride; 
            srarray->size++;
            return 1; 
        }
        return 0; 
    }
    srarray->Rarray[srarray->size++] = ride;
    return 1;
}

//Insere uma viagem nas estruturas necessárias.
void insert_ride(struct rh* rh, struct srarray* sr_male, struct srarray* sr_female, struct srarray* sr_tip, char* line, struct drivers* drivers, struct users* users)
{
    struct ride* ride = init_ride(line);
    GHashTable* hash = rh->hash;

    if(ride->is_valid)
    {
        g_hash_table_insert(hash, ride->id , ride);

        char* driver_id = strdup(ride->driver_id);
        char* user_username = strdup(ride->user_username);

        int active_2 = both_active(driver_id, user_username, drivers, users);

        if (active_2)
        {
            int eq_gender = same_gender(driver_id, user_username, drivers, users);

            if (eq_gender == 1)
            {
                insert_ride_sr(ride, sr_male);
            }

            if (eq_gender == 2)
            {
                insert_ride_sr(ride, sr_female);
            }
        }

        if (ride->tip >= 0)
        {
            insert_ride_sr(ride, sr_tip);
        }

        free(driver_id);
        free(user_username);
    }
    else
    {
        free(ride);
    }
}

//Atualização do mpreco da cidade de uma viagem aquando da leitura de uma mesma válida.
void update_c(char* city, double ride_cost, struct rh* ch)
{
    GHashTable* hash = ch->hash;
    struct mpreco* mpreco;

    if(g_hash_table_lookup(hash, city) != NULL)
    {
        mpreco = g_hash_table_lookup(hash, city);
        free(city);
    }
    else 
    {
        mpreco = init_mpreco();
        g_hash_table_insert(hash, city, mpreco);
    }
    
    mpreco->n_rides += 1;
    mpreco->price += ride_cost;
}

//Atualização dos dados de todos os módulos principais através da leitura de todas as entradas da hash table de rides rh.
void foreach_updt(gpointer key, gpointer value, gpointer userdata)
{
    struct ride* ride = (struct ride*)value;
    struct foreach* foreach = (struct foreach*)userdata;

    char* date_driver = Rget_date(ride);
    char* date_user = Rget_date(ride);
    char* driver_id = Rget_driver_id(ride);
    char* user_username = Rget_user_username(ride);
    char* city = Rget_city(ride);
    int distance = Rget_distance(ride);
    double score_driver = Rget_score_driver(ride);
    double score_user = Rget_score_user(ride);
    double tip = Rget_tip(ride);

    double ride_cost = update_d(date_driver, driver_id, distance, score_driver, city, tip, foreach->drivers);
    update_u(date_user, user_username, ride_cost, distance, score_user, tip, foreach->users);
    update_c(city, ride_cost, foreach->ch);

    ride->ride_cost = ride_cost;

    free(driver_id);
    free(user_username);
}

//Resolução dos cálculos necessários à apresentação do pedido na query 5 pela travessia da hash table rh.
void foreach_q5(gpointer key, gpointer value, gpointer userdata)
{
    struct ride* ride = (struct ride*)value;
    struct foreach_5_6* foreach_5_6 = (struct foreach_5_6*)userdata;

    char* date_1 = strdup(foreach_5_6->date_1);
    char* date_2 = strdup(foreach_5_6->date_2);
    char* ride_date = strdup(ride->date);

    if (belongs(date_1, date_2, ride_date))
    {
        foreach_5_6->cost += ride->ride_cost;
        foreach_5_6->n_rides += 1;
    }
    free(date_1);
    free(date_2);
    free(ride_date);
}

//Resolução dos cálculos necessários à apresentação do pedido na query 6 pela travessia da hash table rh.
void foreach_q6(gpointer key, gpointer value, gpointer userdata)
{
    struct ride* ride = (struct ride*)value;
    struct foreach_5_6* foreach_5_6 = (struct foreach_5_6*)userdata;

    char* date_1 = strdup(foreach_5_6->date_1);
    char* date_2 = strdup(foreach_5_6->date_2);
    char* ride_date = strdup(ride->date);
    char* city = foreach_5_6->city;

    if ((strcmp (city, ride->city) == 0) && belongs(date_1, date_2, ride_date))
    {
        foreach_5_6->distance += ride->distance;
        foreach_5_6->n_rides += 1;
    }
    free(date_1);
    free(date_2);
    free(ride_date);
}

//Verifica se a cidade pedida num comando é uma cidade existente nos dados das viagens.
int real_city(char* city, struct rides* rides)
{
    GHashTable* hash = rides->ch->hash;
    if (g_hash_table_lookup(hash, city) == NULL)
    {
        return 0;
    }
    return 1;
}

/////SORTING/////

//Função de swap de viagens dos algoritmos de ordenação.
void swap_ride(struct ride** rarray, int i, int j)
{
    struct ride* aux = rarray[i];
    rarray[i] = rarray[j];
    rarray[j] = aux;
}

//Função auxiliar do algoritmo de ordenação do array de tips sr_tip.
int partition_rarray_t(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users)
{
    struct ride* pivot = rarray[right];
    int p_distance = Rget_distance(pivot);

    for(int j = left; j < right; j++)
    {
        struct ride* k = rarray[j];
        int k_distance = Rget_distance(k);
        int cmp_dist = k_distance - p_distance;

        if(cmp_dist > 0)
        {
            swap_ride(rarray, left, j);
            left++;
        }

        if(cmp_dist == 0)
        {
            char* p_date = Rget_date(pivot);
            char* k_date = Rget_date(k);
            int cmp_date = compara_datas(k_date, p_date);


            if(cmp_date > 0)
            {
                swap_ride(rarray, left, j);
                left++;
            }

            if(cmp_date == 0)
            {
                if(strcmp(k->id, pivot->id) > 0)
                { 
                    swap_ride(rarray, left, j);
                    left++;
                }        
            } 
            free(p_date);
            free(k_date);                                                                                                      
        }                                                                                   
    }                                                                                                                           
    swap_ride(rarray, left, right);

    return left;
}

//Função auxiliar do algoritmo de ordenação dos arrays sr_male e sr_female.
int partition_rarray_fm(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users)
{
    struct ride* pivot = rarray[right];
    char* p_driver_id = strdup(pivot->driver_id);
    char* p_driver_acc_creation = D_IDget_acc_creation(p_driver_id, drivers);


    for(int j = left; j < right; j++)
    {
        struct ride* k = rarray[j];
        char* k_driver_id = strdup(k->driver_id);
        char* k_driver_acc_creation = D_IDget_acc_creation(k_driver_id, drivers);

        int cmp_date_d = compara_datas(k_driver_acc_creation, p_driver_acc_creation);

        if(cmp_date_d < 0)
        {
            swap_ride(rarray, left, j);
            left++;
        }

        if(cmp_date_d == 0)
        {
            char* p_user_username = strdup(pivot->user_username);
            char* p_user_acc_creation = U_UNget_acc_creation(p_user_username, users);

            char* k_user_username = strdup(k->user_username);
            char* k_user_acc_creation = U_UNget_acc_creation(k_user_username, users);

            int cmp_date_u = compara_datas(k_user_acc_creation, p_user_acc_creation);

            if(cmp_date_u < 0)
            {
                swap_ride(rarray, left, j);
                left++;
            }

            if(cmp_date_u == 0)
            {
                if(strcmp(k->id, pivot->id) < 0)
                { 
                    swap_ride(rarray, left, j);
                    left++;
                }        
            }  
            free(p_user_username);
            free(p_user_acc_creation);
            free(k_user_username);
            free(k_user_acc_creation);                                                                                                      
        }
        free(k_driver_id);
        free(k_driver_acc_creation);                                                                                    
    }                                                                                                                           
    swap_ride (rarray, left, right);

    free(p_driver_id);
    free(p_driver_acc_creation);
    return left;
}

//Algoritmo QSort de ordenação de array sr_tip por valor de gorgeta.
void qSort_rarray_t(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users)
{
    if(left < right)
    {
        int index_pivot = partition_rarray_t(rarray, left, right, drivers, users); 
        qSort_rarray_t(rarray, left, index_pivot-1, drivers, users);
        qSort_rarray_t(rarray, index_pivot+1, right, drivers, users);
    }
}

//Algoritmo QSort de ordenação dos arrays sr_male e sr_female por idade de contas.
void qSort_rarray_fm(struct ride** rarray, int left, int right, struct drivers* drivers, struct users* users)
{
    if(left < right)
    {
        int index_pivot = partition_rarray_fm(rarray, left, right, drivers, users); 
        qSort_rarray_fm(rarray, left, index_pivot-1, drivers, users);
        qSort_rarray_fm(rarray, index_pivot+1, right, drivers, users);
    }
}

//Invoca a função de sort dos arrays de viagens.
void qSort_rides(struct rides* rides, struct drivers* drivers, struct users* users)
{
    int left = 0;
    struct ride** rarray_m = rides->sr_male->Rarray;
    int right_m = rides->sr_male->size - 1;

    struct ride** rarray_f = rides->sr_female->Rarray;
    int right_f = rides->sr_female->size - 1;

    struct ride** rarray_t = rides->sr_tip->Rarray;
    int right_t = rides->sr_tip->size - 1;

    qSort_rarray_fm(rarray_m, left,right_m, drivers, users);
    qSort_rarray_fm(rarray_f, left,right_f, drivers, users);
    qSort_rarray_t(rarray_t, left,right_t, drivers, users);
}

/////LOAD/////

//Lê o ficheiro .csv das viagens e trata os dados lidos.
struct rides* load_rides(char* arg, struct drivers* drivers, struct users* users)
{
    struct rh* rh = init_rh();
    struct rh* ch = init_ch();
    struct foreach* foreach = init_foreach(drivers, users, ch);
    struct srarray* sr_male = init_srarray();
    struct srarray* sr_female = init_srarray();
    struct srarray* sr_tip = init_srarray();

    char* argR  = g_build_path("/", arg, "rides.csv", NULL);

    FILE* fptrR = fopen(argR, "r");

    char* line=NULL;
    ssize_t read;
    size_t len;

    getline(&line, &len, fptrR);
    while ((read = getline(&line, &len, fptrR)) != -1)
    {   
        insert_ride (rh, sr_male, sr_female, sr_tip, line, drivers, users);
    }
    fclose(fptrR);
    free(argR);
    free(line);

    g_hash_table_foreach(rh->hash, foreach_updt, foreach);
    free(foreach);

    struct rides* rides = init_rides(rh, ch, sr_male, sr_female, sr_tip);

    return rides;
}

/////PRINTERS/////

//Imprime o resultado da query 4.
void print_query4(char* city,struct rides* rides, char* filename, int x)
{
    FILE* arq = fopen(filename, "wt");
    GHashTable* hash = rides->ch->hash;

    struct mpreco* mpreco;
    if ((mpreco = g_hash_table_lookup(hash, city)) != NULL)
    {
        if (x != 0)
        {
            fprintf (arq, "%.3f\n", mpreco->price/mpreco->n_rides);
        }
        else
        {
            printf ("%.3f\n", mpreco->price/mpreco->n_rides);
        }    
    }
    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
}

//Imprime o resultado da query 5.
void print_query5(char* date_1, char* date_2, struct rides* rides, char* filename, int x)
{
    FILE* arq = fopen(filename, "wt");
    struct foreach_5_6* foreach_5_6 = init_foreach_5_6(date_1, date_2, NULL);
    g_hash_table_foreach(rides->rh->hash, foreach_q5, foreach_5_6);

    if(foreach_5_6->n_rides != 0)
    {
        if (x != 0)
        {    
            fprintf(arq, "%.3f\n", foreach_5_6->cost/foreach_5_6->n_rides);
        }
        else
        {
            printf("%.3f\n", foreach_5_6->cost/foreach_5_6->n_rides);
        }
    }

    free(foreach_5_6->date_1);
    free(foreach_5_6->date_2);
    free(foreach_5_6);
    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
}

//Imprime o resultado da query 6.
void print_query6(char* city, char* date_1, char* date_2, struct rides* rides, char* filename, int x)
{               
    FILE* arq = fopen(filename, "wt");
    struct foreach_5_6* foreach_5_6 = init_foreach_5_6(date_1, date_2, city);
    g_hash_table_foreach(rides->rh->hash, foreach_q6, foreach_5_6);

    if(foreach_5_6->n_rides != 0)
    {
        if (x != 0)
        {
            fprintf(arq, "%.3f\n", foreach_5_6->distance/foreach_5_6->n_rides);
        }
        else
        {
            printf("%.3f\n", foreach_5_6->distance/foreach_5_6->n_rides);
        }
    }

    free(foreach_5_6->date_1);
    free(foreach_5_6->date_2);
    free(foreach_5_6->city);
    free(foreach_5_6);
    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
}

//Imprime o resultado da query 8 e retorna um valor "linha" que serve de auxilio para o modo interativo do projeto.
int print_query8(char* gender, int acc_age, struct drivers* drivers, struct users* users, struct rides* rides, char* filename, int x, int max, int page_size, int interativo)
{   
    FILE* arq = fopen(filename, "wt");
    struct ride** Rarray;
    struct srarray* Sarray = init_srarray();
    int size;
    int i=0, linha = 0;


    if (strcmp("M",gender) == 0)
    {
        Rarray = rides->sr_male->Rarray;
        size = rides->sr_male->size;
    }
    else
    {
        Rarray = rides->sr_female->Rarray;
        size = rides->sr_female->size;
    }

    while(i < size)
    {
        struct ride* ride = Rarray[i];
        char* driver_id = strdup(ride->driver_id);
        char* user_username = strdup(ride->user_username);
        char* user_acc_creation = U_UNget_acc_creation(user_username, users);
        char* driver_acc_creation = D_IDget_acc_creation(driver_id, drivers);

        int accU_age = calcula_idade(create_Date(user_acc_creation));
        int accD_age = calcula_idade(create_Date(driver_acc_creation));

        if((accU_age - acc_age) >= 0 && (accD_age - acc_age) >= 0)
        {
            if(interativo == 1)
            {
                insert_ride_sr(ride, Sarray);
            }
            char* driver_name = D_IDget_name(driver_id, drivers);
            char* user_name = U_UNget_name(user_username, users);

            if (x != 0)
            {
                fprintf(arq, "%s;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);
            }

            free(driver_name);
            free(user_name);
        }
        i++;
        free(driver_id);
        free(user_username);
        free(user_acc_creation);
        free(driver_acc_creation);
    }
    if (interativo == 1)
    {
        int aux = max - page_size;
        size = Sarray->size;

        while(aux < size && linha != page_size)
        {
            struct ride* ride = Sarray->Rarray[aux];
            char* driver_id = strdup(ride->driver_id);
            char* user_username = strdup(ride->user_username);
            char* driver_name = D_IDget_name(driver_id, drivers);
            char* user_name = U_UNget_name(user_username, users);
            printf("%s;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);
            linha ++;
            aux++;
            free(driver_id);
            free(user_username);
            free(driver_name);
            free(user_name);
        }
    }
    fclose(arq);
    free(Sarray->Rarray);
    free(Sarray); 
    if (x == 0)
    {
        remove(filename);
    }
    return linha;
}

//Imprime o resultado da query 8 e retorna um valor "linha" que serve de auxilio para o modo interativo do projeto.
int print_query9(char* date_1, char* date_2, struct rides* rides, char* filename, int x, int max, int page_size, int interativo)
{    
    FILE* arq = fopen(filename, "wt");
    struct ride** sr_tip = rides->sr_tip->Rarray;
    struct srarray* Sarray = init_srarray();
    int size = rides->sr_tip->size;
    int i=0, linha = 0;


    while(i < size)
    {   
        char* date = strdup(sr_tip[i]->date);
        if(belongs(date_1,date_2,date))
        {
            struct ride* ride = sr_tip[i];
            if(interativo == 1)
            {
                insert_ride_sr(ride, Sarray);
            }
            if (x != 0)
            {
                fprintf(arq, "%s;%s;%d;%s;%.3f\n", sr_tip[i]->id, sr_tip[i]->date, sr_tip[i]->distance, sr_tip[i]->city, sr_tip[i]->tip);
            }
        }
        i++;
        free(date);
    }

    if(interativo==1)
    {
        int aux = max -page_size;
        size = Sarray->size;
        while(aux<size && linha != page_size)
        {
            struct ride* ride = Sarray->Rarray[aux];
            printf("%s;%s;%d;%s;%.3f\n", ride->id, ride->date, ride->distance, ride->city, ride->tip);
            linha++;
            aux++;
        }
    }

    fclose(arq);
    free(Sarray->Rarray);
    free(Sarray);
    if (x == 0)
    {
        remove(filename);
    }
    return linha;
}

/////MEMORY FREES/////

//Liberta a memória alocada pela hash_table rh.
void free_rhash(gpointer rd)
{
    struct ride* ride = (struct ride*)rd;

    free(ride->id);
    free(ride->date);
    free(ride->driver_id);
    free(ride->user_username);
    free(ride->city);
    free(ride);
}

//Liberta a memória alocada pelas keys da hash table ch.
void free_chash_k(gpointer ct)
{
    char* city = (char*)ct;
    free(city);
}

//Liberta a memória alocada pelos values da hash_table ch.
void free_chash_v(gpointer mp)
{
    struct mpreco* mpreco = (struct mpreco*)mp;
    free(mpreco);
}

//Liberta toda a memória alocada para dados de viagens.
void free_mem_r(struct rides* rides)
{
    g_hash_table_destroy(rides->rh->hash);
    g_hash_table_destroy(rides->ch->hash);

    free(rides->rh);
    free(rides->ch);
    free(rides->sr_male->Rarray);
    free(rides->sr_male);
    free(rides->sr_female->Rarray);
    free(rides->sr_female);
    free(rides->sr_tip->Rarray);
    free(rides->sr_tip);
    free(rides);
}


/////GETTERS/////

//Getter da data a partir de uma viagem.
char* Rget_date(struct ride* ride)
{
    char* ret = strdup(ride->date);
    return ret;
}

//Getter do driver_id a partir de uma viagem.
char* Rget_driver_id(struct ride* ride)
{
    char* ret = strdup(ride->driver_id);
    return ret;
}

//Getter do user_username a partir de uma viagem.
char* Rget_user_username(struct ride* ride)
{
    char* ret = strdup(ride->user_username);
    return ret;
}

//Getter da cidade a partir de uma viagem.
char* Rget_city(struct ride* ride)
{
    char* ret = strdup(ride->city);
    return ret;
}

//Getter da distância a partir de uma viagem.
int Rget_distance(struct ride* ride)
{
    return ride->distance;
}

//Getter do score_user a partir de uma viagem.
double Rget_score_user(struct ride* ride)
{
    return ride->score_user;
}

//Getter do score_driver a partir de uma viagem.
double Rget_score_driver(struct ride* ride)
{
    return ride->score_driver;
}

//Getter da tip a partir de uma viagem.
double Rget_tip(struct ride* ride)
{
    return ride->tip;
}

//Getter do id a partir de uma viagem.
char* Rget_id(struct ride* ride)
{
    char* ret = strdup(ride->id);
    return ret;
}