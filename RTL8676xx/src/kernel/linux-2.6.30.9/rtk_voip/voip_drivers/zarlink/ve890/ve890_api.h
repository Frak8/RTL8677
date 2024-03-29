
#ifndef __VE890_API_H__
#define __VE890_API_H__

#include "zarlinkCommon.h"

VpStatusType Ve890SetRingCadenceProfile(RTKLineObj *pLine, uint8 ring_cad);
VpStatusType Ve890SetImpedenceCountry(RTKLineObj *pLine, uint8 country); //AC

VpStatusType Ve890SetFxsAcProfileByBand(RTKLineObj *pLine, int pcm_mode);

#if 0
VpProfilePtrType Ve890RingProfile(uint8 profileId);
VpProfilePtrType Ve890AcProfile(uint8 profileId);
#endif

/********** DAA Function **********/

#endif /* __VE890_API_H__*/

