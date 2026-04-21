#ifndef PORT_BRIDGE_PARTICLE_BANK_BRIDGE_H
#define PORT_BRIDGE_PARTICLE_BANK_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Loads the script+texture BLOB pair for one particle bank from the
 * O2R archive, byte-swaps from big-endian to native, and calls
 * lbParticleSetupBankID so the decomp-layer particle interpreter
 * can drive it.
 *
 *   scripts_lo  — the value the decomp passed into efParticleGetLoadBankID,
 *                 which for the six named banks is &l<Module>ParticleScriptBankLo
 *                 (a distinct per-bank linker-stub address on PC).
 *   bank_id     — the slot assigned by efParticleGetLoadBankID; this is the
 *                 value that lbParticleSetupBankID writes into
 *                 sLBParticleScriptBanksNum[bank_id], etc.
 *
 * Returns 0 on success, negative on failure (all failure paths log via port_log).
 * On failure the bank is registered empty, matching the prior stub behavior. */
int portParticleLoadBank(uintptr_t scripts_lo, int bank_id);

#ifdef __cplusplus
}
#endif

#endif
