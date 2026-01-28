#include <grrlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <fat.h>
#include <wiiuse/wpad.h>
#include <asndlib.h>
#include <mp3player.h>
#include <sys/stat.h>
#include <ogc/lwp_watchdog.h>

char *mbuffer = NULL;
long lSize;
bool use_audio = false;
bool use_video = true;

int main() {
    VIDEO_Init();
    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
    void *xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();

    WPAD_Init();
    if (!fatInitDefault()) {
        printf("Erreur SD !\n");
        while(1) VIDEO_WaitVSync();
    }

    printf("\n=== BAD APPLE SELECTION ===\n");
    printf("(A) Video Seule\n(B) Video + Audio (Sync)\n(2) Audio Seul\n");

    while(1) {
        WPAD_ScanPads();
        u32 btn = WPAD_ButtonsDown(0);
        if (btn & WPAD_BUTTON_A) { use_audio = false; use_video = true; break; }
        if (btn & WPAD_BUTTON_B) { use_audio = true; use_video = true; break; }
        if (btn & WPAD_BUTTON_2) { use_audio = true; use_video = false; break; }
        VIDEO_WaitVSync();
    }

    // Chargement de l'audio depuis le dossier badapple
    if (use_audio) {
        ASND_Init();
        MP3Player_Init();

        // NOUVEAU CHEMIN : sd:/badapple/audio.mp3
        FILE *music = fopen("sd:/badapple/audio.mp3", "rb"); 
        if (music) {
            fseek(music, 0, SEEK_END);
            lSize = ftell(music);
            rewind(music);

            printf("Chargement audio en RAM (%ld octets)...\n", lSize);
            mbuffer = (char *)malloc(lSize); // Allocation en RAM
            if (mbuffer) {
                fread(mbuffer, 1, lSize, music);
                printf("[OK] Audio pret.\n");
            }
            fclose(music);
        } else {
            printf("[ERREUR] sd:/badapple/audio.mp3 introuvable !\n");
            use_audio = false;
        }
    }

    if (use_video) {
        printf("Initialisation GRRLIB...\n");
        VIDEO_WaitVSync();
        GRRLIB_Init();
    }

    int frame = 1;
    char path[128];
    bool playing = false;
    u64 start_time = 0;

    while(1) {
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;

        if (use_audio && !playing && mbuffer) {
            MP3Player_PlayBuffer(mbuffer, lSize, NULL); // Lecture RAM
            playing = true;
            start_time = gettime();
        }

        if (use_video) {
            GRRLIB_2dMode();
            GRRLIB_FillScreen(0x000000FF);

            if (playing || !use_audio) {
                if (use_audio) {
                    u64 ms = ticks_to_millisecs(gettime() - start_time);
                    frame = (ms * 30) / 1000;
                }
                
                if (frame < 1) frame = 1;
                sprintf(path, "sd:/badapple/img_%04d.jpg", frame);
                GRRLIB_texImg *tex = GRRLIB_LoadTextureFromFile(path); // Lecture SD
                if (tex) {
                    GRRLIB_DrawImg(80, 60, tex, 0, 1, 1, 0xFFFFFFFF);
                    GRRLIB_FreeTexture(tex);
                    if (!use_audio) frame++;
                }
            }
            GRRLIB_Render();
        } else {
            VIDEO_WaitVSync();
            if (playing && !MP3Player_IsPlaying()) break;
        }

        if (frame > 6500) break;
    }

    MP3Player_Stop();
    if (mbuffer) free(mbuffer);
    if (use_video) GRRLIB_Exit();
    exit(0);
}