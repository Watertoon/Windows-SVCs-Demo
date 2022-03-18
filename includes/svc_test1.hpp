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

#include "svc_types.h"

namespace wn::svc::x86_64::win64 {
    
    /* Since Windows isn't consistent, we must resolve our Syscall indexes at runtime */
    /* This is a funnier method than storing pointers and using far calls to ntdll.dll */
    void InitializeSysCallTable() {
        /* Get ntdll.dll */
        HMODULE ntdll_process = ::GetModuleHandle("ntdll.dll");
        
        /* Resolve the Syscalls */
        for (u32 i = 0; i < SysCallQueryCount; ++i) {
            /* Query our Syscall function */
           uintptr_t sys_call = reinterpret_cast<uintptr_t>(::GetProcAddress(ntdll_process, SysCallQueryList[i]));
           
           /* Pull the immediate u32 of the syscall id from it's fixed offset */
           u32 sys_call_number = *reinterpret_cast<u32*>(sys_call + 4);
           SysCallQueryTable[i] = sys_call_number;
        }
    }
    
    /* O0 has data alignment issues in the parameters, O2=< has the parameters become too aggressively "optimized" into the wrong registers */
    /* The option doesn't change the function body just how the parameters are handled */
    /* Every SVC returns an NTSTATUS */
    #pragma GCC push_options
    #pragma GCC optimize("-O1")
    
    /* Threads */
    NOINLINE u32 NtCreateThread(Handle *out_handle, ACCESS_MASK desired_access, NativeObjectAttributes *object_attributes, Handle process_handle, ClientId *out_client_id, ThreadContext *thread_context, ThreadStackInfo *initial_teb, bool create_suspended) {
        NTSYSCALLV(NtCreateThread, out_handle, desired_access, object_attributes, process_handle, out_client_id, thread_context, initial_teb, create_suspended);
    }
    
    NOINLINE u32 NtOpenThread(Handle *out_handle, ACCESS_MASK access_mask, NativeObjectAttributes *attributes, ClientId *client_id) {
        NTSYSCALLV(NtOpenThread, out_handle, access_mask, attributes, client_id);
    }
    
    NOINLINE u32 NtTerminateThread(Handle thread_handle, u32 exit_code) {
        NTSYSCALLV(NtTerminateThread, thread_handle, exit_code);
    }
    
    NOINLINE u32 NtResumeThread(Handle thread_handle, u32 *out_previous_suspend_count) {
        NTSYSCALLV(NtResumeThread, thread_handle, out_previous_suspend_count);
    }
    
    NOINLINE u32 NtSuspendThread(Handle thread_handle, u32 *out_previous_suspend_count) {
        NTSYSCALLV(NtSuspendThread, thread_handle, out_previous_suspend_count);
    }
    
    NOINLINE u32 NtQueryInformationThread(Handle thread_handle, u64 thread_info, void *thread_info_buffer, u32 thread_info_buffer_size, u32 *out_size) {
        NTSYSCALLV(NtQueryInformationThread, thread_handle, thread_info, thread_info_buffer, thread_info_buffer_size, out_size);
    }
    
    NOINLINE u32 NtSetInformationThread(Handle thread_handle, u64 thread_info, void *thread_info_buffer, u32 thread_info_buffer_size) {
        NTSYSCALLV(NtSetInformationThread, thread_handle, thread_info, thread_info_buffer, thread_info_buffer_size);
    }
    
    NOINLINE u32 NtGetContextThread(Handle thread_handle, ThreadContext *out_context) {
        NTSYSCALLV(NtGetContextThread, thread_handle, out_context);
    }
    
    NOINLINE u32 NtSetContextThread(Handle thread_handle, ThreadContext *context) {
        NTSYSCALLV(NtSetContextThread, thread_handle, context);
    }
    
    NOINLINE u32 NtWaitForAlertByThreadId(void *address, u64 *timeout) {
        NTSYSCALLV(NtWaitForAlertByThreadId, address, timeout);
    }
    
    NOINLINE u32 NtAlertThreadByThreadId(u32 thread_id) {
        NTSYSCALLV(NtAlertThreadByThreadId, thread_id);
    }
    
    NOINLINE u32 NtAlertThread(Handle thread_handle) {
        NTSYSCALLV(NtAlertThread, thread_handle);
    }
    
