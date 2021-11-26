#include "co_ctx.h"

#include <stdio.h>
#include <string.h>

#define RSP 0
#define RIP 1
#define RBX 2
#define RDI 3
#define RSI 4

#define RBP 5
#define R12 6
#define R13 7
#define R14 8
#define R15 9
#define RDX 10
#define RCX 11
#define R8 12
#define R9 13

enum {
  kRDI = 7,
  kRSI = 8,
  kRETAddr = 9,
  kRSP = 13,
};

extern "C" {
  extern void co_ctx_swap(co_ctx *, co_ctx *) asm("co_ctx_swap");
};

int CoCtxInit(co_ctx *ctx) {
  memset(ctx, 0, sizeof(*ctx));
  return 0;
}

int CoCtxMake(co_ctx *ctx, co_ctx_pfn pfn, const void *s1, const void *s2) {
  char *sp = ctx->ss_sp + ctx->ss_size - sizeof(void *);
  sp = (char *)((unsigned long)sp & -16LL);
  memset(ctx->regs, 0, sizeof(ctx->regs));
  void **ret_addr = (void **)(sp);
  *ret_addr = (void *)pfn;
  ctx->regs[kRSP] = sp;
  ctx->regs[kRETAddr] = (char *)pfn;
  ctx->regs[kRDI] = (char *)s1;
  ctx->regs[kRSI] = (char *)s2;
  return 0;
}
