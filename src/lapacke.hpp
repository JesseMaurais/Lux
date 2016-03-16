#ifndef lapacke_hpp
#define lapacke_hpp

#include <complex>

template <class real> using complex = std::complex<real>;

#define lapack_complex_double complex<double>
#define lapack_complex_float complex<float>
#define lapack_logical bool
#define lapack_int int
#define LAPACK_COMPLEX_CUSTOM

#include <lapacke.h>



///////////////////////////////// PROTOTYPES //////////////////////////////////


// LINEAR EQUATIONS

// s, d, c, z
template <class number>
int gesv(const char order, const int N, const int M, number *A, const int LDA, int *IPIV, number *B, const int LDB);
template <class number>
int gbsv(const char order, const int N, const int KL, const int KU, const int M, number *AB, const int LDAB, int *IPIV, number *B, const int LDB);
template <class number>
int gtsv(const char order, const int N, const int M, number *DL, number *D, number *DU, number *B, const int LDB);
template <class number>
int posv(const char order, const char uplo, const int N, const int M, number *A, const int LDA, number *B, const int LDB);
template <class number> 
int ppsv(const char order, const char uplo, const int N, const int M, number *AP, number *B, const int LDB);
template <class number>
int pbsv(const char order, const char uplo, const int N, const int KD, const int M, number *AB, const int LDAB, number *B, const int LDB);
template <class number, class scalar>
int ptsv(const char order, const int N, const int M, scalar *D, number *E, number *B, const int LDB);
template <class number> 
int sysv(const char order, const char uplo, const int N, const int M, number *A, const int LDA, int *IPIV, number *B, const int LDB);
template <class number>
int spsv(const char order, const char uplo, const int N, const int M, number *AP, int *IPIV, number *B, const int LDB);
// c, z
template <class number>
int hesv(const char order, const char uplo, const int N, const int M, number *A, const int LDA, int *IPIV, number *B, const int LDB);
template <class number>
int hpsv(const char order, const char uplo, const int N, const int M, number *AP, int *IPIV, number *B, const int LDB);

// LINEAR LEAST SQUARES

// s, d, c, z
template <class number>
int gels(const char order, const char trans, const int K, const int N, const int M, number *A, const int LDA, number *B, const int LDB);
template <class number, class scalar>
int gelsy(const char order, const int K, const int N, const int M, number *A, const int LDA, number *B, const int LDB, int *JPVT, scalar r, int *rank);
template <class number, class scalar>
int gelss(const char order, const int K, const int N, const int M, number *A, const int LDA, number *B, const int LDB, scalar *S, scalar r, int *rank);
template <class number, class scalar>
int gelsd(const char order, const int K, const int N, const int M, number *A, const int LDA, number *B, const int LDB, scalar *S, scalar r, int *rank);

// GENERALISED LINEAR LEAST SQUARES

// s, d, c, z
template <class number>
int gglse(const char order, const int N, const int M, const int P, number *A, const int LDA, number *B, const int LDB, number *C, number *D, number *X);
template <class number>
int ggglm(const char order, const int N, const int M, const int P, number *A, const int LDA, number *B, const int LDB, number *D, number *X, number *Y);

// EIGENVALUES AND SINGULAR VALUES

