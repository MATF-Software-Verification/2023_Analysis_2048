# Izveštaj o analizi projekta '2048'

Ovde će biti prikazan detaljan izveštaj o analizi konzolne C++ igre 2048. Projekat je testiran pomoću alata za verifikaciju softvera: 
- [catch2](##1.%Testiranje%jedinica%koda%pomoću%alata%Catch2) - testiranje jedinica koda
- [lcov](##2.%Praćenje%pokrivenosti%koda%pomoću%alata%Lcov) - praćenje pokrivenosti koda jediničnim testovima
- [valgrind](##3.Profajliranje%pomoću%alata%Valgrind) - profajliranje
  - [massif](##3.1.%Profajliranje%pomoću%alata%Massif),
  - [callgrind](##3.2.%Profajliranje%pomoću%alata%Callgrind),
- [cppcheck](##4-statička-analiza-pomoću-alata-cppcheck) - statička analiza

## 1. Testiranje jedinica koda pomoću alata Catch2
Testiranje jedinica koda (unit testing) je tehnika kojom se proverava funkcionisanje delova sistema koji se mogu nezavisno testirati. *Catch2* je framework za testiranje u C++ koji je jednostavan za korišćenje i veoma moćan. Omogućava pisanje testova u čistom C++ jeziku, koristeći čitljivu sintaksu. Testovi se pišu kao *.cpp* datoteke koristeći zaglavlje [catch.hpp](unit_tests/tests/catch.hpp). <br/>
Testovi su fokusirani na proveru ispravnosti osnovnih funkcionalnosti igre: inicijalizovanje table, crtanje table, unos sa tastature, pomeranje brojeva, spajanje brojeva, računanje poena i kraj igre. 

Za reprodukovanje testova potrebno je pokrenuti python script [run_tests.py](unit_tests/run_tests.py). <br/>
Za potrebe testiranja programa, neki delovi koda su refaktorisani. Napravljene izmene se mogu videti u fajlu [custom.patch](custom.patch)

### 1.1. Inicijalizovanje table
Na početku igre, tabla se inicijalizuje tako da su popunjena samo dva polja(vrednostima 2 ili 4), koja se mogu naći na random pozicijama. Ostala polja moraju biti prazna(tj. imati vrednost 0). Ovo je testirano oslanjajući se na činjenicu da je tabla nepromenljive veličine, pa ima određen broj polja n. Potrebno je da tabla na n-2 polja ima 0, a na samo 2 polja vrednosti 2 ili 4.       
![Screenshot from 2024-08-19 16-55-02](https://github.com/user-attachments/assets/dc02c0a5-8f92-4414-b858-f97b5f2dd7df) 

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
Kako je funkcionalnost dodavanja broja na random poziciju proverena u testu inicijalizacije table, i jer bi otežala testiranje pomeranja i spajanja, bilo je potrebno refaktorisati kod tako da je moguće proveriti samo pomeranje i spajanje, nezavisno od dodavanja broja. Izmene su napravljene u [KeyPushManager.cpp](https://github.com/HadesD/2048/blob/a788215bdd9d775579da0901db383b2ed09a0985/src/KeyPushManager.cpp). Dodata je funkcija ```onKeyboardHitRefactored()```, i izmenjene su funkcije ```waitOnKey()``` i ```onKeyboardHit()```, tako da se funkcionalnost koda ne menja. U testovima je korišćena funkcija ```onKeyBoardHit()``` koja je promenjena tako da ne menja konačnu tablu, dok je to dadato u ```onKeyboardHitRefactored()```. Takodje, dodat je metod za dohvatanje izmenjene table pre dodavanja random broja ```getGameBoard()```.

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

## 2.	Praćenje pokrivenosti koda pomoću alata Lcov
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

Za projekat 2048 testiran jediničnim testovima prikazanim u pretnodnom odeljku, ukupan procenat pokrivenosti koda testovima je ```94.7%```(231/244) za linije, i ```96.7%```(29/30) za funkcije. 
![image](https://github.com/user-attachments/assets/dbb9adb5-c137-4a8d-9ef9-9dd7cf505fb1) <br/>
Funkcija koja nije pokrivena testovima je funkcija ```kbhit()``` iz klase Kbhit. Razlog tome je što se ova funkcija ne poziva nigde u kodu, tako da nije potrebna. Ovo povlači neiskorišćenost još jednog dela programa, a to je if grananje u funkciji ```getch()``` iz iste klase: ```if(peek_character != -1){...}```. Program nikad ne ulazi u ovu granu jer ```peek_character``` može biti ```!= -1``` jedino ako je prethodno postavljen na ch unutar funkcije ```kbhit()```. Pokrivenost klase ```Kbhit``` i njenih funkcija je prikazan na sledećoj slici, koja predstavlja deo prethodno pomenutog ```html``` izveštaja. 
![Screenshot from 2024-08-21 15-03-23](https://github.com/user-attachments/assets/fce91393-25f0-4d7a-9a59-15df45af01a8)

## 3. Profajliranje pomoću alata Valgrind
Profajliranje predstavlja vid dinamičke analize koda čĳi je rezultat skup podataka dobĳen izvršavanjem programa sa određenim ulaznim podacima. Primer platforme koja omogućava naprednu analizu je *Valgrind*, koji je besplatan i otvorenog koda. Valgrind distribucĳa sadrži razne alate, kao što su *Massif* i *Callgrind*, koji su korišćeni u analizi ovog projekta. Njihova upotreba i doneti zaključci su prikazani u narednom odeljku. 

### 3.1.	Massif
*Massif* je profajer hip memorije. Pomaže u analiziranju korišćenja memorije tokom vremena. Pored informacija o tome koliko hip memorije se koristi, Massif daje i detaljne informacĳe o tome koji deo programa je odgovoran za alociranje te memorĳe. Pokreće se sledećom komandom:
```bash
vagrind --tool=massif --massif-out-file=massif.out ./2048
```
```ms_print``` pravi čitljivi izveštaj, koji sadrži graf potrošnje memorĳe, kao i detaljne informacĳe o različitim tačkama programa koje su odgovorne o alokacĳi memorĳe:
```bash
ms_print massif.out
```
Tokom analize ovog projekta, *Massif* je pokrenut nekoliko puta, nakon čega su dobijene konzistentni rezultati. 

Prethodno pomenuti graf prikazuje raspodelu korišćenja memorije na hipu tokom vremena. x-osa predstavlja vreme izraženo u broju instrukcija(Mi), a y-osa ukupnu količinu memorije na hipu zauzetu u određenom trenutku(KB). 

![Screenshot from 2024-08-21 20-51-541](https://github.com/user-attachments/assets/172b19bc-35f7-46e8-84f2-4bdd012252fa)

Trenuci koji su posebno obeleženi predstavljaju preseke stanja za koje je dat detaljniji izveštaj. Graf nam govori da je tokom rada programa zauzeće memorije uglavnom ravnomerno, bez prevelikih odstupanja. Peak je trenutak u kome je zabeležena najviša potrošnja memorije. Upoređivanjem preseka rezultata može se primetiti da ovaj broj ostaje isti - ```74664B``` i pojavljuje se u okvirno istom trenutku, pri početku rada programa(u drugom, trećem preseku), oko ```2.56Mi```. U nastavku izveštaja su prikazani podaci o potrošnji memorije za svaki presek, gde je u procentima prikazano korišćenje memorije od strane određenih funkcija. Najviše memorije(oko 97%) se koristi za inicijalizovanje biblioteka(```dl-init```), dok mali procenat od oko 1,3% alocira metod klase ```Game``` za crtanje table ```drawGameBoard()```, koji poziva funkcije za rad sa izlaznim tokom podataka, kao što je ```fwrite```. Na sledećoj slici je prikazan jedan deo ovog izveštaja.

![Screenshot from 2024-08-21 20-51-542](https://github.com/user-attachments/assets/ea8f05b7-4d8e-43e1-8080-d23cb6b4c597)

Detaljnije rezultate, kao i rezultate iz više različitih pokretanja ovog alata je moguće videti u direktorijumu [massif](valgrind/massif). <br/>
Za reprodukciju rezultata, potrebno je pokrenuti skript [run_massif.sh](valgrind/massif/run_massif.sh)

### 3.2. Callgrind
## 4.	Statička analiza pomoću alata Cppcheck
## Zaključak
Analiza ovog projekta dovodi do sledećih zaključaka: 
- program prolazi sve zadate jedinične testove, tako da su sve funkcionalnosti ovog projekta uspešno implementirane,
- kod je skoro u potpunosti pokriven testovima, tako da nema neispitanih delova programa,
- program ravnomerno koristi memoriju, nema curenja, kao ni prekomernih alokacija

  

