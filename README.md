## Dziennik zdarzeń

### **Dzień 1 - Rozpoczęcie projektu:**
- Skonfigurowano **Visual Studio 2022** z odpowiednimi ustawieniami C++.
- Zainstalowano bibliotekę **SFML** i ustawiono ścieżki do nagłówków i bibliotek w projekcie.
- Utworzono nowy projekt C++ w Visual Studio.
- Przygotowano pliki:
  - **`main.cpp`** – główny punkt wejścia do aplikacji,
  - **`Game.h`** i **`Game.cpp`** – deklaracja i definicja klasy `Game`.
- W klasie `Game` stworzono podstawową pętlę główną (`run()`), tworzącą okno gry o rozdzielczości 800×600 px.
- Wprowadzono prosty **system stanów gry** (`enum GameState`), aby rozróżniać ekrany (Start, Rozgrywka, Wygrana, itp.).

---

### **Dzień 2 – Sterowanie statkiem gracza i wprowadzenie pierwszej grafiki:**

#### **Cele dnia:**
1. **Sterowanie statkiem:**
   - Zaimplementowano ruch statku sterowanego strzałkami **Left** i **Right**.
   - Ustalono początkową prędkość na `5.0f`, następnie zastosowano `deltaTime` i zmieniono prędkość na `0.1f`, aby ruch był płynniejszy.

2. **Zastąpienie prostokąta sprite’em:**
   - Dodano `sf::Sprite` + `sf::Texture` do wyświetlania graficznego statku.
   - Zastosowano skalowanie (`sprite.setScale(...)`) w celu dopasowania rozmiaru statku do okna.

3. **Granice ruchu statku:**
   - Upewniono się, że statek nie wyjdzie poza obszar okna, uwzględniając szerokość i wysokość przeskalowanego sprite’a (`sprite.getLocalBounds()`).

#### **Problemy i rozwiązania:**
- **Zbyt szybki ruch** – poprawiono współczynnik prędkości i włączono `deltaTime`.
- **Błędne granice** – właściwe uwzględnienie skalowania sprite’a w obliczeniach krawędzi.

---

### **Dzień 3 - Dodanie wrogów, pocisków gracza i podstawowych kolizji:**

1. **Klasa `Enemy`:**
   - Utworzono klasę `Enemy` reprezentującą wrogów.
   - Wrogowie są ustawiani w formację (3 rzędy, 11 kolumn), poruszają się horyzontalnie i po dotarciu do krawędzi ekranu zmieniają kierunek.

2. **Klasa `Bullet`:**
   - Dodano klasę `Bullet` reprezentującą pociski gracza.
   - Pociski wylatują z pozycji statku i poruszają się w górę ekranu.

3. **Detekcja kolizji:**
   - Kolizja **pocisku gracza** z **wrogiem** powoduje:
     - Usunięcie (erase) wroga z wektora `enemies`.
     - Przyznanie punktów graczowi (np. +100).

#### **Problemy i rozwiązania:**
- **Rozproszony ruch wrogów** – wprowadzono jeden wspólny `direction` dla całej formacji, by zsynchronizować ich ruch.
- **Zbyt duża prędkość wrogów** – skorygowano wartości prędkości, by dopasować dynamikę rozgrywki.

---

### **Dzień 4 - Obsługa wielopoziomowej rozgrywki, zapisywanie/wczytywanie stanu gry i tabela wyników:**

1. **Wielopoziomowa rozgrywka (zmienna `currentLevel`):**
   - Dodano `currentLevel` i metodę `setupLevel()`, która konfiguruje liczbę wrogów, ich prędkość oraz częstotliwość strzałów w zależności od poziomu.
   - Po wyczyszczeniu wszystkich wrogów (`enemies.empty()`) gra zwiększa poziom (1 → 2 → 3).  
   - Na poziomie 3, po wyeliminowaniu wrogów, pojawia się ekran zwycięstwa (`GameState::YouWin`).

2. **Stany gry (`GameState`):**
   - **`StartScreen`** – ekran startowy (wpisywanie nicku, *ENTER/SPACJA* – start, *ESC* – wyjście, *F3* – wczytanie gry, *F4* – tabela wyników).
   - **`Playing`** – rozgrywka (ruch, strzelanie, kolizje, *F1* – pomoc, *F2* – zapisz stan gry, *F3* – wczytaj, *F4* – highscore, *F9* – pominięcie poziomu, *ESC* – potwierdzenie wyjścia).
   - **`Help`** – ekran pomocy (sterowanie, zasady gry, powrót klawiszem *SPACJA* lub *F1*).
   - **`GameOver`** – ekran przegranej (wyświetlenie wyniku, *R* – restart do ekranu startowego, *ESC* – wyjście, *F4* – highscore).
   - **`YouWin`** – ekran zwycięstwa (informacja o wygranej, *R* – restart do ekranu startowego, *ESC* – wyjście, *F4* – highscore).
   - **`ExitConfirmation`** – ekran potwierdzenia wyjścia z gry (*Y* – tak, *N* – nie).

3. **Zapisywanie i wczytywanie gry (funkcje `saveGameState()`, `loadGameState()`):**
   - **Zapis** (`F2`):
     - Nick gracza, wynik, żyć gracza, pozycja statku.
     - Aktualny poziom, kierunek poruszania się wrogów.
     - Pozycje wszystkich wrogów i pocisków (zarówno gracza, jak i wrogów).
   - **Wczytanie** (`F3`):
     - Odtworzenie wszystkich danych z pliku, przywracanie poprzedniego stanu gry (poziom, rozmieszczenie wrogów/pocisków, wynik, liczba żyć).

