/** @file
   Implementacja biblioteki obsługującej wielmiany
   */

#include <stdio.h>
#include <stdlib.h>
#include "poly.h"
#include "utils.h"
#include <assert.h>
#include "utils.h"

/**
 * Alokuje pamięć na tablicę monomianów
 * Dodatkowa sprawdza powodzenie alokowania
 * @param size : długość tablicy
 * @return wskaźnik na tablicę monomianów
 */
Mono* MonoInitialize(int size) {
    Mono* m = malloc(size * sizeof(Mono));
    assert(m != NULL);
    return m;
}

/**
 * Potęguje rekurencyjnie
 * @param[in] coeff : liczba potęgowana
 * @param[in] exp : wykładnik o jaki potęgujemy
 * @return liczba postaci `coeff ^ exp`
 */
poly_coeff_t Power(poly_coeff_t coeff, poly_exp_t exp) {
    poly_coeff_t half;

    if (exp == 0)
        return 1;

    if (exp % 2 == 0) {
        half = Power(coeff, exp / 2);
        return half * half;
    }

    half = Power(coeff, (exp - 1) / 2);
    return half * half * coeff;
}

/**
 * Funkcja pomocnicza do MonoMerge
 * @param[in] low : początek pierwszej połowy tablicy
 * @param[in] mid : początek drugiej połowy tablicy
 * @param[in] high : koniec drugiej połowy tablicy
 * @param[in] monos : tablica posortowana na przedzialach [low, mid], [mid + 1, high]
 * @param[in] help : tablica pomocnicza
 */
void MonoMergeHelp(int low, int mid, int high, Mono *monos, Mono *help) {
    int jumper1 = low;
    int jumper2 = mid + 1;
    int i = low;

    while (jumper1 <= mid && jumper2 <= high) {
        if (monos[jumper1].exp <= monos[jumper2].exp)
            help[i++] = monos[jumper1++];
        else
            help[i++] = monos[jumper2++];
    }

    while (jumper1 <= mid)
        help[i++] = monos[jumper1++];

    while (jumper2 <= high)
        help[i++] = monos[jumper2++];

    for (int j = low; j <= high; j++) {
        monos[j] = help[j];
    }
}

/**
 * Łączy dwa posortowane fragmenty tablicy *monos w tablicę posortowaną
 * Tworzy dodatkową tablicę, którą przekazuje funkcji MonoMergehelp
 * @param[in] low : początek pierwszej połowy tablicy
 * @param[in] mid : początek drugiej połowy tablicy
 * @param[in] high : koniec drugiej połowy tablicy
 * @param[in, out] monos : tablica posortowana na przedzialach [low, mid], [mid + 1, high]
 */
void MonoMerge(int low, int mid, int high, Mono *monos) {
    Mono* help = MonoInitialize(high + 1);
    MonoMergeHelp(low, mid, high, monos, help);
    free(help);
}

/**
 * Funkcja pomocnicza do MonoTableSort
 * @param[in] low : indeks pierwszego elementu sortowanego przedziału
 * @param[in] high : indeks ostatniego elementu sortowanego przedziału
 * @param[in, out] monos : sortowana tablica
 * @param[in] help : tablica pomocnicza
 */
void MonoTableSortHelp(int low, int high, Mono *monos, Mono *help) {
    int mid;
    if (low != high) {
        mid = (low + high) / 2;
        MonoTableSortHelp(low, mid, monos, help);
        MonoTableSortHelp(mid + 1, high, monos, help);
        MonoMergeHelp(low, mid, high, monos, help);
    }
}

/**
 * Sortuje tablicę *monos
 * Algorytm opiera się sortowaniu przez scalanie
 * @param[in] low : indeks pierwszego elementu sortowanego przedziału
 * @param[in] high : indeks ostatniego elementu sortowanego przedziału
 * @param[in, out] monos : sortowana tablica
 */
void MonoTableSort(int low, int high, Mono *monos) {
    Mono* help = MonoInitialize(high + 1);
    MonoTableSortHelp(low, high, monos, help);
    free(help);
}

/**
 * Funkcja pomocnicza do MonoTableNormalize
 * @param[in] count : długość tablicy *monos
 * @param[in, out] monos : posortowana, alokowana dynamicznie tablica jednomianów
 * @param[in, out] help : pomocnicza tablica
 * @return : długość zreallokowanej tablicy *monos
 */
