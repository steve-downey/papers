---
title: 	Allow Templates in Local Classes
document: DnnnnR0
date: today
audience:
  - EWG
author:
  - name: Steve Downey
    email: <sdowney@gmail.com>, <sdowney2@bloomberg.net>
toc: false
---

# Abstract
Local classes should be permitted to have templates, just as lambda objects do.

# Before / After Table

::: tonytable

### Before
```C++
// From LLVM test case temp.mem/p2.cpp
void fun() {
    struct foo {
        template <typename> struct bar {};     // Error
        template <typename> void baz() {}      // Error
        template <typename> void qux();        // Error
        template <typename> using corge = int; // Error
        template <typename T> static T grault; // Error
    };
```

### After
```C++
// From LLVM test case temp.mem/p2.cpp
void fun() {
    struct foo {
        template <typename> struct bar {};     // Allowed
        template <typename> void baz() {}      // Allowed
        template <typename> void qux();        // Allowed
        template <typename> using corge = int; // Allowed
        template <typename T> static T grault; // Allowed
    };
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

Built a stage2 bootstrap of llvm, including libc++, which succeeded. The test suite produced one new set of errors, the tests checking that templates not be declared inside of a local class.

    error: 'error' diagnostics expected but not seen:
      File /home/sdowney/bld/llvm-project/llvm-project/clang/test/CXX/temp/temp.decls/temp.mem/p2.cpp Line 8: templates cannot be declared inside of a local class
      File /home/sdowney/bld/llvm-project/llvm-project/clang/test/CXX/temp/temp.decls/temp.mem/p2.cpp Line 9: templates cannot be declared  of a local class
      File /home/sdowney/bld/llvm-project/llvm-project/clang/test/CXX/temp/temp.decls/temp.mem/p2.cpp Line 10: templates cannot be declared inside of a local class
      File /home/sdowney/bld/llvm-project/llvm-project/clang/test/CXX/temp/temp.decls/temp.mem/p2.cpp Line 11: templates cannot be declared inside of a local class
      File /home/sdowney/bld/llvm-project/llvm-project/clang/test/CXX/temp/temp.decls/temp.mem/p2.cpp Line 12: templates cannot be declared inside of a local class
    5 errors generated.


# Wording

[temp.mem.2]{.pnum} [A local class of non-closure type shall not have member templates."]{.rm}
