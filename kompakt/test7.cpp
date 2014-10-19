#include <dpmi.h>
#include <go32.h>


static __dpmi_regs        callback_regs;
static _go32_dpmi_seginfo callback_info;

void video()
{
    __dpmi_regs r;
    //callback_info.pm_offset = (long)func;

    r.x.ax = 0x3;
//    r.x.cx = mask;
//    r.x.es = callback_info.rm_segment;
//    r.x.dx = callback_info.rm_offset;
    __dpmi_int (0x10, &r);
    //return (r.x.flags & 1) ? -1 : 0;
}

int main()
{
    video();
    return 0;
}


