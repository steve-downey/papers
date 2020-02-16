---
title: 	Allow Templates in Local Classes
document: D1988R1
date: today
audience:
  - EWG
author:
  - name: Steve Downey
    email: <sdowney@gmail.com>, <sdowney2@bloomberg.net>
toc: false
toc-depth: 1
---

# Abstract
Local classes should be permitted to have member templates.

# Before / After Table

::: cmptable

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

# Implementation Experience For C++98

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


# Why C++ 98
In C++98 there is no way for a local class to escape the scope in which it is declared. (Un)Fortunately in C++11 we allowed the `auto` keyword and the deduction of the return type of a function.

What this means for templates in local classes is that the instantiation point may be outside the scope that the class is declared in. This leads to complications.

If the local class is returned from such a function, and the member template is used, the context defining the member, in existing implementations is lost.

~~~C++
template<typename T> auto f(T t) {
    struct X {
      template<typename U> void g() {
        decltype(t) x = 123; }
      };
    return X();
}

void h() {
  f(nullptr).g<int>();
}
~~~

With the patch above, clang will ICE. The corresponding lambda expression is diagnosed as an error.

~~~C++
template <typename T>
auto f(T t) {
    auto y = []<typename U>(U u) { decltype(t) x = 123;};
    return y;
}

void h1() { auto k = f(nullptr);};
~~~

clang reports:

~~~
<source>:3:48: error: cannot initialize a variable of type 'decltype(t)' (aka 'nullptr_t') with an rvalue of type 'int'
    auto y = []<typename U>(U u) { decltype(t) x = 123;};
                                               ^   ~~~
<source>:7:22: note: in instantiation of function template specialization 'f<nullptr_t>' requested here
void h1() { auto k = f(nullptr);};
                     ^
1 error generated.
Compiler returned: 1
~~~

[Compiler Explorer](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAM1QDsCBlZAQwBtMQBGAFlICsupVs1qhkAUgBMAISnTSAZ0ztkBPHUqZa6AMKpWAVwC2tEAGYAbKS3oAMnlqYAcsYBGmYiADspAA6oFQnVaPUMTQX9AtTp7Rxcjd09ORWVMVWCGAmZiAlDjU0sUlWjaTOyCWOc3D29FLJy88OSFeoqHKoSazgBKRVQDYmQOAHICTCNfYTGAanEzHQIAT18tZiNMaYAVOexxAAYAQWYDImmqCE3pgm7Zr1lD6cfp49PF2bMAEVmAVllvj7mC2Wq3W0wAqjsIGDpgYbuI7tMsMhWEsVhBrtMAB7vL6cSRmOayLwAsz3A5PabETAEAa0aaLQn7A7wgGHJkAN1QeHQ0wQnAgcIRL1Q0wA1jizhBaAZWKxfARiN1GcTCbNDgB6dWc7m8yQC27SZ4nEXiuZfc7S2XyxWq0UQHrKkmG4a9VggYbfYakUzDPZe1DunRyOTTBT9QYbKRmThegjuv3dXqikDcTgAOgsmaz2azPjdw24Xp9ftIAeGXoUID2pDjvpdpDgsBgiBQdBoxCMIiG5EoaAmeHYxErsD7vgHHgAkugUMJRAB9BUGWii0vy4KV4YAWk3LXQZokMjkkmY9Ygo/HxCnIGAClozF8CgQqAIq5KG%2B3u/3wZkx5dvUw%2BBEMQ3KCLOwBXgesgyEIeCuMOAquu6nrevG/qBm2JCdrQQzTOyeCYAA7h40wQLghAkLM%2BI9NMkjqpwJa1gmvQIJgzBYJ4AqkMmZgABxpjxAmCUJAkAJxCO6hakEYIDfNWxZoeWihVjWqG9I2UDNhASDnoOPZnqg/aDjOIjAAuxBLiuVADmMQ6UK4qGkK4DjZIs7oxqQfbrPQADytCsK5dakFgWHAOwDn4FS6TspgG4lpgmJpCcIzuQ4Yz5iWrCwcQLl6FgbmxsB0nDDGvQ0PQTBsBwPD8KBJkoN%2B8iZXB8C9Kga50O%2BO4EHunyQUeJ59AMQxcIhHpFg5ZaYjxFibhY3DTMiJkkYuy43KRgEUVGyTTHohnEVtNxBoeMixqpzGsexlCjZJ0myShgVlhWymMaemnaQZY66RQ%2Bl7Z4N53g%2BT4vlZKIeJWED2YFTl3sQAXuZ5WgEL5/nheMJlhYFEVpGo0WxV68WJWM%2BXkPQygOU12Ww7lyUFXgRUlaQZWMCwYXVQIkhCHVfXQU18Gte1tCdZ%2BvUNb%2Bg0RiN4ljfdJaTdNs3zcAyDIMt5mrSRZFAZR0akDtH0XtrkiHQ1p11ompAsWxNScdxZhpmYDuO07ju8PmN0yXJE3uk91YvebyaSHx3wO9wIkWF4ZiSHskgWJw3xS2Y40Pd7Klmw2mlvSA/QEL4Jx6TpXTWBtSSM3QzOVVwvAEdlvj5aNyHyaW7pbdMBGEAgWLy3NC1garFndKbTEWxd1tJiAkiSGmE/TzPM9mFL7t3Y3j1Kb7qnp/AWlZycucvt9BeeBzAHkSXTMVazVc13XUsN17wxRpIrft53M3d0rKvomrooD6nQ%2BW5dnE3Zelup7ZOilKxrzNmPSOaZvheC8LHbgZgvAiT2DxUOPEE5J1linP2Y9vh8WdkQx2Vh8ySGAVwPYoCcGKTwVLchMsFKDz/KQaKQ5ggpiAA%3D%3D%3D)

