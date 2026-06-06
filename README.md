# Build & Run

```bash
# 1. Utwórz katalog build i wejdź do niego
mkdir .build && cd .build

# 2. Skonfiguruj projekt
cmake ..

# 3. Zbuduj
cmake --build .

# 4. Uruchom aplikację
./SmartArrayApp

# 5. Uruchom testy
ctest
# lub bezpośrednio:
./tests/exampleTests
```

Przy pierwszym uruchomieniu CMake pobierze FTXUI i GTest z internetu — może chwilę potrwać.
