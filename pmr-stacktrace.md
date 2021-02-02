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

```
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

  @@[`namespace pmr {`]{.add}@@
    @@[`using stacktrace = std::basic_stacktrace<polymorphic_allocator<stacktrace_entry>>;`]{.add}@@
  @@[`}`]{.add}@@

  // [stacktrace.basic.hash], hash support
  template<class T> struct hash;
  template<> struct hash<stacktrace_entry>;
  template<class Allocator> struct hash<basic_stacktrace<Allocator>>;
}
```
[Compiler Explorer Link](https://bbgb.dev.bloomberg.com/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwgFtM2kgE8N2DgAYAgiya7BAByYTVggunUB2PhdWrMLEdtUlJkFBVQAjAwJMVRBVERZBPGAWTFdkBCZidz43ABF1WR5vbzNzYtUrG3tHW20sjg9S8vKo7VtmKI1NQOYQ1QAVAvzndBAQCKiCky9m8qC%2B21QGA31iWwQ8ZAB9JgYGVGQmImINTzLihtzSy9LKzDsHaJHsptUAejf1AFYeZwcAawIxEeADpfECjF9cqQesFQn9kIDgRIthxNFwOOZcuDiEYZvN4QQAUDHlscUZCtcZh9vr8iYiSRIQWFgpsOFCYQTVK12kdoizEts0RisQikY9XjyOpgulzzHsDkcSMZXt4uQLNlsxYyZZSLK8aRqhejMbltcjogQDLZUpgAGYVaz3V4iRJsJz08WODT5I1az06rq7faHY5dc2k8kq2QmPXmA2fdl04kW5ms5AglioFi6bTs6EVbMAWlzYUwWTt8WQBDw2cEkr0vM6WjlCtDypjqtUADdUHhXIIAO5MWwQP0RiRdeUhpUnTtcABsqiYMPHAYtU7bs5Vi/CAEou94s5gAB4SWwECDHs%2BYC8QJggocjsd71%2BnKnnZzEBTAbmof3fmwEBoAkBAeimkYsBC3BLnaB5xg2bTSrKvShNOiphp2MzeF%2BP5/gBP7AXWYFrhBk5aOh7ZziYu7OPBZyIU2uqaFy6SZP0nKodywKEPWWHnN4fqoF%2BmDWLKGTEBx3FMLxO4LoeqioDawKYZoXRjumWzCUCol5i2ElSSSsnzkuwmcsR4EMhaZJQbiMGqHB776jMUp8ihcKBAZnEeUZBCCN5fSUdu/HlEJIlifp7Ewr5fE0fJ2HFEp5bBWpWgaYKWnhXpLFedJxlxYp/mBBZpFWRKFFbphBV0U58YzHcDw1HULwJd4rp4RO0Q%2Bh6ozjJpnVdIsyyrOsmrBhhHaaJ1NnQZ2c1nCUeQft4NJJp1aaCiCGSCAg%2BYwttCBOCItiLMQBCMchLZcf0xgesQIjVqoB21d4rnNql2B3Q9YHPVo01RvNF1uVdHlBVVX2Pb9milV6zFgx2saxgxeRSHujDSF8UikCw0imFjqDSGpvD8E4ojiF1XCyLQWMELjqNo/8ICyLIILM2z7Mcwu6NSAALFjONSHjpAE1IWOCCApikLTguo6QcCwEgaBtHgDDluQlBK7YKvliAwDIMgRZuKQdoq1ExDi2OdOkGECiZAY0jU6QSu6FBADyLDLFbWDaKwwCq17eDEJg1Z4N29xW6ewciFEDtYwoUQMLHjB4GEwK4poGCSFIjtAng2ix2jzBsCg/D8MnYTi7AdwgOgtj/MApBh8QIDSs4RYLjzr7CxetYJNIRau7IqhFgA6sGw8j6eJJi2TEh0IXGP81bIsAOKaN0biqBAuCECQ6iU/QqgZ8rqv1Afe5H6XvA03TXeM1TIJuAAHE/phuAAnDzbimLQ79PwuRtE68yXjLYW0gxYSylrfOWMBEAgFEAQWw0d1ZERPjregqRd7N3oIOYEtgC7c0xtjZe0g5C0FUIOQgh014bxvjLO%2BTMeYgifs/L4LDf4dwAe/d%2B3M%2BbENASLCBktpZ4y7kArgIChaCKgfQtGTdEjZhADzIAA)
