/** @file
   Implementacja biblioteki obslugujacej parsowanie
   */

#include "parsing.h"
#include <stdio.h>
#include "buffor.h"
#include "utils.h"


int TakeChar(int *c, int *gettedChar);

/**
 * sprawdza czy wczytany znak jest poprawny
 * @param[in] expected_char : oczekiwany znak
 * @param[in, out] c : licznik kolumn
 * @param[in, out] last_char : ostatni wczytany znak
 * @return : true albo false
 */
bool CorrectSign(char expected_char, int *c, int *last_char) {
    int new_char = TakeChar(c, last_char);
    if (new_char == expected_char)
        return true;
    else
        return false;
}

void GoUnitlEnd(int *last_char) {
    *last_char = getchar();
    while (*last_char != '\n' && *last_char != EOF)
        *last_char = getchar();
}

/**
 * Bierze jednego chara i zwieksza licznik o 1
 * @param[in, out] c : licznik kolumn
 * @param[in, out] gettedChar : wzięty char
 * @return : wziety char
 */
int TakeChar(int *c, int *gettedChar) {
    *c += 1;
    *gettedChar = getchar();
    return *gettedChar;
}

/**
 * Cofa wziecie jednego chara i cofa licznik o 1
 * @param[in] sign : wziety char
 * @param[in, out] c : licznik kolumn
 * @return : wziety char
 */
int UnTakeChar(int sign, int *c) {
    *c -= 1;
    return ungetc(sign, stdin);
}

/**
 * funkcja sprawdzajaca czy liczba miesci sie w danym zakresie, w
 * zaleznosci od tego czy uda sie wczytac czy nie zwraca boola.
 * Jezeli funkcja zadzialala prawidlowo to ostatnim wczytanym znakiem
 * jest '\n' lub end_sign
 * @param[in] low : dolne ograniczenie
 * @param[in] high : gorne ograniczenie
 * @param[in, out] c : licznik kolumn
 * @param[in, out] res : wynik
 * @param[in] end_sign : znak konczacy wczytywanie
 * @param[in, out] last_char : ostatni wczytany znak
 * @return
 */
bool TryParseNumber(long low, long high, int *c, long *res, int end_sign, int *last_char) {
    long number = 0;
    long low_bound = -high;
    int buffor = TakeChar(c, last_char);
    bool is_negativ = false;

    if (buffor == '-') {
        is_negativ = true;
        low_bound = low;
        buffor = TakeChar(c, last_char);
    }

    if (buffor > '9' || buffor <'0') {
        return false;
    }

    while (buffor != end_sign && buffor != '\n'){
        if (buffor > '9' || buffor <'0' )
            return false;
        if (number < low_bound / 10)
            return false;
        if (number == LONG_MIN / 10 && buffor == '9')
            return false;

        number = 10 * number - (buffor - '0');

        if (number < low_bound)
            return false;

        if (number > 0)
            return false;


        buffor = TakeChar(c, last_char);
    }

    if (is_negativ)
        *res = number;
    else
        *res = -number;

    return true;
}

bool TryParseLong(long *res) {
    int dummy, dummmy;
    bool bool_res;
    bool_res = TryParseNumber(LONG_MIN, LONG_MAX, &dummy, res, '\n', &dummmy);
    if (!bool_res && dummmy != '\n' && dummmy != EOF) {
        GoUnitlEnd(&dummmy);
    }
    return bool_res;
}

bool TryParseUInt(unsigned *res) {
    long tmp_res = 0;
    bool scanned;
    int dummy, dummmy;
    scanned = TryParseNumber(0, UINT_MAX, &dummy, &tmp_res, '\n', &dummmy);
    if (!scanned && dummmy != '\n' && dummmy != EOF) {
        *res = (unsigned) tmp_res;
        GoUnitlEnd(&dummmy);
    }
    if (scanned)
        *res = (unsigned) tmp_res;
    return scanned;
}

bool TryParsePolyCoeff(int *c, poly_coeff_t *res, int *last_char) {
    long tmp_res;
    bool scanned;
    scanned = TryParseNumber(LONG_MIN, LONG_MAX, c, &tmp_res, ',', last_char);
    if (scanned)
        *res = (poly_coeff_t) tmp_res;
    return scanned;
}

bool TryParsePolyExp(int *c, poly_exp_t *res, int *last_char) {
    long tmp_res;
    bool scanned;
    scanned = TryParseNumber(0, INT_MAX, c, &tmp_res, ')', last_char);
    if (scanned)
        *res = (poly_exp_t) tmp_res;
    return scanned;
}

