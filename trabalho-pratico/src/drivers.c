#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "glib.h"
#include "drivers.h"
#include "m_structs_aux.h"
#include "dates.h"
#include "verifica_input.h"

#define MAXD 10000
#define MAXCS 10

//Estrutura de armazenamento de avaliação média de condutores por cidade.
struct city_score
{
    char* city;
    double city_med_score;
    int city_n_rides;
};

//Estrutura de armazenamento de avaliações médias de condutores.
struct med_score
{
    double main_med_score;
    struct city_score** city_score;
    int size;
    int max;
};

//Estrutura do catálogo de condutores.
struct driver
{
    char* id;
    char* name;
    int age;
    char* gender;
    char* car_class;
    char* city;
    char* acc_creation;
    char* acc_status;
    char* date_ride;
    struct med_score* med_score;
    int n_rides;
    double tot_profit;
    int is_valid;
};

//Estrutura array de condutores.
struct sdarray 
{
    int size;
    int max;
    struct driver** Darray;
};

//Estrutura de hash_table de condutores.
struct dh
{
    GHashTable* hash;
};

//Estrutura principal do módulo de condutores, armazena todas as outras.
struct drivers
{
    struct dh* dh;
    struct sdarray* sdarray;
    struct sdarray* ctarray;
    char* ordered_by;
};

//Inicializa uma struct med_score.
struct med_score* init_med_score()
{
    struct med_score* med_score = malloc(sizeof(struct med_score));
    med_score->main_med_score = 0.000;
    med_score->city_score = malloc(sizeof(struct city_score*) *MAXCS);
    med_score->size = 0;
    med_score->max = MAXCS;

    return med_score;
}

//Inicializa uma struct driver.
struct driver* init_driver(char* line_m)
{
    struct driver* driver = malloc(sizeof(struct driver));
    char* line = strdup(line_m);
    char* linef = line;
    char* ptr;
    int index=0;
    driver->is_valid=1;

    while((ptr=strsep(&line, ";\n")) != NULL && driver->is_valid==1)
    {
        switch (index)
        {
            case 0: 
            {
                char* id_ = strdup(ptr);
                if(verificaTamanho(id_))
                {
                    driver->id = id_; 
                }
                else
                {
                    free(id_);
                    driver->is_valid=0;
                }                    
                break;
            }
            case 1: 
            {
                char* name_ = strdup(ptr);
                if(verificaTamanho(name_))
                {
                    driver->name = name_;
                }
                else
                {
                    free(driver->id);
                    free(name_);
                    driver->is_valid=0;
                }
                break;
            }
            case 2: 
            {
                char* date = strdup(ptr);
                if(!verificaData(date))
                {
                    driver->age = calcula_idade(create_Date(date));
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    driver->is_valid=0;
                }
                free(date);
                break;
            }
            case 3:
            { 
                char* gender_ = strdup(ptr);
                if(verificaTamanho(gender_))
                {
                    driver->gender = gender_; 
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    free(gender_);
                    driver->is_valid=0;
                }
                break;
            }
            case 4:
            {
                char* car_class_ = strdup(ptr);
                if(verificaCarClass(car_class_))
                {
                    driver->car_class = car_class_;
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    free(driver->gender);
                    free(car_class_);
                    driver->is_valid=0;
                }
                break;
            }
            case 5:
            {
                char* license_plate_ = strdup(ptr);
                if(!verificaTamanho(license_plate_))
                {
                    free(driver->id);
                    free(driver->name);
                    free(driver->gender);
                    free(driver->car_class);
                    driver->is_valid=0;
                }
                free(license_plate_);
                break;
            }
            case 6: 
            { 
                char* city_ = strdup(ptr);
                if(verificaTamanho(city_))
                {
                    driver->city = city_; 
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    free(driver->gender);
                    free(driver->car_class);
                    free(city_);
                    driver->is_valid=0;
                }
                break;
            }
            case 7: 
            { 
                char* date = strdup(ptr);
                if(!verificaData(date))
                {
                    driver->acc_creation = date;
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    free(driver->gender);
                    free(driver->car_class);
                    free(driver->city);
                    free(date);
                    driver->is_valid=0;
                }
                break;
            }
            case 8: 
            { 
                char* acc_status_ = strdup(ptr); 
                if(verificaAccStatus(acc_status_))
                {
                    driver->acc_status = acc_status_; 
                }
                else
                {
                    free(driver->id);
                    free(driver->name);
                    free(driver->gender);
                    free(driver->car_class);
                    free(driver->city);
                    free(driver->acc_creation);
                    free(acc_status_);
                    driver->is_valid=0;
                }
                break;
            }         
        }
        index++;
    }