int MonoTableNormalizeHelp(int count, Mono *monos, Mono *help) {
    int i = 1;
    int counter = 0;
    int final_counter = 0;
    Poly poly_help;
    Mono mono_last = monos[0];
    //mono_last jest swojego rodzaju bufforem który o dopowiednim momencie opróżniamy

    if (count == 1) {
        if (PolyIsZero(&monos[0].p))
            return 0;
        return 1;
    }

    while (i < count) {
        if (mono_last.exp == monos[i].exp) {
            while (i < count && mono_last.exp == monos[i].exp) {
                poly_help = PolyAdd(&(mono_last.p), &(monos[i].p));
                MonoDestroy(&mono_last);
                mono_last = MonoFromPoly(&poly_help, monos[i].exp);
                MonoDestroy(&monos[i]);
                i++;
            }
        } else {
            help[counter++] = mono_last;
            mono_last = monos[i++];
        }
    }

    help[counter++] = mono_last;

    for (int j = 0; j < counter; j++) {
        if (!PolyIsZero(&(help[j].p))) {
            monos[final_counter++] = help[j];
        }
    }

    return final_counter;
}

/**
 * Normalizuje tablicę monomianów
 * Redukuje monomiany o tych samych wykładnikach do jednego.
 * Korzysta przy tym z funkcji PolyAdd. Zachowuje posortowanie tablicy oraz redukuje zerowe monomiany.
 * Przykładowo, jeżeli wykładniki monomianów w tablicy monos to 1,2,2,3
 * po wykonaniu funkcji będą one 1,2,3 albo 1,3 w zależności od tego, czy
 * monomiany o tych samych wykładnikach nie sumują się do 0.
 * @param[in] count : długość tablicy *monos
 * @param[in, out] monos : posortowana, alokowana dynamicznie tablica jednomianów
 * @return : długość zreallokowanej tablicy *monos
 */
int MonoTableNormalize(int count, Mono **monos) {
    Mono* help = MonoInitialize(count);
    int tempo = MonoTableNormalizeHelp(count, *monos, help);
    free(help);

    if (tempo == 0) {
        free(*monos);
        *monos = NULL;
    }
    else {
        *monos = realloc(*monos, tempo * sizeof(Mono));
        assert(*monos != NULL);
    }

    return tempo;
}

Poly PolyClone(const Poly* p) {
    Poly poly;

    if (p->size == 0)
        return *p;

    poly.size = p->size;
    poly.mono = MonoInitialize(poly.size);
    for (int i = 0; i < poly.size; i++) {
        poly.mono[i] = MonoClone(&(p->mono[i]));
    }
    poly.wsp = p->wsp;

    return poly;
}

void PolyDestroy(Poly* p) {
    for (int i = 0; i < p->size ; i++) {
        MonoDestroy(&(p->mono[i]));
    }

    if (p->size != 0)
        free(p->mono);
}

/**
 * Dodaje do wielomianu p wielomian stały o wspłczynniku coeff
 * @param[in] coeff : liczba
 * @param[in] p : wielomian
 * @return : suma liczby i wielomianu
 */
Poly PolyAddCoeff (poly_coeff_t coeff, const Poly *p) {
    Poly poly_help;
    Mono* mono;
    int i = 0;
    int size, int_diff;

    if (p->mono[0].exp == 0) {
        if (PolyIsCoeff(&(p->mono[0].p)))
            poly_help = PolyFromCoeff(coeff + p->mono[0].p.wsp);
        else {
            poly_help = PolyFromCoeff(coeff);
            poly_help = PolyAdd(&poly_help, &(p->mono[0].p));
        }

        if (PolyIsZero(&poly_help)) { //jeżeli pierwsze wyrazy sumuja sie do 0;
            size = p->size - 1; //nie może być, że mono[0] zawiera wielomian staly i nie ma innych monomianow, czyli q->size > 1;
            mono = MonoInitialize(size);
        }
        else { //jezeli pierwsze wyrazy nie sumuja sie do 0;
            size = p->size;
            mono = MonoInitialize(size);
            mono[i++] = MonoFromPoly(&poly_help, 0);
        }
    }
    else {
        size = p->size + 1;
        mono = MonoInitialize(size);
        poly_help = PolyFromCoeff(coeff);
        mono[i++] = MonoFromPoly(&poly_help, 0);
    }
    int_diff = size - p->size;

    while (i < size) {
        mono[i] = MonoClone(&(p->mono[i - int_diff]));
        i++;
    }

    return (Poly) {.mono = mono, .size = size};
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly poly;
    Poly poly_pom;
    Mono* mono;
    Mono* help;
    Mono* mono_help;
    int j = 0;
    int size = 0;
    int tempo_size;
    if (PolyIsZero(p))
        return PolyClone(q);

    if (PolyIsZero(q))
        return PolyClone(p);

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->wsp + q->wsp);

    if (PolyIsCoeff(p))
        return PolyAddCoeff(p->wsp, q);

    if (PolyIsCoeff(q))
        return PolyAddCoeff(q->wsp, p);


    tempo_size = p->size + q->size;
    mono = MonoInitialize(tempo_size);

    for (int i = 0; i < p->size; i++)
        mono[i] = p->mono[i];

    for (int i = p->size; i < tempo_size; i++)
        mono[i] = q->mono[i - p->size];

    MonoMerge(0, p->size - 1, tempo_size - 1, mono);

    help = MonoInitialize(tempo_size);
    while (j < tempo_size) {
        if (j + 1 < tempo_size && mono[j].exp == mono[j + 1].exp) {
            poly_pom = PolyAdd(&(mono[j].p), &(mono[j + 1].p));
            if (!PolyIsZero(&poly_pom))
                help[size++] = MonoFromPoly(&poly_pom, mono[j].exp);
            j += 2;
        }
        else {
            help[size++] = MonoClone(&mono[j]);
            j++;
        }
    }
    if (size == 0) {
        free(mono);
        free(help);
        return PolyZero();
    }
    mono_help = realloc(mono, size * sizeof(Mono));
    assert(mono_help != NULL);
    mono = mono_help;
    for (int i = 0; i < size; i++)
        mono[i] = help[i];
    free(help);

    if (size == 1 && mono[0].exp == 0 && PolyIsCoeff(&(mono[0].p))) { //jeżeli dostaliśmy monomian zawierający wielomian zerowy
        poly = PolyFromCoeff(mono[0].p.wsp);
        free(mono);
        return poly;
    }
    else
        return (Poly) {.mono = mono, .size = size};
}

