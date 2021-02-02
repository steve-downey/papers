---
title: "Add a `pmr` alias for `std::stacktrace`"
document: D2301R0
date: today
audience: LEWG, LWG
author:
  - name: Steve Downey
    email: <sdowney2@bloomberg.net>, <sdowney@gmail.com>
toc: false
---


Abstract: This paper proposes to add an alias in the `pmr` namespace defaulting the allocator used by the `std::basic_stacktrace` template to `pmr::allocator`. No changes to the api of `std::stacktrace` is necessary.

# Before / After Table

::: tonytable

### Before
```C++
char buffer[1024];
std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};
std::basic_stacktrace<std::polymorphic_allocator<std::stacktrace_entry>> trace{&pool};
```

### After
```C++
char buffer[1024];
std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};
std::pmr::stacktrace trace{&pool};
```

:::

# Motivation

The type `std::stacktrace` is a class template with an allocator type parameter, the allocator is defaulted to `std::allocator` template parameter, and it models _AllocatorAwareContainer_. All of the work to enable a pmr using type has been done, except for actually specifying the existence of the template alias std::pmr::stacktrace with a  polymorphic_allocator.

# Proposed Wording

Modify __[stacktrace.syn]{.sref}__ as indicated

```cpp
namespace std {
  // [stacktrace.entry], class stacktrace_­entry
  class stacktrace_entry;

  // [stacktrace.basic], class template basic_­stacktrace
  template<class Allocator>
    class basic_stacktrace;

  // basic_­stacktrace typedef names
  using stacktrace = basic_stacktrace<allocator<stacktrace_entry>>;

  // [stacktrace.basic.nonmem], non-member functions
  template<class Allocator>
    void swap(basic_stacktrace<Allocator>& a, basic_stacktrace<Allocator>& b)
      noexcept(noexcept(a.swap(b)));

  string to_string(const stacktrace_entry& f);

  template<class Allocator>
    string to_string(const basic_stacktrace<Allocator>& st);

  template<class charT, class traits>
    basic_ostream<charT, traits>&
      operator<<(basic_ostream<charT, traits>& os, const stacktrace_entry& f);

  template<class charT, class traits, class Allocator>
    basic_ostream<charT, traits>&
      operator<<(basic_ostream<charT, traits>& os, const basic_stacktrace<Allocator>& st);
```
::: add
```cpp
  namespace pmr {
    template<>
      using stacktrace = std::basic_stacktrace< polymorphic_allocator<stacktrace_entry>>;
  }
```
:::

```cpp
  // [stacktrace.basic.hash], hash support
  template<class T> struct hash;
  template<> struct hash<stacktrace_entry>;
  template<class Allocator> struct hash<basic_stacktrace<Allocator>>;
}
```
