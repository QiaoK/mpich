/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2017 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

int MPIR_Reduce_allcomm_nb(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                           MPI_Op op, int root, MPIR_Comm * comm_ptr, MPIR_Errflag_t * errflag)
{
    int mpi_errno = MPI_SUCCESS;
    MPI_Request req = MPI_REQUEST_NULL;
    MPIR_Request *req_ptr = NULL;

    /* just call the nonblocking version and wait on it */
    mpi_errno = MPIR_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm_ptr, &req_ptr);
    MPIR_ERR_CHECK(mpi_errno);
    if (req_ptr)
        req = req_ptr->handle;

    mpi_errno = MPIR_Wait(&req, MPI_STATUS_IGNORE);
    MPIR_ERR_CHECK(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}
