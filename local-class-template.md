---
title: 	Allow Templates in Local Classes
document: D1988R0
date: today
audience:
  - EWG
author:
  - name: Steve Downey
    email: <sdowney@gmail.com>, <sdowney2@bloomberg.net>
toc: false
---

# Abstract
Local classes should be permitted to have member templates.

# Before / After Table

::: tonytable

### Before
```C++
// Extracted From LLVM test case temp.mem/p2.cpp
void fun() {
    struct foo {
        template <typename> struct bar {};     // Error
        template <typename> void baz() {}      // Error
        template <typename> using corge = int; // Error
    };
}
```

### After
```C++
// Extracted From LLVM test case temp.mem/p2.cpp
void fun() {
    struct foo {
        template <typename> struct bar {};     // Allowed
        template <typename> void baz() {}      // Allowed
        template <typename> using corge = int; // Allowed
    };
}


```
:::

# Implementation Experience

Implemented in clang by deleting the check for template inside local class.

```diff
diff --git a/clang/lib/Sema/SemaTemplate.cpp b/clang/lib/Sema/SemaTemplate.cpp
index 5633582d679..d155bf8b5e4 100644
--- a/clang/lib/Sema/SemaTemplate.cpp
+++ b/clang/lib/Sema/SemaTemplate.cpp
@@ -7385,12 +7385,7 @@ Sema::CheckTemplateDeclScope(Scope *S, TemplateParameterList *TemplateParams) {
     if (CXXRecordDecl *RD = dyn_cast<CXXRecordDecl>(Ctx)) {
       // C++ [temp.mem]p2:
       //   A local class shall not have member templates.
-      if (RD->isLocalClass())
-        return Diag(TemplateParams->getTemplateLoc(),
-                    diag::err_template_inside_local_class)
-          << TemplateParams->getSourceRange();
-      else
-        return false;
+      return false;
     }
   }

```

Built a stage2 bootstrap of llvm, including libc++, which succeeded.
```sh
CXX=clang++-8 CC=clang-8 cmake -DCLANG_ENABLE_BOOTSTRAP=On \
-DCLANG_BOOTSTRAP_PASSTHROUGH=\
"CMAKE_INSTALL_PREFIX;CMAKE_BUILD_TYPE;LLVM_USE_LINKER;LLVM_PARALLEL_LINK_JOBS;LLVM_ENABLE_PROJECTS" \
-DCMAKE_INSTALL_PREFIX=~/install/llvm-localclass/ -DLLVM_ENABLE_LIBCXX=yes \
-DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=yes -DLLVM_USE_LINKER=lld \
-DLLVM_PARALLEL_LINK_JOBS=1 \
-DLLVM_ENABLE_PROJECTS=\
"clang;clang-tools-extra;compiler-rt;debuginfo-tests;libcxx;libcxxabi;libunwind;lld;lldb;llvm" \
-G Ninja ../llvm-project/llvm/
```
The test suite produced one new set of errors, the tests checking that templates not be declared inside of a local class.

    error: 'error' diagnostics expected but not seen:
      File <snip>/temp/temp.decls/temp.mem/p2.cpp Line 8: templates cannot be declared inside of a local class
      File <snip>/temp/temp.decls/temp.mem/p2.cpp Line 9: templates cannot be declared inside of a local class
      File <snip>/temp/temp.decls/temp.mem/p2.cpp Line 10: templates cannot be declared inside of a local class
      File <snip>/temp/temp.decls/temp.mem/p2.cpp Line 11: templates cannot be declared inside of a local class
      File <snip>/temp/temp.decls/temp.mem/p2.cpp Line 12: templates cannot be declared inside of a local class
    5 errors generated.


# Wording

[temp.mem.2]{.pnum} [A local class of non-closure type shall not have member
templates.]{.rm} Access control rules apply to member template names. A
destructor shall not be a member template. A non-template member function
([dcl.fct]) with a given name and type and a member function template of the
same name, which could be used to generate a specialization of the same type,
can both be declared in a class. When both exist, a use of that name and type
refers to the non-template member unless an explicit template argument list is
supplied.