Poly PolyAddMonos(unsigned count, const Mono monos[]) {
    int elements_number;
    Poly poly_help;
    Mono *mono = MonoInitialize(count);
    for (unsigned i = 0; i < count; i++)
        mono[i] = monos[i];
    MonoTableSort(0, count - 1, mono);

    elements_number = MonoTableNormalize(count, &mono);

    if (elements_number == 0) //jeżeli wszystkie jednomiany sie zredukowały
        return PolyFromCoeff(0);
    if (elements_number == 1 && mono[0].exp == 0 && PolyIsCoeff(&mono[0].p)) {
        poly_help = mono[0].p;
        free(mono);
        return poly_help;
    }
    return (Poly) {.mono = mono, .size = elements_number};
}


Poly PolyMul(const Poly *p, const Poly *q) {
    Mono* mono;
    Poly poly_help;
    Poly poly;
    int counter = 0;

    if (PolyIsZero(p) || PolyIsZero(q))
        return PolyZero();

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->wsp * q->wsp);


    if (PolyIsCoeff(p)) {
        mono = MonoInitialize(q->size);
        for (int i = 0; i < q->size; i++) {
            poly_help = PolyMul(p, &(q->mono[i].p));
            if (!PolyIsZero(&poly_help))
                mono[counter++] = MonoFromPoly(&poly_help, q->mono[i].exp);
        }
        if (counter == 0) //jeżeli wszystkie monomiany sie wyzerowaly
            poly = PolyFromCoeff(0);
        else
            poly = PolyAddMonos((unsigned)counter, mono);
        free(mono);
        return poly;
    }

    if (PolyIsCoeff(q))
        return PolyMul(q, p);

    mono = MonoInitialize((q->size) * (p->size));
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < q->size; j++) {
            poly_help = PolyMul(&(p->mono[i].p), &(q->mono[j].p));
            mono[counter++] = MonoFromPoly(&poly_help, p->mono[i].exp +
                                                       q->mono[j].exp);
        }
    }

    poly_help = PolyAddMonos((unsigned)(q->size) * (p->size), mono);
    free(mono);

    return poly_help;
}


/**
 * neguje zadany wielomian, przjemuje go na własność
 * @param[in, out] p : negowany wielomian
 */
void PolyNegHelp(Poly *p) {
    poly_coeff_t poly_coeff_pom;
    if (PolyIsCoeff(p)) {
        poly_coeff_pom = p->wsp;
        p->wsp = -poly_coeff_pom;
    }
    else {
        for (int i = 0; i < p->size; i++) {
            PolyNegHelp(&(p->mono[i].p));
        }
    }
}

Poly PolyNeg(const Poly *p) {
    Poly poly_help = PolyClone(p);
    PolyNegHelp(&poly_help);
    return poly_help;
}


Poly PolySub(const Poly *p, const Poly *q) {
    Poly poly;
    Poly poly_help = PolyNeg(q);
    poly = PolyAdd(&poly_help, p);
    PolyDestroy(&poly_help);

    return poly;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx) {
    int max = 0;
    int int_help;

    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    if (var_idx == 0)
        return p->mono[p->size - 1].exp;

    for (int i = 0; i < p->size; i++) { //wykona się co najmniej raz
        int_help = PolyDegBy(&(p->mono[i].p), var_idx - 1);
        if (max < int_help)
            max = int_help;
    }

    return max;
}

