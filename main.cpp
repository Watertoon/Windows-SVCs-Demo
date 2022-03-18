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
#include <svc_test1.hpp>

/* A simple thread function for testing threads */
int ThreadMain(void *arg) {
    WN_UNUSED(arg);
    std::cout << "Welcome from threadland!" << std::endl;
    
    /* One should ensure their thread is properly ready to terminate (ie all deconstructors, stack is reset)*/
    wn::svc::x86_64::win64::NtTerminateThread(wn::svc::x86_64::win64::NtCurrentThread(), 0);
    return 0; /* "ret" segfaults as there's nothing to return to unless you set that up in the thread stack */
}

/* Test main testing basic thread and handle functions */
int main() {
    HANDLE thread = nullptr;
    HANDLE process = wn::svc::x86_64::win64::NtCurrentProcess(); /* process handle (pseudo) */
    
    /* Initialize our SysCall id table from NTDLL */
    wn::svc::x86_64::win64::InitializeSysCallTable();
    
    /* Query our basic process info */
    wn::svc::ProcessBasicInfo basic_info = {};
    std::cout << wn::svc::x86_64::win64::NtQueryInformationProcess(process, wn::svc::ProcessInfo_BasicInfo, std::addressof(basic_info), sizeof(wn::svc::ProcessBasicInfo), nullptr)  << std::endl;
    
    /* Open a handle (real) to our process, overwrites sudo */
    wn::svc::ClientId process_id = { .process = basic_info.unique_id, .thread = 0 };
    wn::svc::NativeObjectAttributes process_attr = { .size_of_this = sizeof(wn::svc::NativeObjectAttributes) };
    std::cout << wn::svc::x86_64::win64::NtOpenProcess(std::addressof(process), PROCESS_ALL_ACCESS, std::addressof(process_attr), std::addressof(process_id))  << std::endl;
    
    /* We specify our own user stack in NtCreateThread */
    void *stack_limit = new char[0x2000];
    void *stack_base = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(stack_limit) + 0x2000);
    wn::svc::ThreadStackInfo thread_stack = { .expandable_stack_base = stack_base, .expandable_stack_limit = stack_base, .expandable_stack_bottom = stack_limit };
    
    /* Setup thread context. We specify our own starting cpu registers here. */
    wn::svc::ThreadContext thread_context = {};
    wn::svc::ThreadContext *arg = std::addressof(thread_context);
    
    /* Defaults chosen from RtlInitializeContext */
    thread_context.context_flags = CONTEXT_AMD64; /* This enum seems to be required yet the same value between different architectures? */
    thread_context.rax = 0;
    thread_context.rbx = 1;
    thread_context.rcx = reinterpret_cast<unsigned long long>(arg); /* Rcx is the register used for the first argument of a function */
    thread_context.rsi = 4;
    thread_context.rdi = 5;
    thread_context.r8  = 8;
    thread_context.r9  = 0xf0e0d0c0a0908070;
    thread_context.r10 = 10;
    thread_context.r11 = 11;
    thread_context.r12 = 12;
    thread_context.r13 = 13;
    thread_context.r14 = 14;
    thread_context.r15 = 15;
    thread_context.mx_csr = 0x1F80; /* SSE control and status register. Precision, Underflow, Overflow, DivideByZero, Denormal, and InvalidOperation masks are set */
    thread_context.rsp = reinterpret_cast<unsigned long long>(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(stack_base) - sizeof(void*))); /* Our thread stack */
    thread_context.rbp = 0;
    thread_context.rip = reinterpret_cast<unsigned long long>(ThreadMain); /* Our thread entrypoint */
    thread_context.eflags = 0x200;
    
    wn::svc::ClientId id = {};
    
    std::cout << wn::svc::x86_64::win64::NtCreateThread(std::addressof(thread), THREAD_ALL_ACCESS, nullptr, process, std::addressof(id), std::addressof(thread_context), std::addressof(thread_stack), true) << std::endl;
    std::cout << wn::svc::x86_64::win64::NtResumeThread(thread, nullptr) << std::endl;
    std::cout << wn::svc::x86_64::win64::NtWaitForSingleObject(thread, false, nullptr) << std::endl;
    std::cout << wn::svc::x86_64::win64::NtClose(thread) << std::endl;
    
    delete[](reinterpret_cast<char*>(stack_limit));
    
    std::cout << "Success.";
    return 0;
}