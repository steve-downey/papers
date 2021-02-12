---
title: "Add a `pmr` alias for `std::stacktrace`"
document: P2301R0
date: today
audience: LEWG, LWG
author:
  - name: Steve Downey
    email: <sdowney2@bloomberg.net>, <sdowney@gmail.com>
toc: false
---


Abstract: This paper proposes to add an alias in the `pmr` namespace defaulting the allocator used by the `std::basic_stacktrace` template to `pmr::allocator`. No changes to the api of `std::stacktrace` are necessary.

# Before / After Table

::: tonytable

### Before
```C++
char buffer[1024];

std::pmr::monotonic_buffer_resource pool{
    std::data(buffer), std::size(buffer)};

std::basic_stacktrace<
    std::pmr::polymorphic_allocator<std::stacktrace_entry>>
    trace{&pool};
```

### After
```C++
char buffer[1024];

std::pmr::monotonic_buffer_resource pool{
    std::data(buffer), std::size(buffer)};

std::pmr::stacktrace trace{&pool};
```

:::

# Motivation

The type `std::basic_stacktrace` is a class template with an allocator type parameter, the allocator is fixed to be `std::allocator` in the `std::stacktrace` alias, and it models _AllocatorAwareContainer_. All of the work to enable a pmr using type has been done, except for actually specifying the existence of the template alias std::pmr::stacktrace with a polymorphic_allocator.

In general a template should have an alias in the `pmr` namespace when the primary template supports the allocator model and the allocator template parameter is defaulted to be `std::allocator`. Providing the `pmr` alias is a convenience for changing the default.



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
[Compiler Explorer Link](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAM1QDsCBlZAQwBtMQBGAFlICsupVs1qhkAUgBMAISnTSAZ0ztkBPHUqZa6AMKpWAVwC2tQVvQAZPLUwA5YwCNMxEAFYAbKQAOqBYXW0eoYmgj5%2BanRWNvZGTi4eisqYqgEMBMzEBEHGppyJKhG0aRkEUXaOzm6eCumZ2SF5NSVlMXFVAJSKqAbEyBwA5FIAzNbIhlgA1OJDOkaYRiQAntPY4gAMAILDo%2BOYUzNzC8SLAPrEmArdvZgr61ubd7TMcwpezH0TNehTAOyymxMJgB6IFTVzSJrIADWBGI70wADotLDlq4ACKkCZjZgKBSfdLQ2Hwk7iHSScQbNHI453QHY3H494wuF9E7U5ZDf73DaAkFgiEE5nwhEOHF4CTozH0vEEeZeYSyiaivzIElkiloyFCvq0iayozy5iy6Y6aUTDasVioFhEYi3AGArHCBnK8UnLVEnWcu66vmu1Wk8mUj0svYERZeTBYKgTJ4vXUGPyiRmE0P7NFKsWqkPwk1sK02kgmnOs9m3Iarb0PB188TgkuI/0I2h0Q51jGxugAWkOcQmVAMtBSdAUuv1huNMzNFoLRqLFd1gIAbqg8N8FAB3ZheCD%2B92Cz03GYz61zu0LyTuCbMTF7hsmk%2BF8%2BrS9K9qLx0tzAADz6XgIEBfr%2BmD/hAzAIpu267u0MHTFyuo1MQ1jAHqqD7khogQGgtA1Cm2qYGy9A0q%2BVDvlW3KAuOCpHqazp4o%2BZ72jyjqIchqHochWEjgQmYqvuTKHg%2BlqnratyvjUZHwQ6VFGjRZrIAgGQACpSnRepwoQo4Lg6gJ7r4sKYM8JoKcpmJEppYnuB%2BgKoJGcKiTMJq7lmJz6ecRlTopxAqepzAWReV6%2BFK3F4YehEolIV6kXBPrSXK1HGWpJneapOIyhpBAKKlDIMQ5qw6bxbpuYZRjGV5PnmZllnWRMtnOIxjkzM5fHFR5prlWZGVaS%2BgVZViIV3geoZCbOeWRfikmxcxcYXG8HxeEYxC/FyjoTImbENum%2BLoCAICDQJw0zD4rCLEcXgIG6%2BYifOOgNuFNILo9K2/GiU2%2BqCdYCgdwpNopCgIO2mJ/QgnwGF4PiZGO8WyYlaUTEpKz4sQBiqBMwMxXFBoJY12BIyjPHozMd1lk9UNYzDU5qbl8644h%2BNoziAMzPtqa5sewlPuWlbwT8r3Vhs1g8U4NDnBA7TLXcyVKgYVBUM4n2cGskjcO2GPcl8u0LS4IALC2RC0G6Dgy3LxBnBcVzzag%2BjiH8H4ayA6BGswu7G84nTbbtfgAF6YC7stuzbr3kXc9ss/hJp2wQO0gFrmv6KdJDnZdHMNbdUee0NxIk5W%2BXMZRw225ePjW7zauB1NgvXlQsrEGLEubFLRv%2B3a4KK8rqvB5s9uxzrdCoPrhuu6b5yXD0lsl89gL2476R%2Byb7v297vtN/P5edxs3eLRn30fIJhfuMXrBrzzaL9J0rAgP0rj9KQpj9GsN%2BoJfOhyHInwW3swycDfBCXw/MGkChCAIYQwEQgPARAyBngL79G4DfO%2BD9SBP36DfBQIA1ikF/vfM%2BpA4CwCQGgA0eB2BkAoFxIhJCUDCFECcWEg4oSkCoMQmuaDdx/xvg4awGRFiX2/qQQhcx6AAHlaAnXYaQLARgRDAHYOI/A5wUhLguOIn8yQDCyl4TfQWyhxGsDwA4OExw9BYE0ZgpCRhNGdBoPQJgbAOA8H4IIahYhX4yCEPotBkBOi2UKGg/oXYuxfGmGiCQMg5CSEQUoAoARNDaHqLkUg5gWgVBcHkMI/g6DxNCL4DJtBkmxEqI0JIw4ii1CyPoHIggonJEKMUTI%2BS2iNDKVkppzRrDlAKakzoo9rhcHPpfa%2Bt9xHIO/AADncF2dw3AnTSImBAOhtAoTiwgLgQgJApiSCGHkCYegKHOA2Vs8WL8wkyB/uwgBQCQFgMgTckB0DL5wKGdgpBl9UHoMwec3BMBEAgG6AQLw6jyCUEIV4YhhTEn4FtIIaxjAWCyIcRuOEXhLFCAGfA4Zl8v4TA3IQEGYyJlTJmcmeZyNFntDOdgi5wDQG3Nuai2B6LnnILeRgrB/9%2Bn9EkIyxBzKPmUs6Eo4gfgNDcCAA)
