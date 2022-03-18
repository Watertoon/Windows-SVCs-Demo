 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#pragma once
#include <windows.h>
#include <iostream>
#include <memory>
#include "util_defines.h"

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

namespace wn::svc {
    
    /* These are NtDll functions that directly correspond to a Syscall on Windows */
    constexpr const char *SysCallQueryList[] = {
        /* Threads */
        "NtCreateThread", 
        "NtOpenThread", 
        "NtTerminateThread", 
        "NtResumeThread", 
        "NtSuspendThread", 
        "NtQueryInformationThread", 
        "NtSetInformationThread", 
        "NtGetContextThread", 
        "NtSetContextThread", 
        "NtWaitForAlertByThreadId", 
        "NtAlertThreadByThreadId",
        "NtAlertThread",
        "NtYieldExecution",
        /* Processes */
        "NtOpenProcess", 
        "NtTerminateProcess", 
        "NtQueryInformationProcess", 
        "NtSetInformationProcess",
        /* Events */
        "NtCreateEvent", 
        "NtClearEvent", 
        "NtResetEvent", 
        "NtSetEvent", 
        "NtPulseEvent", 
        "NtQueryEvent",
        /* Common Handle Operations */
        "NtClose", 
        "NtWaitForSingleObject", 
        "NtWaitForMultipleObjects",
        /* Virtual Memory */
        "NtAllocateVirtualMemory", 
        "NtAllocateVirtualMemoryEx", 
        "NtFreeVirtualMemory", 
        "NtReadVirtualMemory", 
        "NtWriteVirtualMemory", 
        "NtProtectVirtualMemory", 
        "NtFlushVirtualMemory", 
        "NtQueryVirtualMemory", 
        "NtSetInformationVirtualMemory", 
        "NtLockVirtualMemory", 
        "NtUnlockVirtualMemory"
    };
    constexpr const uint32_t SysCallQueryCount = sizeof(SysCallQueryList) / sizeof(char*);
    constinit uint32_t SysCallQueryTable[SysCallQueryCount] = {};
    
    /* Matches a string in the syscall query list to it's index at compile time */
    consteval uint32_t ResolveSysCallQueryTableIndexByName(const char *name) {
        for (u32 i = 0; i < SysCallQueryCount; ++i) {
            if (::strcmp(SysCallQueryList[i], name) == 0) {
                return i;
            }
        }
        /* If this occurs, we should throw a compile time error */
        return 0xFFFFFFFF;
    }
    
