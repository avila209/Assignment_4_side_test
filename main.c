#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t food_ready_customer1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t food_ready_customer2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t food_ready_customer3 = PTHREAD_COND_INITIALIZER;

pthread_cond_t customer1_done_eating = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer2_done_eating = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer3_done_eating = PTHREAD_COND_INITIALIZER;

pthread_mutex_t Customer1_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Customer2_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Customer3_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Chef_Mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t Chef, Customer1, Customer2, Customer3;

int food[3];
int meal_count_c1 = 0;
int meal_count_c2 = 0;
int meal_count_c3 = 0;
int total_meal_count = 0;

bool received_soda_1 = false;
bool received_soda_2 = false;
bool received_fries_1 = false;
bool received_fries_3 = false;
bool received_hamburger_2 = false;
bool received_hamburger_3 = false;

void CookItUp() {
    food[0] = rand() % 3;
    bool unique = false;
    int temp;
    while (!unique) {
        temp = rand() % 3;
        if (temp == food[0]) continue;
        else {
            food[1] = temp;
            total_meal_count = total_meal_count + 2;
            unique = true;
        }
    }

    printf("Chef made "); //0 == hamburger, 1 == fries, 2 == soda
    if (food[0] == 0) printf("hamburger and ");
    else if (food[0] == 1) printf("fries and ");
    else if (food[0] == 2) printf("soda and ");

    if (food[1] == 0) printf("hamburger\n");
    else if (food[1] == 1) printf("fries\n");
    else if (food[1] == 2) printf("soda\n");
}

