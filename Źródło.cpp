#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>

using namespace std;


    //generowanie napisu pocz¹tkowego
void wstep()
{
    //tworzenie dwóch tablic znaków, z przodu i z ty³u g³ównego napisu
    char tab_front[16] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O', '\0' };
    char tab_back[16] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O', '\0' };

    for (int i = 0; i < 16; i++)
    {
        tab_front[i] = int(tab_front[i] + 85);      //konwersja znaków na "znaki specjalne"
        tab_back[i] = int(tab_back[i] + 159);       //przy wykorzystaniu tablicy ASCII
    }


    //wypisanie tablicy znaków z przodu g³ównego napisu
    for (int i = 0; i < 16; i++)
    {
        cout << tab_front[i] << " ";
    }

    cout << "     SZYFR CEZARA     ";   //g³ówny napis wstêpu


    //wypisanie tablicy znaków z ty³u g³ównego napisu
    for (int i = 0; i < 16; i++)
    {
        cout << tab_back[i] << " ";
    }

    cout << "\n" << endl;
}

void read_from_file(string path)
{
 
    //otworzenie pliku do odczytu   

    ifstream file(path);

    //sprawdzenie czy plik jest otwarty  

    if (file.is_open()) 
    {
        cout << "Plik wejsciowy otwarto pomyslnie!" << endl;
        
    }
    else if(!file.is_open())
    {
        cout << "Nie mozna otworzyc pliku." << endl;
        cout << "Wcisnij enter aby kontynuowac...";
        system("PAUSE");
        exit(0);
    }

    file.close();
}

void encryption(string path_1,string path_2, int shifts, int uppercase, int lowercase)
{
    char tab[1024] = { '\0' };                          //tablica przechowujaca stringi z pliku

    string loading;                                     //string do ktorego pobierana bêdzie linijka
    string result;

    ifstream file_1(path_1);
    while (getline(file_1, loading))                    //wczytywanie tekstu z pliku do stringu linijka po linijce
    {
        strcpy_s(tab, loading.c_str());                 //kopiowanie stringa do tablicy znakowej

        for (int i = 0; i < loading.length(); i++)                    //algorytmy szyfrowania/odszyfrowania:
        {
            if (int(tab[i] >= 65 && tab[i] <= 90))                    //zamiana duzych liter
                result += char(int(tab[i] + shifts - uppercase) % 26 + uppercase);
            else if (int(tab[i] >= 32 && tab[i] <= 64))
                result += int(tab[i] = tab[i]);                       //jesli znaki specjalne, to nie zmieniaj
            else
                result += char(int(tab[i] + shifts - lowercase) % 26 + lowercase);    //zamiana malych liter
        }
    }

    ofstream file_2(path_2);                            //zapis rezultatu do drugiego pliku
    if (file_2)
    {
        file_2 << result;
        file_2.close();
    }
    else
    {
        cout << "BLAD!!!" << endl;
        exit(0);
    }
    file_1.close();
}

void interpretation(string path)
{
    ifstream file(path);
    if (file.is_open())
    {
        //udane otworzenie pliku
        string loading;     //zmienna do przechowywania odczytanych wierszy tekstu

        while (getline(file, loading))    //czyta a¿ do koñca pliku
        {
            cout << loading << endl; // wyœwietlenie w konsoli                      
        }
    }
    else
    {
        cout << "B£¥D: nie mo¿na otworzyæ pliku do odczytu." << endl;
        exit(0);
    }
    file.close();
}
int main()
{
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    wstep();                            //wywolanie funkcji przechowuj¹cej napis

    string path_1 = "file_1.txt";             
    string path_2 = "file_2.txt";

    read_from_file(path_1);             //sprawdzenie, czy pliki zosta³y otworzone prawid³owo
    read_from_file(path_2);
    cout << "\n";

    int uppercase, lowercase;           //zmienne pomocnicze przy szyfrowaniu/odszyfrowaniu
    int condition;                      //zmienna ktora zawiera warunek: szyfrowanie lub odszyfrowanie
    int shifts;                         //rodzaj szyfrowania/odszyfrowania(ilosc przesuniec)
                        
    string error = "BLAD!!!\n";

    cout << "Podaj o ile znakow chcesz zaszyfrowac/odszyfrowac tekst [-26...26]: ";
    cin >> shifts;
    cout << "" << endl;

    while (shifts> 26 || shifts<-26)
    {
        SetConsoleTextAttribute(hOut, FOREGROUND_RED);
        
        cout << error << endl;
        cout << "Wprowadz ponownie klucz przesuniecia: ";
        cin >> shifts;
        cout << "" << endl;
    }

    SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Szyfrowanie wcisnij [1] || Odszyfrowanie wcisnij [2]:  ";
    cin >> condition;
    cout << " " << endl;

    switch(condition)
    {
    case 1:                             //szyfrowanie
        uppercase = 65;                         //pierwsza du¿a litera w kodzie ASCII
        lowercase = 97;//pierwsza ma³a litera w kodzie ASCII
        encryption(path_1, path_2, shifts, uppercase, lowercase);
        break;
    case 2:                             //odszyfrowanie
        uppercase = 90;                         //ostatnia du¿a litera w kodzie ASCII
        lowercase = 122;                        //ostatnia ma³a litera w kodzie ASCII
        encryption(path_1, path_2, -shifts, uppercase, lowercase);
        break;

    default:
        cout << error;
        exit(0);
        break;
    }
    
    if (condition==1)
    { 
        cout << "Szyfrowanie zakonczone. \n" << endl;

        cout << "Tekst przed szyfrowaniem: ";
        interpretation(path_1);

        cout << "Tekst po szyfrowaniu: ";
        interpretation(path_2);

    }
    else if(condition == 2)
    {
        cout << "Odszyfrowanie zakonczone. \n" << endl;

        cout << "Tekst przed odszyfrowaniem: ";
        interpretation(path_1);

        cout << "Tekst po odszyfrowaniu: ";
        interpretation(path_2);
    }
    else 
        cout<<error<<endl;

    cout << " " << endl;
    
    system("PAUSE");
}






