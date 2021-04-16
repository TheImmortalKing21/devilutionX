/**
 * @file dead.cpp
 *
 * Implementation of functions for placing dead monsters.
 */
#include "dead.h"

#include "gendung.h"
#include "lighting.h"
#include "misdat.h"
#include "monster.h"

namespace devilution {

DeadStruct dead[MAXDEAD];
int8_t stonendx;

void InitDead()
{
	int8_t mtypes[MAXMONSTERS];

	for (int8_t &mtype : mtypes)
		mtype = 0;

	int8_t nd = 0;

	for (int i = 0; i < nummtypes; i++) {
		if (mtypes[Monsters[i].mtype] == 0) {
			for (int d = 0; d < 8; d++)
				dead[nd]._deadData[d] = Monsters[i].Anims[MA_DEATH].Data[d];
			dead[nd]._deadFrame = Monsters[i].Anims[MA_DEATH].Frames;
			dead[nd]._deadWidth = Monsters[i].width;
			dead[nd]._deadWidth2 = Monsters[i].width2;
			dead[nd]._deadtrans = 0;
			nd++;

			Monsters[i].mdeadval = nd;
			mtypes[Monsters[i].mtype] = nd;
		}
	}

	for (auto &d : dead[nd]._deadData)
		d = misfiledata[MFILE_BLODBUR].mAnimData[0];
	dead[nd]._deadFrame = 8;
	dead[nd]._deadWidth = 128;
	dead[nd]._deadWidth2 = 32;
	dead[nd]._deadtrans = 0;
	nd++;

	for (auto &d : dead[nd]._deadData)
		d = misfiledata[MFILE_SHATTER1].mAnimData[0];

	dead[nd]._deadFrame = 12;
	dead[nd]._deadWidth = 128;
	dead[nd]._deadWidth2 = 32;
	dead[nd]._deadtrans = 0;
	nd++;

	stonendx = nd;

	for (int i = 0; i < nummonsters; i++) {
		int mi = monstactive[i];
		if (monster[mi]._uniqtype != 0) {
			for (int d = 0; d < 8; d++)
				dead[nd]._deadData[d] = monster[mi].MType->Anims[MA_DEATH].Data[d];
			dead[nd]._deadFrame = monster[mi].MType->Anims[MA_DEATH].Frames;
			dead[nd]._deadWidth = monster[mi].MType->width;
			dead[nd]._deadWidth2 = monster[mi].MType->width2;
			dead[nd]._deadtrans = monster[mi]._uniqtrans + 4;
			nd++;

			monster[mi]._udeadval = nd;
		}
	}

	assert(nd <= MAXDEAD);
}

void AddDead(int dx, int dy, int8_t dv, int ddir)
{
	dDead[dx][dy] = (dv & 0x1F) + (ddir << 5);
}

void SetDead()
{
	for (int i = 0; i < nummonsters; i++) {
		int mi = monstactive[i];
		if (monster[mi]._uniqtype != 0) {
			for (int dx = 0; dx < MAXDUNX; dx++) {
				for (int dy = 0; dy < MAXDUNY; dy++) {
					if ((dDead[dx][dy] & 0x1F) == monster[mi]._udeadval)
						ChangeLightXY(monster[mi].mlid, dx, dy);
				}
			}
		}
	}
}

} // namespace devilution