    NOINLINE u32 NtYieldExecution() {
        NTSYSCALL(NtYieldExecution);
    }
    
    /* Processes */
    NOINLINE u32 NtOpenProcess(Handle *out_handle, ACCESS_MASK access_mask, NativeObjectAttributes *attributes, ClientId *client_id) {
        NTSYSCALLV(NtOpenProcess, out_handle, access_mask, attributes, client_id);
    }
    
    NOINLINE u32 NtTerminateProcess(Handle process_handle, u32 exit_code) {
        NTSYSCALLV(NtTerminateProcess, process_handle, exit_code);
    }
    
    NOINLINE u32 NtQueryInformationProcess(Handle process, u64 process_info, void *process_info_buffer, u32 process_info_buffer_size, u32 *out_size) {
        NTSYSCALLV(NtQueryInformationProcess, process, process_info, process_info_buffer, process_info_buffer_size, out_size);
    }
    
    NOINLINE u32 NtSetInformationProcess(Handle process, u64 process_info, void *process_info_buffer, u32 process_info_buffer_size) {
        NTSYSCALLV(NtSetInformationProcess, process, process_info, process_info_buffer, process_info_buffer_size);
    }
    
    /* Events */
    NOINLINE u32 NtCreateEvent(Handle *out_handle, ACCESS_MASK access_mask, NativeObjectAttributes *attributes, u32 is_auto_reset, bool start_signaled) {
        NTSYSCALLV(NtCreateEvent, out_handle, access_mask, attributes, is_auto_reset, start_signaled);
    }
    
    NOINLINE u32 NtClearEvent(Handle event_handle) {
        /* Unsignal and outs state */
        NTSYSCALLV(NtClearEvent, event_handle);
    }
    
    NOINLINE u32 NtResetEvent(Handle event_handle, u32 *out_previous_state) {
        /* Unsignal and outs state */
        NTSYSCALLV(NtResetEvent, event_handle, out_previous_state);
    }
    
    NOINLINE u32 NtSetEvent(Handle event_handle, u32 *out_previous_state) {
        /* Signal */
        NTSYSCALLV(NtSetEvent, event_handle, out_previous_state);
    }
    
    NOINLINE u32 NtPulseEvent(Handle event_handle, u32 *out_previous_state) {
        /* Signal All */
        NTSYSCALLV(NtPulseEvent, event_handle, out_previous_state);
    }
    
    NOINLINE u32 NtQueryEvent(Handle event_handle, u32 event_info, void *event_info_buffer, u32 event_info_buffer_size, u32 *out_size) {
        NTSYSCALLV(NtQueryEvent, event_handle, event_info, event_info_buffer, event_info_buffer_size, out_size);
    }
    
    /* Common Handle operations */
    NOINLINE u32 NtClose(Handle handle) {
        /* Closes any kind of open handle */
        NTSYSCALLV(NtClose, handle);
    }
    
    NOINLINE u32 NtWaitForSingleObject(Handle handle, bool is_alertable, u64 *timeout) {
        /* Waits until the specified open handle is signaled */
        NTSYSCALLV(NtWaitForSingleObject, handle, is_alertable, timeout);
    }
    
    NOINLINE u32 NtWaitForMultipleObjects();
    
    /* Virtual Memory */
    NOINLINE u32 NtAllocateVirtualMemory(Handle process_handle, void **in_out_base_address, u32 zero_bits, size_t *in_out_allocation_size, u32 allocation_type, u32 protect) {
        /* Reserves or commits memory into the process's virtual address space */
        NTSYSCALLV(NtAllocateVirtualMemory, process_handle, in_out_base_address, zero_bits, in_out_allocation_size, allocation_type, protect);
    }
    
    NOINLINE u32 NtAllocateVirtualMemoryEx(Handle process_handle, void **in_out_base_address, u32 zero_bits, size_t *in_out_allocation_size, u32 allocation_type, u32 protect, void *extended_parameter_array, u32 extended_param_count) {
        NTSYSCALLV(NtAllocateVirtualMemoryEx, process_handle, in_out_base_address, zero_bits, in_out_allocation_size, allocation_type, protect, extended_parameter_array, extended_param_count);
    }
    
