# Izveštaj o analizi projekta '2048'

Ovde će biti prikazan detaljan izveštaj o analizi konzolne C++ igre 2048. Projekat je testiran pomoću alata za verifikaciju softvera: 
- [catch2](#1-testiranje-jedinica-koda-pomoću-alata-catch2) - testiranje jedinica koda
- [lcov](#2-praćenje-pokrivenosti-koda-pomoću-alata-lcov) - praćenje pokrivenosti koda jediničnim testovima
- [valgrind](#3-profajliranje-pomoću-alata-valgrind) - profajliranje
  - [massif](#31-massif)
  - [callgrind](#32-callgrind)
- [cppcheck](#4-statička-analiza-pomoću-alata-cppcheck) - statička analiza

## 1. Testiranje jedinica koda pomoću alata Catch2
Testiranje jedinica koda (unit testing) je tehnika kojom se proverava funkcionisanje delova sistema koji se mogu nezavisno testirati. *Catch2* je framework za testiranje u C++ koji je jednostavan za korišćenje i veoma moćan. Omogućava pisanje testova u čistom C++ jeziku, koristeći čitljivu sintaksu. Testovi se pišu kao *.cpp* datoteke koristeći zaglavlje [catch.hpp](unit_tests/tests/catch.hpp). <br/>
Testovi su fokusirani na proveru ispravnosti osnovnih funkcionalnosti igre: inicijalizovanje table, crtanje table, unos sa tastature, pomeranje brojeva, spajanje brojeva, računanje poena i kraj igre. 

Za reprodukovanje testova potrebno je pokrenuti python script [run_tests.py](unit_tests/run_tests.py). <br/>
Za potrebe testiranja programa, neki delovi koda su refaktorisani. Napravljene izmene se mogu videti u fajlu [custom.patch](custom.patch)

### 1.1. Inicijalizovanje table
Na početku igre, tabla se inicijalizuje tako da su popunjena samo dva polja(vrednostima 2 ili 4), koja se mogu naći na random pozicijama. Ostala polja moraju biti prazna(tj. imati vrednost 0). Ovo je testirano oslanjajući se na činjenicu da je tabla nepromenljive veličine, pa ima određen broj polja n. Potrebno je da tabla na n-2 polja ima 0, a na samo 2 polja vrednosti 2 ili 4. <br/>
Na slici ispod je prikazana ispravno inicijalizovana tabla. <br/>
![Screenshot from 2024-08-19 16-55-02](https://github.com/user-attachments/assets/dc02c0a5-8f92-4414-b858-f97b5f2dd7df) <br/>
Prolaznost testova je 100%, pa je ova funkcionalnost uspešno implementirana. 

### 1.2. Crtanje table
Nakon inicijalizovanja table i nakon njenog svakog menjanja, potrebno je prikazati tablu na konzoli. Kako nacrtana tabla ukljucuje i bojenje brojeva određenim bojama, radi jednostavnijeg testiranja, izlaz je čitan zanemarujući boje. <br/>
Prolaznost testova je 100%.

### 1.3. Unos sa tastature
Da bi se igra igrala, potrebno je da korisnik unese jedan karakter s tastature, koji predstavlja smer u kom želi da se polja pomere. Za smer gore, treba uneti ’w’ ili ’k’, za dole ’s’ ili ’j’, levo ’a’ ili ’h’, desno ’d’ ili ’l’. <br/>
U ovom testu je analizirano da li program dobro čita ulaz. To je izvedeno koristeći pipe sistem za simulaciju standardnog ulaza. Prolaznosti testova je 100%.

Naredni test obuhvata proveru da li se polja kreću u željenom smeru nakon zadatog kataktera.

### 1.4. Pomeranje i spajanje polja
Ova funkcionalnost predstavlja srž igre. Polja, tj. brojevi na tabli se pomeraju u onom smeru koji korisnik zatraži unosom s tastature(wasd ili hjkl). Nakon toga se polja sa istim vrednostima spajaju, tj. sabiraju na određeni način tako da se na tabli pojavljuju samo brojevi 2,4,8,16...
Nakon pomeranja, na tablu se dodaje jos jedan broj(2 ili 4) na random mesto. <br/>
Kako je funkcionalnost dodavanja broja na random poziciju proverena u testu inicijalizacije table, i jer bi otežala testiranje pomeranja i spajanja, bilo je potrebno refaktorisati kod tako da je moguće proveriti samo pomeranje i spajanje, nezavisno od dodavanja broja. Izmene su napravljene u [KeyPushManager.cpp](https://github.com/HadesD/2048/blob/a788215bdd9d775579da0901db383b2ed09a0985/src/KeyPushManager.cpp). Dodata je funkcija ```onKeyboardHitRefactored()```, i izmenjene su funkcije ```waitOnKey()``` i ```onKeyboardHit()```, tako da se funkcionalnost koda ne menja. U testovima je korišćena funkcija ```onKeyBoardHit()``` koja je promenjena tako da ne menja konačnu tablu, dok je to dadato u ```onKeyboardHitRefactored()```. Takodje, dodat je metod za dohvatanje izmenjene table pre dodavanja random broja ```getGameBoard()``` (videti promene u [custom.patch](custom.patch)).

Polja se mogu pomerati u četiri smera, pa su na taj način i testovi podeljeni. Testovi proveravaju ispravnost smera kretanja, Kako je već prethodnih testom proverena ispravnost unosa, ovde je radi jendostavnosti dodat metod za eksplicitno postavljanje slova koje označava smer ```setMKey()```. Pored provere ispravnosti smera, testovi proveravaju i razne slučajeve pomeranja i spajanja polja, kao što su postojanje praznih redova i kolona, slučajeva kada jeste ili nije moguće spojiti neka polja, kada postoje višestruka spajanja i kada nije moguće pomerati polja.

Na slici ispod je prikazan primer ispravnog pomeranja polja udesno
![image](https://github.com/user-attachments/assets/77db4cfb-a3dc-45f0-8e3c-2516f3eeb850) <br/>
Prolaznost testova za svaki od smerova i svaki od opisanih slučajeva je 100%.

### 1.5. Poeni
Poeni se dobijaju kada se dva polja spoje. Ako su spojena dva polja sa vrednošću *n*, na ukupan rezultat se dodaje *n*. Testirani su razni slučajevi, uključujući početni slučaj(kada je broj poena 0), kada ima ili nema spajanja, kada ima višestrukih spajanja... <br/>
Prolaznost testova je 100%.

### 1.6. Kraj igre
Potrebno je još proveriti da li se igra ispravno završava. Kraj igre treba da nastupi kada je tabla popunjena i nije moguće napraviti pokret ni u jednom smeru, inače se može igrati beskonačno. Testirane su situacije kada igra i treba i ne treba da se zavrsi(kada tabla nije popunjena ili postoje mogući pokreti). <br/>
Prolaznost testova je 100%.

## 2. Praćenje pokrivenosti koda pomoću alata Lcov
Pokrivenost koda (code coverage) predstavlja procenat ukupnog izvornog koda programa koji se izvršava kada se pokrene određeni skup testova, tj. to je broj elemenata programa ispitanih testovima u odnosu na ukupan broj tih elemenata. *Lcov* je grafički alat za analizu pokrivenosti koda koji koristi podatke iz *gcov*, alata za testiranje pokrivenosti kod GCC-a. Prikuplja i obrađuje podatke iz testova, pružajući izveštaje o pokrivenosti koda kao što su pokrivenost linija i funkcija. <br/>
Potrebno je prevesti program i izvršiti testove koristeći sledeće opcije: ```-profile-arcs``` i ```-ftest-coverage```
Tim putem se kreiraju ```.gcno``` i ```.gcda``` fajlovi koji su neophodni za funkcionisanje ovog alata.

Izvršavanjem komande 
```bash
lcov --capture --directory . --output-file coverage.info
```
(komanda se pokreće iz direktorijuma [lcov](lcov) u kome se nalaze potrebni ```.gcno``` i ```.gcda``` fajlovi) kreira se izveštaj o pokrivenosti koda [coverage.info](lcov/coverage.info). 

Nakon toga je moguće videti sažet izveštaj pomoću komande
```bash
lcov --list coverage.info
```

Takođe, moguće je kreirati i html stranicu koja sadrži izvorni kod programa označen tako da vizualizuje njegovu pokrivenost:
```bash
genhtml coverage.info --output-direcotory out
```
Nakon toga je samo potrebno otvoriti ```.html``` fajl, koji se nalazi na putanji [out/index.html](lcov/out/index.html).

Za projekat 2048 testiran jediničnim testovima prikazanim u pretnodnom odeljku, ukupan procenat pokrivenosti koda testovima je 94.7% (231/244) za linije, i 96.7% (29/30) za funkcije. 

![image](https://github.com/user-attachments/assets/dbb9adb5-c137-4a8d-9ef9-9dd7cf505fb1)

Funkcija koja nije pokrivena testovima je funkcija ```kbhit()``` iz klase Kbhit. Razlog tome je što se ova funkcija ne poziva nigde u kodu, tako da nije potrebna. Ovo povlači neiskorišćenost još jednog dela programa, a to je *if* grananje u funkciji ```getch()``` iz iste klase: <br/>
```if(peek_character != -1){...}```. Program nikad ne ulazi u ovu granu jer ```peek_character``` može biti ```!= -1``` jedino ako je prethodno postavljen na ```ch``` unutar funkcije ```kbhit()```. Pokrivenost klase ```Kbhit``` i njenih funkcija je prikazan na sledećoj slici, koja predstavlja deo prethodno pomenutog ```.html``` izveštaja. 
![Screenshot from 2024-08-21 15-03-23](https://github.com/user-attachments/assets/fce91393-25f0-4d7a-9a59-15df45af01a8)

## 3. Profajliranje pomoću alata Valgrind
Profajliranje predstavlja vid dinamičke analize koda čĳi je rezultat skup podataka dobĳen izvršavanjem programa sa određenim ulaznim podacima. Primer platforme koja omogućava naprednu analizu je *Valgrind*, koji je besplatan i otvorenog koda. Valgrind distribucĳa sadrži razne alate, kao što su *Massif* i *Callgrind*, koji su korišćeni u analizi ovog projekta. Njihova upotreba i doneti zaključci su prikazani u narednom odeljku. 

### 3.1. Massif
*Massif* je profajer hip memorije. Pomaže u analiziranju korišćenja memorije tokom vremena. Pored informacija o tome koliko hip memorije se koristi, Massif daje i detaljne informacĳe o tome koji deo programa je odgovoran za alociranje te memorĳe. Pokreće se sledećom komandom:
```bash
vagrind --tool=massif --massif-out-file=massif.out ./2048
```
```ms_print``` pravi čitljivi izveštaj, koji sadrži graf potrošnje memorĳe, kao i detaljne informacĳe o različitim tačkama programa koje su odgovorne o alokacĳi memorĳe:
```bash
ms_print massif.out
```
Tokom analize ovog projekta, Massif je pokrenut nekoliko puta, nakon čega su dobijene konzistentni rezultati. 

Prethodno pomenuti graf prikazuje raspodelu korišćenja memorije na hipu tokom vremena. x-osa predstavlja vreme izraženo u broju instrukcija(Mi), a y-osa ukupnu količinu memorije na hipu zauzetu u određenom trenutku(KB). 

![Screenshot from 2024-08-21 20-51-541](https://github.com/user-attachments/assets/172b19bc-35f7-46e8-84f2-4bdd012252fa)

Trenuci koji su posebno obeleženi predstavljaju preseke stanja za koje je dat detaljniji izveštaj. Graf nam govori da je tokom rada programa zauzeće memorije uglavnom ravnomerno, bez prevelikih odstupanja. *Peak* je trenutak u kome je zabeležena najviša potrošnja memorije. Upoređivanjem preseka rezultata može se primetiti da ovaj broj ostaje isti - ```74664B``` i pojavljuje se u okvirno istom trenutku, pri početku rada programa(u drugom, trećem preseku), oko ```2.56Mi```. U nastavku izveštaja su prikazani podaci o potrošnji memorije za svaki presek, gde je u procentima prikazano korišćenje memorije od strane određenih funkcija. Najviše memorije(oko 97%) se koristi za inicijalizovanje biblioteka(```dl-init```), dok mali procenat od oko 1,3% alocira metod klase ```Game``` za crtanje table ```drawGameBoard()```, koji poziva funkcije za rad sa izlaznim tokom podataka, kao što je ```fwrite```. Na sledećoj slici je prikazan jedan deo ovog izveštaja.

![Screenshot from 2024-08-21 20-51-542](https://github.com/user-attachments/assets/ea8f05b7-4d8e-43e1-8080-d23cb6b4c597)

Detaljnije rezultate, kao i rezultate iz više različitih pokretanja ovog alata je moguće videti u direktorijumu [massif](valgrind/massif). <br/>
Za reprodukciju rezultata, potrebno je pokrenuti skript [run_massif.sh](valgrind/massif/run_massif.sh)

### 3.2. Callgrind
*Callgrind* je alat koji generiše listu poziva funkcija korisničkog programa u vidu grafa. U osnovnim podešavanjima sakupljeni podaci sastoje se od broja izvršenih instrukcija, njihov odnos sa linijom u izvršnom kodu, odnos pozivaoc/pozvan između funkcija, kao i broj takvih poziva. Navedene informacije mogu biti korisne u procesu optimizacije programa jer je moguće uočiti delove koda koji se najviše izvršavaju i troše najviše resursa. 

Callgrind se pokreće sledećom komandom:
```bash
valgrind --tool=callgrind ./2048
```
Podaci se analiziraju i zapisuju u datoteku ```callgrind.out.<pid>```. Za detaljniji i čitljiviji izveštaj, moguće je koristiti alat *KCachegrind*, koji pored ostalog pruža i grafičku vizuelizaciju dobijenih podataka.

Na sledećoj slici možemo videti deo podataka iz alata KCachegrind. 
Sa leve strane se nalazi lista funkcija koja se može sortirati na različite načine tako dobivši različite informacije. Kolona ```incl.``` prikazuje procentualno ukupno vreme provedeno u funkciji i svim funkcijama koje ona poziva. U ovom primeru vidimo da program najviše vremena provodi u funkcijama ```drawGameBoard()``` i ```update()```, klase ```Game```, koje poziva funkcija ```main```. Ove funkcije su zadužene za crtanje table i njeno menjanje nakon svakog pokreta. 

![Screenshot from 2024-08-22 14-44-57](https://github.com/user-attachments/assets/21615c1e-1dc2-4e7a-93b3-a2773afdcc9b)

Sledeća kolona prikazuje vreme provedeno samo u određenoj funkciji, nezavisno od njenih poziva. Vidimo da najviše zasebnog vremena zauzima funkcija ```fwrite```, zadužena za izlazni tok podataka. Sa desne strane se nalazi lista funkcija koja poziva ovu funkciju. Vidimo da je među njima funkcija ```Game::drawGameBoard()```.

![Screenshot from 2024-08-22 15-14-17](https://github.com/user-attachments/assets/40d6d75c-a3a3-4c64-9bed-9e2ba9ec5247)

Još jedan način na koji se mogu sortirati dati podaci jeste po tome koliko je koja funkcija puta pozivana. Ovde vidimo da se najviše pozivaju funkcije iz standargne biblioteke za rad sa vektorima ```std::vector```, kao što su ```size()```, ```at()```, ```operator[]```. Ove funkcije su najčešće pozivane od strane funkcije ```Game::update()``` za ažuriranje table.

![Screenshot from 2024-08-22 15-21-30](https://github.com/user-attachments/assets/6b4d70ed-214c-4d24-8673-f46a04ac4341)

Dodatno, *Callgrind* može i da vrši analizu upotrebe keš memorĳe, korišćenjem dodatne opcije ```--cache-sim=yes```.
Dobijeni podaci pokazuju efikasnu upotrebu keša sa vrlo malim procentom promašaja. Nakon upoređivanja proseka rezultata nakon dužeg igranja, zaključak je da procenat promašaja na nivou **I1** ostaje približno 0.16%, na **D1** 0.1%, a na poslednjim nivoima **LLi** i **LLd** ostaje jako mali sa čak 0.01% na **LLi** i 0.1% na **LLd**. U situacijama kada se igra kraće, promašaji keša blago rastu, naviše na nivou **D1** gde dostižu i 1.1%. 

![Screenshot from 2024-08-22 15-39-22](https://github.com/user-attachments/assets/e9784b1c-91bb-4ef5-a78a-06726ca8e71b)

U direktorijumu [callgrind](valgrind/callgrind) se mogu naći izveštaji iz više instanci pokretanja alata, kao i grafici značajnih funkcija. Takođe, u fajlu [callgrind_cache_report.txt](valgrind/callgrind/callgrind_cache_report.txt) se nalazi više analiza upotrebe keša. <br/>
Za reprodukciju rezultata, potrebno je pokrenuti skript [run_callgrind.sh](valgrind/massif/run_massif.sh), koji će i otvoriti izveštaj u alatu *KCachegrind*, ukoliko je instaliran.

## 4. Statička analiza pomoću alata Cppcheck
Statička analiza je analiza koda bez njegovog izvršavanja sa ciljem pronalaženja defekata u kodu. *Cppcheck* je alat koji se koristi za analizu C/C++ koda. On otkriva greške koje kompajleri često ne uspevaju da otkriju, kao što su problemi s pokazivačima, prekoračenja bafera, neinicijalizovane promenljive. Jednostavan je za korišćenje i nudi detaljne izveštaje o potencijalnim problemima, što ga čini korisnim alatom za poboljšanje kvaliteta i sigurnosti koda.

Tokom pokretanja alata korišćene su sledeće opcije: <br/>
```--enable=all``` - uključuje sve provere <br/>
```--inconclusive``` - uključuje neodređena upozorenja <br/>
```--suppress=missingInclude``` - zanemaruje sve greške koje se dobijaju iz zaglavlja <br/>
Alat je pokrenut komandom:
```bash
cppcheck --enable=all --inconclusive --supress=missingInclude --output-file=output.xml --xml ../2048
```
Izveštaj će biti smešten u ```.xml``` fajl, nakon čega je moguće generisati čitljiviji ```.html``` izveštaj:
```bash
cppcheck-htmlreport --file=output.xml --report-dir=report
```
Ovaj izvešaj takođe pokazuje gde tačno u kodu je došlo do greške. To se može videti klikom na fajl označen pre liste grešaka za isti.

Html izveštaj je prikazan na sledećoj slici:
![Screenshot from 2024-08-22 16-53-16](https://github.com/user-attachments/assets/47db75eb-e329-49dc-a4e3-550517bb3f1e) <br/>
Postoje tri neodređena upozorenja vezana za performanse i stil, koja govore da određene funkcije mogu biti označene kao **static** (```drawBarrier()``` i ```fillRandomPos()``` iz klase ```Game```), odnosno kao **const** (```update()```, funkcija klase ```Game```). Prikazano je i nekoliko upozorenja o neinicijalizovanim promenljivim iz klasa ```Game``` i ```KeyPushManager```. Postoji jedna funkcija koja je neiskorišćena - ```kbhit()``` iz klase ```Kbhit``` (o njoj je bilo reči i ranije kada se govorilo o pokrivenosti koda). Nađene su još neke greške vezana za stil i performanse programa. 

Prateći rezultate, kod je izmenjen tako da reši većinu navedenih problema. Jedini problem koji nije mogao biti rešen je neodređeno upozorenje da se funkcija ```Game::update()``` označi kao **const**, što govori o osobini opcije ```--inconclusive``` da nekada daje lažno negativna upozorenja. Napravljene promene u kodu se mogu videti u fajlu [changes.patch](cppcheck/changes.patch), a izveštaj nakon izmena u [report_improved/index.html](cppcheck/report_improved/index.html).

Za reprodukciju rezultata, potrebno je pokrenuti skript [run_cppcheck.sh](cppcheck/run_cppcheck.sh).

## Zaključak
Analizom ovog projekta dolazi se do sledećih zapažanja:
- program prolazi sve zadate jedinične testove, tako da su sve funkcionalnosti ovog projekta uspešno implementirane
- kod je skoro u potpunosti pokriven testovima, tako da nema neispitanih delova programa
- program ravnomerno koristi memoriju, nema curenja, kao ni prekomernih alokacija
- analizom broja instrukcija utvrđeno je da većinu vremena troše pozivi funkcija za rad sa vektorima, pa se optimizacija može vršiti kod funkcija klase ```Game```: ```update()``` i ```drawGameBoard()```, promašaji keša su minimalni
- statička analiza programa je pokazala nekoliko propusta, kao što su postojanje neinicijalizovanih promenljivih i neupotrebljene funkcije, ali nije bilo kritičnih grešaka

Konačni zaključak je da je projekat kvalitetan, sa dobro implementiranim funkcionalnostima i vrlo dobrim performansama.
 
