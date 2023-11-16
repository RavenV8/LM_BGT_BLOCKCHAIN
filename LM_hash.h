string Hashing(string code){
    vector<char> codeHashed;
    for (int i = 0; i < 64; i++) codeHashed.push_back('0');
    char simbolis; // naudojama laikyti reiksmei kuri bus priskirta "codeHashed".
    long int simbolioASCII;
    long int numeriukasRaides, numeriukasLokacijos;
    long int ilgioMaisymas = 1;
    long int praeitasSimbolis = 1;
    long int simboliuSuma = 1;
    vector<char> codeToVector(code.begin(), code.end());

    for (int i = 0; i < code.size(); i++){
        simbolioASCII = (int)codeToVector[i];
        if(simbolioASCII > 91) ilgioMaisymas += 3;
        simboliuSuma += (int)codeToVector[i];
    }

    for (int i = 0; i < code.size(); i++){
        simbolioASCII = (int)codeToVector[i]; // randa kiekvieno "code" simbolio ASCII.
        if(simbolioASCII < 0) simbolioASCII *= -1; // paprasciausias sutvarkymas del non ASCII characters.
        // suskaiciuojama kokia reiksme bus priskirta pagal hex koduote. 
        // '*(i+1)' skirta, jog tie patys simboliai neuzimtu tu paciu vietu.
        // saknies liekana is 16, nes sudaryta is hex.
        numeriukasRaides = ((simbolioASCII*(i+1)+ilgioMaisymas + praeitasSimbolis)+ simboliuSuma) % 16;
        
        // naudojama paversti 10-15 skaicius i a-f raides.
        // 0-9 skaiciai paverciami i string tipo.
        if (numeriukasRaides >= 10 && numeriukasRaides <= 15) {
        simbolis = 'a' + (numeriukasRaides - 10);
        } else {simbolis = '0' + numeriukasRaides;}
       
        // panasus principas kaip simbolio rinkime.
        // tik is saknies liekanos is 16 pakeiciama i 64.
        numeriukasLokacijos = ((simbolioASCII*(i+1)+ilgioMaisymas + praeitasSimbolis)* simboliuSuma * 5) % 64;
        codeHashed[numeriukasLokacijos] = simbolis; // priskiriamas simbolis i jam priklausancia vieta.

        // papildomai dar vienas hasavimas jog daugiau simboliu butu pakeista.
        praeitasSimbolis = numeriukasRaides;
        numeriukasRaides = ((ilgioMaisymas + 5) * (simboliuSuma % 5) + praeitasSimbolis) % 16;
        numeriukasLokacijos = ((ilgioMaisymas + 7) * (simboliuSuma * 3 % 3) + praeitasSimbolis) % 64; 

        if (numeriukasRaides >= 10 && numeriukasRaides <= 15) {
        simbolis = 'a' + (numeriukasRaides - 10);
        } else {simbolis = '0' + numeriukasRaides;}
        codeHashed[numeriukasLokacijos] = simbolis;

        // papildomai dar vienas hasavimas jog daugiau simboliu butu pakeista.
        praeitasSimbolis = numeriukasRaides;
        numeriukasRaides = (i * 3 + simboliuSuma + ilgioMaisymas * praeitasSimbolis) % 16;
        numeriukasLokacijos = (i * 8 + simboliuSuma + ilgioMaisymas * praeitasSimbolis) % 64; 
        if (numeriukasRaides >= 10 && numeriukasRaides <= 15) {
        simbolis = 'a' + (numeriukasRaides - 10);
        } else {simbolis = '0' + numeriukasRaides;}
        codeHashed[numeriukasLokacijos] = simbolis;

        praeitasSimbolis = simbolioASCII; // naudojamas hashavimui
    }
    string result = "";
    for (int i = 0; i < codeHashed.size(); i++){
        result += codeHashed[i];     
    }
    return result;
}