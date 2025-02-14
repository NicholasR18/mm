#include "global.h"

s32 D_801C5E30[] = { 0, 0x2000, 0x4000, 0x6000, 0x8000, 0xC000 };

u16 D_801C5E48[] = {
    0, 2, 10, 12, 2, 4, 12, 14, 10, 12, 20, 22, 12, 14, 22, 24, 1, 3, 5, 6, 7, 8, 9, 11, 13, 15, 16, 17, 18, 19, 21, 23,
};
s32 D_801C5E88[] = { 0, 0x7C0, 0xF80, 0x1740, 0x1F00 };

s32 D_801C5E9C[] = { 0, 0x7C0, 0xF80, 0x1740, 0x1F00 };

s32 D_801C5EB0[] = { 0, 0x7C0, 0xF80, 0x7C0, 0 };

s16 D_801C5EC4[] = {
    0,  16, 19, 18, 16, 1,  20, 19, 1,  17, 21, 20, 17, 5,  22, 21, 18, 19, 23, 2,  19, 20,
    3,  23, 20, 21, 24, 3,  21, 22, 7,  24, 2,  23, 26, 25, 23, 3,  27, 26, 3,  24, 28, 27,
    24, 7,  29, 28, 25, 26, 30, 10, 26, 27, 11, 30, 27, 28, 31, 11, 28, 29, 15, 31,
};

#pragma GLOBAL_ASM("asm/non_matchings/code/z_vr_box/func_80142440.s")

void func_80143148(SkyboxContext* skyboxCtx, s32 arg1) {
    static struct_801C5F44 D_801C5F44[] = {
        { -64, 64, -64, 32, -32 }, { 64, 64, 64, -32, -32 }, { -64, 64, 64, -32, -32 },
        { 64, 64, -64, 32, -32 },  { -64, 64, 64, 32, -32 }, { -64, -64, -64, 32, 32 },
    };
    s32 ret = 0;
    struct_801C5F44* unkStruct;
    s32 face;

    for (face = 0; face < arg1; face++) {
        unkStruct = &D_801C5F44[face];
        ret = func_80142440(skyboxCtx, skyboxCtx->roomVtx, ret, unkStruct->unk0, unkStruct->unk4, unkStruct->unk8,
                            unkStruct->unkC, unkStruct->unk10, face);
    }
}

void func_801431E8(GameState* gameState, SkyboxContext* skyboxCtx, s16 skyType) {
    PlayState* play = (PlayState*)gameState;
    size_t size;
    void* offset;
    s32 pad;

    skyboxCtx->rotZ = 0.0f;

    if (skyType != 1) {
        if (skyType != 2) {
            return;
        }
    } else {
        // Send a DMA request for the cloudy sky texture
        skyboxCtx->skyboxStaticSegment[0] = &D_80025D00;
        size = SEGMENT_ROM_SIZE(d2_cloud_static);
        offset = (void*)ALIGN8((u32)skyboxCtx->skyboxStaticSegment[0] + size);
        DmaMgr_SendRequest0(skyboxCtx->skyboxStaticSegment[0], SEGMENT_ROM_START(d2_cloud_static), size);

        // Send a DMA request for the clear sky texture
        skyboxCtx->skyboxStaticSegment[1] = offset;
        size = SEGMENT_ROM_SIZE(d2_fine_static);
        offset = (void*)ALIGN8((u32)offset + size);
        DmaMgr_SendRequest0(skyboxCtx->skyboxStaticSegment[1], SEGMENT_ROM_START(d2_fine_static), size);

        // Send a DMA request for the skybox palette
        skyboxCtx->skyboxPaletteStaticSegment = offset;
        size = SEGMENT_ROM_SIZE(d2_fine_pal_static);
        offset = (void*)ALIGN8((u32)offset + size);
        DmaMgr_SendRequest0(skyboxCtx->skyboxPaletteStaticSegment, SEGMENT_ROM_START(d2_fine_pal_static), size);

        skyboxCtx->primR = 145;
        skyboxCtx->primG = 120;
        skyboxCtx->primB = 155;

        skyboxCtx->envR = 40;
        skyboxCtx->envG = 0;
        skyboxCtx->envB = 40;

        // Inverted Stone Tower Temple and Inverted Stone Tower
        if ((play->sceneNum == SCENE_F41) || (play->sceneNum == SCENE_INISIE_R)) {
            skyboxCtx->rotZ = 3.15000009537f;
        }
    }
}

