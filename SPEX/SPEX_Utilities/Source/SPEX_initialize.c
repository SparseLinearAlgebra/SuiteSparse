//------------------------------------------------------------------------------
// SPEX_Utilities/SPEX_initialize: initialize SPEX
//------------------------------------------------------------------------------

// SPEX_Utilities: (c) 2019-2023, Christopher Lourenco, Jinhao Chen,
// Lorena Mejia Domenzain, Timothy A. Davis, and Erick Moreno-Centeno.
// All Rights Reserved.
// SPDX-License-Identifier: GPL-2.0-or-later or LGPL-3.0-or-later

//------------------------------------------------------------------------------

// SPEX_initialize initializes the working evironment for SPEX

#include "spex_util_internal.h"

//------------------------------------------------------------------------------
// global variable access
//------------------------------------------------------------------------------

// a global variable, but only accessible within this file.

bool spex_initialize_has_been_called ;
bool spex_initialize_has_been_called = false;

bool spex_initialized ( void )
{
    return (spex_initialize_has_been_called);
}

void spex_set_initialized (bool s)
{
    spex_initialize_has_been_called = s;
}

//------------------------------------------------------------------------------
// SPEX_initialize
//------------------------------------------------------------------------------

SPEX_info SPEX_initialize ( void )
{
    if (spex_initialized( ))
    {
        printf ("SPEX PANIC: already initialized\n") ;
        return (SPEX_PANIC);
    }

    // SPEX requires GMP to support bit counts that are 64-bit integers
    if (sizeof (mp_bitcnt_t) < sizeof (uint64_t))
    {
        printf ("SPEX PANIC: GMP misconfigured\n") ;
        return (SPEX_PANIC);
    }

    // tell GMP and MPFR which memory allocation functions to use
    mp_set_memory_functions
    (
        spex_gmp_allocate,      // malloc function
        spex_gmp_reallocate,    // realloc function
        spex_gmp_free           // free function
    );

    // initialize the SPEX GMP interface for the primary thread
    SPEX_info info = spex_gmp_initialize (1) ;
    if (info != SPEX_OK)
    {
        printf ("SPEX ERROR: GMP not initialized: %d\n", info) ;
    }
    if (info == SPEX_OK)
    {
        spex_set_initialized (true);
    }
    return (info) ;
}

