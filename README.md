## Dziennik zdarzeń

### Dzień 1 - Rozpoczęcie projektu:
- Skonfigurowano **Visual Studio 2022** z odpowiednimi ustawieniami C++.
- Zainstalowana biblioteka **SFML** i skonfigurowane ścieżki do plików nagłówkowych i bibliotek w projekcie.
- Utworzono nowy projekt C++ w Visual Studio.
- Skonfigurowano pliki **`main.cpp`**, **`Game.h`** oraz **`Game.cpp`**.
- Skonfigurowano bibliotekę **SFML** w projekcie do obsługi grafiki, okna i systemu.
- Przygotowano szkielet klasy `Game` z pętlą główną gry, inicjalizując okno gry o rozdzielczości 800x600 px.

### Dzień 2 – Rozwój gry:
Cele dnia:

Dostosowanie ruchu statku:

Została zaimplementowana logika ruchu statku, który sterowany jest przez klawisze Left i Right.
Początkowo prędkość statku ustawiono na speed = 5.0f, co powodowało, że ruch był zbyt szybki, co utrudniało kontrolowanie statku. Zmieniono prędkość na speed = 0.1f, co poprawiło płynność i kontrolę nad ruchem.
Zamiana kwadratu na obrazek statku:

Aby ulepszyć wygląd gry, zamiast kwadratu użyto obrazka statku za pomocą klasy sf::Sprite i sf::Texture. Obrazek został załadowany z folderu assets/.
Początkowo obrazek był zbyt mały, co skutkowało jego nieczytelnością i niewłaściwym wyświetlaniem. Skala statku została dostosowana do sprite.setScale(0.5f, 0.5f), co poprawiło widoczność.
Dostosowanie granic ruchu:

Po załadowaniu obrazu statku z teksturą i jego skalowaniu, wystąpiły problemy z ruchem – statek zatrzymywał się na połowie ekranu. Powód leżał w tym, że przeskalowanie obrazu nie zostało uwzględnione przy obliczaniu granic.
Granice poruszania się statku zostały poprawione, uwzględniając przeskalowaną szerokość statku. Zmodyfikowano kod odpowiedzialny za granice, używając sprite.getLocalBounds().width * sprite.getScale().x, aby prawidłowo obliczyć szerokość statku po zastosowaniu skali.
Problemy, które wystąpiły:

Zbyt szybki ruch statku: Początkowa prędkość była zbyt duża, co sprawiało, że statek poruszał się zbyt szybko. Problem został rozwiązany poprzez zmniejszenie wartości prędkości do speed = 0.1f.
Błędy z granicami ruchu: Po załadowaniu obrazu i zastosowaniu skali, granice poruszania statku nie były odpowiednio obliczane, co powodowało, że statek zatrzymywał się na połowie ekranu. Zmieniono logikę granic, uwzględniając przeskalowaną szerokość statku.
Niewidoczność statku: Początkowy obrazek statku był zbyt mały, przez co nie był dobrze widoczny w grze. Problem rozwiązano poprzez skalowanie statku do odpowiedniego rozmiaru za pomocą funkcji sprite.setScale().

### **Podsumowanie dnia 3 - Dodanie wrogów, strzałów oraz podstawowej mechaniki kolizji**

#### **Wykonane zadania:**
1. **Utworzenie klasy `Enemy`:**
   - Stworzono klasę `Enemy`, która reprezentuje wroga w grze. Każdy wróg posiada kształt (`sf::RectangleShape`), pozycję oraz metodę `update`, która umożliwia poruszanie wrogiem w odpowiednim kierunku. W tym etapie wrogowie byli reprezentowani przez prostokąty.

2. **Dodanie klasy `Bullet`:**
   - Zaimplementowano klasę `Bullet`, która reprezentuje pocisk wystrzelony przez gracza. Pocisk porusza się w górę ekranu i sprawdzana jest jego kolizja z wrogami. Po trafieniu wroga, wróg zostaje usunięty.

3. **Rozmieszczenie wrogów na ekranie:**
   - Wrogowie zostali rozmieszczeni w układzie 3 rzędów po 11 kolumn, poruszając się na razie za pomocą prostych prostokątów. Wrogowie zmieniają kierunek ruchu po dotarciu do krawędzi ekranu.

4. **Implementacja mechaniki strzałów:**
   - Dodano możliwość strzelania przez gracza po naciśnięciu spacji. Pocisk porusza się w górę ekranu i sprawdzana jest jego kolizja z wrogami.

5. **Podstawowa detekcja kolizji:**
   - Po trafieniu pocisku w wroga, wróg zostaje usunięty z ekranu, a pocisk również.

#### **Problemy:**

1. **Problem z poruszaniem się wrogów:**
   - Wrogowie poruszali się, ale zdarzało się, że nie zachowywali się jak jeden obiekt, a każda kolumna odbijała się osobno, co skutkowało nachodzeniem wrogów na siebie. Dodatkowo, poruszały się za szybko.

   **Rozwiązanie**:
   - Wprowadzono mechanizm, który sprawił, że cała formacja wrogów poruszała się jako jeden obiekt, zmieniając kierunek po osiągnięciu krawędzi ekranu i obniżając całą formację wrogów po zmianie kierunku.

2. **Zbyt szybki ruch wrogów:**
   - Początkowo ruch wrogów był zbyt szybki, co utrudniało rozgrywkę.

   **Rozwiązanie**:
   - Zredukowano prędkość ruchu wrogów, aby była bardziej proporcjonalna do reszty elementów w grze i dostosowana do grywalności.



#### **GitHub Update:**

**Dodano:**
- Zaimplementowano klasę `Enemy`, która obsługuje wrogów.
- Dodano możliwość strzelania przez gracza z wykorzystaniem klasy `Bullet`.
- Implementowano podstawową detekcję kolizji między pociskami a wrogami.
- Dodano mechanikę poruszania się wrogów w szyku, zmieniających kierunek po dotarciu do krawędzi ekranu i obniżających się po zmianie kierunku.

**Poprawki:**
- Zoptymalizowano ruch wrogów, dostosowano go do lepszej grywalności.
- Dodano prostą detekcję kolizji oraz system punktacji.
