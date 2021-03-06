/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. 8F-30005.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */

#include "ad_daos.h"

#include "hint_fns.h"

void ADIOI_DAOS_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
    static char myname[] = "ADIOI_DAOS_SETINFO";

    if ((fd->info) == MPI_INFO_NULL) {
        /* part of the open call */
        MPI_Info_create(&(fd->info));

        ADIOI_Info_set(fd->info, "romio_daos_chunk_size", "0");
        fd->hints->fs_hints.daos.chunk_size = 0;

        ADIOI_Info_set(fd->info, "romio_daos_obj_class", "OC_UNKNOWN");
        fd->hints->fs_hints.daos.obj_class = OC_UNKNOWN;

        if (users_info != MPI_INFO_NULL) {
            char *oclass = NULL;

            /* Chunk size in each dkey */
            ADIOI_Info_check_and_install_int(fd, users_info, "romio_daos_chunk_size",
                                             &(fd->hints->fs_hints.daos.chunk_size), myname,
                                             error_code);

            /* object class for each file */
            ADIOI_Info_check_and_install_str(fd, users_info, "romio_daos_obj_class",
                                             &oclass, myname, error_code);

            if (oclass) {
                fd->hints->fs_hints.daos.obj_class = daos_oclass_name2id(oclass);
                ADIOI_Free(oclass);
            }
        }
    }

    /* set the values for collective I/O and data sieving parameters */
    ADIOI_GEN_SetInfo(fd, users_info, error_code);

    *error_code = MPI_SUCCESS;
}
