
#include "stm32f4xx.h"


uint32_t data;

void FlASH_Write(uint32_t flashAddress , uint32_t flashData){

FLASH_Unlock();
FLASH_EraseSector(FLASH_Sector_11,VoltageRange_3); // 32 bit yazacak sekilde sector ü sildik
FLASH_ProgramWord(flashAddress , flashData);
FLASH_Lock();

}

uint32_t FLASH_Read(uint32_t flashAddress){

uint32_t data = *(uint32_t*)flashAddress; // flashAddress'in pointer i olarak aldýk,yani bir deger olarak aldýk
return data ;

}


int main(void){

FlASH_Write(0x080E0000,55);

while(1){

data=FLASH_Read(0x080E0000);

}

}