Interestingly, there is some divergence in detecting the error. GCC doesn't unless k is invoked:

~~~C++
void h2() { auto k = f(nullptr); k(1);};
~~~

~~~
<source>: In instantiation of 'f(T) [with T = std::nullptr_t]::<lambda(U)> [with U = int]':
<source>:8:37:   required from here
<source>:3:52: error: cannot convert 'int' to 'std::nullptr_t' in initialization
      |     auto y = []<typename U>(U u) { decltype(t) x = 123;};
      |                                                    ^~~
      |                                                    |
      |                                                    int
Compiler returned: 1
~~~
[Compiler Explorer](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAM1QDsCBlZAQwBtMQBGAFlICsupVs1qhkAUgBMAISnTSAZ0ztkBPHUqZa6AMKpWAVwC2tQVvQAZPLUwA5YwCNMxEAA5SAB1QLC62nsMTQS8fNTorG3sjJxd3JRUw2gYCZmICAONTTkVlTFU/ZNSCCLtHZzdFFLSMoOyFKuLrUujy1wBKRVQDYmQOAHICTCMPYUGAanEAZh0CAE8PLWYjTDGAFSnscQAGAEFmAyIxqghVsYI2iYB2WV2xu7H9w9mJyYARCYBWWQ/XqZn5xbLMYAVQ2EGBYwMF3E1zGWGQrDmCwg5zGAA8Xu9OJJJlNZJdfpMbjt7mNiJgCN1aGNZnjtjsYb9dvSAG6oPDoMYITgQaGwx6oMYAa0xRwgtAMrFYHgIxDadIJeImuzZHK5kl5V2kDwOgpFU3exwlUplcqVQognHlRMZSr6HVYID6Hz6pFMfS2rtQTp0cjkYwUXR6Kykk04roITs9bQ6QpA3E4ADoAGyptPptOXIRO7iu92e0jevquhQgLakSMe%2B2kOCwGCIFB0GjEIwiXrkShoYZ4djEUuwLseHvOACS6BQwlEAH1ZQZaELCzK/KW%2BgBaVf1dAGiQyOSSZjViCD4fEMcgYAKWjMDwKBCoAiLxIr9eb7d%2BmT7%2B0dTD4IjEDmCJOwBnjusgyEIeAOP2vIOk6LpulGXo%2Bk2JCtrQvRjCyeCYAA7s4YwQLghAkBMOJWmMkgAPScAWlbRh0CCYMwWAuLypBxpMriJq4PG8XxPEAJzZn0uakEYIAfOW%2BZIcWihlhWiEdLWUD1hASDHr2HZHqg3a9hOIjADOxBzguVA9oMfaUA4iGkA41ipLMTrhqQXbLPQADytCsI5VakFgaHAOwNn4OS%2BQspgK4FpgaJ5Ac/TOdYgyOr5rCQcQDl6FgTkRv%2B4l9OGHQ0PQTBsBwPD8IBBkoO%2B8ipVB8AdKgS50M%2BG4EFubygXuB45AkfiaNoNRZKQ5glFEMTBN4vh0ENk2hH4Y1lC4dS5PkdCFNU%2BiZII8R5IkG2NJES07Q0s11A0i0tMtHSBt0vRcLBzp5jZRZoq4yarsm3BjAiBkEbO84XIRv4kaG2RjHoun4WDFy%2BruMgRopDFMSxlCPaJ4mSQhvlFiW8l0YeqnqTpQ6aRQ2lQy4F5Xjed4PmZiLOKWEDWb5dlXsQPnOa5WgEJ53nBUMBlBb5IV7Xg4WRa60WxYM2XkPQyg2XV6Wc5l8U5XgeUFaQRWMCwQXlQIkhCFVXXgXV0GNc1tCta%2BnU1Z%2BvXixoEDmGdI3aJdE3ZCE03%2BFttSeFNiTe%2BUK19etp2B8Nu1rUkF1NON4eVEUHv1EUYfXZ0d1lY98HSYWTpvR9X1jMAyDIP9xmAwRRF/qRYakBDJMno3kiwzViNVjGpCMcx5RsRxkyJpMY/jxP4%2B8MlGMSVJL1Onj5YE73caSFxHxj9wAnJpckySFskjJpwHzCZMz044vCk9zWqlEyAXQEB4BxaRpKc/sRy263Q%2BulVwvA4XSh4bK%2BcL4FiLGDMYOFCAIHRO9T631fqiGriZNo3d6J9xRoPWMIBJCSETHgwhRCiGTGErPLGhdcZyWXopW%2B8A1IPwOM/B85M34uBNh/P8gg9YlUNgAoBIDhIFwXn0UMkgoEwLgaXb6Fcq4ohrkKNB18MH91RmxGerpMbz0vrJUsNCe44P3omD4lxLjH24JMS4AktiuG3u4ZK59sbgKvivHBHwuKT08ePZMwlJCaK4FsbRzjZKuN8WAmS6CvykHCn2fq3AgA%3D%3D)

