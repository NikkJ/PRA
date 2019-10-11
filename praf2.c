#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define mb pow(2, 20)

typedef struct dados {
    unsigned long longo;
    float dec;
} Dados;

int compareDados(const void *a, const void *b) {
    if (((Dados *)a)->longo < ((Dados *)b)->longo)
        return -1;
    if (((Dados *)a)->longo == ((Dados *)b)->longo) {
        if (((Dados *)a)->dec < ((Dados *)b)->dec) {
            return -1;
        } else if (((Dados *)a)->dec > ((Dados *)b)->dec) {
            return 1;
        } else {
            return 0;
        }
    }
    if (((Dados *)a)->longo > ((Dados *)b)->longo)
        return 1;
    return 0;
}

long tamArq(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    rewind(fp);
    return size / sizeof(Dados);
}

FILE *criaArq(char *nome, size_t tam) {
    size_t size = 0;
    FILE *fp = fopen(nome, "wb+");
    Dados dados;
    int c = 0;
    while (size < tam) {
        dados.dec = (float)rand();
        //if(c%2==0) caso queria testar numeros iguais
        dados.longo = (unsigned long)(((unsigned long)rand()) * rand());
        fwrite(&dados, sizeof(Dados), 1, fp);
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        c++;
    }
    fseek(fp, 0, SEEK_SET);
    return fp;
}

void lerArq(FILE *fp, Dados *dados) {
    Dados dadosAux;
    int n = 0;
    while (fread(&dadosAux, 1, sizeof(Dados), fp) && n < 5) {
        dados[n].longo = dadosAux.longo;
        dados[n].dec = dadosAux.dec;
        n++;
    }
}

int main() {
    FILE *f[5];  //,*f2,*f3,*f4,*f5;
    Dados dados[5];
    size_t qtdElem;
    srand(time(0));
    f[0] = criaArq("dados1.txt", mb / 1000);
    f[1] = criaArq("dados2.txt", mb / 1000);
    f[2] = criaArq("dados3.txt", mb / 1000);
    f[3] = criaArq("dados4.txt", mb / 1000);
    f[4] = criaArq("dados5.txt", mb / 1000);
    for (int i = 0; i < 5; i++) {
        qtdElem = tamArq(f[i]);
        //dados = (Dados *)malloc(sizeof(Dados) * 5);
        //lerArq(f[i], dados);
        while (!feof(f[i])) {
            fread(&dados, sizeof(Dados), 5, f[i]);
            qsort(dados, 5, sizeof(Dados), compareDados);
            printf("a\n");
        }
        //for (int j = 0; j < 5; j++) {
          //  printf("%d: %lu %f\n", j, dados[j].longo, dados[j].dec);
        //}
        fclose(f[i]);
        char nome[20];
        sprintf(nome, "sdados%d.txt", i + 1);
        f[i] = fopen(nome, "wb+");
        for (int j = 0; j < qtdElem; j++) {
            fwrite(&dados[j], sizeof(Dados), 1, f[i]);
        }
        fclose(f[i]);
    }
    FILE *fout = fopen("ord.txt", "wb+");
    FILE *fin[5];
    Dados d[5];
    fin[0] = fopen("sdados1.txt", "rb+");
    fin[1] = fopen("sdados2.txt", "rb+");
    fin[2] = fopen("sdados3.txt", "rb+");
    fin[3] = fopen("sdados4.txt", "rb+");
    fin[4] = fopen("sdados5.txt", "rb+");
    for (int i = 0; i < 5; i++) {
        fread(&d[i], sizeof(Dados), 1, fin[i]);
    }
    int r = 5;  // remaining
    while (r) {
        Dados smallest = d[0];
        int si = 0;
        for (int i = 1; i < r; i++) {
            if (d[i].longo < smallest.longo) {
                smallest = d[i];
                si = i;
            } else if (d[i].longo == smallest.longo) {
                if (d[i].dec < smallest.dec) {
                    smallest = d[i];
                    si = i;
                }
            }
        }
        //printf("%lu %f\n",smallest.longo,smallest.dec);
        fwrite(&smallest, sizeof(Dados), 1, fout);
        fread(&d[si], sizeof(Dados), 1, fin[si]);
        if (feof(fin[si])) {
            r--;
            for (int i = si; i < r - 1; i++) {
                fin[i] = fin[i + 1];
                d[i] = d[i + 1];
            }
        }
    }
    //free(dados);
    return 0;
}