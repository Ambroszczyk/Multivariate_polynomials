/** @file
   Interfejs wczytywania z wejscia
   */

#include <stdbool.h>
#include "poly.h"
#include "stack.h"
#include "error.h"
#include <limits.h>

#ifndef WIELOMIANY_PARSING_H
#define WIELOMIANY_PARSING_H

/**
 * Funkcja idzie do końca linni az natrafi na '\n' lub EOF
 * @param[in, out] last_char : ostatni wczytany znak
 */
void GoUnitlEnd(int *last_char);

/**
 * probuje wczytac wiersz z wejscia i wrzucic go na stos
 * (jezeli zostal wczytany poprawnie)
 * @param[in] r : numer wiersza
 * @param[in, out] stack : stos
 * @param[in, out] last_char : ostatni wczytany znak
 */

void TryParseRow(int r, StackPtr *stack, int *last_char);

/**
 * Probuje wczytac komende, jezeli sie uda to wykonuje
 * odpowiednia operacje na stosie. W przeciwnym wypadku
 * wypisuje komunikat o bledzie
 * @param[in] r : numer wiersza
 * @param[in] stack : stos
 * @return : czy udalo sie wczytac komende
 */
error_t TryParseCommand(int r, StackPtr *stack);

/**
 * probuje wczytac wielomian
 * @param[out] p : wczytany wielomian
 * @param[in, out] c : licznik kolumn
 * @param[in] end_sign : znak konczacy wczytywanie
 * @param[in, out] last_char : ostatni wczytany znak
 * @return : true lub false
 */
bool TryParsePoly(Poly *p, int *c, int end_sign, int *last_char);

/**
 * Probuje wczytac monomian, zakladamy ze *m jest zaalokowana
 * (statycznie lub dynamicznie)
 * @param[in, out] m : wczytywany monomian
 * @param[in, out] c : licznik kolum
 * @param[in, out] last_char : ostatni wczytany char
 * @return : true lub false
 */
bool TryParseMono(Mono *m, int *c, int *last_char);


/**
 * wczytuje longa
 * @param[in, out] res : wynik
 * @return : czy wczytywanie sie udalo
 */
bool TryParseLong(long *res);

/**
 * wczytuje unsigned
 * @param[in, out] res : wynik
 * @return : czy wczytywanie sie udalo
 */
bool TryParseUInt(unsigned *res);

/**
 * wczytuje poly_coeff_t
 * @param[in,out] c : licznik kolumn
 * @param[in, out] res : wynik
 * @param[in, out] last_char : ostatni wczytany char
 * @return : czy wczytywanie sie udalo
 */
bool TryParsePolyCoeff(int *c, poly_coeff_t *res, int *last_char);

/**
 * wczytuje poly_exp_t
 * @param[in,out] c : licznik kolumn
 * @param[in, out] res : wynik
 * @param[in, out] last_char : ostatni wczytany char
 * @return : czy wczytywanie sie udalo
 */
bool TryParsePolyExp(int *c, poly_exp_t *res, int *last_char);

/**
 * Porownuje, czy wczytane size znakow jest takie same jak
 * zawartosc tablicy sign. Jezeli nie to "cofa" wczytywanie
 * (korzysta z ungetc())
 * @param[in] sign : tablica charow
 * @param[in] size : rozmiar tablicy
 * @return czy wczytane znaki sie zgadzaja
 */
bool StringImputCompare(char *sign, int size);
#endif //WIELOMIANY_PARSING_H
