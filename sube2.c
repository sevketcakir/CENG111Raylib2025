#include <raylib.h>
#include <stdio.h>
#define GENISLIK 500
#define YUKSEKLIK 600
#define RAKIP_SAYISI 5

typedef struct arac {
    Rectangle konum;
    Color renk;
    bool aktif;
    int dokuIndeksi;
} Arac;

typedef struct oyun
{
    Arac oyuncu;
    Arac rakipler[RAKIP_SAYISI];
    bool oyunDurumu;
    int skor;
    Texture2D doku;
    Sound altinSesi;
    Sound carpmaSesi;
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
    //DrawRectangleRec(oyun->oyuncu.konum, BLUE);
    DrawTexturePro(oyun->doku, 
        arac_koordinat[oyun->oyuncu.dokuIndeksi],
        oyun->oyuncu.konum,
        (Vector2) {0,0},
        0.0f,
        RAYWHITE
    );
}

void oyuncuBaslat(Oyun *oyun) {
    oyun->oyuncu.dokuIndeksi = 0;
    oyun->oyuncu.konum = (Rectangle) {GENISLIK/2-50, YUKSEKLIK-arac_koordinat[oyun->oyuncu.dokuIndeksi].height / 10.0f, 
    arac_koordinat[oyun->oyuncu.dokuIndeksi].width / 10.0f, 
    arac_koordinat[oyun->oyuncu.dokuIndeksi].height / 10.0f};
}
void seritCizdir() {
    DrawRectangle(GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
    DrawRectangle(2 * GENISLIK / 3, 0, 5, YUKSEKLIK, RAYWHITE);
}

void rakipOlustur(Oyun *oyun) {
    int rastgele = GetRandomValue(0, 100);
    Color renkler[] = {YELLOW, RED, GREEN};
    if(rastgele < 1 ) {
        int serit = GetRandomValue(0,2);
        for(int i=0; i < RAKIP_SAYISI; i++) {
            if(!oyun->rakipler[i].aktif) {
                oyun->rakipler[i].dokuIndeksi = GetRandomValue(1, 8);
                //Rakibi oluştur
                Arac *rakip = &oyun->rakipler[i];
                // rakip->konum.width = 100;
                // rakip->konum.height = 150;
                rakip->konum.width = arac_koordinat[oyun->rakipler[i].dokuIndeksi].width / 10.0;
                rakip->konum.height = arac_koordinat[oyun->rakipler[i].dokuIndeksi].height / 10.0;
                rakip->konum.x = serit * GENISLIK / 3 + GENISLIK / 6 - 50;
                rakip->konum.y = -150;
                rakip->aktif = true;
                //rakip->renk = renkler[GetRandomValue(0,2)];
                break;
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
                PlaySound(oyun->altinSesi);
                break;
            }
        }
    }
}

void rakipCizdir(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif) {
            //DrawRectangleRec(oyun->rakipler[i].konum, oyun->rakipler[i].renk);
            DrawTexturePro(oyun->doku, arac_koordinat[oyun->rakipler[i].dokuIndeksi],
            oyun->rakipler[i].konum, (Vector2) {0,0},
            0.0f, RAYWHITE);
        }
    }
}

bool carpismaAlgila(Oyun *oyun) {
    for(int i=0; i < RAKIP_SAYISI; i++) {
        if(oyun->rakipler[i].aktif &&
            CheckCollisionRecs(oyun->oyuncu.konum, oyun->rakipler[i].konum)) {
                oyun->oyunDurumu = false;
                PlaySound(oyun->carpmaSesi);
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
    InitAudioDevice();
    oyun.doku = LoadTexture("resources/vehicles.png");
    Music oyunMuzigi = LoadMusicStream("resources/music.mp3");
    oyun.altinSesi = LoadSound("resources/gold-coin-sound.mp3");
    oyun.carpmaSesi = LoadSound("resources/crash.mp3");
    PlayMusicStream(oyunMuzigi);
    while(!WindowShouldClose()) {
        // Güncelleme işlemleri
        if (oyun.oyunDurumu) {
            oyuncuGuncelle(&oyun);
            rakipOlustur(&oyun);
            rakipGuncelle(&oyun);
            carpismaAlgila(&oyun);
            UpdateMusicStream(oyunMuzigi);
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
                SeekMusicStream(oyunMuzigi, 0);
            }
        }
        EndDrawing();
    }
    UnloadMusicStream(oyunMuzigi);
    UnloadSound(oyun.altinSesi);
    UnloadSound(oyun.carpmaSesi);
    CloseAudioDevice();
    UnloadTexture(oyun.doku);
    CloseWindow();
    return 0;
}
