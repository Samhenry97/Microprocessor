PHDRS {
    kernel PT_LOAD AT(0x0000) ;
    program PT_LOAD AT(0x8000) ;
}

SECTIONS
{
    .kernel 0x0000 : {
        obj\libsys.a:boot.o
        obj\libsys.a:kernel.o
    } :kernel


    . = 0x8000;
    .text 0x8000 :  {
        obj\libc.a:crt.o
        obj\libc.a:libc.o
        *.o
    } :program

}

EXTERN(_reset)
ENTRY(_start)
