/*

    素因数分解プログラム
    大きい数もできる様にした修正版

 */


#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    char c[256];
    long int n0, n;
    long int ns = 2;
    long int j, k;
    time_t timer;

    struct timeval myTime;
    struct tm *time_st;
    double s_time, e_time;

    int N_Trials;

    /* 整数を入力 */
    printf("\nInput integer number: ");
    fflush(stdout);
    gets(c);
    n0 = atol(c);

    /* 試行回数を入力 */
    printf("\nInput number of trials: ");
    fflush(stdout);
    gets(c);
    N_Trials = atoi(c);

    printf("\n");

    /*　時間の計測開始 */
    gettimeofday(&myTime, NULL);
    time_st = localtime(&myTime.tv_sec);

    s_time = (double) time_st->tm_hour * 60 * 60
             + (double) time_st->tm_min * 60
             + (double) time_st->tm_sec
             + (double) myTime.tv_usec / 1000000;


    for (k = 1; k < N_Trials; k++) {


        for (n = n0, ns = 2; n >= ns; ns++) {

            for (j = 0; n % ns == 0; j++) {
                n /= ns;
            }

            if (j == 0) continue;


        }


    }

    printf("******Prime factor dcomposition is shown******\n\n");

    for (n = n0, ns = 2; n >= ns; ns++) {

        for (j = 0; n % ns == 0; j++) {
            n /= ns;
        }

        if (j == 0) continue;

        printf("Prime factor:%ld  Multiplier:%ld\n", ns, j);

    }

    printf("\n**********************************************\n\n");

    /*　時間の計測終了 */
    gettimeofday(&myTime, NULL);
    time_st = localtime(&myTime.tv_sec);

    e_time = (double) time_st->tm_hour * 60 * 60
             + (double) time_st->tm_min * 60
             + (double) time_st->tm_sec
             + (double) myTime.tv_usec / 1000000;


    printf("Tatal processing time: %f[sec]\n", e_time - s_time);

    printf("Time per one process: %e[sec]\n", (double) (e_time - s_time) / N_Trials);

    return 0;
}