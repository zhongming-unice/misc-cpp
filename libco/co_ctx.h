#ifndef __CO_CTX_H__
#define __CO_CTX_H__

#include <stdlib.h>

typedef void *(*co_ctx_pfn)(void *, void *);

struct co_ctx_param {
  const void *s1;
  const void *s2;
};

//    | regs[0]: r15 |
//    | regs[1]: r14 |
//    | regs[2]: r13 |
//    | regs[3]: r12 |
//    | regs[4]: r9  |
//    | regs[5]: r8  |
//    | regs[6]: rbp |
//    | regs[7]: rdi |  // first argument
//    | regs[8]: rsi |  // second argument
//    | regs[9]: ret |  //ret func addr 0(%rsp)
//    | regs[10]: rdx |
//    | regs[11]: rcx |
//    | regs[12]: rbx |
//    | regs[13]: rsp |

struct co_ctx {
  void *regs[14];
  size_t ss_size;               // co_attr.stack_size
  char *ss_sp;                  // stack_mem->stack_buffer 栈低地址
};

int CoCtxInit(co_ctx *ctx);
int CoCtxMake(co_ctx *ctx, co_ctx_pfn pfn, const void *s1, const void *s2);

#endif  // __CO_CTX_H__
