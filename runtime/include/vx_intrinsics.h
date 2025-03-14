#ifndef VX_INTRINSICS_H
#define VX_INTRINSICS_H

#include <VX_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ASSEMBLY__
#define __ASM_STR(x)	x
#else
#define __ASM_STR(x)	#x
#endif

#define csr_read(csr) ({                        \
	unsigned __r;	               		        \
	__asm__ __volatile__ ("csrr %0, %1" : "=r" (__r) : "i" (csr)); \
	__r;							            \
})

#define csr_write(csr, val)	({                  \
	unsigned __v = (unsigned)(val);             \
	if (__builtin_constant_p(val) && __v < 32)  \
        __asm__ __volatile__ ("csrw %0, %1"	:: "i" (csr), "i" (__v));  \
    else                                        \
        __asm__ __volatile__ ("csrw %0, %1"	:: "i" (csr), "r" (__v));  \
})

#define csr_swap(csr, val) ({                   \
    unsigned __r;                               \
	unsigned __v = (unsigned)(val);	            \
	if (__builtin_constant_p(val) && __v < 32)  \
        __asm__ __volatile__ ("csrrw %0, %1, %2" : "=r" (__r) : "i" (csr), "i" (__v)); \
    else                                        \
        __asm__ __volatile__ ("csrrw %0, %1, %2" : "=r" (__r) : "i" (csr), "r" (__v)); \
	__r;						                \
})

#define csr_read_set(csr, val) ({               \
	unsigned __r;                               \
	unsigned __v = (unsigned)(val);	            \
    if (__builtin_constant_p(val) && __v < 32)  \
	    __asm__ __volatile__ ("csrrs %0, %1, %2" : "=r" (__r) : "i" (csr), "i" (__v)); \
    else                                        \
        __asm__ __volatile__ ("csrrs %0, %1, %2" : "=r" (__r) : "i" (csr), "r" (__v)); \
	__r;							            \
})

#define csr_set(csr, val) ({                    \
	unsigned __v = (unsigned)(val);	            \
    if (__builtin_constant_p(val) && __v < 32)  \
	    __asm__ __volatile__ ("csrs %0, %1"	:: "i" (csr), "i" (__v));  \
    else                                        \
        __asm__ __volatile__ ("csrs %0, %1"	:: "i" (csr), "r" (__v));  \
})

#define csr_read_clear(csr, val) ({             \
	unsigned __r;                               \
	unsigned __v = (unsigned)(val);	            \
    if (__builtin_constant_p(val) && __v < 32)  \
	    __asm__ __volatile__ ("csrrc %0, %1, %2" : "=r" (__r) : "i" (csr), "i" (__v)); \
    else                                        \
        __asm__ __volatile__ ("csrrc %0, %1, %2" : "=r" (__r) : "i" (csr), "r" (__v)); \
	__r;							            \
})

#define csr_clear(csr, val)	({                  \
	unsigned __v = (unsigned)(val);             \
	if (__builtin_constant_p(val) && __v < 32)  \
        __asm__ __volatile__ ("csrc %0, %1"	:: "i" (csr), "i" (__v)); \
    else                                        \
        __asm__ __volatile__ ("csrc %0, %1"	:: "i" (csr), "r" (__v)); \
})

// Texture load
#define vx_tex(unit, u, v, lod) ({              \
	unsigned __r;                               \
    __asm__ __volatile__ (".insn r4 0x5b, 0, %1, %0, %2, %3, %4" : "=r"(__r) : "i"(unit), "r"(u), "r"(v), "r"(lod)); \
	__r;							            \
})

// Conditional move
#define vx_cmov(c, t, f) ({                     \
	unsigned __r;		                        \
    __asm__ __volatile__ (".insn r4 0x5b, 1, 0, %0, %1, %2, %3" : "=r"(__r : "r"(c), "r"(t), "r"(f)); \
	__r;							            \
})

// Set thread mask
inline void vx_tmc(unsigned thread_mask) {
    asm volatile (".insn s 0x6b, 0, x0, 0(%0)" :: "r"(thread_mask));
}

// Set thread predicate
inline void vx_pred(unsigned condition) {
    asm volatile (".insn s 0x6b, 0, x1, 0(%0)" :: "r"(condition));
}

typedef void (*vx_wspawn_pfn)();

// Spawn warps
inline void vx_wspawn(unsigned num_warps, vx_wspawn_pfn func_ptr) {
    asm volatile (".insn s 0x6b, 1, %1, 0(%0)" :: "r"(num_warps), "r"(func_ptr));
}

// Split on a predicate
inline void vx_split(int predicate) {
    asm volatile (".insn s 0x6b, 2, x0, 0(%0)" :: "r"(predicate));
}

// Join
inline void vx_join() {
  asm volatile (".insn s 0x6b, 3, x0, 0(x0)");
}

// Warp Barrier
inline void vx_barrier(unsigned barried_id, unsigned num_warps) {
    asm volatile (".insn s 0x6b, 4, %1, 0(%0)" :: "r"(barried_id), "r"(num_warps));
}

// Prefetch
inline void vx_prefetch(unsigned addr) {
    asm volatile (".insn s 0x6b, 5, x0, 0(%0)" :: "r"(addr) );
}

// Return active warp's thread id
inline int vx_thread_id() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_WTID));
    return result;
}

// Return active core's local thread id
inline int vx_thread_lid() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_LTID));
    return result;
}

// Return processsor global thread id
inline int vx_thread_gid() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_GTID));
    return result;
}

// Return active core's local warp id
inline int vx_warp_id() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_LWID));
    return result;
}

// Return processsor's global warp id
inline int vx_warp_gid() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_GWID));
    return result;
}

// Return processsor core id
inline int vx_core_id() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_GCID));
    return result;
}

// Return current threadk mask
inline int vx_thread_mask() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_TMASK));
    return result;
}

// Return the number of threads in a warp
inline int vx_num_threads() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_NT));
    return result;
}

// Return the number of warps in a core
inline int vx_num_warps() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_NW));
    return result;
}

// Return the number of cores in the processsor
inline int vx_num_cores() {
    int result;
    asm volatile ("csrr %0, %1" : "=r"(result) : "i"(CSR_NC));
    return result;
}

inline void vx_fence() {
    asm volatile ("fence iorw, iorw");
}

inline void vx_sleep() {
  asm volatile (".insn i 0x6b, 6, x0, x0, 0");
}

#define __if(b) vx_split(b); \
                if (b)

#define __else else

#define __endif vx_join();

#define __DIVERGENT__ __attribute__((annotate("divergent")))

#ifdef __cplusplus
}
#endif

#endif