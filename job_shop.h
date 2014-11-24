#include <string>
//#include "problem.h"

#ifndef JOB_SHOP_H
#define JOB_SHOP_H

/*!
 * \brief Klasa implementuje problem gniazdowy
 *  Umo¿liwia wczytywanie danych z pliku, s³u¿y do harmonogramowania zadañ
 *  w przypadku problemu gniazdowego
 */
class job_shop
{
private:
    int n;      /** liczba operacji*/
    int z;      /** liczba zadañ */
    int m;      /** liczba maszyn */
    int *czasi; /** czas wykonywania i-tej operacji */
    int *pi;    /** struktura przechowujaca permutacje: pi = [0 operacje_maszyny_1 0 operacje_maszyny_2 0 ... 0] */
    int *ti;    /** nastêpniki technologiczne */
    int *T;
    int *ps;    /** pozycje i-tych operacji w tabeli permutacji */
    int *lp;    /** poprzedniki */
    int *ci;    /** czas zakonczenia i-tej operacji */
	int *ph;	/** tablica pomocnicza w wyznaczaniu œcie¿ki krytycznej*/
	int *cPath;	/** œcie¿ka krytyczna: [liczba_elementow 0 1.element 2.element ... i.element 0]*/
	int *cPathColor;	/** tablica zapamiêtuj¹ca pocz¹tek i koniec bloku: -1 pocz¹tek bloku, 1 koniec bloku */
	int *cPathIndexes;	/** tablica zapamiêtuje indexy pocz¹tku i koñca bloku*/
	int blockSwaps;
public:
    /*!
     * \brief job_shop - konstruktor bezparametryczny
     *  Wszystkie zmienne s¹ wyzerowane, wskaŸniki ustawione na NULL
     */
    job_shop();

    /*!
     * \brief Destruktor
     * Gdy wskaŸniki s¹ ró¿ne od NULL, pamiêæ na któr¹ wskazuj¹ zostaje zwolniona.
     */
    ~job_shop();
    /*!
     * \brief Metoda wczytuj¹ca dane z pliku
     * Wczytywane dane s¹ wpisywane do tablicy pi jako dopuszczalna permutacja problemu.
     * Wype³nione zostaj¹ równie¿ tablice ps, ti oraz czasi.
     * \param file - nazwa pliku do wczytania
     * \return false jeœli nie uda³o siê otworzyæ pliku, true w przeciwnym przypadku
     */
    bool readFile(std::string file = "ta01.txt");
    /*!
     * \brief Metoda tworz¹ca obiekt
     * Na podstawie wczytanej wczeœniej liczby zadañ i maszyn, zostaje zaalokowana pamiêæ dla
     * tabli pi, ps, ti oraz czasi.
     */
    void createPi();
    /*!
     * \brief Metoda tworzy tablicê nastêpników technologicznych
     * Do dzia³ania potrzebuje permutacji pi i wype³nionej tablicy ps.
     */
    void makeTi();
	/*!
	* \brief Metoda tworzy tablice liczba poprzednikow
	* Tablica ta jest niezbêdna dla algorytmu tworz¹cego
	* harmonogram, zlicza ile operacji jest gotowych w danym momencie
	* wykonywania algorytmu.
	*/
    void makeLp();
	/*!
	* \brief Metoda s³u¿y do wypisywania wszystkich zmiennych klasy.
	* Zmienne te wypisywane s¹ na stdout.
	*/
    void logClass();
	/*!
	* \brief Obliczanie œcie¿ki krytycznej
	*/
	void evalPH(int i);
	/*!
	* \brief Metoda pomocnicza funkcji job_shop::createSchedule().
	* Oblicza czas zakoñczenia ::i -tej operacji: c_max(i) = max(T[i-1],pi[ps[i]-1])+czas_wykonywania
	* gdzie T[i-1] to poprzednik technologiczny, pi[ps[i]-1] to poprzednik maszynowy.
	* \param i  - numer operacji
	*/
	void evalCi(int i);
	/*!
	* \brief Metoda tworz¹ca harmonogram dla danej permutacji.
	*/
    void createSchedule();
	int findMaxCi();
	void createCPath();
	void createBlocks();
	void swapBlocks();

};

#endif // JOB_SHOP_H
