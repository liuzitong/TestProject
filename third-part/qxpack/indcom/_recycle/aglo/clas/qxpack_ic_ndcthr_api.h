#ifndef QXPACK_IC_NDCTHR_API_H
#define QXPACK_IC_NDCTHR_API_H

#include "../qxpack_ic_aglo_def.h"

// ////////////////////////////////////////////////////////////////////////////
//                      lowlevel API
// ////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

//! @brief generate an array from triu by nearest distance classification
//! @param clas_ary       [out]  the class array information pointer, must be alignment to sizeof(int)
//! @param clas_ary_sz    [in]   the class array size in bytes
//! @param dist_triu      [in]   the distance matrix, is a upper triangular
//! @param dist_triu_dim  [in]   the distance matrix dimension
//! @param dist_thresh    [in]   the threshold value used in classify
//! @return if clas_ary is not NULL, return total class number; \n
//!         if clas_ary is NULL,     return required size of clas_ary ( clas_ary_sz )
QXPACK_IC_HIDDEN int  QxPack_IcNdcThrAryFromTriu (
    void *clas_ary, int clas_ary_sz, const float *dist_triu, int dist_triu_dim, float dist_thresh
);

//! @brief generate an NDC table from class array
//! @param clas_tbl      [out]  the class table information pointer, must be alignment to sizeof(int)
//! @param clas_tbl_sz   [in]   the class table size in bytes
//! @param clas_ary      [in]   the class array generated by QxPack_IcClasAryFromDistTriu()
//! @param clas_ary_sz   [in]   the class array size in bytes
//! @return if clas_tbl is not NULL, return the total class number \n
//!         if clas_tbl is NULL,     return required size of clas_tbl
QXPACK_IC_HIDDEN int  QxPack_IcNdcThrTblFromAry (
    void *clas_tbl, int clas_tbl_sz, const void *clas_ary, int clas_ary_sz
);


#ifdef __cplusplus
}
#endif


#endif
