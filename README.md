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
