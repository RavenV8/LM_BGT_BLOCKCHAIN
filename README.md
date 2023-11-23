# Blokų grandinių 2 užduotis
Centralizuota blokų grandinė.
## Įdiegimo ir naudojimosi instrukcija
- Atsidaryti projektą VS code aplinkoje
- Konsolėje (CMD) parašyti komandą g++ -O2 LM_BLOCKCHAIN.cpp -o v01 && ./v01
## Kaip veikia programa?
- Sugeneruojama 1000 vartotojų
- Sugeneruojama 10000 transakcijų
- Išrenkama random 100 transakcijų bei sukuriamas merkle tree
- Vyksta bloko kasimas, kol gaunamas reikalavimus atitinkantis hash'as
- Naujame bloke esančios transakcijos įvykdomos
- Eliminuojamos įvykdytos transakcijos
- Naujai iškastas blokas pridedamas prie pagrindinės grandinės (blockchain)
