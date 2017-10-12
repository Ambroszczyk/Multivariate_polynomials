/** @file
   Interfejs błedów parsowania
   */


#include <stdio.h>
#include "error.h"
#include "utils.h"

/**
 * w zaleznosci od erroru zwraca odpowiedni komunikat bledu
 * @param[in] error : error
 * @param[in] r : aktualny wiersz
 */
void ErrorPrint(error_t error, int r) {
    if (error == STACK_UNDERFLOW)
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", r);
    else if (error == W_VARIABLE)
        fprintf(stderr, "ERROR %d WRONG VARIABLE\n", r);
    else if (error == W_COMMAND)
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", r);
    else if (error == W_VALUE)
        fprintf(stderr, "ERROR %d WRONG VALUE\n", r);
    else if (error == W_COUNT)
        fprintf(stderr, "ERROR %d WRONG COUNT\n", r);
    //jak jest NO_ERROR to nic nie wypisujemy
}