I believe that clang is being too aggressive in instatiation of the `operator()` of the lambda, but I have not constructed an example that errors on otherwise well-formed code.

# Modules and Reachability
The problem of unnamable types whose members might be templates is similar to that of types in modules that are not exported but are present in interfaces that are.

~~~C++
export module M;
struct X {};

export {
    X func(){return X{};}
}
~~~
The name X is unavailable outside the module, but the function `func` is. `auto x = funct();` is valid code.

This suggests we could leverage Reachability in order to provide meaning to local classes outside their declarative region.
From [module.reach](http://eel.is/c++draft/module.reach)

>[3]{.pnum} A declaration D is reachable if, for any point P in the instantiation context ([module.context]),
>
>[3.1]{.pnum} D appears prior to P in the same translation unit, or
>
>[3.2]{.pnum} D is not discarded ([module.global.frag]), appears in a translation unit that is reachable from P, and either does not appear within a private-module-fragment or appears in a private-module-fragment of the module containing P.
>
>[ Note: Whether a declaration is exported has no bearing on whether it is reachable. — end note ]


The language already requires that the declaration of D, the local class, appear before any use of it, because of the existing rules for `auto`.

[module.global.frag](http://eel.is/c++draft/module#global.frag) provides

> [4]{.pnum} A declaration D in a global module fragment of a module unit is discarded if D is not decl-reachable from any top-level-declaration in the top-level-declaration-seq of the translation unit. [ Note: A discarded declaration is neither reachable nor visible to name lookup outside the module unit, nor in template instantiations whose points of instantiation ([temp.point]) are outside the module unit, even when the instantiation context ([module.context]) includes the module unit. — end note ]

If we extend decl-reachable to include local classes that are made available via the return type of its declaring function, the defintion of the local class would be _reachable_ for purposes of instantiating its member functions.

I do not know if this is viable in any sense as an implementation, but it seems to be a viable direction in terms of the standard.

# Wording

>[temp.mem.2]{.pnum} [A local class of non-closure type shall not have member
>templates.]{.rm} Access control rules apply to member template names. A
>destructor shall not be a member template. A non-template member function
>([dcl.fct]) with a given name and type and a member function template of the
>same name, which could be used to generate a specialization of the same type,
>can both be declared in a class. When both exist, a use of that name and type
>refers to the non-template member unless an explicit template argument list is
>supplied.

I have not figured out how to word decl-reachable yet.
