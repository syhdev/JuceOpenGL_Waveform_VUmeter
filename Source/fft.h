///*
//  ==============================================================================
//
//	fft.h
//	Created: 6 Jun 2020 9:17:46pm
//	Author:  Sylvain
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include <complex>
//
//#define MAXFACTORS 32
///* e.g. an fft of length 128 has 4 factors
// as far as kissfft is concerned
// 4*4*4*2
// */
//
//template<class T>
//struct fft_state {
//	int nfft;
//	int inverse;
//	int factors[2 * MAXFACTORS];
//	std::complex<T> twiddles[1];
//	std::complex<T>* tmpbuf;
//	std::complex<T>* super_twiddles;
//};
//
//typedef struct fft_state<float>* fft_cfg;
//
//
//template<class T>
//class FFT
//{
//	FFT() {};
//	~FFT() {};
//
//	/*
//	 nfft must be even
//
//	 If you don't care to allocate space, use mem = lenmem = NULL
//	*/
//	void initialiseFFT(int nfft, int inverse_fft, void* mem, size_t* lenmem) {
//
//		int i;
//		kiss_fftr_cfg st = NULL;
//		size_t subsize = 0, memneeded;
//
//		if (nfft & 1) {
//			//fprintf(stderr, "Real FFT optimization must be even.\n");
//			return NULL;
//		}
//
//		nfft >>= 1;
//
//		kiss_fft_alloc(nfft, inverse_fft, NULL, &subsize);
//		memneeded = sizeof(struct kiss_fftr_state) + subsize + sizeof(kiss_fft_cpx) * (nfft * 3 / 2);
//
//		if (lenmem == NULL) {
//			st = (kiss_fftr_cfg)KISS_FFT_MALLOC(memneeded);
//		}
//		else {
//			if (*lenmem >= memneeded)
//				st = (kiss_fftr_cfg)mem;
//			*lenmem = memneeded;
//		}
//		if (!st)
//			return NULL;
//
//		st->substate = (kiss_fft_cfg)(st + 1); /*just beyond kiss_fftr_state struct */
//		st->tmpbuf = (kiss_fft_cpx*)(((char*)st->substate) + subsize);
//		st->super_twiddles = st->tmpbuf + nfft;
//		fft_alloc(nfft, inverse_fft, st->substate, &subsize);
//
//		for (i = 0; i < nfft / 2; ++i) {
//			double phase =
//				-3.14159265358979323846264338327 * ((double)(i + 1) / nfft + .5);
//			if (inverse_fft)
//				phase *= -1;
//			kf_cexp(st->super_twiddles + i, phase);
//		}
//		config = st;
//	}
//	
//
//
//	/*
//	 input timedata has nfft scalar points
//	 output freqdata has nfft/2+1 complex points
//	*/
//	void proceedFFT(fft_cfg cfg, const T* timedata, kiss_fft_cpx* freqdata) {
//		
//	}
//	
//
//	/*
//	 input freqdata has  nfft/2+1 complex points
//	 output timedata has nfft scalar points
//	*/
//	void proceedIFFT(fft_cfg cfg, const std::complex<T>* freqdata, T* timedata);
//	
//
//private:
//	fft_cfg config;
//};