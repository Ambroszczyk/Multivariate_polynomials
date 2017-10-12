/** @file
   Interfejs stosu wielomianow
   */

#include "poly.h"

#ifndef WIELOMIANY_STACK_H
#define WIELOMIANY_STACK_H

/**
 * struktura trzymajaca stos wielomianow
 */
struct Stack {
    Poly poly; ///< wielomian
    struct Stack *prev; ///< wskaznik do nizszego elementu sotsu
};

/**
 * Typ stosu
 */
typedef struct Stack Stack;

/**
 * Typ wskaźnikowy na stos
 */
typedef struct Stack* StackPtr;

/**
 * inicjalizuje stos
 * @return : zainicjalizowany stos
 */
StackPtr StackInit();

/**
 * sprawdza czy stos jest pusty
 * @param[in, out] stack : stos
 * @return czy jest pusty
 */
bool StackEmpty(StackPtr stack);

/**
 * przejmuje na wlasnosc wielomian poly i kladzie go na gore stosu
 * @param[in] poly : wielomian
 * @param[in, out] stack
 */
void StackPush(Poly *poly, StackPtr *stack);

/**
 * zwraca wielomian na gorze stosu
 * @param[in] stack : stos
 * @return : wielomian
 */
Poly StackTop(StackPtr stack);

/**
 * zwraca wielomian z wierzcholka stosu i zmniejsza stos
 * nie usuwa wielomianu
 * @param[in, out] stack : stos
 * @return : wielomian
 */
Poly StackPop(StackPtr *stack);

/**
 * usuwa zadeklarowany stos razem z wielomianami
 * @param[in, out] stack : stos
 */
void StackClear(StackPtr stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
void StackZero(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackIsCoeff(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackIsZero(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackClone(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackAdd(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackMul(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackNeg(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackSub(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackIsEq(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackDeg(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 * @param[in] idx : indeks zmiennej
 */
bool StackDegBy(StackPtr *stack, unsigned idx);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 * @param[in] x : liczba ktora podstawiamy pod x_0
 */
bool StackAt(StackPtr *stack, poly_coeff_t x);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackPrint(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 */
bool StackPopDel(StackPtr *stack);

/**
 * funkcja posiadajaca odpowiednik w poly.h
 * wyspecyfinowanie funkcji znajduje sie w tresci zadania wielomiany
 * @param[in, out] stack : rozważany stos
 * @param[in] count : liczba rozwazanych wielomianow + 1
 */
bool StackCompose(StackPtr *stack, unsigned count);

#endif //WIELOMIANY_STACK_H
