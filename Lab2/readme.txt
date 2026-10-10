Hitta dubbletter
================

- Ungefärligt antal timmar spenderade på de två delarna av labben (valfritt):

del a:4
del b:6

- Vad är tidskomplexiteten på följande operationer i din hashtabell?

insert_key: o(1)
  find_key: o(1)
 erase_key: o(1)


- Antag att vi vill sätta in många element i en Hash_Map<int, int>. Vi sätter
  därmed kapaciteten till 1000. Sedan sätter vi in element med nycklarna
  1000, 2000, 3000, 4000, och så vidare tills vi har satt in 500 element.
  Kommer hashtabellen fungera bra i det här fallet? Varför/varför inte?

Det kommer inte vara optimalt efterom nycklarna kommer hasha till samma bucket, det kommer i princip leda till (O(n)) i tidskomplexiteten, förutsatt att modulu används som hasfunktion (samma hashfunktion som del a av labben). 
En annan hasfunktion kan ledea till ett bättre resultat.

- När testprogrammet jämför Hash_Map med std::unordered_map (alternativ 9) så
  använder vi int som både nyckel och värde. Typen int är lite speciell i
  avseendet att både hashning och jämförelse av int är mycket billiga. Titta
  på resultaten av jämförelsen, med fokus på "max chain". Vad tror du skulle
  hända med exempelvis tiden för uppslagning om vi i stället använder strängar
  som nyckel? Anta att strängarna är långa (ca 1000 tecken vardera) och att
  de första 500 tecknen i alla strängar är desamma.

Det kommer öka tiden eftersom en std::string i princip är en vector med karaktärer och att jämföra 1000 karaktärer tar tid även om uppslagning på vector är o(1).

- Vad är tidskomplexiteten på "slow.cpp" och din implementation av "fast.cpp",
  uttryckt i antalet bilder (n).

slow: o(n²)
fast: o(n)


- Hur lång tid tar det att köra "slow.cpp" respektive "fast.cpp" på de olika
  datamängderna?
  Tips: Använd flaggan "--nowindow" för enklare tidsmätning.
  Tips: Det är okej att uppskatta tidsåtgången för de fall du inte orkar vänta
  på att slow blir klar med det största testfallet. Om du gör detta är det en
  bra idé att beräkna tiden det tar att läsa in och skala ner bilderna separat
  från tiden det tar att jämföra bilderna. (Varför?) Du kan anta att inläsning
  är det dyra, och att inläsning tar lika lång tid i både "slow" och "fast"

|--------+-----------+--------+--------|
|        | inläsning |  slow  |  fast  |
|--------+-----------+--------+--------|
| tiny   |    7      |  143   |  80    |
| small  |    75     |  465   |  516   |
| medium |    360    |  2221  |  1395  |
| large  |    8189   | 397171 |  99491 |
|--------+-----------+--------+--------|


- Testa olika värden på "summary_size" (exempelvis mellan 6 och 10). Hur
  påverkar detta vilka dubbletter som hittas i datamängden "large"?

Det påverkar hur stor känslig testet är mot bilders ljustryrka desto större värde

- Algoritmen som implementeras i "compute_summary" kan ses som att vi beräknar
  en hash av en bild. Det är dock inte helt lätt att hitta en bra sådan funktion
  som helt motsvarar vad vi egentligen är ute efter. Vilken eller vilka
  egenskaper behöver "compute_summary" ha för att vi ska kunna använda den för
  att hitta bilder som liknar varandra? (Dvs. vilka egenskaper förväntar sig
  kod som *använder* "compute_summary"?) Tycker du att implementationen av
  "compute_summary" som är given i labbhandledningen uppfyller dessa egenskaper?

Det viktiga är att outputet från compute_summary är samma för alla bilder som ser likadna ut, alltså måste compute_summary alltid ge samma output för likadnat input. 
För att alla utdata som compute_summary ger ska gå att jämföras, måste alla bilder i olika format som exempelvis olika uplösningar ge utdata is samma format efter de körts genom compute_summary.
Sedan är det bra att ha en variabel som kan justera hur stor toleransen är för skillander i bilder vilket vi upplevde att summary_size gjorde gnaksa bra.

- Ser du några problem med metoden för att se om två bilder är lika dana?
  Fundera exempelvis på vilka typer av olikheter som tolereras, och vilka
  typer av olikheter som anses vara för stora. Matchar detta din uppfattning
  om vad som borde vara lika?

Det kan vara lite problem med färgere eftersom testet bara tar hänsyn till ljustryrka samt att nedskalningen av bilden gör den mindre känslig för skillnader i ljustryka
