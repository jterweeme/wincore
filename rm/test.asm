segment .text
        mov     dx,hello 
        mov     ah,0x41
        int     0x21
      mov     ax,0x4c00 
        int     0x21

hello:
    db      'test.txt', 0