// s, d, c, z
template <class number>
int gesvd(const char order, const int N, const int M, number *A, const int LDA, int *IPIV, number *B, const int LDB);
// s, d
template <class number>
int gees(const char order, const char job, const char sort, const int N, number *A, const int LDA, int *DIM, number *WR, number *WI, number *VS, const int LDVS);
template <class number>
int geev(const char order, const char jobVL, const char jobVR, const int N, number *A, const int LDA, number *WR, number *WI, number *VL, const int LDVL, number *VR, const int LDVR);
template <class number>
int syev(const char order, const char job, const char uplo, const int N, number *A, const int LDA, number *W);
template <class number>
int spev(const char order, const char job, const char uplo, const int N, number *AP, number *W, number *Z, const int LDZ);
template <class number>
int sbev(const char order, const char job, const char uplo, const int N, const int KD, number *AB, const int LDAB, number *W, number *Z, const int LDZ);
template <class number>
int stev(const char order, const char job, const int N, number *D, number *E, number *Z, const int LDZ);
// c, z
template <class number>
int gees(const char order, const char job, const char sort, const int N, number *A, const int LDA, int *DIM, number *W, number *VS, const int LDVS);
template <class number>
int geev(const char order, const char jobVL, const char jobVR, const int N, number *A, const int LDA, number *W, number *VL, const int LDVL, number *VR, const int LDVR);
template <class number>
int heev(const char order, const char job, const char uplo, const int N, complex<number> *A, const int LDA, number *W);
template <class number>
int hpev(const char order, const char job, const char uplo, const int N, complex<number> *AP, number *W, complex<number> *Z, const int LDZ);
template <class number>
int hbev(const char order, const char job, const char uplo, const int N, const int KD, complex<number> *AB, const int LDAB, number *W, complex<number> *Z, const int LDZ);



/////////////////////////// SINGLE PRECISION FLOAT ////////////////////////////


// LINEAR EQUATIONS