    if (driver->is_valid == 1)
    {
        char* date = "00/00/0000";
        driver->date_ride = strdup (date);
        driver->med_score = init_med_score();
        driver->n_rides = 0;
        driver->tot_profit = 0.000;
    }

    free(linef);

    return driver;
}

//Inicializa uma struct dh.
struct dh* init_dh()
{
    struct dh* dh = malloc(sizeof(struct dh));
    GHashTable* hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free_dhash);
    dh->hash = hash;

    return dh;
}

//Inicializa uma struct sdarray.
struct sdarray* init_sdarray()
{
    struct sdarray* sdarray = malloc(sizeof(struct sdarray));

    sdarray->size = 0;
    sdarray->max = MAXD;
    sdarray->Darray= malloc(sizeof(struct driver*)*sdarray->max);

    return sdarray;
}

//Inicializa uma struct drivers.
struct drivers* init_drivers(struct dh* dh, struct sdarray* sdarray, struct sdarray* ctarray)
{
    struct drivers* drivers = malloc(sizeof(struct drivers));
    drivers->dh = dh;
    drivers->sdarray = sdarray;
    drivers->ctarray = ctarray;
    drivers->ordered_by = strdup("Nothing");

    return drivers;
}

//Redimensiona um array de city_score.
int resize_city_score_arr(struct med_score* med_score)
{
    if (!med_score->city_score) 
    {
        return 0;
    } 
    med_score->max *= 2; 
    med_score->city_score = realloc(med_score->city_score, med_score->max*sizeof(struct city_score*));
    if (!med_score->city_score) 
    {
        return 0;
    } 
    return 1; 
}

//Redimensiona um sdarray.
int resize_sdarray(struct sdarray* sdarray)
{ 
    if (!sdarray->Darray) 
    {
        return 0;
    } 
    sdarray->max *= 2; 
    sdarray->Darray = realloc(sdarray->Darray, sdarray->max*sizeof(struct driver*));
    if (!sdarray->Darray)
    { 
        return 0;
    } 
    return 1; 
}

//Insere um driver num sdarray.
int insert_driver_sd(struct driver* driver, struct sdarray* sdarray)
{
    if (!sdarray)
    {
        return 0;
    }
    if (sdarray->size == sdarray->max) 
    {
        if (resize_sdarray(sdarray)) 
        {
            sdarray->Darray[sdarray->size] = driver; 
            sdarray->size++;
            return 1; 
        }
        return 0; 
    }
    sdarray->Darray[sdarray->size++] = driver;
    return 1;
}

//Insere um driver nas estruturas necessárias.
void insert_driver(struct dh* dh, struct sdarray* sdarray, struct sdarray* ctarray, char* line)
{
    struct driver* driver = init_driver(line);
    GHashTable* hash = dh->hash;

    if (driver->is_valid)
    {
        g_hash_table_insert(hash, driver->id , driver);
        char* status = strdup(driver->acc_status);
        if (verificaActive(status))
        {
            insert_driver_sd(driver, sdarray);
            insert_driver_sd(driver, ctarray);
        }

        free(status);
    }
    else
    {
        free(driver);
    }
}

//Lê o ficheiro .csv dos condutores e trata os dados lidos.
struct drivers* load_drivers(char* arg)
{
    struct dh* dh = init_dh();
    struct sdarray* sdarray = init_sdarray();
    struct sdarray* ctarray = init_sdarray();

    char* argD  = g_build_path("/", arg, "drivers.csv", NULL);

    FILE* fptrD = fopen(argD, "r");

    char* line=NULL;
    ssize_t read;
    size_t len;

    getline(&line, &len, fptrD);
    while ((read = getline(&line, &len, fptrD)) != -1)
    {   
        insert_driver (dh, sdarray, ctarray, line);
    }
    fclose(fptrD);
    free(argD);
    free(line);

    struct drivers* drivers = init_drivers(dh, sdarray, ctarray);

    return drivers;
}

