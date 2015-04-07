#ifndef _LINUX64_H_
#define _LINUX64_H_
extern "C" int tiep1(int rdi, int rsi, int rdx, int rax);
extern "C" int tiep2(const char *rdi, int rsi, int rdx, int rax);
extern "C" int tiep3(int rdi, const char *rsi, int rdx, int rax);
extern "C" int tiep4(int rdi, char *rsi, int rdx, int rax);
extern "C" int tiep5(int rdi, void *rsi, int rdx, int rax);
extern "C" int tiep6(int rdi, const void *rsi, int rdx, int rax);
#endif



