#ifndef Z_OBJ_PZLBLOCK_H
#define Z_OBJ_PZLBLOCK_H

#include "global.h"

struct ObjPzlblock;

typedef void (*ObjPzlblockActionFunc)(struct ObjPzlblock*, PlayState*);

#define OBJPZLBLOCK_GET_7F(thisx) ((thisx)->params & 0x7F)
#define OBJPZLBLOCK_GET_700(thisx) (((thisx)->params >> 8) & 7)
#define OBJPZLBLOCK_GET_1000(thisx) (((thisx)->params >> 0xC) & 1)
#define OBJPZLBLOCK_GET_ROTZ(thisx) ((thisx)->home.rot.z & 7)

typedef struct ObjPzlblock {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ObjPzlblockActionFunc actionFunc;
    /* 0x160 */ s32 unk_160;
    /* 0x164 */ f32* unk_164;
    /* 0x168 */ f32 unk_168;
    /* 0x16C */ s16 unk_16C;
    /* 0x16E */ s16 unk_16E[4];
    /* 0x176 */ s16 unk_176;
    /* 0x178 */ s16 unk_178;
    /* 0x17A */ s8 unk_17A;
} ObjPzlblock; // size = 0x17C

extern const ActorInit Obj_Pzlblock_InitVars;

#endif // Z_OBJ_PZLBLOCK_H