//Atualiza o score médio por cidade de um condutor.
int update_city_score(struct driver* driver, double score_driver, char* city)
{
    struct city_score** city_score_arr = driver->med_score->city_score;
    int size = driver->med_score->size;
    int max = driver->med_score->max;
    int i = 0;

    while ((i < size) && ((strcmp(city,city_score_arr[i]->city)) != 0))
    {
        i++;
    }

    if (i == size)
    {
        if (size == max)
        {
            resize_city_score_arr(driver->med_score);
        }
        struct city_score* city_score = malloc (sizeof (struct city_score));
        city_score->city = strdup(city);
        city_score->city_med_score = score_driver;
        city_score->city_n_rides = 1;
        city_score_arr[i] = city_score;
        driver->med_score->size++;

        return 0;
    }

    struct city_score* city_score = city_score_arr[i]; 
    city_score->city_n_rides += 1;
    city_score->city_med_score = (city_score->city_med_score*(city_score->city_n_rides-1) + score_driver)/city_score->city_n_rides;

    return 0;
}

//Atualiza as informações de um condutor face à leitura de uma nova viagem válida.
double update_d(char* date_driver, char* driver_id, int distance, double score_driver, char* city, double tip, struct drivers* drivers)
{
    struct driver* driver;
    driver = g_hash_table_lookup(drivers->dh->hash, driver_id);

    char* car_class = driver->car_class;
    double profit = ride_cost(car_class, distance);

    update_city_score(driver, score_driver, city);

    driver->n_rides += 1;
    driver->med_score->main_med_score = (driver->med_score->main_med_score*(driver->n_rides-1) + score_driver)/driver->n_rides;
    driver->tot_profit += profit + tip;

    if(compara_datas(driver->date_ride, date_driver) < 0)
    {
        free(driver->date_ride);
        driver->date_ride = date_driver;
    } 
    else
    {
        free(date_driver);
    }
    return profit;
}

//Verifica se o array ctarray se encontra ordenado pela cidade fornecida como argumento.
void is_ordered(char* city, struct drivers* drivers)
{
    if ((strcmp(city,drivers->ordered_by)) != 0)
    {
        int left = 0;
        int right = drivers->ctarray->size - 1;

        qSort_darray_ct(drivers->ctarray->Darray, left, right, city);
        free(drivers->ordered_by);
        drivers->ordered_by = strdup(city);
    }
}

/////SORTING/////

//Função de swap de condutores dos algoritmos de ordenação.
void swap_driver(struct driver** darray, int i, int j)
{
    struct driver* aux = darray[i];
    darray[i] = darray[j];
    darray[j] = aux;
}

//Função auxiliar do algoritmo de ordenação.
int partition_darray_ct(struct driver** darray, int left, int right, char* city)
{
    struct driver* pivot = darray[right];
    struct med_score* med_score_p = pivot->med_score;
    int ip = 0;
    double med_p;
    while ((ip < med_score_p->size) && ((strcmp(city, med_score_p->city_score[ip]->city)) != 0))
    {
        ip++;
    }

    if (ip == med_score_p->size)
    {   
        med_p = 0;
    }

    else
    {
        struct city_score* city_score = med_score_p->city_score[ip];
        med_p = round(city_score->city_med_score * 10000000);
    }

    for(int j = left; j < right; j++)
    {
        double med_k;
        int ik = 0;
        struct driver* k = darray[j];
        struct med_score* med_score_k = k->med_score;

        while ((ik < med_score_k->size) && ((strcmp(city, med_score_k->city_score[ik]->city)) != 0))
        {
            ik++;
        }

        if (ik == med_score_k->size)
        {   
            med_k = 0;
        }

        else
        {
            struct city_score* city_score_k = med_score_k->city_score[ik];
            med_k = round(city_score_k->city_med_score * 10000000);
        }


        if(med_k < med_p)
        {
            swap_driver(darray, left, j);
            left++;
        }

        else
        if(med_k == med_p)
        {
            if(strcmp(k->id, pivot->id) < 0)
            {
                swap_driver(darray, left, j);
                left++;                                                                                                   
            }                                                                                                           
        }  
    }
    swap_driver (darray, left, right);
    return left;
}

