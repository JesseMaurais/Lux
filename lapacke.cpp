/**
 * LAPACKE module for Lua
 *
 * Linear Algebra PACKage is the defacto standard for high-end computation of
 * of common expressions in linear algebra and finding the solution to linear
 * systems of equations. Like BLAS it was first written in FORTRAN 77 but has
 * been ported to a number of different languages. The LAPACKE, used here, is 
 * the preferend C binding to the Fortran library. Lux uses C++ to bind the C
 * code to Lua. So there's 4 languages at use here, or maybe 3.5 if you count
 * C++ as only partially above C. 
 *
 * Several functions ending with "*fsx" and "*vxx" are absent from the shared
 * library even though they are defined in the header. They are commented out
 * in the registry below to prevent linking errors when importing the module.
 */

// Use boolean for Lua
#define lapack_logical bool

// Complex as pairs of floats
#define LAPACK_COMPLEX_CUSTOM
#define lapack_complex_float float
#define lapack_complex_double double

#include <lapacke.h>
#include "lux.hpp"

// Put library prefix in the table, omit in name
#define REG(reg) {#reg, lux_cast(LAPACKE_##reg)}

extern "C" int luaopen_lapacke(lua_State *state)
{
	luaL_Reg regs[] =
	{
	// SINGLE PRECISION REAL
	REG(sbdsdc), REG(sbdsqr), REG(sdisna), REG(sgbbrd), REG(sgbcon),
	REG(sgbequ), REG(sgbequb), REG(sgbrfs),/*REG(sgbrfsx),*/REG(sgbsv),
	REG(sgbsvx),/*REG(sgbsvxx),*/REG(sgbtrf), REG(sgbtrs), REG(sgebak),
	REG(sgebal), REG(sgebrd), REG(sgecon), REG(sgeequ), REG(sgeequb),
	REG(sgees), REG(sgeesx), REG(sgeev), REG(sgeevx), REG(sgehrd),
	REG(sgejsv), REG(sgelqf), REG(sgels), REG(sgelsd), REG(sgelss),
	REG(sgelsy), REG(sgeqlf), REG(sgeqp3), REG(sgeqpf), REG(sgeqrf),
	REG(sgeqrfp), REG(sgerfs),/*REG(sgerfsx),*/REG(sgerqf), REG(sgesdd),
	REG(sgesv), REG(sgesvd), REG(sgesvj), REG(sgesvx), /*REG(sgesvxx),*/
	REG(sgetrf), REG(sgetri), REG(sgetrs), REG(sggbak), REG(sggbal),
	REG(sgges), REG(sggesx), REG(sggev), REG(sggevx), REG(sggglm),
	REG(sgghrd), REG(sgglse), REG(sggqrf), REG(sggrqf), REG(sggsvd),
	REG(sggsvp), REG(sgtcon), REG(sgtrfs), REG(sgtsv), REG(sgtsvx),
	REG(sgttrf), REG(sgttrs), REG(shgeqz), REG(shsein), REG(shseqr),
	REG(sopgtr), REG(sopmtr), REG(sorgbr), REG(sorghr), REG(sorglq),
	REG(sorgql), REG(sorgqr), REG(sorgrq), REG(sorgtr), REG(sormbr),
	REG(sormhr), REG(sormlq), REG(sormql), REG(sormqr), REG(sormrq),
	REG(sormrz), REG(sormtr), REG(spbcon), REG(spbequ), REG(spbrfs),
	REG(spbstf), REG(spbsv), REG(spbsvx), REG(spbtrf), REG(spbtrs),
	REG(spftrf), REG(spftri), REG(spftrs), REG(spocon), REG(spoequ),
	REG(spoequb), REG(sporfs),/*REG(sporfsx),*/REG(sposv), REG(sposvx),
	/*REG(sposvxx),*/REG(spotrf), REG(spotri), REG(spotrs), REG(sppcon),
	REG(sppequ), REG(spprfs), REG(sppsv), REG(sppsvx), REG(spptrf),
	REG(spptri), REG(spptrs), REG(spstrf), REG(sptcon), REG(spteqr),
	REG(sptrfs), REG(sptsv), REG(sptsvx), REG(spttrf), REG(spttrs),
	REG(ssbev), REG(ssbevd), REG(ssbevx), REG(ssbgst), REG(ssbgv),
	REG(ssbgvd), REG(ssbgvx), REG(ssbtrd), REG(ssfrk), REG(sspcon),
	REG(sspev), REG(sspevd), REG(sspevx), REG(sspgst), REG(sspgv),
	REG(sspgvd), REG(sspgvx), REG(ssprfs), REG(sspsv), REG(sspsvx),
	REG(ssptrd), REG(ssptrf), REG(ssptri), REG(ssptrs), REG(sstebz),
	REG(sstedc), REG(sstegr), REG(sstein), REG(sstemr), REG(ssteqr),
	REG(ssterf), REG(sstev), REG(sstevd), REG(sstevr), REG(sstevx),
	REG(ssycon), REG(ssyequb), REG(ssyev), REG(ssyevd), REG(ssyevr),
	REG(ssyevx), REG(ssygst), REG(ssygv), REG(ssygvd), REG(ssygvx),
	REG(ssyrfs),/*REG(ssyrfsx),*/REG(ssysv), REG(ssysvx),/*REG(ssysvxx),*/
	REG(ssytrd), REG(ssytrf), REG(ssytri), REG(ssytrs), REG(stbcon),
	REG(stbrfs), REG(stbtrs), REG(stfsm), REG(stftri), REG(stfttp),
	REG(stfttr), REG(stgevc), REG(stgexc), REG(stgsen), REG(stgsja),
	REG(stgsna), REG(stgsyl), REG(stpcon), REG(stprfs), REG(stptri),
	REG(stptrs), REG(stpttf), REG(stpttr), REG(strcon), REG(strevc),
	REG(strexc), REG(strrfs), REG(strsen), REG(strsna), REG(strsyl),
	REG(strtri), REG(strtrs), REG(strttf), REG(strttp), REG(stzrzf),
	// DOUBLE PRECISION REAL
	REG(dbdsdc), REG(dbdsqr), REG(ddisna), REG(dgbbrd), REG(dgbcon),
	REG(dgbequ), REG(dgbequb), REG(dgbrfs),/*REG(dgbrfsx),*/REG(dgbsv),
	REG(dgbsvx),/*REG(dgbsvxx),*/REG(dgbtrf), REG(dgbtrs), REG(dgebak),
	REG(dgebal), REG(dgebrd), REG(dgecon), REG(dgeequ), REG(dgeequb),
	REG(dgees), REG(dgeesx), REG(dgeev), REG(dgeevx), REG(dgehrd),
	REG(dgejsv), REG(dgelqf), REG(dgels), REG(dgelsd), REG(dgelss),
	REG(dgelsy), REG(dgeqlf), REG(dgeqp3), REG(dgeqpf), REG(dgeqrf),
	REG(dgeqrfp), REG(dgerfs),/*REG(dgerfsx),*/REG(dgerqf), REG(dgesdd),
	REG(dgesv), REG(dgesvd), REG(dgesvj), REG(dgesvx),/*REG(dgesvxx),*/
	REG(dgetrf), REG(dgetri), REG(dgetrs), REG(dggbak), REG(dggbal),
	REG(dgges), REG(dggesx), REG(dggev), REG(dggevx), REG(dggglm),
	REG(dgghrd), REG(dgglse), REG(dggqrf), REG(dggrqf), REG(dggsvd),
	REG(dggsvp), REG(dgtcon), REG(dgtrfs), REG(dgtsv), REG(dgtsvx),
	REG(dgttrf), REG(dgttrs), REG(dhgeqz), REG(dhsein), REG(dhseqr),
	REG(dopgtr), REG(dopmtr), REG(dorgbr), REG(dorghr), REG(dorglq),
	REG(dorgql), REG(dorgqr), REG(dorgrq), REG(dorgtr), REG(dormbr),
	REG(dormhr), REG(dormlq), REG(dormql), REG(dormqr), REG(dormrq),
	REG(dormrz), REG(dormtr), REG(dpbcon), REG(dpbequ), REG(dpbrfs),
	REG(dpbstf), REG(dpbsv), REG(dpbsvx), REG(dpbtrf), REG(dpbtrs),
	REG(dpftrf), REG(dpftri), REG(dpftrs), REG(dpocon), REG(dpoequ),
	REG(dpoequb), REG(dporfs),/*REG(dporfsx),*/REG(dposv), REG(dposvx),
	/*REG(dposvxx),*/REG(dpotrf), REG(dpotri), REG(dpotrs), REG(dppcon),
	REG(dppequ), REG(dpprfs), REG(dppsv), REG(dppsvx), REG(dpptrf),
	REG(dpptri), REG(dpptrs), REG(dpstrf), REG(dptcon), REG(dpteqr),
	REG(dptrfs), REG(dptsv), REG(dptsvx), REG(dpttrf), REG(dpttrs),
	REG(dsbev), REG(dsbevd), REG(dsbevx), REG(dsbgst), REG(dsbgv),
	REG(dsbgvd), REG(dsbgvx), REG(dsbtrd), REG(dsfrk), REG(dspcon),
	REG(dspev), REG(dspevd), REG(dspevx), REG(dspgst), REG(dspgv),
	REG(dspgvd), REG(dspgvx), REG(dsprfs), REG(dspsv), REG(dspsvx),
	REG(dsptrd), REG(dsptrf), REG(dsptri), REG(dsptrs), REG(dstebz),
	REG(dstedc), REG(dstegr), REG(dstein), REG(dstemr), REG(dsteqr),
	REG(dsterf), REG(dstev), REG(dstevd), REG(dstevr), REG(dstevx),
	REG(dsycon), REG(dsyequb), REG(dsyev), REG(dsyevd), REG(dsyevr),
	REG(dsyevx), REG(dsygst), REG(dsygv), REG(dsygvd), REG(dsygvx),
	REG(dsyrfs),/*REG(dsyrfsx),*/REG(dsysv), REG(dsysvx),/*REG(dsysvxx),*/
	REG(dsytrd), REG(dsytrf), REG(dsytri), REG(dsytrs), REG(dtbcon),
	REG(dtbrfs), REG(dtbtrs), REG(dtfsm), REG(dtftri), REG(dtfttp),
	REG(dtfttr), REG(dtgevc), REG(dtgexc), REG(dtgsen), REG(dtgsja),
	REG(dtgsna), REG(dtgsyl), REG(dtpcon), REG(dtprfs), REG(dtptri),
	REG(dtptrs), REG(dtpttf), REG(dtpttr), REG(dtrcon), REG(dtrevc),
	REG(dtrexc), REG(dtrrfs), REG(dtrsen), REG(dtrsna), REG(dtrsyl),
	REG(dtrtri), REG(dtrtrs), REG(dtrttf), REG(dtrttp), REG(dtzrzf),
	// SINGLE PRECISION COMPLEX
	REG(cbdsqr), REG(cgbbrd), REG(cgbcon), REG(cgbequ), REG(cgbequb),
	REG(cgbrfs),/*REG(cgbrfsx),*/REG(cgbsv), REG(cgbsvx),/*REG(cgbsvxx),*/
	REG(cgbtrf), REG(cgbtrs), REG(cgebak), 	REG(cgebal), REG(cgebrd),
	REG(cgecon), REG(cgeequ), REG(cgeequb), REG(cgees), REG(cgeesx),
	REG(cgeev), REG(cgeevx), REG(cgehrd), REG(cgelqf), REG(cgels),
	REG(cgelsd), REG(cgelss), REG(cgelsy), REG(cgeqlf), REG(cgeqp3),
	REG(cgeqpf), REG(cgeqrf), REG(cgeqrfp), REG(cgerfs),/*REG(cgerfsx),*/
	REG(cgerqf), REG(cgesdd), REG(cgesv), REG(cgesvd), REG(cgesvx),
	/*REG(cgesvxx),*/REG(cgetrf), REG(cgetri), REG(cgetrs), REG(cggbak),
	REG(cggbal), REG(cgges), REG(cggesx), REG(cggev), REG(cggevx),
	REG(cggglm), REG(cgghrd), REG(cgglse), REG(cggqrf), REG(cggrqf),
	REG(cggsvd), REG(cggsvp), REG(cgtcon), REG(cgtrfs), REG(cgtsv),
	REG(cgtsvx), REG(cgttrf), REG(cgttrs), REG(chbev), REG(chbevd),
	REG(chbevx), REG(chbgst), REG(chbgv), REG(chbgvd), REG(chbgvx),
	REG(chbtrd), REG(checon), REG(cheequb), REG(cheev), REG(cheevd),
	REG(cheevr), REG(cheevx), REG(chegst), REG(chegv), REG(chegvd),
	REG(chegvx), REG(cherfs),/*REG(cherfsx),*/REG(chesv), REG(chesvx),
	/*REG(chesvxx),*/REG(chetrd), REG(chetrf), REG(chetri), REG(chetrs),
	REG(chfrk), REG(chgeqz), REG(chpcon), REG(chpev), REG(chpevd),
	REG(chpevx), REG(chpgst), REG(chpgv), REG(chpgvd), REG(chpgvx),
	REG(chprfs), REG(chpsv), REG(chpsvx), REG(chptrd), REG(chptrf),
	REG(chptri), REG(chptrs), REG(chsein), REG(chseqr), REG(cpbcon),
	REG(cpbequ), REG(cpbrfs), REG(cpbstf), REG(cpbsv), REG(cpbsvx),
	REG(cpbtrf), REG(cpbtrs), REG(cpftrf), REG(cpftri), REG(cpftrs),
	REG(cpocon), REG(cpoequ), REG(cpoequb), REG(cporfs),/*REG(cporfsx),*/
	REG(cposv), REG(cposvx),/*REG(cposvxx),*/REG(cpotrf), REG(cpotri),
	REG(cpotrs), REG(cppcon), REG(cppequ), REG(cpprfs), REG(cppsv),
	REG(cppsvx), REG(cpptrf), REG(cpptri), REG(cpptrs), REG(cpstrf),
	REG(cptcon), REG(cpteqr), REG(cptrfs), REG(cptsv), REG(cptsvx),
	REG(cpttrf), REG(cpttrs), REG(cspcon), REG(csprfs), REG(cspsv),
	REG(cspsvx), REG(csptrf), REG(csptri), REG(csptrs), REG(cstedc),
	REG(cstegr), REG(cstein), REG(cstemr), REG(csteqr), REG(csycon),
	REG(csyequb), REG(csyrfs),/*REG(csyrfsx),*/REG(csysv), REG(csysvx),
	/*REG(csysvxx),*/REG(csytrf), REG(csytri), REG(csytrs), REG(ctbcon),
	REG(ctbrfs), REG(ctbtrs), REG(ctfsm), REG(ctftri), REG(ctfttp),
	REG(ctfttr), REG(ctgevc), REG(ctgexc), REG(ctgsen), REG(ctgsja),
	REG(ctgsna), REG(ctgsyl), REG(ctpcon), REG(ctprfs), REG(ctptri),
	REG(ctptrs), REG(ctpttf), REG(ctpttr), REG(ctrcon), REG(ctrevc),
	REG(ctrexc), REG(ctrrfs), REG(ctrsen), REG(ctrsna), REG(ctrsyl),
	REG(ctrtri), REG(ctrtrs), REG(ctrttf), REG(ctrttp), REG(ctzrzf),
	REG(cungbr), REG(cunghr), REG(cunglq), REG(cungql), REG(cungqr),
	REG(cungrq), REG(cungtr), REG(cunmbr), REG(cunmhr), REG(cunmlq),
	REG(cunmql), REG(cunmqr), REG(cunmrq), REG(cunmrz), REG(cunmtr),
	REG(cupgtr), REG(cupmtr),
	// DOUBLE PRECISION COMPLEX
	REG(zbdsqr), REG(zgbbrd), REG(zgbcon), REG(zgbequ), REG(zgbequb),
	REG(zgbrfs),/*REG(zgbrfsx),*/REG(zgbsv), REG(zgbsvx),/*REG(zgbsvxx),*/
	REG(zgbtrf), REG(zgbtrs), REG(zgebak), 	REG(zgebal), REG(zgebrd),
	REG(zgecon), REG(zgeequ), REG(zgeequb), REG(zgees), REG(zgeesx),
	REG(zgeev), REG(zgeevx), REG(zgehrd), REG(zgelqf), REG(zgels),
	REG(zgelsd), REG(zgelss), REG(zgelsy), REG(zgeqlf), REG(zgeqp3),
	REG(zgeqpf), REG(zgeqrf), REG(zgeqrfp), REG(zgerfs),/*REG(zgerfsx),*/
	REG(zgerqf), REG(zgesdd), REG(zgesv), REG(zgesvd), REG(zgesvx),
	/*REG(zgesvxx),*/REG(zgetrf), REG(zgetri), REG(zgetrs), REG(zggbak),
	REG(zggbal), REG(zgges), REG(zggesx), REG(zggev), REG(zggevx),
	REG(zggglm), REG(zgghrd), REG(zgglse), REG(zggqrf), REG(zggrqf),
	REG(zggsvd), REG(zggsvp), REG(zgtcon), REG(zgtrfs), REG(zgtsv),
	REG(zgtsvx), REG(zgttrf), REG(zgttrs), REG(zhbev), REG(zhbevd),
	REG(zhbevx), REG(zhbgst), REG(zhbgv), REG(zhbgvd), REG(zhbgvx),
	REG(zhbtrd), REG(zhecon), REG(zheequb), REG(zheev), REG(zheevd),
	REG(zheevr), REG(zheevx), REG(zhegst), REG(zhegv), REG(zhegvd),
	REG(zhegvx), REG(zherfs),/*REG(zherfsx),*/REG(zhesv), REG(zhesvx),
	/*REG(zhesvxx),*/REG(zhetrd), REG(zhetrf), REG(zhetri), REG(zhetrs),
	REG(zhfrk), REG(zhgeqz), REG(zhpcon), REG(zhpev), REG(zhpevd),
	REG(zhpevx), REG(zhpgst), REG(zhpgv), REG(zhpgvd), REG(zhpgvx),
	REG(zhprfs), REG(zhpsv), REG(zhpsvx), REG(zhptrd), REG(zhptrf),
	REG(zhptri), REG(zhptrs), REG(zhsein), REG(zhseqr), REG(zpbcon),
	REG(zpbequ), REG(zpbrfs), REG(zpbstf), REG(zpbsv), REG(zpbsvx),
	REG(zpbtrf), REG(zpbtrs), REG(zpftrf), REG(zpftri), REG(zpftrs),
	REG(zpocon), REG(zpoequ), REG(zpoequb), REG(zporfs),/*REG(zporfsx),*/
	REG(zposv), REG(zposvx),/*REG(zposvxx),*/REG(zpotrf), REG(zpotri),
	REG(zpotrs), REG(zppcon), REG(zppequ), REG(zpprfs), REG(zppsv),
	REG(zppsvx), REG(zpptrf), REG(zpptri), REG(zpptrs), REG(zpstrf),
	REG(zptcon), REG(zpteqr), REG(zptrfs), REG(zptsv), REG(zptsvx),
	REG(zpttrf), REG(zpttrs), REG(zspcon), REG(zsprfs), REG(zspsv),
	REG(zspsvx), REG(zsptrf), REG(zsptri), REG(zsptrs), REG(zstedc),
	REG(zstegr), REG(zstein), REG(zstemr), REG(zsteqr), REG(zsycon),
	REG(zsyequb), REG(zsyrfs),/*REG(zsyrfsx),*/REG(zsysv), REG(zsysvx),
	/*REG(zsysvxx),*/REG(zsytrf), REG(zsytri), REG(zsytrs), REG(ztbcon),
	REG(ztbrfs), REG(ztbtrs), REG(ztfsm), REG(ztftri), REG(ztfttp),
	REG(ztfttr), REG(ztgevc), REG(ztgexc), REG(ztgsen), REG(ztgsja),
	REG(ztgsna), REG(ztgsyl), REG(ztpcon), REG(ztprfs), REG(ztptri),
	REG(ztptrs), REG(ztpttf), REG(ztpttr), REG(ztrcon), REG(ztrevc),
	REG(ztrexc), REG(ztrrfs), REG(ztrsen), REG(ztrsna), REG(ztrsyl),
	REG(ztrtri), REG(ztrtrs), REG(ztrttf), REG(ztrttp), REG(ztzrzf),
	REG(zungbr), REG(zunghr), REG(zunglq), REG(zungql), REG(zungqr),
	REG(zungrq), REG(zungtr), REG(zunmbr), REG(zunmhr), REG(zunmlq),
	REG(zunmql), REG(zunmqr), REG(zunmrq), REG(zunmrz), REG(zunmtr),
	REG(zupgtr), REG(zupmtr),
	{nullptr}
	};
	luaL_newlib(state, regs);
	return 1;	
}