void func_80143324(PlayState* play, SkyboxContext* skyboxCtx, s16 skyType) {
    size_t size;

    if (1) {}

    if (skyType == 1) {
        osCreateMesgQueue(&skyboxCtx->loadQueue, &skyboxCtx->loadMsg, 1);

        if (play->envCtx.unk_10 == 0) {
            // Send a DMA request for the clear sky texture
            size = SEGMENT_ROM_SIZE(d2_fine_static);

            DmaMgr_SendRequestImpl(&skyboxCtx->unk188, skyboxCtx->skyboxStaticSegment[0],
                                   SEGMENT_ROM_START(d2_fine_static), size, 0, &skyboxCtx->loadQueue, NULL);
        } else {
            // Send a DMA request for the cloudy sky texture
            size = SEGMENT_ROM_SIZE(d2_cloud_static);

            DmaMgr_SendRequestImpl(&skyboxCtx->unk188, skyboxCtx->skyboxStaticSegment[0],
                                   SEGMENT_ROM_START(d2_cloud_static), size, 0, &skyboxCtx->loadQueue, NULL);
        }

        osRecvMesg(&skyboxCtx->loadQueue, NULL, OS_MESG_BLOCK);
        osCreateMesgQueue(&skyboxCtx->loadQueue, &skyboxCtx->loadMsg, 1);

        if (play->envCtx.unk_11 == 0) {
            // Send a DMA request for the clear sky texture
            size = SEGMENT_ROM_SIZE(d2_fine_static);

            DmaMgr_SendRequestImpl(&skyboxCtx->unk1A8, skyboxCtx->skyboxStaticSegment[1],
                                   SEGMENT_ROM_START(d2_fine_static), size, 0, &skyboxCtx->loadQueue, NULL);
        } else {
            // Send a DMA request for the cloudy sky texture
            size = SEGMENT_ROM_SIZE(d2_cloud_static);

            DmaMgr_SendRequestImpl(&skyboxCtx->unk1A8, skyboxCtx->skyboxStaticSegment[1],
                                   SEGMENT_ROM_START(d2_cloud_static), size, 0, &skyboxCtx->loadQueue, NULL);
        }

        osRecvMesg(&skyboxCtx->loadQueue, NULL, OS_MESG_BLOCK);
        osCreateMesgQueue(&skyboxCtx->loadQueue, &skyboxCtx->loadMsg, 1);

        size = SEGMENT_ROM_SIZE(d2_fine_pal_static);

        // Send a DMA request for the skybox palette
        DmaMgr_SendRequestImpl(&skyboxCtx->unk1C8, skyboxCtx->skyboxPaletteStaticSegment,
                               SEGMENT_ROM_START(d2_fine_pal_static), size, 0, &skyboxCtx->loadQueue, NULL);

        osRecvMesg(&skyboxCtx->loadQueue, NULL, OS_MESG_BLOCK);
    }
}

void func_801434E4(GameState* gameState, SkyboxContext* skyboxCtx, s16 skyType) {
    skyboxCtx->skyboxShouldDraw = 0;
    skyboxCtx->rotX = skyboxCtx->rotY = skyboxCtx->rotZ = 0.0f;

    func_801431E8(gameState, skyboxCtx, skyType);

    if (skyType != 0) {
        skyboxCtx->dListBuf = THA_AllocEndAlign16(&gameState->heap, 0x3840);

        if (skyType == 5) {
            // Allocate enough space for the vertices for a 6 sided skybox (cube)
            skyboxCtx->roomVtx = THA_AllocEndAlign16(&gameState->heap, sizeof(Vtx) * 32 * 6);
            func_80143148(skyboxCtx, 6);
        } else {
            // Allocate enough space for the vertices for a 5 sided skybox (bottom is missing)
            skyboxCtx->roomVtx = THA_AllocEndAlign16(&gameState->heap, sizeof(Vtx) * 32 * 5);
            func_80143148(skyboxCtx, 5);
        }
    }
}
