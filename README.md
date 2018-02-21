# **<p align="center">Inżynieria Oprogramowania</p>**

## <p align="center"><b>Dokumentacja gry <u><i>"Statki"</i><u></b></p>

<p align="center"><i>Pliki źródłowe diagramów znajdują się w folderze "docs" w repozytorium.</i></p>

<br><br>

# **<p align="center">Spis Treści</p>**


**1. Opis projektu**
* Cele
* Założenia
* Problemy
* Podstawowe moduły

**2. Moduły**
* Mediator
* Network
* NetworkAction
* ViewsManager

**3. Wzorce projektowe**
* Klient-Serwer
* Obserwator
* Singleton
* Mediator
* Most
* RAII
* Iterator

***


# **<p align="center">1. Opis projektu</p>**

**<u>Cele</u>**<br>
Celem projektu jest stworzenie sieciowej, dwuosobowej, multiplatformowej gry - [Statki](https://pl.wikipedia.org/wiki/Okr%C4%99ty) oraz dokumentacji projektowej aby otrzymać zaliczenie z przedmiotu Inżynieria Oprogramowania.

**<u>Założenia</u>**
* W grze bierze udział 2 graczy
* Gra odbywa się w sieci lokalnej (LAN)
* Zespól projektowy składa się z 2 osób
* Gra jest wykonana w technologii multiplatformowej Qt (C++/QML) co pozwala na jej uruchomienie na bardzo wielu systemach operacyjnych (m.in Windows, Linux, OS X, Android, iOS, ...)
* Obaj gracze musza znajdować się w tej samej sieci LAN, aby nawiązać połączenie
* Połączenie dwóch graczy odbywa się automatycznie, tworząc stół gracz zaczyna rozgłaszać swój adres w sieci, dołączający do stołu nasłuchuje i gdy napotka na wiadomość, dołącza do serwera
* Kod źródłowy przechowywany jest na repozytorium [github](https://github.com/macias0/IO) i jest objęty licencją [LGPL-3.0](https://github.com/macias0/IO/blob/master/LICENSE)


**<u>Problemy</u>**
* Termin oddania projektu
* Dostępność czasowa członków zespołu
* Zerowy budżet :)

**<u>Podstawowe moduły</u>**
* Moduł GUI (pliki QML) - odpowiadają za warstwę wizualną aplikacji i interakcje z użytkownikiem
* Moduł sieciowy - odpowiada nawiązanie połączenia i wymianę danych z drugim graczem
* Moduł logiczny - spaja oba powyższe moduły<br>



***


# **<p align="center">2. Moduły</p>**

Diagram klas wykorzystanych w projekcie (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/class_diagram.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/class_diagram.png)


Diagram przypadków użycia (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/use_case.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/use_case.png)


Diagram aktywności dla przebiegu rozgrywki (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/activity_diagram.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/activity_diagram.png)

<br><br>
**<u>Mediator</u>**
Służy do łączenia logiki gry z GUI. cała komunikacja przetwarzana jest właśnie przez tę klasę.


Diagram sekwencji procesu tworzenia/dołączania do nowej gry (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/Mediator-create_join_game.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/Mediator-create_join_game.png)
<br>

Zmienne readonly, ich zmiana wywołuje aktualizację GUI:
* `EView::View activeView` - ekran wyświetlany aktualnie przez GUI
* `bool yourTurn` - czy trwa tura gracza
* `boardIsValid` - czy plansza zawiera potrzebne statki
* `boardHasErrors` - czy statki są ustawione poprawnie
* `preparingBoardPhase` - czy trwa faza przygotowywania gry
* `waitingForEnemyBoard` - czy przeciwnik przygotowuje planszę do gry
* `waitingToConnect` - czy trwa oczekiwanie na połączenie
Moduł łączący logikę zapisaną w C++ z GUI pisanym w QML.
Obsługa rozpoczę

Funkcje wykorzystywane przez GUI:

* `void createServer()` - utwórz serwer gry
* `void joinGame()` - dołącz do gry
* `void exitGame()` - zamknij grę
* `void playerIsReady()` - zaakceptuj przygotowaną planszę
* `void toggleFirstTile(int x, int y)` - przestaw pierwsze pole planszy
* `void toggleTile(int x, int y)` - przestaw pole planszy
* `void attackTile(int x, int y)` - zaatakuj pole
* `void surrender()` - poddaj się
* `void goBackToMainMenu()` - wróc do głównego menu
* `void showCredits()` - wyświetl napisy końcowe
* `void exitFromCredits()` - zamknij napisy końcowe

Sygnały nasłuchiwane przez GUI:

* `prepareToNewGame()` - przygotuj się do nowej gry
* `renderShip(int id, int x, int y)` - wyświetl statek na planszy
* `removeShip(int id)` - usuń statek z planszy
* `renderShot(int x, int y, bool accurate, bool enemyBoard)` - wyświetl strzał na planszy. Jeśli `accurate == true`, to trafiony został statek. Jeśli `enemyBoard == true`, strzał został wykonany na planszy przeciwnika. W przeciwnym razie strzał został oddany na naszej planszę.
* `newMessageToDisplay(QString message)` - wyświetl krótką wiadomość


**<u>Network</u>**

Klasa odpowiedzialna za łączenie graczy ze sobą oraz komunikacji poprzez sieć. Jest wykorzystywana w postaci singletona w klasie `Mediator`.

Diagram sekwencji procesu tworzenia nowego połączenia sieciowego (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-establishing_connection.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-establishing_connection.png)

Diagram sekwencji procesu przesyłania i odbierania danych (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-send_receive_message.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-send_receive_message.png)

Diagram sekwencji procesu zamykania połączenia (kliknij, aby powiększyć):
[![](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-close_connection.png)](https://raw.githubusercontent.com/macias0/IO/master/docs/Network-close_connection.png)

**<u>NetworkAction</u>**

Pojedyncza akcja przesyłana przez sieć. Możliwe rodzaje akcji to "Shot", "BoardExchange", "Surrend".

**<u>ViewsManager</u>**

Obiekt odpowiedzialny za zmianę aktualnie wyświetlanego ekranu. Nasłuchuję na zmienną `activeView`, klasy `Mediator` i w zależności od jej wartości aktywuje odpowiedni widok.

***

# **<p align="center">3. Wzorce projektowe</p>**
**<u>Klient-Serwer</u>**<br>
Gracz tworzący grę jest serwerem - udostępnia usługę gry i czeka na połączenie gracza dołączającego, który jest klientem.<br>
[Klient](../blob/master/src/cpp/network.h#L65)<br>
[Serwer](../blob/master/src/cpp/network.h#L64)<br>

**<u>Obserwator</u>**<br>
Cały mechanizm sygnałów w Qt działają w oparciu o ten wzorzec. Kiedy łączymy jakiś slot do sygnału to sygnał staje się obserwowanym, a sloty do niego podłączone - obserwującymi.<br>
[Łączenie sygnału ze slotem](../blob/master/src/cpp/network.cpp#L7)<br>

**<u>Singleton</u>**<br>
Klasa `Network` jest singletonem, ponieważ nie przewidujemy połączenia z więcej niż jednym graczem podczas rozgrywki.<br>
[Network.h](../blob/master/src/cpp/network.h#L25-L29)<br>

**<u>Mediator</u>**<br>
Klasa `Mediator` jak sama nazwa wskazuje jest mediatorem, ponieważ komunikuje się innymi klasami, którymi zarządza, a do QML'a wystawia tylko prosty interfejs<br>
[Mediator](../blob/master/src/cpp/mediator.h)

**<u>Most</u>**<br>
Wszystkie klasy (c++) w tym projekcie korzystają z tego wzorca, ponieważ oddzielają interfejs od implementacji rozmieszczając je odpowiednio w plikach `*.h` i `*.cpp`.

**<u>RAII</u>**<br>
_Zabór Zasobu Jest Inicjalizacją_ (Resource Acquisition Is Initialization) - z tego wzorca korzysta klasa `Network`, ponieważ używa `QScopedPointer` który w momencie jego niszczenia, niszczy tez obiekt przypisany do niego, w tym przypadku sockety i serwer.<br>
[Network.h](../blob/master/src/cpp/network.h#L64-L66)<br>

**<u>Iterator</u>**<br>
Obiekt zapewniający sekwencyjny dostęp do podobiektów zgrupowanych w większym obiekcie.<br>
[Mediator.cpp](../blob/master/src/cpp/mediator.cpp#L293-L299)<br>


