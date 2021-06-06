#include <utils.h>
#include <lib.h>
#include <cpuid.h>
#include <cpuidflags.h>

void printDate() {
	dateType currDate;
	fillDate(&currDate);
	print_f(1, "Date: %d/%d/%d\nTime: %d:%d:%d (UTC)\n", currDate.day, 
                                                         currDate.month, 
                                                         currDate.year + 2000, 
                                                         currDate.hour, 
                                                         currDate.minute, 
                                                         currDate.second);
}

void help() {
    print_f(1, "Los comandos disponibles son: help, inforeg, printmem, printDate, divisionByZero, invalidOpcode, printFeatures.\n");
}

void printmem() {
    uint8_t arr[BYTES_TO_READ] = {0};
    print_f(1, "Ingrese la dirección a partir de la cual quiere leer: ");

    // TODO: Hacer la lectura de la dirección de memoria a leer
    uint64_t dir = 0x0000001000000000; // Habría que leer

    // Leer dirección dada por el usuario
    print_f(1, "%x\n", fillMem(dir, arr, BYTES_TO_READ));

    for (int i = 0; i < BYTES_TO_READ; i++)
        print_f(1, "%x ", arr[i]);
    
    print_f(1, "\n");
}

void printFeatures() {
    // Chequear si se pude usar cpuid
    uint32_t eax, ebx, ecx, edx;

    if (supports_cpuid()){
        print_f(1, "CPUID: YES\n");

        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        print_f(1, "MX: %s\n", edx & CPUID_FEAT_EDX_MMX == 0 ? "NO":"YES");
        print_f(1, "SSE: %s\n", edx & CPUID_FEAT_EDX_SSE == 0 ? "NO":"YES");
        print_f(1, "SSE2: %s\n", edx & CPUID_FEAT_EDX_SSE2 == 0 ? "NO":"YES");
        print_f(1, "SSE3: %s\n", ecx & CPUID_FEAT_ECX_SSE3 == 0 ? "NO":"YES");
        print_f(1, "SSE41: %s\n", ecx & CPUID_FEAT_ECX_SSE4_1 == 0 ? "NO":"YES");
        print_f(1, "SSE42: %s\n", ecx & CPUID_FEAT_ECX_SSE4_2 == 0 ? "NO":"YES");
        print_f(1, "AESNI: %s\n", ecx & CPUID_FEAT_ECX_AES == 0 ? "NO":"YES");
        print_f(1, "PCLMULQDQ: %s\n", ecx & CPUID_FEAT_ECX_PCLMUL == 0 ? "NO":"YES");
        print_f(1, "AVX: %s\n", ecx & CPUID_FEAT_ECX_AVX == 0 ? "NO":"YES");
        print_f(1, "VAESNI: %s\n",  supports_vaesni() == 0 ? "NO":"YES");
        print_f(1, "VPCLMULQDQ: %s\n",  supports_vpclmulqdq() == 0 ? "NO":"YES");
        print_f(1, "F16C: %s\n",  supports_f16c() == 0 ? "NO":"YES");
        print_f(1, "FMA: %s\n",  supports_fma() == 0 ? "NO":"YES");
        print_f(1, "AVX2: %s\n",  supports_avx2() == 0 ? "NO":"YES");
    } else {
        print_f(1, "CPUID: NO\n");
    }
    
}