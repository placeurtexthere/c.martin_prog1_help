#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct koktelok {
    char koktel_nev[30];
    char pohar_tipus[30];
    char recept[1000];
    struct koktelok *next;
} koktelok, *koktelok_ptr;

typedef struct poharak {
    char pohar_tipus[30];
    float urtartalom;
    char leiras[500];
    struct poharak *next;
} poharak, *poharak_ptr;


char* elkeszites_letrehozo(char* recept, char* nev, float mennyiseg, float alkohol) {
    strcat(recept, "- ");
    strcat(recept, nev);
    if (alkohol != 0)
    {
        char puffer[10];
        gcvt(alkohol, 3, puffer);
        strcat(recept, "(");
        strcat(recept, puffer);
        strcat(recept, "%)");
    }
    strcat(recept, " - ");
    char puffer[10];
    gcvt(mennyiseg, 3, puffer);
    strcat(recept, puffer);
    strcat(recept, " dl\n");

    return recept;
}


koktelok_ptr koktelok_beolvaso(koktelok_ptr head, char nev[]) {
    FILE *koktelok_file = fopen(nev, "r");

    if(koktelok_file == NULL) {
        printf("A koktelok filet nem sikrült megnyitni!");
        return 0;
    }

    char sor[1000];
    char elvkar[] = "#";
    char elvkar2[] = ",";

    while (fgets(sor, sizeof(char)*1000, koktelok_file) != NULL) {
        koktelok_ptr k = (koktelok *)malloc(sizeof(koktelok));
        if (k == NULL) {
            printf("Nem sikerült memóriát foglalni!");
            return 0;
        }
        char recept[1000] = "";
        k->next = head;
        strcpy(k->koktel_nev, strtok(sor, elvkar));
        strcpy(k->pohar_tipus, strtok(NULL, elvkar));

        char *alapanyag = strtok(NULL, elvkar);

        char* ptr_tomb[10];
        int i = 0;
        while (alapanyag != NULL) {
            char *a = strtok(NULL, elvkar);
            ptr_tomb[i] = alapanyag;
            i++;
            alapanyag = a;
        }
        for (int j = 0; j < i; j++)
        {
            elkeszites_letrehozo(recept ,strtok(ptr_tomb[j], elvkar2), (float)atof(strtok(NULL, elvkar2)), (float)atof(strtok(NULL, elvkar2)));
        }
        strcpy(k->recept, recept);
        head = k;
    }

    fclose(koktelok_file);
    return head;
}

poharak_ptr poharak_beolvaso(poharak_ptr head, char nev[]) {
    FILE *poharak_file = fopen(nev, "r");

    if(poharak_file == NULL) {
        printf("A poharak filet nem sikrült megnyitni!");
        return 0;
    }

    char sor[1000];
    char elvkar[] = "#";

    while (fgets(sor, sizeof(char)*1000, poharak_file) != NULL) {
        poharak_ptr p = (poharak *)malloc(sizeof(poharak));
        if (p == NULL) {
            printf("Nem sikerült memóriát foglalni!");
            return 0;
        }
        p->next = head;

        strcpy(p->pohar_tipus, strtok(sor, elvkar));
        p->urtartalom = (float)atof(strtok(NULL, elvkar));
        strcpy(p->leiras, strtok(NULL, elvkar));

        head = p;
    }
    fclose(poharak_file);
    return head;
}

int menu(koktelok_ptr head) {
    int i = 1;
    printf("Kérem válasszon egy koktélt a listából és írja be a számát a recept megtekintéséhez!\n");
    for (koktelok_ptr k = head; k->next != NULL; k = k->next)
    {
        printf("%d. %s\n", i, k->koktel_nev);
        i++;
    }
    printf("Koktél száma: ");
    int ki = 0;
    scanf("%d", &ki);
    return ki;
}

koktelok_ptr koktel_kereso(koktelok_ptr head, int k_szam) {
    int i = 0;
    for (koktelok_ptr k = head; k->next != NULL; k = k->next) {
        if(k_szam == i+1) {
            return k;
        }
        i++;
    }
}

poharak_ptr pohar_kereso(koktelok_ptr k_head, poharak_ptr p_head) {
    for (poharak_ptr p = p_head; p->next != NULL; p = p->next) {
        if (strcmp(k_head->pohar_tipus, p->pohar_tipus) == 0) {
            return p;
        }
    }
}

void kiirato(koktelok_ptr koktel, poharak_ptr pohar) {
    printf("\n* Neve: %s\n* A alapanyagok és a belőlük kellő mennyiség:\n%s########################################\n* A kellő pohár neve: %s\n* Űrtartalma: %1.f dl\n* A pohárról egy rövid leírás: %s.", koktel->koktel_nev, koktel->recept, pohar->pohar_tipus, pohar->urtartalom, pohar->leiras);
}

int main() {
    koktelok_ptr koktelok_head = (koktelok_ptr)malloc(sizeof(koktelok));               //dinamikus memória foglalása
    koktelok_ptr koktelok = koktelok_beolvaso(koktelok_head,"koktelok.txt");             //beolvassa a fájlt

    poharak_ptr poharak_head = (poharak_ptr)malloc(sizeof(poharak));                           //dinamikus memória foglalás
    poharak_ptr poharak = poharak_beolvaso(poharak_head,"poharak.txt"); 

    koktelok_ptr k = koktel_kereso(koktelok, menu(koktelok));

    poharak_ptr p = pohar_kereso(k, poharak);

    kiirato(k, p);

    return 0;
}