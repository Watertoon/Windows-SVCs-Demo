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
/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Modifications to Atmosphere code include renaming of a lot of structures to match my libraries naming scheme */
#pragma once

#define ALIGNED(algn) __attribute__((aligned(algn)))
#define NORETURN __attribute__((noreturn))
#define WEAK_SYMBOL __attribute__((weak))
#define NOINLINE __attribute__((noinline))
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define ALWAYS_INLINE_LAMBDA __attribute__((always_inline))

#define NON_COPYABLE(classname) \
    classname(const classname&) = delete;\
    classname& operator=(const classname&) = delete

#define NON_MOVEABLE(classname) \
    classname(classname&&) = delete;\
    classname& operator=(classname&&) = delete
    
#define CONCATENATE_IMPL(s1,s2) s1##s2
#define CONCATENATE(s1,s2) CONCATENATE_IMPL(s1,s2)

#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define STRINGIZE_IMPL(x) #x

#define BITSIZEOF(x) (sizeof(x) * CHAR_BIT)

#define WN_PREDICT(expr, value, _probability) __builtin_expect_with_probability(expr, value, ({ \
                                                constexpr double probability = _probability; \
                                                static_assert(0.0 <= probability); \
                                                static_assert(probability <= 1.0); \
                                                probability; \
                                              }))

#define WN_PREDICT_TRUE(expr, probability) WN_PREDICT(!!(expr), 1, probability)
#define WN_PREDICT_FALSE(expr, probability) WN_PREDICT(!!(expr), 0, probability)

#define WN_LIKELY(expr) WN_PREDICT_TRUE(expr, 1.0)
#define WN_UNLIKELY(expr) WN_PREDICT_FALSE(expr, 1.0)

namespace wn::detail {
    
    template<typename... ArgTypes>
    constexpr ALWAYS_INLINE void UnusedImpl(ArgTypes &&... args) {
        (static_cast<void>(args), ...);
    }
}

#define WN_UNUSED(...) ::wn::detail::UnusedImpl(__VA_ARGS__)

#define WN__NARG__(...)  WN__NARG_I_(__VA_ARGS__,WN__RSEQ_N())
#define WN__NARG_I_(...) WN__ARG_N(__VA_ARGS__)
#define WN__ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define WN__RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

#define WN__VMACRO_(name, n) name##_##n
#define WN__VMACRO(name, n) WN__VMACRO_(name, n)
#define WN_VMACRO(func, ...) WN__VMACRO(func, WN__NARG__(__VA_ARGS__)) (__VA_ARGS__)
#define WN_NAMED_VMACRO(name, func, ...) WN__VMACRO(func, WN__NARG__(__VA_ARGS__)) (name, __VA_ARGS__)