poly_exp_t PolyDeg(const Poly *p) {
    int max = 0;
    int int_help;

    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    for (int i = 0; i < p->size; i++) {
        int_help = PolyDeg(&(p->mono[i].p));
        if (int_help + p->mono[i].exp > max)
            max = int_help + p->mono[i].exp;
    }

    return max;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    bool poly_is_eq = true;

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return (p->wsp == q->wsp);

    if (PolyIsCoeff(p) || PolyIsCoeff(q) || p->size != q->size)
        return false;

    for (int i = 0; i < p->size; i++) {
        if (!PolyIsEq(&(p->mono[i].p), &(q->mono[i].p)) || p->mono[i].exp != q->mono[i].exp)
            poly_is_eq = false;
    }

    return poly_is_eq;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    Poly poly = PolyZero();
    Poly poly_help;
    Poly poly_help2;

    if (PolyIsCoeff(p))
        return *p;

    for (int i = 0; i < p->size ; i++) {
        poly_help = PolyFromCoeff(Power(x, p->mono[i].exp));
        poly_help = PolyMul(&poly_help, &(p->mono[i].p)); //nie tracimy pamięci bo wielomian jest stały
        poly_help2 = PolyAdd(&poly_help, &poly);

        if (!PolyIsCoeff(&poly_help))
            PolyDestroy(&poly_help);

        if (!PolyIsCoeff(&poly))
            PolyDestroy(&poly);

        poly = poly_help2;
    }

    return poly;
}

/**
 * wypisuje rekurencyjnie wielomian
 * @param[in] p
 */
void PolyPrint(const Poly *p) {
    if (PolyIsCoeff(p))
        printf("%ld", p->wsp);
    else {
        for (int i = 0; i < p->size; i++) {
            printf("(");
            PolyPrint(&(p->mono[i].p));
            printf(",");
            printf("%d", p->mono[i].exp);
            printf(")");

            if (i != p->size - 1)
                printf("+");
        }
    }
}
/**
 * Poteguje wielomian
 * @param[in] p
 * @param[in] exp
 * @return wielomian p^{exp}
 */
Poly PolyPower(const Poly *p, poly_exp_t exp) {
        Poly half;
        Poly result;
        Poly dummy;

        if (exp == 0)
            return PolyFromCoeff(1);

        if (exp % 2 == 0) {
            half = PolyPower(p, exp / 2);
            result = PolyMul(&half, &half);
            PolyDestroy(&half);
            return result;
        }
        else {
            half = PolyPower(p, (exp - 1) / 2);
            result = PolyMul(&half, &half);
            PolyDestroy(&half);
            dummy = result;
            result = PolyMul(&result, p);
            PolyDestroy(&dummy);
            return result;
        }
}

/**
 * Funkcja zwraca wartość wielomianu w punkcie (0,0,...,0)
 * @param[in] p : wielomian
 * @return wartość w zerze
 */
Poly PolyAtZeroPoint(const Poly *p) {
    if (p->size == 0)
        return PolyFromCoeff(p->wsp);
    else {
        if (p->mono[0].exp != 0)
            return PolyZero();
        else
            return PolyAtZeroPoint(&(p->mono[0].p));
    }
}

/**
 * Funkcja zwraca wielomian p(x[0], x[1], ..., x[count - 1], 0, 0, 0, ...)
 * Nie przejmuje nic na własność
 * @param[in] p : wielomian pod który podstawiamy
 * @param[in] count : ilość zmiennych pod które podstawiamy wielomiany
 * @param[in] x : tablica podstawianych wielomianów
 * @return wynikowy wielomian
 */
Poly PolyCompose(const Poly *p, unsigned count, const Poly x[]) {
    Poly result;
    Poly partial_result;
    Poly helper, helper2;
    Poly last_poly;

    if (count == 0) {
        return PolyAtZeroPoint(p);
    }

    if (p->size == 0) {
        return PolyFromCoeff(p->wsp);
    }

    last_poly = PolyPower(&x[0], p->mono[0].exp);
    helper = PolyCompose(&p->mono[0].p, count - 1, x + 1);
    result = PolyMul(&last_poly, &helper);
    PolyDestroy(&helper);

    for (int i = 1; i < p->size; i++) {
        helper = last_poly;
        helper2 = PolyPower(&x[0], p->mono[i].exp - p->mono[i-1].exp);
        last_poly = PolyMul(&helper, &helper2);
        PolyDestroy(&helper);
        PolyDestroy(&helper2);

        helper = PolyCompose(&p->mono[i].p, count - 1, x + 1);
        partial_result = PolyMul(&helper, &last_poly);
        PolyDestroy(&helper);

        helper = result;
        result = PolyAdd(&result, &partial_result);
        PolyDestroy(&helper);
        PolyDestroy(&partial_result);
    }
    PolyDestroy(&last_poly);
    return result;
}

