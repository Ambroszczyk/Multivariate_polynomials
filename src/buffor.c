/** @file
   Buffor tablicowy
   */

#include "buffor.h"
#include <stdlib.h>
#include <assert.h>
#include "utils.h"

buffor_t BufforInit() {
    return (buffor_t) {.mono = NULL, .size = 0};
}

/**
 * sprawdza czy liczba jest potega 2
 * @param[in] number : liczba
 * @return : true albo false
 */
bool IsTwoPower(unsigned number) {
    if (number == 1)
        return true;
    else {
        if (number % 2 == 1)
            return false;
        else
            return IsTwoPower(number / 2);
    }
}


void BufforAdd(Mono *new_mono, buffor_t *buffor) {
     Mono* mono_help;
    if (buffor->size == 0) {
        buffor->mono = MonoInitialize(1);
        buffor->mono[0] = *new_mono;
    }
    else {
        if (IsTwoPower(buffor->size)) {
            mono_help = realloc(buffor->mono, 2 * buffor->size * sizeof(Mono));
            assert(mono_help != NULL);
            buffor->mono = mono_help;
            buffor->mono[buffor->size] = *new_mono;
        } else {
            buffor->mono[buffor->size] = *new_mono;
        }
    }
        buffor->size++;
}

void BufforClear(buffor_t *buffor) {
    free(buffor->mono);
    //dla bezpieczenstwa
    buffor->mono = NULL;
    buffor->size = 0;
}

void BufforClearHard(buffor_t *buffor) {
    for (unsigned i = 0; i < buffor->size; i++)
        MonoDestroy(&(buffor->mono[i]));
    BufforClear(buffor);
}