4. **Tabela wyników (pliki `highscores.txt`):**
   - Funkcja `updateHighscores()` – dodaje obecny wynik do listy, sortuje malejąco (przez `std::sort()`) i przycina do top 5.
   - Funkcja `showHighscoresWindow()` (klawisz *F4*) – otwiera osobne okno SFML, wyświetlające listę najlepszych wyników (z pliku `highscores.txt`).

5. **Pominięcie poziomu (`skipLevel()`, klawisz `F9`):**
   - Usuwa obecnych wrogów i pociski.
   - Natychmiast przechodzi do następnego poziomu (lub ekranu `YouWin`, jeśli był to ostatni poziom).

#### **Problemy i rozwiązania:**
- **Format danych przy zapisie i odczycie** – wymagał jasnej kolejności (nick → wynik → pozycje itp.).  
- **Sortowanie rekordów** – zastosowano lambdę w `std::sort()`, by uszeregować wyniki malejąco.

---

### **Dzień 5 - Ulepszenie wizualne i dźwiękowe, kolizje z pociskami wroga i ekrany końcowe:**

1. **Nowe ekrany i tekstury:**
   - **Tło główne** rozgrywki (`tlo.jpg`) skalowane do 800×600.
   - **Ekran startowy** (`start.jpg`).
   - **Ekran pomocy** (`help.jpg`).
   - **Ekran wyjścia** (`exit.png`) – potwierdzenie wyjścia.
   - **Ekran Game Over** (`gameover.png`).
   - **Ekran zwycięstwa** (`youwin.png`).
   - Każda z tekstur skalowana dynamicznie w metodzie `render()` (np. `sprite.setScale(...)`).

2. **Dźwięk strzału (`shoot.wav`):**
   - Ładowany do `shootBuffer`, odtwarzany przez `shootSound.play()` po naciśnięciu **Spacji** (strzał gracza).

3. **Rozszerzona mechanika kolizji – pociski wroga (`EnemyBullet`):**
   - Dodano klasę `EnemyBullet`.  
   - Losowy wróg (`enemyToShoot`) strzela pociskami w gracza z określoną częstotliwością (zależną od poziomu).  
   - Kolizja **pocisk wroga → gracz** odbiera graczowi życie (przy 0 życiach → `GameOver`).
   - Kolizja **pocisk gracza → wróg** zwiększa wynik i usuwa wroga (jak wcześniej).

4. **Wyświetlanie informacji w trakcie gry:**
   - W **stanie `Playing`** w lewym górnym rogu widoczne są:
     - **Wynik** (Score),
     - **Życia** (Lives),
     - **Nick** gracza (Player),
     - **Aktualny poziom** (Level).
   - Wszystko odświeżane na bieżąco w metodzie `render()`.

5. **Optymalizacja i porządki w kodzie:**
   - Ujednolicono nazewnictwo metod i zmiennych (`enemyToShoot`, `lastEnemyBulletTime`, itp.).
   - Komentarze wewnątrz kodu opisują kluczowe fragmenty logiki.
   - Zadbano o zwalnianie nieużywanych zasobów (np. usuwanie pocisków wychodzących poza ekran).

#### **Najczęstsze problemy i ich rozwiązania:**
- **Niewłaściwe skalowanie tła lub sprite’ów** – stałe przeliczanie przez `sprite.getLocalBounds()` i skalowanie do rozdzielczości 800×600.
- **Konflikty przy wielokrotnym odtwarzaniu dźwięku** – wykorzystanie jednego obiektu `sf::Sound` i jednego `sf::SoundBuffer`.

---

### **Podsumowanie obecnej funkcjonalności gry „Space Invaders”:**
- **Wieloetapowa rozgrywka**:
  - 3 poziomy trudności (modyfikowana liczba rzędów wrogów, prędkość i tempo strzałów).
  - Ekran zwycięstwa po ukończeniu 3. poziomu.
- **Sterowanie statkiem** (strzałki lewo/prawo) oraz **strzelanie** (Spacja).
- **Kolizje** i **punkty**:
  - Pocisk gracza → wróg (+100 pkt i usunięcie wroga).
  - Pocisk wroga → gracz (utrata życia, przy 0 → `GameOver`).
- **Tabela wyników** (`highscores.txt`) z top 5, wyświetlana w osobnym oknie (F4).
- **Zapisywanie gry** (F2) i **wczytywanie** (F3) z pliku (`save.txt`) – zapisywane są stan, poziom, pozycje wrogów i pocisków, punktacja, liczba żyć itp.
- **Pominięcie poziomu** (F9) – pozwala szybko przejść do następnego etapu.
- **Ekran pomocy** (F1) – opis sterowania i zasad gry.
- **Ekran potwierdzenia wyjścia** (ESC) – umożliwia anulowanie zamknięcia gry (klawisz N) lub potwierdzenie (klawisz Y).
- **Dźwięk strzału** (`shoot.wav`) – odtwarzany przy wystrzale gracza.
- **Wielokrotne stany gry** zapewniają przejrzystą nawigację między ekranami (Start, Help, Playing, GameOver, YouWin, ExitConfirmation).

Dzięki temu dziennikowi mamy jasny obraz rozwoju projektu – od podstawowej konfiguracji aż do obecnego stanu, w którym gra **Space Invaders** zawiera rozbudowane mechaniki rozgrywki, wielopoziomowy system wrogów, zapisywanie i wczytywanie stanu gry, tabelę najlepszych wyników, zaawansowaną obsługę kolizji oraz różnorodne ekrany, stanowiące atrakcyjną oprawę wizualną i dźwiękową.
