#include <raylib.h>
#include <stdio.h>
#define GENISLIK 500
#define YUKSEKLIK 600
#define RAKIP_SAYISI 5

typedef struct arac {
    Rectangle konum;
    Color renk;
    bool aktif;
} Arac;

typedef struct oyun
{
    Arac oyuncu;
    Arac rakipler[RAKIP_SAYISI];
    bool oyunDurumu;
    int skor;
} Oyun;

void oyuncuGuncelle(Oyun *oyun) {
    if(IsKeyDown(KEY_LEFT))
        oyun->oyuncu.konum.x -= 5;
    if(IsKeyDown(KEY_RIGHT))
        oyun->oyuncu.konum.x += 5;
    if (oyun->oyuncu.konum.x < 0)
        oyun->oyuncu.konum.x = 0;
    if(oyun->oyuncu.konum.x > GENISLIK - 100)
        oyun->oyuncu.konum.x = GENISLIK - 100;
}

void oyuncuCizdir(Oyun *oyun) {
    DrawRectangleRec(oyun->oyuncu.konum, BLUE);
}

void oyuncuBaslat(Oyun *oyun) {
    oyun->oyuncu.konum = (Rectangle) {GENISLIK/2-50, YUKSEKLIK-150, 100, 150};
}
void seritCizdir() {
    DrawRectangle(GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
    DrawRectangle(2 * GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
}

void rakipOlustur(Oyun *oyun) {
    int rastgele = GetRandomValue(0, 100);
    Color renkler[] = {YELLOW, RED, GREEN};
    if(rastgele < 2 ) {
        int serit = GetRandomValue(0,2);
        for(int i=0; i < RAKIP_SAYISI; i++) {
            if(!oyun->rakipler[i].aktif) {
                //Rakibi oluştur
                Arac *rakip = &oyun->rakipler[i];
                rakip->konum.width = 100;
                rakip->konum.height = 150;
                rakip->konum.x = serit * GENISLIK / 3 + GENISLIK / 6 - 50;
                rakip->konum.y = -150;
                rakip->aktif = true;
                rakip->renk = renkler[GetRandomValue(0,2)];
            }
        }
    }
}

void rakipGuncelle(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif) {
            oyun->rakipler[i].konum.y += 5;
            if(oyun->rakipler[i].konum.y > YUKSEKLIK) {
                oyun->rakipler[i].aktif = false; // Ekrandan çıktı
                oyun->skor += 1;
                break;
            }
        }
    }
}

void rakipCizdir(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif) {
            DrawRectangleRec(oyun->rakipler[i].konum, oyun->rakipler[i].renk);
        }
    }
}

bool carpismaAlgila(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif &&
            CheckCollisionRecs(oyun->oyuncu.konum, oyun->rakipler[i].konum)) {
                oyun->oyunDurumu = false;
            return true;
        }
    }
    return false;
}

void oyunuSifirla(Oyun *oyun) {
    oyuncuBaslat(oyun);
    for(int i=0; i < RAKIP_SAYISI; i++) {
        oyun->rakipler[i].aktif = false;
    }
    oyun->skor = 0;
    oyun->oyunDurumu = true;
}

void skorYazdir(int skor) {
    char skorMetni[20];
    sprintf(skorMetni, "Skor: %d", skor);
    DrawText(skorMetni, 0, 0, 24, WHITE);
}

int main(int argc, char const *argv[])
{
    // Başlatma işlemleri
    //Rectangle konum = {0, 50, 100, 150};
    Oyun oyun;
    oyunuSifirla(&oyun);
    InitWindow(GENISLIK, YUKSEKLIK, "Şube 2 Ambulans Oyunu");
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // Güncelleme işlemleri
        if (oyun.oyunDurumu) {
            oyuncuGuncelle(&oyun);
            rakipOlustur(&oyun);
            rakipGuncelle(&oyun);
            carpismaAlgila(&oyun);
        }
        // Çizim işlemleri
        BeginDrawing();
        ClearBackground(GRAY);
        seritCizdir();
        oyuncuCizdir(&oyun);
        rakipCizdir(&oyun);
        skorYazdir(oyun.skor);
        if(!oyun.oyunDurumu) {
            DrawRectangle(0, YUKSEKLIK / 2 -20, GENISLIK, 200, BLACK);
            DrawText("Oyun Bitti!", 150, YUKSEKLIK / 2, 48, RED);
            DrawText("Enter -> Yeniden Baslat", 10, YUKSEKLIK / 2 + 100, 36, RED);
            if(IsKeyPressed(KEY_ENTER)) {
                oyunuSifirla(&oyun);
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
