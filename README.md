# Analiza projekta '2048'

**2048** je implementacija istoimene igre u C++. Pokreće se u terminalu. Na početku igre je prikazana tabla veličine 4x5, na kojoj su dva polja popunjena brojevima, a ostala prazna. Jedini brojevi koji se mogu naći na tabli su stepeni dvojke (2, 4, 8, 16,...). Igra se tako što se polja pomeraju levo, desno, gore, dole, na taj način da se svako polje sa table mora pomeriti u zadatom smeru. Smer određuje korisnik unosom sa tastature. Kada se dva polja sa istom vrednošću dodirnu, spoje se u jedno polje sa duplo većom vrednošću. Nakon svakog pomeranja, uvodi se novi broj. Tako se u svakom koraku broj popunjenih polja povećava dodavanjem novih brojeva, a može se smanjiti samo spajanjem polja. Spajanjem polja se dobijaju poeni i cilj igre je imati što više poena. Igra se završava kada je tabla popunjena i nije moguće pomeriti polja ni u jednom smeru.

Izvorni kod projekta - **master** grana: [2048](https://github.com/HadesD/2048) <br/>
Hash kod commit-a: ```a788215bdd9d775579da0901db383b2ed09a0985```

## Korišćeni alati
- Catch2 - jedinični testovi
- Lcov - pokrivenost koda jediničnim testovima
- Valgrind - profajliranje
  - Massif
  - Callgrind
- Cppcheck - statička analiza

Detaljan izveštaj o analizi projekta, kao i uputstva za pokretanje alata, mogu se naći u [ProjectAnalysisReport.md](ProjectAnalysisReport.md). <br/>
Takođe, skriptovi za pokretanje alata su dostupni u odgovarajućim folderima.

## Zaključak
Konačni zaključak je da je projekat kvalitetan, sa dobro implementiranim funkcionalnostima i vrlo dobrim performansama. <br/>
Detaljna zapažanja je moguće pročitati u [ProjectAnalysisReport.md](ProjectAnalysisReport.md), u odeljku **Zaključak**.

## Autor 
Nevena Mijailović 1067/23
