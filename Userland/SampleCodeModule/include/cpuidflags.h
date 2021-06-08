#ifndef __CPUIDFLAGS_H__
#define __CPUIDFLAGS_H__

enum {
    CPUID_FEAT_ECX_SSE3         = 1 << 0, 
    CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
    CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
    CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
    CPUID_FEAT_ECX_AES          = 1 << 25,  
    CPUID_FEAT_ECX_AVX          = 1 << 28,
    
    CPUID_FEAT_EDX_MMX          = 1 << 23, 
    CPUID_FEAT_EDX_SSE          = 1 << 25, 
    CPUID_FEAT_EDX_SSE2         = 1 << 26
};

int32_t supports_cpuid(void);
int32_t supports_vaesni(void);
int32_t supports_vpclmulqdq(void);
int32_t supports_f16c(void);
int32_t supports_fma(void);
int32_t supports_avx2(void);

#endif