//Função auxiliar do algoritmo de ordenação.
int partition_darray(struct driver** darray, int left, int right)
{
    struct driver* pivot = darray[right];
    double med_p = round(pivot->med_score->main_med_score*10000000);

    for(int j = left; j < right; j++)
    {
        struct driver* k = darray[j];
        double med_k = round(k->med_score->main_med_score * 10000000);

        if(med_k < med_p )
        {
            swap_driver(darray, left, j);
            left++;
        }

        else
            if(med_k == med_p)
            {
                char* k_date = strdup(k->date_ride);
                char* p_date = strdup(pivot->date_ride);
                int date_cmp = compara_datas(k_date,p_date);

                if(date_cmp < 0)
                {
                    swap_driver(darray, left, j);
                    left++;
                }
                if(date_cmp == 0)
                {
                    if(strcmp(k->id, pivot->id) > 0)
                    {
                        swap_driver(darray, left, j);
                        left++;
                    }  
                } 
                free(k_date);
                free(p_date);                                                                                                       
            }                                                                                                           
    }            
    swap_driver(darray, left, right);
    return left;
}

//Algoritmo QSort de ordenação de array ctarray por avaliação média.
void qSort_darray_ct(struct driver** darray, int left, int right, char* city)
{
    if(left < right)
    {
        int index_pivot = partition_darray_ct(darray, left, right,city);
        qSort_darray_ct(darray, left, index_pivot-1,city);
        qSort_darray_ct(darray, index_pivot+1, right,city);
    }
}

//Algoritmo QSort de ordenação de array sdarray por avaliação média.
void qSort_darray(struct driver** darray, int left, int right)
{
    if(left < right)
    {
        int index_pivot = partition_darray(darray, left, right);
        qSort_darray(darray, left, index_pivot-1);
        qSort_darray(darray, index_pivot+1, right);
    }
}

//Invoca a função de sort do sdarray.
void qSort_drivers(struct drivers* drivers)
{
    struct driver** darray = drivers->sdarray->Darray;
    int left = 0;
    int right = drivers->sdarray->size - 1;

    qSort_darray(darray,left,right);
}

/////PRINTERS/////

//Imprime o resultado da query 1 para condutores.
char* print_query1_d(char* driver_id, struct drivers* drivers)
{
    GHashTable* hash = drivers->dh->hash;
    struct driver* driver = g_hash_table_lookup(hash, driver_id);

    char* name = Dget_name(driver);
    char* gender = Dget_gender(driver);
    
    char buff [100];
    sprintf (buff, "%s;%s;%d;%.3f;%d;%.3f" ,name, gender, driver->age, driver->med_score->main_med_score, driver->n_rides, driver->tot_profit);
    char* print_2_file = strdup(buff);

    free(name);
    free(gender);
    return print_2_file;
}

//Imprime o resultado da query 2.
int print_query2(int driver_number, struct drivers* drivers, char* filename, int x, int max, int page_size, int interativo)
{
    FILE* arq = fopen(filename, "wt");
    struct driver** Darray = drivers->sdarray->Darray;
    int size = drivers->sdarray->size;
    int i = 1;

    if( interativo == 1)
    {
        i = max - page_size + 1;// para o i ser = 1 no início
    }
    else
    {
        i = 1;
        max = size;
    }

    while(i <= driver_number && i <= size && i <= max)
    {
        struct driver* driver = Darray[size-i];
        char* id = Dget_id(driver);
        char* name = Dget_name(driver);
        if (x != 0)
        {
            fprintf(arq, "%s;%s;%.3f\n", id, name, driver->med_score->main_med_score);
        }
        else
        {
            printf("%s;%s;%.3f\n", id, name, driver->med_score->main_med_score);
        }
        i++;
        free(id);
        free(name);
    }

    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
    return i;
}