    #define NTSYSCALL_1(name, _1) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_2(name, _1, _2) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_3(name, _1, _2, _3) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_4(name, _1, _2, _3, _4) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_5(name, _1, _2, _3, _4, _5) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_6(name, _1, _2, _3, _4, _5, _6) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5), "g" (_6) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_7(name, _1, _2, _3, _4, _5, _6, _7) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5), "g" (_6), "g" (_7) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_8(name, _1, _2, _3, _4, _5, _6, _7, _8) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5), "g" (_6), "g" (_7), "g" (_8) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_9(name, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5), "g" (_6), "g" (_7), "g" (_8), "g" (_9) : "memory"  \
    );                                                             \
    return ret
    
    #define NTSYSCALL_10(name, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index), "c" (_1), "d" (_2), "q" (_3), "q" (_4), "g" (_5), "g" (_6), "g" (_7), "g" (_8), "g" (_9), "g" (_10) : "memory"  \
    );                                                             \
    return ret
    
    /* Special case for no syscalls with no parameters */
    #define NTSYSCALL(name) \
    static_assert(ResolveSysCallQueryTableIndexByName(STRINGIZE(name)) != 0xFFFFFFFF); \
    constexpr uint32_t* index = ::wn::svc::SysCallQueryTable + ::wn::svc::ResolveSysCallQueryTableIndexByName(STRINGIZE(name));  \
    uint32_t ret = 0; \
    __asm__ volatile( \
    " mov %%rcx, %%r10 \n mov (%[resolve]), %%eax \n syscall"      \
    : "=a" (ret) : [resolve] "a" (index) : "memory", "rcx", "rdx", "r8", "r9"  \
    );                                                             \
    return ret
    
    #define NTSYSCALLV(name, ...) WN_NAMED_VMACRO(name, NTSYSCALL, __VA_ARGS__)
    
    using Handle = HANDLE;
    
    struct XmmSaveArea32 {
        s16 control;
        s16 status;
        u8  tag;
        u8  reserve0;
        s16 error_opcode;
        u16 error_offset;
        s16 error_selector;
        s16 reserve1;
        u16 data_offset;
        s16 data_selector;
        s16 reserve2;
        u32 mx_csr;
        u32 mx_csr_mask;
        __m128 float_registers[8];
        __m128 xmm_registers[16];
        u8 reserve3[96];
    };
    
    /* The Context of our thread as specified here: https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-context */
    struct ThreadContext {
        u64 p1_home;
        u64 p2_home;
        u64 p3_home;
        u64 p4_home;
        u64 p5_home;
        u64 p6_home;
        u32 context_flags;
        u32 mx_csr;
        u16 seg_cs;
        u16 seg_ds;
        u16 seg_es;
        u16 seg_fs;
        u16 seg_gs;
        u16 seg_ss;
        u32 eflags;
        u64 dr0;
        u64 dr1;
        u64 dr2;
        u64 dr3;
        u64 dr6;
        u64 dr7;
        u64 rax;
        u64 rcx;
        u64 rdx;
        u64 rbx;
        u64 rsp;
        u64 rbp;
        u64 rsi;
        u64 rdi;
        u64 r8;
        u64 r9;
        u64 r10;
        u64 r11;
        u64 r12;
        u64 r13;
        u64 r14;
        u64 r15;
        u64 rip;
        union {
            XmmSaveArea32 save_area;
            __m128 q[16];
            u64    d[32];
            struct {
                __m128 header[2];
                __m128 legacy[8];
                __m128 xmm0;
                __m128 xmm1;
                __m128 xmm2;
                __m128 xmm3;
                __m128 xmm4;
                __m128 xmm5;
                __m128 xmm6;
                __m128 xmm7;
                __m128 xmm8;
                __m128 xmm9;
                __m128 xmm10;
                __m128 xmm11;
                __m128 xmm12;
                __m128 xmm13;
                __m128 xmm14;
                __m128 xmm15;
            };
            u32 s[32];
        };
        __m128 vector_registers[26];
        u64 vector_control;
        u64 debug_control;
        u64 last_branch_to_rip;
        u64 last_branch_from_rip;
        u64 last_exception_to_rip;
        u64 last_exception_from_rip;
    } CONTEXT;
    static_assert(sizeof(ThreadContext) == sizeof(::CONTEXT));
    
    /* Undocumented WinNt OBJECT_ATTRIBUTES struct*/
    struct NativeObjectAttributes {
        u32 size_of_this;
        Handle root_directory;
        char16_t object_name;
        u32 attributes;
        void *security_descriptor;
        void *security_qos; /* Quality of Service */
        
        constexpr void SetDefaults() {
            this->size_of_this = sizeof(NativeObjectAttributes);
            this->attributes = 0x20;
        }
    } OBJECT_ATTRIBUTES;
    static_assert(sizeof(OBJECT_ATTRIBUTES) == 0x28);
    
    /* Undocumented WinNt CLIENT_ID struct*/
    struct ClientId{
        Handle process;
        Handle thread;
    } CLIENT_ID;
    static_assert(sizeof(ClientId) == 0x10);
    
    /* Undocumented WinNt INITIAL_TEB struct*/
    struct ThreadStackInfo {
        /* Fixed Stack is not supported on Windows 10 */
        void *fixed_stack_base;
        void *fixed_stack_limit;
        void *expandable_stack_base;
        void *expandable_stack_limit;
        void *expandable_stack_bottom;
    } INITIAL_TEB, USER_STACK;
    static_assert(sizeof(ThreadStackInfo) == 0x28);
    
    /* Windows TEB block */
    /* Thank you Geoff Chappell */
    struct ProcessEnvironmentBlock;
    struct ThreadEnvironmentBlock {
        /* NT_TEB */
        void *exception_handling_frame;                     /* 0x0 */
        void *stack_base;                                   /* 0x8 */
        void *stack_limit;                                  /* 0x10 */
        void *sub_system_tib;                               /* 0x18 */
        void *fiber_data;                                   /* 0x20 */
        void *arbitrary_data_slot;                          /* 0x28 */
        void *linear_address_this_teb;                      /* 0x30 */
        /* Win32 */
        void *environment_pointer;                          /* 0x38 */
        ClientId client_id;                                 /* 0x40 */
        void *active_rpc_handle;                            /* 0x50 */
        void *thread_local_storage;                         /* 0x58 */
        ProcessEnvironmentBlock *process_environment_block; /* 0x60 */
        u32 last_error;                                     /* 0x68 */
        u32 owned_critical_sections_count;                  /* 0x6C */
        void *csr_client_thread;
        void *win32_thread_info;
        u32 user32_reserved[0x1A];
        u64 user_reserved[5];
        void *wow32_reserved;
        u32 current_locale;
        u32 fp_software_status_register;
        void *reserved_for_debugger_instrumentation [0x10];
        void *system_reserved_1[0x1A];
        /* unfinished */
    };
    //static_assert(sizeof(ThreadEnvironmentBlock) == 0x1830);
    
    enum ProcessInfo {
        ProcessInfo_BasicInfo = 0
    };
    
    struct ProcessBasicInfo {
        u32 exit_status;
        void *environment_block;
        KAFFINITY affinity;
        u32 priority;
        Handle unique_id;
        Handle inherited_id;
    };
}