#ifndef _RP_CORE_EXTRA_CONFIG_H
#define _RP_CORE_EXTRA_CONFIG_H

/** Includes base + custom rp_toolkit configuration. */

/* if using Pico SDK, include "pico.h" */
#if defined(PICO_BOARD)
#include "pico.h"
#endif

#if defined(RP_EXTRA_CONFIG_HEADER)
#include RP_EXTRA_CONFIG_HEADER
#endif

#endif
