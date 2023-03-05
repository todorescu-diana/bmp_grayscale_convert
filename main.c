#include <stdio.h>
#include <stdlib.h>

FILE *colourFile;           //fisierul color original
FILE *grayscaleFile;        //fisierul ce va fi creat in urma rularii programului; am ales sa fac o copie alb negru a fisierului original color
                            //in loc sa suprascriu fisierul original, pentru a vedea similaritatile&diferentele + pt posibilitatea pastrarii
                            //fisierului original

int width;
int height;
int size;

void readAndConvertBMP()    //citirea din fisierul color original, cat si scrierea in fisierul grayscale ce urmeaza a fi creat se face in aceeasi functie,
                            //nefiind nevoie de doua functii separate avand in vedere simplitatea codului
{
    unsigned char *data;

    if((colourFile=fopen("colour.BMP","rb"))==NULL) //deschidere fisier color original pt citire
    {
        printf("Eroare deschidere fisier\e\n");
        fclose(colourFile);
        fclose(grayscaleFile);
        exit(EXIT_FAILURE);
    }

    if((grayscaleFile=fopen("grayscale.BMP","wb"))==NULL)   //deschidere fisier grayscale pt scriere
    {
        printf("Eroare deschidere fisier\e\n");
        fclose(colourFile);
        fclose(grayscaleFile);
        exit(EXIT_FAILURE);
    }

    unsigned char info[54];

    fread(info, sizeof(unsigned char), 54, colourFile);      //citirea header-ului de 54 de bytes tipic fisierului .bmp
    fwrite(info,sizeof(unsigned char),54,grayscaleFile);       //informatiile din header se scriu nemodificate in fisierul grayscale;

    width = *(int*)&info[18];               //retinerea marimior width&height din header
    height = *(int*)&info[22];

    int size = 3 * width * abs(height);        //alocarea a 3 bytes per pixel: pentru valorile RGB: red, green respectiv blue

    if((data = (unsigned char *)malloc(size*sizeof(unsigned char)))==NULL)      //alocare dinamica vector ce retine datele pixelilor
    {
        printf("Memorie insuficienta.\n");
        fclose(colourFile);
        exit(EXIT_FAILURE);
    }

    fread(data, sizeof(unsigned char), size, colourFile);    //citirea restului de informatie de dupa header "data" deodata

    //valorile RGB (respectiv BGR) ce ne intereseaza sunt stocate la data[3 * (i * width + j)], data[3 * (i * width + j)+1], respectiv data[3 * (i * width + j)+2]

    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
            {
                unsigned char medie =  ( 0.114*data[3 * (i * width + j)] + 0.587*data[3 * (i * width + j)+1] + 0.299*data[3 * (i * width + j)+2] ); //instructiunea ce transforma de fapt imaginea in grayscale;
                                                                                                                                                    //mdificand valorile RGB(BGR) cu o "medie" a lor, obtinem grayscale
                data[3 * (i * width + j)] = medie;              //inlocuirea informatiei RGB(BGR) in "media" lor
                data[3 * (i * width + j)+1] = medie;
                data[3 * (i * width + j)+2] = medie;
            }
    }

    fwrite(data,sizeof(unsigned char), size, grayscaleFile);    //scrierea datei "transformate" in grayscale in fisierul grayscale
    fclose(colourFile);
    fclose(grayscaleFile);

    /*for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];        //schimbarea ordinii a celor 3 bytes de RGB (acestea fiind stocate ca BGR) //EDIT:instructiuni puse ca si comentariu
                                                                deoarece nu este relevant pentru problema in ce ordine sunt; conteaza doar sa
                                                                se cunoasca faptul ca sunt stocati ca BGR, pentru a folosi coeficientii corect in variabila "medie" folosita la linia 125.
            data[i] = data[i+2];
            data[i+2] = tmp;
    }*/
}

int main()
{

    readAndConvertBMP();

    return 0;
}