    NOINLINE u32 NtFreeVirtualMemory(Handle process_handle, void **in_out_base_address, size_t *in_out_free_size, u32 free_type) {
        NTSYSCALLV(NtFreeVirtualMemory, process_handle, in_out_base_address, in_out_free_size, free_type);
    }
    
    NOINLINE u32 NtReadVirtualMemory(Handle process_handle, void *base_address, void *buffer, size_t buffer_size, size_t *out_size_read) {
        NTSYSCALLV(NtReadVirtualMemory, process_handle, base_address, buffer, buffer_size, out_size_read);
    }
    
    NOINLINE u32 NtWriteVirtualMemory(Handle process_handle, void *base_address, void *buffer, size_t buffer_size, size_t *out_size_written) {
        NTSYSCALLV(NtWriteVirtualMemory, process_handle, base_address, buffer, buffer_size, out_size_written);
    }
    
    NOINLINE u32 NtProtectVirtualMemory(Handle process_handle, void **in_out_base_address, size_t *in_out_region_size, u32 new_protect, u32 *out_old_protect) {
        NTSYSCALLV(NtProtectVirtualMemory, process_handle, in_out_base_address, in_out_region_size, new_protect, out_old_protect);
    }
    
    NOINLINE u32 NtFlushVirtualMemory(Handle process_handle, void **in_out_base_address, size_t *in_out_flush_size, void *out_status_block) {
        NTSYSCALLV(NtFlushVirtualMemory, process_handle, in_out_base_address, in_out_flush_size, out_status_block);
    }
    
    NOINLINE u32 NtQueryVirtualMemory(Handle process_handle, void *base_address, u32 memory_info_type, void *memory_info_buffer, size_t memory_info_buffer_size, size_t *out_size) {
        NTSYSCALLV(NtQueryVirtualMemory, process_handle, base_address, memory_info_type, memory_info_buffer, memory_info_buffer_size, out_size);
    }
    
    NOINLINE u32 NtSetInformationVirtualMemory(Handle process_handle, u32 memory_info_type, u32 *memory_entry_count, void *memory_entry_array, void *memory_info_buffer, u32 memory_info_buffer_size) {
        NTSYSCALLV(NtSetInformationVirtualMemory, process_handle, memory_info_type, memory_entry_count, memory_entry_array, memory_info_buffer, memory_info_buffer_size);
    }
    
    NOINLINE u32 NtLockVirtualMemory(Handle process_handle, void **in_out_base_address, size_t *in_out_region_size, u32 lock_type) {
        NTSYSCALLV(NtLockVirtualMemory, process_handle, in_out_base_address, in_out_region_size, lock_type);
    }
    
    NOINLINE u32 NtUnlockVirtualMemory(Handle process_handle, void **in_out_base_address, size_t *in_out_region_size, u32 lock_type) {
        NTSYSCALLV(NtUnlockVirtualMemory, process_handle, in_out_base_address, in_out_region_size, lock_type);
    }
    
    /* Files */
    NOINLINE u32 NtCreateFile();
    
    NOINLINE u32 NtOpenFile();
    
    NOINLINE u32 NtDeleteFile();
    
    NOINLINE u32 NtReadFile();
    
    NOINLINE u32 NtWriteFile();
    
    NOINLINE u32 NtCancelIoFile();
    
    NOINLINE u32 NtFlushBuffersFile();
    
    NOINLINE u32 NtQueryInformationFile();
    
    NOINLINE u32 NtSetInformationFile();
    
    NOINLINE u32 NtQueryDirectoryFile();
    
    NOINLINE u32 NtQueryVolumeInformationFile();

    NOINLINE u32 NtSetVolumeInformationFile();
    
    NOINLINE u32 NtNotifyChangeDirectoryFile();
    
    NOINLINE u32 NtLockFile();
    
    NOINLINE u32 NtUnlockFile();
    
    #pragma GCC pop_options
    
    /* Sudo handle getters */
    constexpr ALWAYS_INLINE Handle NtCurrentProcess() {
        uintptr_t n = 0;
        n -= 1;
        return reinterpret_cast<Handle>(n);
    }
    
    constexpr ALWAYS_INLINE Handle NtCurrentThread() {
        uintptr_t n = 0;
        n -= 2;
        return reinterpret_cast<Handle>(n);
    }
}