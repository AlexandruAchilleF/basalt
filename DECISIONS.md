# Decizii

## 1. Dimensiunea eșantionului (N = 20.000)

- **Decizie:** Am redus setul de date la un volum fix de 20.000 de rânduri.
- **Motiv:** Dimensiunea este suficientă pentru a acoperi multiple row group-uri și a testa scanarea pe blocuri fără a depăși limita GitHub de 100 MB și fără a bloca execuția testelor cu Valgrind în CI.
- **Alternative respinse:** Păstrarea tuturor rândurilor valide într-un CSV generat din Parquet-ul de 67 MB (acest CSV ar fi avut 157 MB, depășind limita de 100 MB din GitHub și încetinind masiv rularea în CI sub Valgrind) sau selectarea a prea puține rânduri (720 de rânduri ar fi încăput într-un singur row group, împiedicând testarea trecerii între blocuri).

## 2. Metoda de eșantionare (Reservoir Sampling cu seed 42)

- **Decizie:** Am extras cele 20.000 de înregistrări prin reservoir sampling cu seed-ul `42` (`USING SAMPLE 20000 ROWS (reservoir, 42)`).
- **Motiv:** Metoda garantează un eșantion distribuit uniform pe toată luna septembrie și produce un set de date perfect reproductibil.
- **Alternative respinse:** Păstrarea primelor 20.000 de rânduri (`LIMIT 20000`), care ar fi introdus o dispersie temporală nerealistă, acoperind doar cursele din data de 1 septembrie între orele 00:00 și 08:00.

## 3. Sortarea după data de preluare (`tpep_pickup_datetime`)

- **Decizie:** Am sortat rândurile cronologic crescător după `tpep_pickup_datetime` la generarea fișierului CSV.
- **Motiv:** Sortarea menține intervalele min/max (zone maps) ale fiecărui row group compacte și disjuncte, permițând mecanismului de row group skipping să sară peste blocuri întregi la filtrările temporale.
- **Alternative respinse:** Păstrarea ordinii neordonate din eșantionare, ceea ce ar fi suprapus intervalele de timp din toate blocurile și ar fi forțat motorul să scaneze fiecare row group.

## 4. Filtrarea pe intervalul lunii septembrie 2026

- **Decizie:** Am păstrat strict înregistrările cu `tpep_pickup_datetime >= '2026-09-01'` și `< '2026-10-01'`.
- **Motiv:** În fișierele brute există înregistrări cu ceasuri decalate (de exemplu curse raportate în 2009); un singur pickup din 2009 ar strica valoarea minimă din zone map pentru întregul row group, compromițând row group skipping-ul.
- **Alternative respinse:** Păstrarea tuturor rândurilor fără filtrare pe interval de date.

## 5. Filtrarea valorilor non-pozitive (`trip_distance > 0` și `fare_amount > 0`)

- **Decizie:** Am eliminat înregistrările cu `trip_distance <= 0` sau `fare_amount <= 0`.
- **Motiv:** Valorile negative de `fare_amount` reflectă stornări contabile sau curse disputate, iar `trip_distance` egal cu 0 indică curse anulate pe loc sau erori de semnal GPS ale aparatului de taxat.
- **Alternative respinse:** Păstrarea acestor înregistrări, ceea ce ar fi denaturat calculele de distanță și tarif mediu.

## 6. Păstrarea metodelor de plată Card și Cash (`payment_type IN (1, 2)`)

- **Decizie:** Am filtrat datele pentru a reține exclusiv tipurile de plată 1 (card) și 2 (cash).
- **Motiv:** Păstrarea a două categorii permite verificarea filtrelor și a agregărilor `GROUP BY` pe o coloană cu cardinalitate redusă, chiar dacă setul TLC nu înregistrează bacșișul plătit în numerar (valoarea fiind mereu 0).
- **Alternative respinse:** Includerea codului 0 (înregistrări incomplete sau „flex fare” cărora le lipsesc `passenger_count` și `RatecodeID`), a codurilor 3 (no charge) și 4 (dispute), ori reținerea exclusivă a plăților cu cardul.

## 7. Reducerea la 7 coloane (eliminarea `total_amount` și `DOLocationID`)

- **Decizie:** Am păstrat doar 7 coloane de bază, excluzând câmpurile `total_amount` și `DOLocationID`.
- **Motiv:** Nicio interogare din cadrul proiectului nu folosește `total_amount`, iar agregarea `GROUP BY` pe perechi de locații (pickup-dropoff) este planificată pentru o etapă viitoare.
- **Alternative respinse:** Păstrarea tuturor coloanelor din fișierul original, ceea ce ar fi ocupat inutil spațiu pe disc și memorie cu date nefolosite.
