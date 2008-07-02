/*
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 */

#include <stdio.h>
#include "x86info.h"

void get_cpu_vendor(struct cpudata *cpu)
{
	unsigned int maxi, maxei, vendor;

	cpuid(cpu->number, 0, &maxi, &vendor, NULL, NULL);
	maxi &= 0xffff;		/* The high-order word is non-zero on some Cyrix CPUs */
	cpu->maxi = maxi;

	cpuid(cpu->number, 0x80000000, &maxei, NULL, NULL, NULL);
	cpu->maxei = maxei;

	cpuid(cpu->number, 0xC0000000, &maxei, NULL, NULL, NULL);
	cpu->maxei2 = maxei;

	switch (vendor) {
	case 0x756e6547:
		cpu->vendor = VENDOR_INTEL;
		break;
	case 0x68747541:
		cpu->vendor = VENDOR_AMD;
		break;
	case 0x69727943:
		cpu->vendor = VENDOR_CYRIX;
		break;
	case 0x746e6543:
		cpu->vendor = VENDOR_CENTAUR;
		break;
	case 0x646f6547:
		cpu->vendor = VENDOR_NATSEMI;
		break;
	case 0x52697365:
	case 0x65736952:
		cpu->vendor = VENDOR_RISE;
		break;
	case 0x20536953:
		cpu->vendor = VENDOR_SIS;
		break;
	default:
		printf("Unknown vendor (%x)\n", vendor);
		return;
	}
}

void identify(struct cpudata *cpu)
{
	switch (cpu->vendor) {
	case VENDOR_INTEL:
		Identify_Intel(cpu);
		break;
	case VENDOR_AMD:
		Identify_AMD(cpu);
		break;
	case VENDOR_CYRIX:
		Identify_Cyrix(cpu);
		break;
	case VENDOR_CENTAUR:
		identify_centaur(cpu);
		break;
	case VENDOR_NATSEMI:
		Identify_NatSemi(cpu);
		break;
	case VENDOR_RISE:
		Identify_RiSE(cpu);
		break;
	case VENDOR_SIS:
		Identify_SiS(cpu);
		break;
	}
}

void show_info(struct cpudata *cpu)
{
	if (silent)
		return;

	switch (cpu->vendor) {
	case VENDOR_AMD:
		display_AMD_info(cpu);
		break;

	case VENDOR_CYRIX:
		display_Cyrix_info(cpu);
		break;

	case VENDOR_CENTAUR:
		display_centaur_info(cpu);
		break;

	case VENDOR_INTEL:
		display_Intel_info(cpu);
		break;

	case VENDOR_NATSEMI:
		display_NatSemi_info(cpu);
		break;

	case VENDOR_RISE:
		display_RiSE_info(cpu);
		break;

	case VENDOR_SIS:
		display_SiS_info(cpu);
		break;

	default:
		break;
	}
}
