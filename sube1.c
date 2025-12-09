#include <raylib.h>
#include <stdio.h>
#define GENISLIK 500
#define YUKSEKLIK 600
#define ARAC_GENISLIK 100
#define ARAC_YUKSEKLIK 150
#define RAKIP_SAYISI 5

typedef struct arac {
    Rectangle konum;
    Color renk;
    bool aktif;
    int dokuIndeksi;
} Arac;

typedef struct Oyun {
    Arac oyuncu;
    Arac rakipler[RAKIP_SAYISI];
    int skor;
    bool oyunDurumu;
    int maxSkor;
    Texture2D doku;
} Oyun;

Rectangle arac_koordinat[] = {
    { 2800, 60, 1100, 2340 },
    { 1698, 213, 1002, 2187 },
    { 3950, 405, 1000, 1995 },
    { 900, 843, 600, 1557 },
    { 100, 992, 700, 1408 },
    { 400, 2762, 1000, 2188 },
    { 4150, 3005, 800, 1945 },
    { 3000, 3032, 800, 1918 },
    { 1800, 3035, 900, 1915 },
};


void oyuncuSifirla(Oyun *oyun) {
    oyun->oyuncu.konum.x = GENISLIK / 2 - ARAC_GENISLIK / 2;
    // oyun->oyuncu.konum.width = ARAC_GENISLIK;
    // oyun->oyuncu.konum.height = ARAC_YUKSEKLIK;
    //oyun->oyuncu.renk = PINK;
    oyun->oyuncu.dokuIndeksi = 0;
    oyun->oyuncu.konum.width = arac_koordinat[oyun->oyuncu.dokuIndeksi].width / 10.0f;
    oyun->oyuncu.konum.height = arac_koordinat[oyun->oyuncu.dokuIndeksi].height / 10.0f;
    oyun->oyuncu.konum.y = YUKSEKLIK - oyun->oyuncu.konum.height;

}

void oyuncuGuncelle(Oyun *oyun){
    if(IsKeyDown(KEY_LEFT))
        oyun->oyuncu.konum.x -= 5;
    if(IsKeyDown(KEY_RIGHT))
        oyun->oyuncu.konum.x += 5;
    if(oyun->oyuncu.konum.x<0)
        oyun->oyuncu.konum.x=0;
    if(oyun->oyuncu.konum.x>GENISLIK-ARAC_GENISLIK)
        oyun->oyuncu.konum.x = GENISLIK - ARAC_GENISLIK;
}

void oyuncuCizdir(Oyun *oyun) {
    //DrawRectangleRec(oyun->oyuncu.konum, oyun->oyuncu.renk);
    DrawTexturePro(oyun->doku,
    arac_koordinat[oyun->oyuncu.dokuIndeksi],
    oyun->oyuncu.konum,
    (Vector2) {0,0},
    0.0f, RAYWHITE
    );
} 

void seritCizdir() {
    DrawRectangle(GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
    DrawRectangle(2 * GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
}

void rakipSifirla(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        oyun->rakipler[i].aktif = false;
    }
}

void oyunSifirla(Oyun *oyun) {
    oyuncuSifirla(oyun);
    rakipSifirla(oyun);
    oyun->oyunDurumu = true;
    oyun->skor = 0;
}

void rakipOlustur(Oyun *oyun) {
    int ihtimal = GetRandomValue(0, 100);
    Color renkler[] = {BLUE, RED, YELLOW, GREEN};
    if(ihtimal < 2) {
        for (int i=0; i < RAKIP_SAYISI; i++) {
            if (!oyun->rakipler[i].aktif) {
                Arac *rakip = &oyun->rakipler[i];
                rakip->dokuIndeksi = GetRandomValue(1,8);
                int serit = GetRandomValue(0,2);
                rakip->konum.y = -ARAC_YUKSEKLIK;
                rakip->konum.x =serit * GENISLIK / 3 + GENISLIK / 6 - ARAC_GENISLIK / 2;
                rakip->konum.width = arac_koordinat[rakip->dokuIndeksi].width / 10.0f;
                rakip->konum.height = arac_koordinat[rakip->dokuIndeksi].height / 10.0f;
                // rakip->konum.width = ARAC_GENISLIK;
                // rakip->konum.height = ARAC_YUKSEKLIK;
                //rakip->renk = renkler[GetRandomValue(0, 3)];
                rakip->aktif = true;
                break;
            }
        }
    }
}

void rakipGuncelle(Oyun *oyun) {
    for(int i=0;i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif) {
            oyun->rakipler[i].konum.y += 5;
            if(oyun->rakipler[i].konum.y > YUKSEKLIK) {
                oyun->rakipler[i].aktif = false;
                oyun->skor += 1;
            }
        }
    }
}

void rakipCizdir(Oyun *oyun) {
    for(int i=0;i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif) {
            //DrawRectangleRec(oyun->rakipler[i].konum, oyun->rakipler[i].renk);
            DrawTexturePro(oyun->doku, 
            arac_koordinat[oyun->rakipler[i].dokuIndeksi],
            oyun->rakipler[i].konum,
            (Vector2){0,0},
            0.0f, RAYWHITE
            );
        }
    }
}

bool carpismaAlgila(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif &&
            CheckCollisionRecs(oyun->oyuncu.konum, oyun->rakipler[i].konum)) {
            oyun->oyunDurumu = false;
            if(oyun->skor > oyun->maxSkor)
                oyun->maxSkor = oyun->skor;
            return true;
        }
    }
    return false;
}

void skorCizdir(Oyun *oyun) {
    char mesaj[20];
    sprintf(mesaj, "Skor: %d", oyun->skor);
    DrawText(mesaj, 0, 0, 24, WHITE);
}

int main(int argc, char const *argv[])
{
    Oyun oyun;
    oyun.maxSkor = 0;
    oyunSifirla(&oyun);
    InitWindow(GENISLIK, YUKSEKLIK, "Şube 1 Ambülans Oyunu");
    SetTargetFPS(60);
    oyun.doku = LoadTexture("resources/vehicles.png");
    while(!WindowShouldClose()) {
        if(oyun.oyunDurumu) {
            oyuncuGuncelle(&oyun);
            rakipOlustur(&oyun);
            rakipGuncelle(&oyun);
            carpismaAlgila(&oyun);
        }
        BeginDrawing();
        ClearBackground(GRAY);
        seritCizdir();
        oyuncuCizdir(&oyun);
        rakipCizdir(&oyun);
        skorCizdir(&oyun);
        if(!oyun.oyunDurumu) {
            char skor[20];
            sprintf(skor, "En yuksek: %d", oyun.maxSkor);

            DrawRectangle(0, 200, GENISLIK, 200, BLACK);
            DrawText("Oyun Bitti!", 150, 300, 48, RED );
            DrawText(skor, 100,  350, 36, YELLOW);
            if(IsKeyPressed(KEY_ENTER))
                oyunSifirla(&oyun);
        }
        EndDrawing();
    }
    UnloadTexture(oyun.doku);
    CloseWindow();

    return 0;
}
