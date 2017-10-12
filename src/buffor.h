/** @file
   Interfejs tablicowego buffora
   */

#include "poly.h"

#ifndef WIELOMIANY_BUFFOR_H
#define WIELOMIANY_BUFFOR_H

/**
 * struktura nagromadzajaca monomiany.
 * W zlozonosci liniowej pozwala rozszerzac zaalokowana
 * pamiec. Slozy do trzymania duzej ilosci monomianow
 */
typedef struct Buffor {
    Mono* mono; ///< monomian
    unsigned size; ///< aktualny rozmiar bufora
}buffor_t;

/**
 * inicjalizuje buffor
 * @return : buffor
 */
buffor_t BufforInit();

/**
 * Dodaje do buffora elemanty, jezeli rozmiar jest za maly to go zwieksza
 * @param[in, out] mono : tablica monomianow
 * @param[in, out] buffor : buffor
 */
void BufforAdd(Mono *mono, buffor_t *buffor);

/**
 * czysci liste w bufforze
 * @param[in, out] buffor : buffor
 */
void BufforClear(buffor_t *buffor);

/**
 * czysci liste buffora oraz bedace na niej zapisane monomiany
 * @param[in, out] buffor : buffor
 */
void BufforClearHard(buffor_t *buffor);

#endif //WIELOMIANY_BUFFOR_H