error_t TryParseCommand(int r, StackPtr *stack) {
    int buffor = getchar();
    error_t error = NO_ERROR;
    bool stack_under = true;
    if (('a' <= buffor && buffor <= 'z') || ('A' <= buffor && buffor <= 'Z')) {
        ungetc(buffor, stdin);
        if (StringImputCompare("ZERO\n", 5))
            StackZero(stack);
        else if (StringImputCompare("IS_COEFF\n", 9))
            stack_under = StackIsCoeff(stack);
        else if (StringImputCompare("IS_ZERO\n", 8))
            stack_under = StackIsZero(stack);
        else if (StringImputCompare("CLONE\n", 6))
            stack_under = StackClone(stack);
        else if (StringImputCompare("ADD\n", 4))
            stack_under = StackAdd(stack);
        else if (StringImputCompare("MUL\n", 4))
            stack_under =StackMul(stack);
        else if (StringImputCompare("NEG\n", 4))
            stack_under = StackNeg(stack);
        else if (StringImputCompare("SUB\n", 4))
            stack_under = StackSub(stack);
        else if (StringImputCompare("IS_EQ\n", 6))
            stack_under = StackIsEq(stack);
        else if (StringImputCompare("DEG\n", 4))
            stack_under = StackDeg(stack);
        else if (StringImputCompare("DEG_BY ", 7)) {
            unsigned idx;
            if (TryParseUInt(&idx))
                stack_under = StackDegBy(stack, idx);
            else
                error = W_VARIABLE;
            }
        else if (StringImputCompare("AT ", 3)) {
            poly_coeff_t x;
            if (TryParseLong(&x))
                stack_under = StackAt(stack, x);
            else
                error = W_VALUE;
        }
        else if (StringImputCompare("PRINT\n", 6))
            stack_under = StackPrint(stack);
        else if (StringImputCompare("POP\n", 4))
            stack_under = StackPopDel(stack);
        else if (StringImputCompare("COMPOSE ", 8)) {
            unsigned x;
            if (TryParseUInt(&x))
                stack_under = StackCompose(stack, x);
            else
                error = W_COUNT;
        }

        else
            error = W_COMMAND;

        if (!stack_under)
            error = STACK_UNDERFLOW;
        ErrorPrint(error, r);
        return error;
    }
    else {
        ungetc(buffor, stdin);
        return NO_COMMAND;
    }
}

bool TryParsePoly(Poly *p, int *c, int end_sign, int *last_char) {
    int first_char = TakeChar(c, last_char);
    int tmp_char;
    bool correct;
    poly_coeff_t wsp;
    if (first_char != '(') {
        UnTakeChar(first_char, c);
        correct = TryParsePolyCoeff(c, &wsp, last_char);
        if (correct) {
            *p = PolyFromCoeff(wsp);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        UnTakeChar(first_char, c);
        buffor_t buffor = BufforInit();
        Mono mono_help;
        while(1) {
            correct = TryParseMono(&mono_help, c, last_char);
            if (!correct) {
                BufforClearHard(&buffor);
                return false;
            }
            BufforAdd(&mono_help, &buffor);

            tmp_char = TakeChar(c, last_char);
            if (tmp_char == end_sign) {
                *p = PolyAddMonos(buffor.size, buffor.mono);
                BufforClear(&buffor);
                return true;
            }
            if (tmp_char != '+') {
                BufforClearHard(&buffor);
                return false;
            }
        }
    }
}


bool TryParseMono(Mono *m, int *c, int *last_char) {
    Poly p;
    bool correct;
    poly_exp_t exp;
    correct = CorrectSign('(', c, last_char);
    if (!correct)
        return false;
    correct = TryParsePoly(&p, c, ',', last_char);
    if (!correct || *last_char == '\n')
        return false;
    correct = TryParsePolyExp(c, &exp, last_char);
    if (!correct || *last_char == '\n') {
        PolyDestroy(&p);
        return false;
    }
    *m = MonoFromPoly(&p, exp);
    return true;

}


void TryParseRow(int r, StackPtr *stack, int *last_char) {
    int c = 0;
    Poly p;
    error_t command;

    TakeChar(&c, last_char);
    if (*last_char == EOF)
        return;
    UnTakeChar(*last_char, &c);
    command = TryParseCommand(r, stack);
    if(command == NO_COMMAND) {
         //ostatnia rownosc zapewnia dobrze przy wczytywaniu np. 1111,\n
         if (TryParsePoly(&p, &c, '\n', last_char) && *last_char == '\n') {
             StackPush(&p, stack);
         }
        else {
             if (*last_char != '\n')
                 GoUnitlEnd(last_char);
             fprintf(stderr, "ERROR %d %d\n", r, c);
         }
    }
    else {
        if (command == W_COMMAND)
            GoUnitlEnd(last_char);
    }
}

bool StringImputCompare(char *sign, int size) {
    int i = 0;
    int buffor = 1;
    while (i < size) {
        buffor = getchar();
        if (sign[i] != buffor) {
            break;
        }
        i++;
    }

    if (i != size) {
        ungetc(buffor, stdin);
        i--;
        while (i >= 0) {
            ungetc(sign[i], stdin);
            i--;
        }
        return false;
    }
    return true;
}