//Imprime o resultado da query 7.
int print_query7(int driver_number, char* city, struct drivers* drivers, char* filename, int print, int x, int max, int page_size, int interativo)
{
    FILE* arq = fopen(filename, "wt");
    struct driver** Darray = drivers->ctarray->Darray;
    int size = drivers->ctarray->size - 1;
    int i;

    if( interativo == 1)
        {
            i = max - page_size; // i = 0 no início
        }

    else
        {
            i = 0;
            max = size;
        }

    if (print == 1)
        {
            while (i < driver_number && i < size && i < max)
            {
                struct driver* driver = Darray[size-i];
                struct med_score* med_score = driver->med_score;
                char* id = Dget_id(driver);
                char* name = Dget_name(driver);
                int med_size = med_score->size;
                int j = 0;
                double med;

                while ((j < med_size) && ((strcmp(city, med_score->city_score[j]->city)) != 0))
                {
                    j++;
                }

                if (j == med_size)
                {   
                    med = 0;
                }
                else
                {
                    struct city_score* city_score = med_score->city_score[j];
                    med = city_score->city_med_score;
                }

                if(x != 0)
                {
                    fprintf(arq, "%s;%s;%.3f\n", id, name, med);
                }
                else
                {
                    printf("%s;%s;%.3f\n", id, name, med);
                }

                i++;
                free(id);
                free(name);
            }
        }

    fclose(arq);
    if (x == 0)
    {
        remove(filename);
    }
    return i;
}
/////MEMORY FREES/////

//Liberta a memória alocada pelas struct med_score.
void free_med_score(struct driver* driver)
{
    int size = driver->med_score->size;

    for (int i = 0; i < size; i++)
    {
        free(driver->med_score->city_score[i]->city);
        free(driver->med_score->city_score[i]);
    }

    free(driver->med_score->city_score);
    free(driver->med_score);
}

//Liberta a memória alocada pela hash_table.
void free_dhash(gpointer drv)
{
    struct driver* driver = (struct driver*)drv;

    free(driver->id);
    free(driver->name);
    free(driver->gender);
    free(driver->car_class);
    free(driver->city);
    free(driver->acc_creation);
    free(driver->acc_status);
    free(driver->date_ride);
    free_med_score(driver);
    free(driver);
}

//Liberta toda a memória alocada para dados de condutores.
void free_mem_d(struct drivers* drivers)
{
    g_hash_table_destroy(drivers->dh->hash);
    free(drivers->sdarray->Darray);
    free(drivers->sdarray);
    free(drivers->ctarray->Darray);
    free(drivers->ctarray);
    free(drivers->dh);
    free(drivers->ordered_by);
    free(drivers);
}


/////GETTERS/////

//Getter da car_class a partir do id de um condutor.
char* D_IDget_car_class(char* driver_id, struct drivers* drivers)
{
    char* ret;
    GHashTable* h = drivers->dh->hash;
    struct driver* driver = g_hash_table_lookup(h, driver_id);

    ret = strdup(driver->car_class);
    return ret;
}

//Getter do acc_status a partir do id de um condutor.
int D_IDcheck_acc_status(char* driver_id, struct drivers* drivers)
{
    GHashTable* h = drivers->dh->hash;
    struct driver* driver;
    if((driver = g_hash_table_lookup(h, driver_id)) == NULL)
    {
        return 0;
    }

    char* acc_status = strdup(driver->acc_status);
    int driver_status = verificaActive(acc_status);

    free(acc_status);
    return driver_status;
}

//Getter do género a partir do id de um condutor.
int D_IDcheck_gender(char* driver_id, struct drivers* drivers)
{
    GHashTable* h = drivers->dh->hash;
    struct driver* driver = g_hash_table_lookup(h, driver_id);

    if (strcmp (driver->gender, "M") == 0)
    {
        return 1;
    }
    return 0; 
}

//Getter do nome a partir do id de um condutor.
char* D_IDget_name(char* driver_id, struct drivers* drivers)
{
    char* ret;
    GHashTable* h = drivers->dh->hash;
    struct driver* driver = g_hash_table_lookup(h, driver_id);

    ret = strdup(driver->name);
    return ret;
}

//Getter da acc_creation a partir do id de um condutor.
char* D_IDget_acc_creation(char* driver_id, struct drivers* drivers)
{
    char* ret;
    GHashTable* h = drivers->dh->hash;
    struct driver* driver = g_hash_table_lookup(h, driver_id);

    ret = strdup(driver->acc_creation);
    return ret;
}

//Getter do nome a partir de um condutor.
char* Dget_name(struct driver* driver)
{
    char* ret = strdup(driver->name);
    return ret; 
}

//Getter do género a partir de um condutor.
char* Dget_gender(struct driver* driver)
{
    char* ret = strdup(driver->gender);
    return ret; 
}

//Getter do id a partir de um condutor.
char* Dget_id(struct driver* driver)
{
    char* ret = strdup(driver->id);
    return ret; 
}