OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY 
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2M
	DTCM (xrw): ORIGIN = 0x20000000, LENGTH = 128K
	
}

_debut_pile = ORIGIN(DTCM) + LENGTH(DTCM);

SECTIONS {
	

	  .vectors :
    {
        KEEP(*(.vectors))
        . = ALIGN(4);
    } > FLASH
    
    
    
    .text : {
    	KEEP(*(init))
        *(.text)
        . = ALIGN(4);
    } > FLASH



    .data :
    {
        _sidata = LOADADDR(.data); 
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > DTCM AT> FLASH



	_debutbss = .;
	
    .bss : {
        *(.bss)
        . = ALIGN(4);
    } > DTCM 
    
    _finbss = .;
   
   
   
    .rodata : {
        *(.rodata)
        . = ALIGN(4);
    }  > DTCM
    
    
    

}