#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struktura przechowująca nasz tag ID3v1
struct tagID3
{
    char tytul[30];
    char artysta[30];
    char album[30];
    char rok[4];
    char komentarz[30];
    unsigned char gatunek;
};

// lista gatunków muzycznych dla ID3v1
char *gatunek[] = {"Blues", "Classic_Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip_Hop", "Jazz", "Metal", "New_Age", "Oldies", "Other", "Pop", "RnB", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska", "Death_Metal", "Pranks", "Soundtrack", "Euro_Techno", "Ambient", "Trip_Hop", "Vocal", "Jazz_Funk", "Fusion", "Trance", "Classical", "Instrumental", "Acid", "House", "Game", "Sound_Clip", "Gospel", "Noise", "AlternRock", "Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental_Pop", "Instrumental_Rock", "Ethnic", "Gothic", "Darkwave", "Techno_Industrial", "Electronic", "Pop_Folk", "Eurodance", "Dream", "Southern_Rock", "Comedy", "Cult", "Gangsta", "Top_40", "Christian_Rap", "Pop_Funk", "Jungle", "Native_American", "Cabaret", "New_Wave", "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo_Fi", "Tribal", "Acid_Punk", "Acid_Jazz", "Polka", "Retro", "Musical", "Rock_n_Roll", "Hard_Rock", "Folk", "Folk_Rock", "National_Folk", "Swing", "Fast_Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic_Rock", "Progressive_Rock", "Psychedelic_Rock", "Symphonic_Rock", "Slow_Rock", "Big_Band", "Chorus", "Easy_Listening", "Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber_Music", "Sonata", "Symphony", "Booty_Bass", "Primus", "Porn_Groove", "Satire", "Slow_Jam", "Club", "Tango", "Samba", "Folklore", "Ballad", "Power_Ballad", "Rhythmic_Soul", "Freestyle", "Duet", "Punk_Rock", "Drum_Solo", "Acapella", "Euro_House", "Dance_Hall"};

//funkcja odczytująca tag ID3v1 z pliku
int odczytajTag(char *p)
{
    char c, c1, c2, c3;
    FILE *plik;

    plik = fopen(p, "rb");
    if (plik == NULL)
    {
        perror("Nie udało się otworzyć pliku!");
        return 0;
    }

    //w standardzie ID3v1 TAG znajduje się 128 znaków od końca
    //przejście w to miejsce
    // fseek(plik, -128, 2);
    fseek(plik, -128L, SEEK_END);

    //pobieranie pierwszych 3 znaki poczynając od 128 znaku od końca pliku
    c1 = fgetc(plik);
    c2 = fgetc(plik);
    c3 = fgetc(plik);
    //sprawdzenie czy te znaki to TAG (a raczej czy nie są różne od TAG)
    if (c1 != 'T' || c2 != 'A' || c3 != 'G')
    {
        printf("Plik nie ma tagu ID3v1!\n");
        return 0; //zwrócone 0, czyli nie znaleziono TAG w pliku
    }

    //----------------------wypisanie tagu ID3v1-------------------
    //wypisanie tytułu
    printf("Tytuł: ");
    for (int i = 30; i > 0; i--)
        printf("%c", fgetc(plik));
    printf("\n");

    //wypisanie artysty
    printf("Artysta: ");
    for (int i = 30; i > 0; i--)
        printf("%c", fgetc(plik));
    printf("\n");

    //wypisanie albumu
    printf("Album: ");
    for (int i = 30; i > 0; i--)
        printf("%c", fgetc(plik));
    printf("\n");

    //wypisanie roku
    printf("Rok: ");
    for (int i = 4; i > 0; i--)
        printf("%c", fgetc(plik));
    printf("\n\n");

    //wypisanie komentarza
    printf("Komentarz: ");
    for (int i = 30; i > 0; i--)
        printf("%c", fgetc(plik));
    printf("\n");

    //wypisanie gatunku
    printf("Gatunek: ");
    printf("%s", gatunek[fgetc(plik)]);
    printf("\n");

    fclose(plik);
    return 1; //udało się znaleźć TAG
}

//funkcja umożliwiająca ręczne wpisanie tagów ID3v1
void dodajTagRecznie(char *p)
{
    struct tagID3 nowyTag;

    // wypełnienie nowej struktury binarnymi zerami
    for (int i = 0; i < 30; i++)
    {
        nowyTag.tytul[i] = '\0';
        nowyTag.artysta[i] = '\0';
        nowyTag.album[i] = '\0';
        nowyTag.komentarz[i] = '\0';
    }
    for (int i = 0; i < 4; i++)
        nowyTag.rok[i] = '\0';
    nowyTag.gatunek = '\0';

    //-------------pobieranie danych do tagu od użytkownika-------------///
    printf("Podaj tytuł (do 30 znaków): ");
    fgets(nowyTag.tytul, 30, stdin);

    printf("Podaj artystę (do 30 znaków): ");
    fgets(nowyTag.artysta, 30, stdin);

    printf("Podaj album (do 30 znaków): ");
    fgets(nowyTag.album, 30, stdin);

    printf("Podaj rok (4 znaki): ");
    fgets(nowyTag.rok, 5, stdin);

    //czyszczenie stdin (było oporne na fflush)
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Podaj komentarz (do 30 znaków): ");
    fgets(nowyTag.komentarz, 30, stdin);

    printf("Podaj nr gatunku (1-126): ");
    scanf("%3d", &(nowyTag.gatunek));

    zapiszTagWpliku(p, nowyTag);
}

//fukcja zapisująca tag ID3v1 w pliku
void zapiszTagWpliku(char *p, struct tagID3 nowyTag)
{
    FILE *plik;
    plik = fopen(p, "r+b");
    if (plik == NULL)
    {
        perror("Nie udało się otworzyć pliku!");
        return 0;
    }

    //w standardzie ID3v1 TAG znajduje się 128 znaków od końca
    //przejście w to miejsce
    fseek(plik, -128L, SEEK_END);

    fwrite("TAG", sizeof(char) * 3, 1, plik);
    fwrite(nowyTag.tytul, sizeof(char) * 30, 1, plik);
    fwrite(nowyTag.artysta, sizeof(char) * 30, 1, plik);
    fwrite(nowyTag.album, sizeof(char) * 30, 1, plik);
    fwrite(nowyTag.rok, sizeof(char) * 4, 1, plik);
    fwrite(nowyTag.komentarz, sizeof(char) * 30, 1, plik);
    fputc(nowyTag.gatunek, plik);

    fclose(plik);
}

//funkcja zmieniająca nazwę pliku na podstawie tagu ID3v1
void tagNaNazwe(char *p)
{
    //sprawdzenie czy w pliku jest tag ID3v1
    if (odczytajTag(p) == 1)
    {
        FILE *plik;
        plik = fopen(p, "rb");
        if (plik == NULL)
        {
            perror("Nie udało się otworzyć pliku!");
            return 0;
        }

        //w standardzie ID3v1 TAG znajduje się 128 znaków od końca
        //przejście w to miejsce
        // z pominięciem słowa TAG
        fseek(plik, -125L, SEEK_END);

        //-------------pobieranie danych z tagu pliku i tworzenie ciągu znaków NAZWA-------------///

        char nowaNazwaPliku[97];
        char c;
        int j = 0;

        for (int i = 0; i < 94; i++)
        {
            c = fgetc(plik);
            if (c > ' ' && c < '{')
            {
                nowaNazwaPliku[j] = c;
                j++;
            }
            if (i == 29 || i == 59 || i == 89)
            {
                nowaNazwaPliku[j] = '-';
                j++;
            }
        }

        strcat(nowaNazwaPliku, ".mp3");
        printf("Nowa nazwa pliku: %s\n", nowaNazwaPliku);

        if (rename(p, nowaNazwaPliku) == 0)
            printf("Udało się zmienić nazwę pliku!\n");
        else
            printf("Nie udało się zmienić nazwy pliku!\n");

        fclose(plik);
    }
    else
    {
        printf("Plik nie zawiera tagu ID3v1!");
    }
}

//funkcja ustawiająca tag ID3v1 pliku na podstawie nazwy pliku
void tagZnazwy()
{

}

int main(int argc, char const *argv[])
{
    /* code */
    odczytajTag(argv[1]);
    dodajTagRecznie(argv[1]);
    //tagNaNazwe(argv[1]);
    return 0;
}
