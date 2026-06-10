# Wymagane publiczne metody

## Wymagania na 3.0

- Konstruktor bezparametryczny - tworzy pustą tablicę
- Konstruktor (unsigned size) - tworzy tablicę z zarezerwowaną pamięcią dla size elementów ale wielkością 0
- T at(unsigned index) - zwraca element pod indeksem index albo wyrzuca wyjątek std::out_of_range
- T operator[](unsigned index) - zwraca element pod indeksem index (bez sprawdzania, czy jest w zakresie)
- bool isEmpty() - sprawdza czy tablica jest pusta (true jeżeli jest pusta)
- unsigned size() - zwraca liczbę elementów przechowywanych w tablicy
- unsigned capacity() - zwraca liczbę elementów, dla których została zarezerwowana pamięć
- void reserve(unsigned size) - rezerwuje pamięć dla size elementów, ale nie zwiększa liczby przechowywanych elementów
- void resize(unsigned size), void resize(unsigned size, T elem) - zmiana rozmiaru tablicy (nie pojemności). Zmniejszanie, jeżeli zbyt duża, powiększanie, jeżeli zbyt mała (w tym przypadku może się zwiększyć pojemność). Wypełnianie domyślnymi wartościami albo elem
- void clear() - usuwanie wszystkich elementów, rozmiar -> 0, pojemność nie zmienia się
- void insert(unsigned index, T elem) - wstawianie elementu elem pod podany indeks index, przesuwanie pozostałych elementów albo wyrzuca wyjątek std::out_of_range
- void erase(unsigned index) - usuwanie elementu spod podanego indeksu index albo wyrzuca wyjątek std::out_of_range
- void erase(unsigned index, unsigned amount) - usuwanie podanej liczby elementów (amount) zaczynając od indeksu index albo wyrzuca wyjątek std::out_of_range
- void pushBack(T elem) - dodawanie elementu na koniec tablicy
- void popBack() - usuwanie ostatniego elementu z tablicy (pojemność zostaje ta sama, rozmiar się zmienia)

## Wymagania na 4.0

- Konstruktor (unsigned size, T elem) - tworzy tablicę wypełnioną size elementami o wartości elem
- bool operator==(SmartArray array) - sprawdzanie, czy tablice są takie same (wszystkie elementy przechowywane są takie same)

## Wymagania na 5.0

- void shrinkToFit() - dostosowuje pojemność tablicy do aktualnie przechowywanej liczby elementów
- void pushFront(T elem) - dodawanie elementu na początek tablicy
- void popFront() - usuwanie elementu z początku tablicy
- wypisywanie zawartości tablicy do strumienia cout (dodatkowe)

