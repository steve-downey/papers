<div class="ABSTRACT">
Abstract: Building C++ is especially difficult to do reliably. Modern infrastructure and tools can help.

</div>

<div class="ABSTRACT">
Audience Takeaways: A better sense of why the C++ package ecosystem is so awful, why building C++ is so hard, and some tools and techniques for improving the situation.

</div>


# Table of Contents

1.  [Introduction](#org740a0cb)
2.  [No standardised package manager](#orga0ec6ae)
3.  [Library ABI is a problem &#x2013; C++ leaks across boundaries](#orga956cbe)
4.  [Dependency changes require rebuilds](#orge5e3675)
5.  [Replacing OS supplied packages is risky](#org0eb34d9)
6.  [Shared libraries are risky - DLLHell](#org8bdd580)
7.  [Containers and Build Isolation](#orge5258e0)
8.  [Deployment Isolation](#org142e411)
9.  [Quick automated demo](#org9a4cba5)


<a id="org740a0cb"></a>

# Introduction

C++ is notable among modern languages in not providing a package management system by which developers can easily install and reuse code developed by others into their own projects. It shares this with C, and for similar reasons. Until recently, source distribution was the exception, rather than the rule, and pre-compiled libraries are fragile, as they must, in general, use the exact same set of dependent libraries, compiled the same way. This largely limits reuse of packages to those provided by an OS or distro vendor. A modern Linux or BSD system may provide 1000s of such packages, however changing any of them may cause the entire system to fail. There is huge tension between Long Term Support versions and making current packages available.


<a id="orga0ec6ae"></a>

# No standardised package manager

Unlike tools such as node.js, python, or rust, there is no standard package manager. There isn't even a widely used one that isn't what comes with an OS. This is partly because C++ is not a single vendor system, so no vendor is in a position to standardise a solution, and also because of actual technical difficulties with shippint C++ binaries that will work.


<a id="orga956cbe"></a>

# Library ABI is a problem &#x2013; C++ leaks across boundaries

C++ libraries usually pass by value, which means that layout of an object must match exactly. Inline functions also cross between translation units, which means that even if an object is passed by reference or pointer, manipulating it requires agreement on layout. This is also true transitively, so that any type used by a library must be the same everywhere in a program. There's an official rule, the One Definition Rule, and if it is broken, there is no defined behavior, which usually means crashing.


<a id="orge5e3675"></a>

# Dependency changes require rebuilds

If anything your C++ code depends on changes, to be safe you must recompile your code, as well as all other code that depends on that. If you are library code, you now have to rebuild everything that depends on you. Package build managers like debian'a sbuild do some of this work, keeping a distro in sync.


<a id="org0eb34d9"></a>

# Replacing OS supplied packages is risky

You're using system supplied packages because they are easily available and most Linux systems provide a wide range of them. But you want to upgrade one of them because you need a new feature. So you upgrade it, put it in /usr/lib and now your OS won't boot properly because some other critical component used it. The standard solution for this is install in /usr/local, but that has similar scaling issues. The more software in /usr/local, the harder it is to just swap out one library.


<a id="org8bdd580"></a>

# Shared libraries are risky - DLLHell

Shared libraries have versioning issues. It's possible to specify that a shared library is compatible with older versions, but it's very easy to get wrong. See the library ABI issues from before.


<a id="orge5258e0"></a>

# Containers and Build Isolation

Dpkg has been using isolation in the form of \`chroot\` jails basically forever. This forces software being built to not look outside a particular directory tree in the filesystem, changing the root of the filesystem. Containers such as docker take this to a greater level, providing even more isolation. Isolating the build of your system from everything else can give you fine grained control of your environment and during the build. It can be useful to build software to be deployed into an organization standard location that is separate from any other system software. For example building software systems to run in \`/opt/bb/\` with a GNU style FHS within there &#x2013; \`/opt/bb/bin/\`, \`/opt/bb/etc/\`, \`/opt/bb/share\`, and so forth. This helps prevent collision, and since the only things in the container are what you choose to put there, the chances of accident are low.


<a id="org142e411"></a>

# Deployment Isolation

Containers also provide deployment isolation. You don't have to worry about incompatible shared object libraries from some other application or system because there are no other applications or systems in the container. However, because of the dependency problem, shared objects do not provide huge benefits. Many C++ experts prefer and recommend static linking, rather than deferring the link to runtime.


<a id="org9a4cba5"></a>

# Quick automated demo

Hey, Rocky! Watch me pull the rabbit out of my hat!
