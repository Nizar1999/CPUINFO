#include <stdio.h>

int main() {

	puts("CPU-INFO UTILITY");
	puts("----------------\n");

	//Vendor ID string
	char VendorIdStr[13];		//12 bytes for the string, with 1 for the null termination
	__asm {
		lea edi, VendorIdStr	//Load the address of vendor string into edi
		xor eax, eax			//Xor eax to tell the cpu we want to print the vendor string
		cpuid				//Call cpuid
		mov [edi], ebx			//Copy the result string from the registers into the variable
		mov [edi+4], edx
		mov [edi+8], ecx
	}

	VendorIdStr[12] = 0;		//Null terminate it
	printf("Vendor ID: \t\t%s\n", VendorIdStr);

	//Check if can read PBS
	int PBSSize = 0;
	__asm {
		mov eax, 0x80000000
		cpuid
		mov PBSSize, eax
	}

	if (PBSSize < 0x80000004) {
		puts("Processor Brand String Not Supported!");
	}
	else {
		//Processor brand string
		char ProcessorBrandStr[49];
		__asm {
			lea edi, ProcessorBrandStr
			mov eax, 0x80000002
			cpuid
			mov [edi], eax
			mov [edi+4], ebx
			mov [edi+8], ecx
			mov [edi+12], edx

			mov eax, 0x80000003
			cpuid
			mov[edi+16], eax
			mov[edi+20], ebx
			mov[edi+24], ecx
			mov[edi+28], edx

			mov eax, 0x80000004
			cpuid
			mov[edi+32], eax
			mov[edi+36], ebx
			mov[edi+40], ecx
			mov[edi+44], edx
		}
		ProcessorBrandStr[48] = 0;
		printf("Processor Brand: \t%s", ProcessorBrandStr);
	}
}