template <> inline
int gesv(const char order, const int N, const int M, float *A, const int LDA, int *IPIV, float *B, const int LDB)
{
	return LAPACKE_sgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int gbsv(const char order, const int N, const int KL, const int KU, const int M, float *AB, const int LDAB, int *IPIV, float *B, const int LDB)
{
	return LAPACKE_sgbsv(order, N, KL, KU, M, AB, LDAB, IPIV, B, LDB);
}
template <> inline
int gtsv(const char order, const int N, const int M, float *DL, float *D, float *DU, float *B, const int LDB)
{
	return LAPACKE_sgtsv(order, N, M, DL, D, DU, B, LDB);
}
template <> inline
int posv(const char order, const char uplo, const int N, const int M, float *A, const int LDA, float *B, const int LDB)
{
	return LAPACKE_sposv(order, uplo, N, M, A, LDA, B, LDB);
}
template <> inline
int ppsv(const char order, const char uplo, const int N, const int M, float *AP, float *B, const int LDB)
{
	return LAPACKE_sppsv(order, uplo, N, M, AP, B, LDB);
}
template <> inline
int pbsv(const char order, const char uplo, const int N, const int KD, const int M, float *AB, const int LDAB, float *B, const int LDB)
{
	return  LAPACKE_spbsv(order, uplo, N, KD, M, AB, LDAB, B, LDB);
}
template <> inline
int ptsv(const char order, const int N, const int M, float *D, float *E, float *B, const int LDB)
{
	return LAPACKE_sptsv(order, N, M, D, E, B, LDB);
}
template <> inline
int sysv(const char order, const char uplo, const int N, const int M, float *A, const int LDA, int *IPIV, float *B, const int LDB)
{
	return LAPACKE_ssysv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int spsv(const char order, const char uplo, const int N, const int M, float *AP, int *IPIV, float *B, const int LDB)
{
	return LAPACKE_sspsv(order, uplo, N, M, AP, IPIV, B, LDB);
}

// LINEAR LEAST SQUARES

template <> inline
int gels(const char order, const char trans, const int K, const int N, const int M, float *A, const int LDA, float *B, const int LDB)
{
	return LAPACKE_sgels(order, trans, K, N, M, A, LDA, B, LDB);
}
template <> inline
int gelsy(const char order, const int K, const int N, const int M, float *A, const int LDA, float *B, const int LDB, int *JPVT, float R, int *RANK)
{
	return LAPACKE_sgelsy(order, K, N, M, A, LDA, B, LDB, JPVT, R, RANK);
}
template <> inline
int gelss(const char order, const int K, const int N, const int M, float *A, const int LDA, float *B, const int LDB, float *S, float R, int *RANK)
{
	return LAPACKE_sgelss(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}
template <> inline
int gelsd(const char order, const int K, const int N, const int M, float *A, const int LDA, float *B, const int LDB, float *S, float R, int *RANK)
{
	return LAPACKE_sgelsd(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}

// GENERALISED LINEAR LEAST SQUARES

template <> inline
int gglse(const char order, const int N, const int M, const int P, float *A, const int LDA, float *B, const int LDB, float *C, float *D, float *X)
{
	return LAPACKE_sgglse(order, N, M, P, A, LDA, B, LDB, C, D, X);
}
template <> inline
int ggglm(const char order, const int N, const int M, const int P, float *A, const int LDA, float *B, const int LDB, float *D, float *X, float *Y)
{
	return LAPACKE_sggglm(order, N, M, P, A, LDA, B, LDB, D, X, Y);
}

// EIGENVALUES AND SINGULAR VALUES

template <> inline
int gees(const char order, const char job, const char sort, const int N, float *A, const int LDA, int *DIM, float *WR, float *WI, float *VS, const int LDVS)
{
	return LAPACKE_sgees(order, job, sort, nullptr, N, A, LDA, DIM, WR, WI, VS, LDVS);
}
template <> inline
int geev(const char order, const char jobVL, const char jobVR, const int N, float *A, const int LDA, float *WR, float *WI, float *VL, const int LDVL, float *VR, const int LDVR)
{
	return LAPACKE_sgeev(order, jobVL, jobVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR);
}
template <> inline
int gesvd(const char order, const int N, const int M, float *A, const int LDA, int *IPIV, float *B, const int LDB)
{
	return LAPACKE_sgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int syev(const char order, const char job, const char uplo, const int N, float *A, const int LDA, float *W)
{
	return LAPACKE_ssyev(order, job, uplo, N, A, LDA, W);
}
template <> inline
int spev(const char order, const char job, const char uplo, const int N, float *AP, float *W, float *Z, const int LDZ)
{
	return LAPACKE_sspev(order, job, uplo, N, AP, W, Z, LDZ);
}
template <> inline
int sbev(const char order, const char job, const char uplo, const int N, const int KD, float *AB, const int LDAB, float *W, float *Z, const int LDZ)
{
	return LAPACKE_ssbev(order, job, uplo, N, KD, AB, LDAB, W, Z, LDZ);
}
template <> inline
int stev(const char order, const char job, const int N, float *D, float *E, float *Z, const int LDZ)
{
	return LAPACKE_sstev(order, job, N, D, E, Z, LDZ);
}



/////////////////////////// DOUBLE PRECISION FLOAT ////////////////////////////


// LINEAR EQUATIONS

template <> inline
int gesv(const char order, const int N, const int M, double *A, const int LDA, int *IPIV, double *B, const int LDB)
{
	return LAPACKE_dgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int gbsv(const char order, const int N, const int KL, const int KU, const int M, double *AB, const int LDAB, int *IPIV, double *B, const int LDB)
{
	return LAPACKE_dgbsv(order, N, KL, KU, M, AB, LDAB, IPIV, B, LDB);
}
template <> inline
int gtsv(const char order, const int N, const int M, double *DL, double *D, double *DU, double *B, const int LDB)
{
	return LAPACKE_dgtsv(order, N, M, DL, D, DU, B, LDB);
}
template <> inline
int posv(const char order, const char uplo, const int N, const int M, double *A, const int LDA, double *B, const int LDB)
{
	return LAPACKE_dposv(order, uplo, N, M, A, LDA, B, LDB);
}
template <> inline
int ppsv(const char order, const char uplo, const int N, const int M, double *AP, double *B, const int LDB)
{
	return LAPACKE_dppsv(order, uplo, N, M, AP, B, LDB);
}
template <> inline
int pbsv(const char order, const char uplo, const int N, const int KD, const int M, double *AB, const int LDAB, double *B, const int LDB)
{
	return LAPACKE_dpbsv(order, uplo, N, KD, M, AB, LDAB, B, LDB);
}
template <> inline
int ptsv(const char order, const int N, const int M, double *D, double *E, double *B, const int LDB)
{
	return LAPACKE_dptsv(order, N, M, D, E, B, LDB);
}
template <> inline
int sysv(const char order, const char uplo, const int N, const int M, double *A, const int LDA, int *IPIV, double *B, const int LDB)
{
	return LAPACKE_dsysv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int spsv(const char order, const char uplo, const int N, const int M, double *AP, int *IPIV, double *B, const int LDB)
{
	return LAPACKE_dspsv(order, uplo, N, M, AP, IPIV, B, LDB);
}

// LINEAR LEAST SQUARES

template <> inline
int gels(const char order, const char trans, const int K, const int N, const int M, double *A, const int LDA, double *B, const int LDB)
{
	return LAPACKE_dgels(order, trans, K, N, M, A, LDA, B, LDB);
}
template <> inline
int gelsy(const char order, const int K, const int N, const int M, double *A, const int LDA, double *B, const int LDB, int *JPVT, double R, int *RANK)
{
	return LAPACKE_dgelsy(order, K, N, M, A, LDA, B, LDB, JPVT, R, RANK);
}
template <> inline
int gelss(const char order, const int K, const int N, const int M, double *A, const int LDA, double *B, const int LDB, double *S, double R, int *RANK)
{
	return LAPACKE_dgelss(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}
template <> inline
int gelsd(const char order, const int K, const int N, const int M, double *A, const int LDA, double *B, const int LDB, double *S, double R, int *RANK)
{
	return LAPACKE_dgelsd(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}

// GENERALISED LINEAR LEAST SQUARES

template <> inline
int gglse(const char order, const int N, const int M, const int P, double *A, const int LDA, double *B, const int LDB, double *C, double *D, double *X)
{
	return LAPACKE_dgglse(order, N, M, P, A, LDA, B, LDB, C, D, X);
}
template <> inline
int ggglm(const char order, const int N, const int M, const int P, double *A, const int LDA, double *B, const int LDB, double *D, double *X, double *Y)
{
	return LAPACKE_dggglm(order, N, M, P, A, LDA, B, LDB, D, X, Y);
}

// EIGENVALUES AND SINGULAR VALUES

template <> inline
int gees(const char order, const char job, const char sort, const int N, double *A, const int LDA, int *DIM, double *WR, double *WI, double *VS, const int LDVS)
{
	return LAPACKE_dgees(order, job, sort, nullptr, N, A, LDA, DIM, WR, WI, VS, LDVS);
}
template <> inline
int geev(const char order, const char jobVL, const char jobVR, const int N, double *A, const int LDA, double *WR, double *WI, double *VL, const int LDVL, double *VR, const int LDVR)
{
	return LAPACKE_dgeev(order, jobVL, jobVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR);
}
template <> inline
int gesvd(const char order, const int N, const int M, double *A, const int LDA, int *IPIV, double *B, const int LDB)
{
	return LAPACKE_dgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int syev(const char order, const char job, const char uplo, const int N, double *A, const int LDA, double *W)
{
	return LAPACKE_dsyev(order, job, uplo, N, A, LDA, W);
}
template <> inline
int spev(const char order, const char job, const char uplo, const int N, double *AP, double *W, double *Z, const int LDZ)
{
	return LAPACKE_dspev(order, job, uplo, N, AP, W, Z, LDZ);
}
template <> inline
int sbev(const char order, const char job, const char uplo, const int N, const int KD, double *AB, const int LDAB, double *W, double *Z, const int LDZ)
{
	return LAPACKE_dsbev(order, job, uplo, N, KD, AB, LDAB, W, Z, LDZ);
}
template <> inline
int stev(const char order, const char job, const int N, double *D, double *E, double *Z, const int LDZ)
{
	return LAPACKE_dstev(order, job, N, D, E, Z, LDZ);
}



////////////////////////// SINGLE PRECISION COMPLEX ///////////////////////////


// LINEAR EQUATIONS

template <> inline
int gesv(const char order, const int N, const int M, complex<float> *A, const int LDA, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_cgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int gbsv(const char order, const int N, const int KL, const int KU, const int M, complex<float> *AB, const int LDAB, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_cgbsv(order, N, KL, KU, M, AB, LDAB, IPIV, B, LDB);
}
template <> inline
int gtsv(const char order, const int N, const int M, complex<float> *DL, complex<float> *D, complex<float> *DU, complex<float> *B, const int LDB)
{
	return LAPACKE_cgtsv(order, N, M, DL, D, DU, B, LDB);
}
template <> inline
int posv(const char order, const char uplo, const int N, const int M, complex<float> *A, const int LDA, complex<float> *B, const int LDB)
{
	return LAPACKE_cposv(order, uplo, N, M, A, LDA, B, LDB);
}
template <> inline
int ppsv(const char order, const char uplo, const int N, const int M, complex<float> *AP, complex<float> *B, const int LDB)
{
	return LAPACKE_cppsv(order, uplo, N, M, AP, B, LDB);
}
template <> inline
int pbsv(const char order, const char uplo, const int N, const int KD, const int M, complex<float> *AB, const int LDAB, complex<float> *B, const int LDB)
{
	return  LAPACKE_cpbsv(order, uplo, N, KD, M, AB, LDAB, B, LDB);
}
template <> inline
int ptsv(const char order, const int N, const int M, float *D, complex<float> *E, complex<float> *B, const int LDB)
{
	return LAPACKE_cptsv(order, N, M, D, E, B, LDB);
}
template <> inline
int sysv(const char order, const char uplo, const int N, const int M, complex<float> *A, const int LDA, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_csysv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int spsv(const char order, const char uplo, const int N, const int M, complex<float> *AP, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_cspsv(order, uplo, N, M, AP, IPIV, B, LDB);
}
template <> inline
int hesv(const char order, const char uplo, const int N, const int M, complex<float> *A, const int LDA, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_chesv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int hpsv(const char order, const char uplo, const int N, const int M, complex<float> *AP, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_chpsv(order, uplo, N, M, AP, IPIV, B, LDB);
}

// LINEAR LEAST SQUARES

template <> inline
int gels(const char order, const char trans, const int K, const int N, const int M, complex<float> *A, const int LDA, complex<float> *B, const int LDB)
{
	return LAPACKE_cgels(order, trans, K, N, M, A, LDA, B, LDB);
}
template <> inline
int gelsy(const char order, const int K, const int N, const int M, complex<float> *A, const int LDA, complex<float> *B, const int LDB, int *JPVT, float R, int *RANK)
{
	return LAPACKE_cgelsy(order, K, N, M, A, LDA, B, LDB, JPVT, R, RANK);
}
template <> inline
int gelss(const char order, const int K, const int N, const int M, complex<float> *A, const int LDA, complex<float> *B, const int LDB, float *S, float R, int *RANK)
{
	return LAPACKE_cgelss(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}
template <> inline
int gelsd(const char order, const int K, const int N, const int M, complex<float> *A, const int LDA, complex<float> *B, const int LDB, float *S, float R, int *RANK)
{
	return LAPACKE_cgelsd(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}

// GENERALISED LINEAR LEAST SQUARES

template <> inline
int gglse(const char order, const int N, const int M, const int P, complex<float> *A, const int LDA, complex<float> *B, const int LDB, complex<float> *C, complex<float> *D, complex<float> *X)
{
	return LAPACKE_cgglse(order, N, M, P, A, LDA, B, LDB, C, D, X);
}
template <> inline
int ggglm(const char order, const int N, const int M, const int P, complex<float> *A, const int LDA, complex<float> *B, const int LDB, complex<float> *D, complex<float> *X, complex<float> *Y)
{
	return LAPACKE_cggglm(order, N, M, P, A, LDA, B, LDB, D, X, Y);
}

// EIGENVALUES AND SINGULAR VALUES

template <> inline
int gees(const char order, const char job, const char sort, const int N, complex<float> *A, const int LDA, int *DIM, complex<float> *W, complex<float> *VS, const int LDVS)
{
	return LAPACKE_cgees(order, job, sort, nullptr, N, A, LDA, DIM, W, VS, LDVS);
}
template <> inline
int geev(const char order, const char jobVL, const char jobVR, const int N, complex<float> *A, const int LDA, complex<float> *W, complex<float> *VL, const int LDVL, complex<float> *VR, const int LDVR)
{
	return LAPACKE_cgeev(order, jobVL, jobVR, N, A, LDA, W, VL, LDVL, VR, LDVR);
}
template <> inline
int gesvd(const char order, const int N, const int M, complex<float> *A, const int LDA, int *IPIV, complex<float> *B, const int LDB)
{
	return LAPACKE_cgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int heev(const char order, const char job, const char uplo, const int N, complex<float> *A, const int LDA, float *W)
{
	return LAPACKE_cheev(order, job, uplo, N, A, LDA, W);
}
template <> inline
int hpev(const char order, const char job, const char uplo, const int N, complex<float> *AP, float *W, complex<float> *Z, const int LDZ)
{
	return LAPACKE_chpev(order, job, uplo, N, AP, W, Z, LDZ);
}
template <> inline
int hbev(const char order, const char job, const char uplo, const int N, const int KD, complex<float> *AB, const int LDAB, float *W, complex<float> *Z, const int LDZ)
{
	return LAPACKE_chbev(order, job, uplo, N, KD, AB, LDAB, W, Z, LDZ);
}



////////////////////////// DOUBLE PRECISION COMPLEX ///////////////////////////


// Linear Equations

template <> inline
int gesv(const char order, const int N, const int M, complex<double> *A, const int LDA, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int gbsv(const char order, const int N, const int KL, const int KU, const int M, complex<double> *AB, const int LDAB, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zgbsv(order, N, KL, KU, M, AB, LDAB, IPIV, B, LDB);
}
template <> inline
int gtsv(const char order, const int N, const int M, complex<double> *DL, complex<double> *D, complex<double> *DU, complex<double> *B, const int LDB)
{
	return LAPACKE_zgtsv(order, N, M, DL, D, DU, B, LDB);
}
template <> inline
int posv(const char order, const char uplo, const int N, const int M, complex<double> *A, const int LDA, complex<double> *B, const int LDB)
{
	return LAPACKE_zposv(order, uplo, N, M, A, LDA, B, LDB);
}
template <> inline
int ppsv(const char order, const char uplo, const int N, const int M, complex<double> *AP, complex<double> *B, const int LDB)
{
	return LAPACKE_zppsv(order, uplo, N, M, AP, B, LDB);
}
template <> inline
int pbsv(const char order, const char uplo, const int N, const int KD, const int M, complex<double> *AB, const int LDAB, complex<double> *B, const int LDB)
{
	return  LAPACKE_zpbsv(order, uplo, N, KD, M, AB, LDAB, B, LDB);
}
template <> inline
int ptsv(const char order, const int N, const int M, double *D, complex<double> *E, complex<double> *B, const int LDB)
{
	return LAPACKE_zptsv(order, N, M, D, E, B, LDB);
}
template <> inline
int sysv(const char order, const char uplo, const int N, const int M, complex<double> *A, const int LDA, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zsysv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int spsv(const char order, const char uplo, const int N, const int M, complex<double> *AP, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zspsv(order, uplo, N, M, AP, IPIV, B, LDB);
}
template <> inline
int hesv(const char order, const char uplo, const int N, const int M, complex<double> *A, const int LDA, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zhesv(order, uplo, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int hpsv(const char order, const char uplo, const int N, const int M, complex<double> *AP, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zhpsv(order, uplo, N, M, AP, IPIV, B, LDB);
}

// LINEAR LEAST SQUARES

template <> inline
int gels(const char order, const char trans, const int K, const int N, const int M, complex<double> *A, const int LDA, complex<double> *B, const int LDB)
{
	return LAPACKE_zgels(order, trans, K, N, M, A, LDA, B, LDB);
}
template <> inline
int gelsy(const char order, const int K, const int N, const int M, complex<double> *A, const int LDA, complex<double> *B, const int LDB, int *JPVT, double R, int *RANK)
{
	return LAPACKE_zgelsy(order, K, N, M, A, LDA, B, LDB, JPVT, R, RANK);
}
template <> inline
int gelss(const char order, const int K, const int N, const int M, complex<double> *A, const int LDA, complex<double> *B, const int LDB, double *S, double R, int *RANK)
{
	return LAPACKE_zgelss(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}
template <> inline
int gelsd(const char order, const int K, const int N, const int M, complex<double> *A, const int LDA, complex<double> *B, const int LDB, double *S, double R, int *RANK)
{
	return LAPACKE_zgelsd(order, K, N, M, A, LDA, B, LDB, S, R, RANK);
}

// GENERALISED LINEAR LEAST SQUARES

template <> inline
int gglse(const char order, const int N, const int M, const int P, complex<double> *A, const int LDA, complex<double> *B, const int LDB, complex<double> *C, complex<double> *D, complex<double> *X)
{
	return LAPACKE_zgglse(order, N, M, P, A, LDA, B, LDB, C, D, X);
}
template <> inline
int ggglm(const char order, const int N, const int M, const int P, complex<double> *A, const int LDA, complex<double> *B, const int LDB, complex<double> *D, complex<double> *X, complex<double> *Y)
{
	return LAPACKE_zggglm(order, N, M, P, A, LDA, B, LDB, D, X, Y);
}

// EIGENVALUES AND SINGULAR VALUES

template <> inline
int gees(const char order, const char job, const char sort, const int N, complex<double> *A, const int LDA, int *DIM, complex<double> *W, complex<double> *VS, const int LDVS)
{
	return LAPACKE_zgees(order, job, sort, nullptr, N, A, LDA, DIM, W, VS, LDVS);
}
template <> inline
int geev(const char order, const char jobVL, const char jobVR, const int N, complex<double> *A, const int LDA, complex<double> *W, complex<double> *VL, const int LDVL, complex<double> *VR, const int LDVR)
{
	return LAPACKE_zgeev(order, jobVL, jobVR, N, A, LDA, W, VL, LDVL, VR, LDVR);
}
template <> inline
int gesvd(const char order, const int N, const int M, complex<double> *A, const int LDA, int *IPIV, complex<double> *B, const int LDB)
{
	return LAPACKE_zgesv(order, N, M, A, LDA, IPIV, B, LDB);
}
template <> inline
int heev(const char order, const char job, const char uplo, const int N, complex<double> *A, const int LDA, double *W)
{
	return LAPACKE_zheev(order, job, uplo, N, A, LDA, W);
}
template <> inline
int hpev(const char order, const char job, const char uplo, const int N, complex<double> *AP, double *W, complex<double> *Z, const int LDZ)
{
	return LAPACKE_zhpev(order, job, uplo, N, AP, W, Z, LDZ);
}
template <> inline
int hbev(const char order, const char job, const char uplo, const int N, const int KD, complex<double> *AB, const int LDAB, double *W, complex<double> *Z, const int LDZ)
{
	return LAPACKE_zhbev(order, job, uplo, N, KD, AB, LDAB, W, Z, LDZ);
}



#endif // file