void *Kitchen(void *argument){
    srand(time(NULL));

    pthread_mutex_lock(&Chef_Mutex);
    while(total_meal_count < 200){
            usleep(5000);
            CookItUp();
            if(food[0] == 0){ // Hamburger = Customer 2 + 3
                if(meal_count_c2 == meal_count_c3){                     //Might need to add checkers to see if customer already has item.
                    if(received_hamburger_2 == true && received_hamburger_3 == false){
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_hamburger_3 == true && received_hamburger_2 == false){
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_hamburger_2 == false && received_hamburger_3 == false){
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        //Both have hamburgers
                        if(received_soda_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else if(received_fries_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else{
                            printf("Throwing away hamburger\n");
                        }
                    }
                }
                else if(meal_count_c2 > meal_count_c3){
                    if(received_hamburger_2 == true && received_hamburger_3 == false){
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_hamburger_3 == true && received_hamburger_2 == false){
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_hamburger_2 == false && received_hamburger_3 == false){
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        //Both have hamburgers
                        if(received_fries_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else if(received_soda_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else{
                            printf("Throwing away hamburger\n");
                        }
                    }
                }
                else{
                    if(received_hamburger_2 == true && received_hamburger_3 == false){
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_hamburger_3 == true && received_hamburger_2 == false){
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_hamburger_2 == false && received_hamburger_3 == false){
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        //Both have hamburgers
                        if(received_soda_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else if(received_fries_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else{
                            printf("Throwing away hamburger\n");
                        }
                    }
                }
            }

            else if(food[0] == 1){ // Fries = Customer 1 + 3
                if(meal_count_c1 == meal_count_c3){
                    if(received_fries_1 == true && received_fries_3 == false){
                        received_fries_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_fries_3 == true && received_fries_1 == false){
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_fries_1 == false && received_fries_3 == false){
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else{
                        //Both have fries
                        if(received_soda_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else if(received_hamburger_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else{
                            printf("Throwing away fries\n");
                        }
                    }
                }
                else if(meal_count_c1 > meal_count_c3){
                    if(received_fries_1 == true && received_fries_3 == false){
                        received_fries_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_fries_3 == true && received_fries_1 == false){
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_fries_1 == false && received_fries_3 == false){
                        received_fries_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        //Both have fries
                        if(received_hamburger_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else if(received_soda_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else{
                            printf("Throwing away fries\n");
                        }
                    }
                }
                else{
                    if(received_fries_1 == true && received_fries_3 == false){
                        received_fries_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_fries_3 == true && received_fries_1 == false){
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_fries_1 == false && received_fries_3 == false){
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else{
                        //Both have fries
                        if(received_soda_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else if(received_hamburger_3){
                            pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                            received_hamburger_3 = true;
                            pthread_cond_signal(&food_ready_customer3);
                        }
                        else{
                            printf("Throwing away fries\n");
                        }
                    }
                }
            }

            else if(food[0] == 2){ // Soda = Customer 1 + 2
                if(meal_count_c1 == meal_count_c2){
                    if(received_soda_1 == true && received_soda_2 == false){
                        received_soda_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_soda_2 == true && received_soda_1 == false){
                        received_soda_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_soda_1 == false && received_soda_2 == false){
                        received_soda_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else{
                        //Both have soda
                        if(received_fries_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else if(received_hamburger_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else{
                            printf("Throwing away soda\n");
                        }
                    }
                }
                else if(meal_count_c1 > meal_count_c2){
                    if(received_soda_1 == true && received_soda_2 == false){
                        received_soda_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_soda_2 == true && received_soda_1 == false){
                        received_soda_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_soda_1 == false && received_soda_2 == false){
                        received_soda_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        //Both have soda
                        if(received_hamburger_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else if(received_fries_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else {
                            printf("Throwing away soda\n");
                        }
                    }
                }
                else{
                    if(received_soda_1 == true && received_soda_2 == false){
                        received_soda_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_soda_2 == true && received_soda_1 == false){
                        received_soda_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_soda_1 == false && received_soda_2 == false){
                        received_soda_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else{
                        //Both have soda
                        if(received_fries_1){
                            pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                            received_fries_1 = true;
                            pthread_cond_signal(&food_ready_customer1);
                        }
                        else if(received_hamburger_2){
                            pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                            received_hamburger_2 = true;
                            pthread_cond_signal(&food_ready_customer2);
                        }
                        else{
                            printf("Throwing away soda\n");
                        }
                    }
                }
            }

        if(food[1] == 0){ // Hamburger = Customer 2 + 3
            if(meal_count_c2 == meal_count_c3){                     //Might need to add checkers to see if customer already has item.
                if(received_hamburger_2 == true && received_hamburger_3 == false){
                    received_hamburger_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_hamburger_3 == true && received_hamburger_2 == false){
                    received_hamburger_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_hamburger_2 == false && received_hamburger_3 == false){
                    received_hamburger_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else{
                    //Both have hamburgers
                    if(received_soda_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_fries_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        printf("Throwing away hamburger\n");
                    }
                }
            }
            else if(meal_count_c2 > meal_count_c3){
                if(received_hamburger_2 == true && received_hamburger_3 == false){
                    received_hamburger_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_hamburger_3 == true && received_hamburger_2 == false){
                    received_hamburger_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_hamburger_2 == false && received_hamburger_3 == false){
                    received_hamburger_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else{
                    //Both have hamburgers
                    if(received_fries_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_soda_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        printf("Throwing away hamburger\n");
                    }
                }
            }
            else{
                if(received_hamburger_2 == true && received_hamburger_3 == false){
                    received_hamburger_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_hamburger_3 == true && received_hamburger_2 == false){
                    received_hamburger_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_hamburger_2 == false && received_hamburger_3 == false){
                    received_hamburger_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else{
                    //Both have hamburgers
                    if(received_soda_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_fries_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        printf("Throwing away hamburger\n");
                    }
                }
            }
        }

        else if(food[1] == 1){ // Fries = Customer 1 + 3
            if(meal_count_c1 == meal_count_c3){
                if(received_fries_1 == true && received_fries_3 == false){
                    received_fries_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_fries_3 == true && received_fries_1 == false){
                    received_fries_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_fries_1 == false && received_fries_3 == false){
                    received_fries_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else{
                    //Both have fries
                    if(received_soda_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_hamburger_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        printf("Throwing away fries\n");
                    }
                }
            }
            else if(meal_count_c1 > meal_count_c3){
                if(received_fries_1 == true && received_fries_3 == false){
                    received_fries_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_fries_3 == true && received_fries_1 == false){
                    received_fries_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_fries_1 == false && received_fries_3 == false){
                    received_fries_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else{
                    //Both have fries
                    if(received_hamburger_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else if(received_soda_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else{
                        printf("Throwing away fries\n");
                    }
                }
            }
            else{
                if(received_fries_1 == true && received_fries_3 == false){
                    received_fries_3 = true;
                    pthread_cond_signal(&food_ready_customer3);
                }
                else if(received_fries_3 == true && received_fries_1 == false){
                    received_fries_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_fries_1 == false && received_fries_3 == false){
                    received_fries_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else{
                    //Both have fries
                    if(received_soda_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_hamburger_3){
                        pthread_cond_wait(&customer3_done_eating, &Customer3_Mutex);
                        received_hamburger_3 = true;
                        pthread_cond_signal(&food_ready_customer3);
                    }
                    else{
                        printf("Throwing away fries\n");
                    }
                }
            }
        }

        else if(food[1] == 2){ // Soda = Customer 1 + 2
            if(meal_count_c1 == meal_count_c2){
                if(received_soda_1 == true && received_soda_2 == false){
                    received_soda_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_soda_2 == true && received_soda_1 == false){
                    received_soda_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_soda_1 == false && received_soda_2 == false){
                    received_soda_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else{
                    //Both have soda
                    if(received_fries_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_hamburger_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        printf("Throwing away soda\n");
                    }
                }
            }
            else if(meal_count_c1 > meal_count_c2){
                if(received_soda_1 == true && received_soda_2 == false){
                    received_soda_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_soda_2 == true && received_soda_1 == false){
                    received_soda_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_soda_1 == false && received_soda_2 == false){
                    received_soda_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else{
                    //Both have soda
                    if(received_hamburger_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else if(received_fries_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else {
                        printf("Throwing away soda\n");
                    }
                }
            }
            else{
                if(received_soda_1 == true && received_soda_2 == false){
                    received_soda_2 = true;
                    pthread_cond_signal(&food_ready_customer2);
                }
                else if(received_soda_2 == true && received_soda_1 == false){
                    received_soda_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else if(received_soda_1 == false && received_soda_2 == false){
                    received_soda_1 = true;
                    pthread_cond_signal(&food_ready_customer1);
                }
                else{
                    //Both have soda
                    if(received_fries_1){
                        pthread_cond_wait(&customer1_done_eating, &Customer1_Mutex);
                        received_fries_1 = true;
                        pthread_cond_signal(&food_ready_customer1);
                    }
                    else if(received_hamburger_2){
                        pthread_cond_wait(&customer2_done_eating, &Customer2_Mutex);
                        received_hamburger_2 = true;
                        pthread_cond_signal(&food_ready_customer2);
                    }
                    else{
                        printf("Throwing away soda\n");
                    }
                }
            }
        }
    }

    printf("My shift over coach\n");
    printf("Customer 1 ate %d times. \n", meal_count_c1);
    printf("Customer 2 ate %d times. \n", meal_count_c2);
    printf("Customer 3 ate %d times. \n", meal_count_c3);

    pthread_cancel(Customer1);
    pthread_cancel(Customer2);
    pthread_cancel(Customer3);

    pthread_kill(Customer1, 1);
    pthread_kill(Customer2, 1);
    pthread_kill(Customer3, 1);

    pthread_mutex_unlock(&Chef_Mutex);
    pthread_exit(NULL);
}

void *WaitingRoom1(void *argument){
    pthread_mutex_lock(&Customer1_Mutex);

    while(total_meal_count < 200) {
        printf("Customer 1 is waiting, total_meal_count = %d\n", total_meal_count);
        pthread_cond_wait(&food_ready_customer1, &Customer1_Mutex); //Wait until their food is ready

        if (received_fries_1 && received_soda_1) {
            meal_count_c1++;
            printf("Customer 1 is eating\n");
            usleep(1000);
            printf("Customer 1 just ate, total meals = %d\n", meal_count_c1);
            received_fries_1 = false;
            received_soda_1 = false;
            pthread_cond_signal(&customer1_done_eating);
        }

    }

    printf("Customer 1 finished completely\n");
    pthread_mutex_unlock(&Customer1_Mutex);
    pthread_exit(NULL);
}

void *WaitingRoom2(void *argument){
    pthread_mutex_lock(&Customer2_Mutex);

    while(total_meal_count < 200) {
        printf("Customer 2 is waiting, total_meal_count = %d\n", total_meal_count);
        pthread_cond_wait(&food_ready_customer2, &Customer2_Mutex); //Wait until their food is ready

        if (received_hamburger_2 && received_soda_2) {
            meal_count_c2++;
            printf("Customer 2 is eating\n");
            usleep(1000);
            printf("Customer 2 just ate, total meals = %d\n", meal_count_c2);
            received_hamburger_2 = false;
            received_soda_2 = false;
            pthread_cond_signal(&customer2_done_eating);
        }
    }

    printf("Customer 2 finished completely\n");
    pthread_mutex_unlock(&Customer2_Mutex);
    pthread_exit(NULL);
}

void *WaitingRoom3(void *argument){
    pthread_mutex_lock(&Customer3_Mutex);

    while(total_meal_count < 200) {
        printf("Customer 3 is waiting, total_meal_count = %d\n", total_meal_count);
        pthread_cond_wait(&food_ready_customer3, &Customer3_Mutex); //Wait until their food is ready

        if (received_hamburger_3 && received_fries_3) {
            meal_count_c3++;
            printf("Customer 3 is eating\n");
            usleep(1000);
            printf("Customer 3 just ate, total meals = %d\n", meal_count_c3);
            received_hamburger_3 = false;
            received_fries_3 = false;
            pthread_cond_signal(&customer3_done_eating);
        }
    }

    printf("Customer 3 finished completely\n");
    pthread_mutex_unlock(&Customer3_Mutex);
    pthread_exit(NULL);
}


int main() {
    srand(time(NULL));

    pthread_create(&Customer1, NULL, &WaitingRoom1, NULL);
    pthread_create(&Customer2, NULL, &WaitingRoom2, NULL);
    pthread_create(&Customer3, NULL, &WaitingRoom3, NULL);
    pthread_create(&Chef, NULL, &Kitchen, NULL);

    pthread_join(Chef, NULL);
    pthread_join(Customer1, NULL);
    pthread_join(Customer2, NULL);
    pthread_join(Customer3, NULL);

    return 0;
}