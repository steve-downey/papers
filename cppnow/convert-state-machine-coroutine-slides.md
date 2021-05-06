# Converting a State Machine to a C++ 20 Coroutine

Steve Downey

© 2021 Bloomberg Finance L.P. All rights reserved.


# Abstract

C++ 20 coroutines can naturally express in linear code components that are today written as state machines that wait on async operations.

This talk walks through using the low-level machinery and customization points in C++ 20 to convert a state machine, which waits at the end of steps for async service operations to complete, into a single coroutine that \`co\_awaits\` those operations.


# Super brief and inaccurate summary of C++20 Coroutines

-   Whatever you think they are?
-   It's probably less than that.


### Like a lambda

Except

-   The lambda is the return type
-   They control when they suspend
-   No stacks, threads, or fibers


#### Stackful vs Stackless

-   "Stackless"
-   Which means they execute on the regular stack.
-   The architectural model is very different.
-   If you don't know these terms - forget they exist.
-   Coroutine == Resumable Stateful Function


### If it co\_awaits, it's a coroutine

-   `co_await`
-   `co_yield`
    -   is a co\_await
-   `co_return`
    -   is a very special co\_await †


### A Coroutine's body

```c++
{
   promise-type promise{promise-constructor-arguments};
   try {
      co_await promise.initial_suspend() ;
      function-body
   } catch ( ... ) {
      if (!initial-await-resume-called)
         throw ;
      promise.unhandled_exception() ;
   }
final-suspend :
   co_await promise.final_suspend() ;
}
```


### Terms defined

-   **promise-type:** determined by coroutine\_traits<>, but usually a typedef in the return type.
-   **promise-constructor-arguments:** the parameters if there's a valid overload for promise-type that takes them, otherwise empty.
-   **function-body:** the body of the coroutine function.
-   **initial-await-resume-called:** was the await\_resume of the initial suspend called. Did we start.
-   **final-suspend:** † target for `co_return` which calls either `return_value` or `return_void` first then executes `goto final-suspend`.


### Awaitable and Promise


#### Awaitables are easy:

-   **bool await\_ready():** proceed or suspend - false is suspend
-   **await\_suspend:** called if await\_ready is (contextually) false
    -   **void await\_suspend(coroutine\_handle<> h):** call await\_suspend and suspend
    -   **bool await\_suspend(coroutine\_handle<> h):** call await\_suspend and resume if false
    -   **std​::​coroutine\_handle<Z> await\_suspend(coroutine\_handle<> h):** call resume() on return
-   **T await\_resume():** call when resume, T is the result of co\_await

    Awaitable interface is programmer facing.


#### Minimal Example

```c++
template <typename T>
struct awaitable : public std::suspend_always {
    // constexpr bool await_ready() const noexcept { return false; } // from std::suspend always
    constexpr void await_suspend(coroutine_handle<> h) const noexcept { h.resume(); }
    constexpr T await_resume() const noexcept { return T{}; }
};
```


#### Promises are a little harder:

-   **ReturnType::promise\_type:** typedef for the promise
-   **get\_return\_object():** the return type of the coroutine
-   return\_
    -   **return\_value():** return value or &#x2026;
    -   **return\_void():** return void
-   **initial\_suspend():** initial suspend before body
-   **final\_suspend():** final suspend after body
-   **unhandled\_exception():** called if an exception escapes Promises and coro return types are library writer facing.


### GCC's implementation is almost exactly lambda

There's an instance of an unnamable type that is tied to the particular coroutine frame. The type has a bit of state that indicates where the \`jmp\` to upon entry goes to. The coroutine function allocates one of these, and ties it to the return type via the promise.


### Minimal Boring Coroutine

```c++
#include <coroutine>

struct MinimalCoro {
  struct promise_type {
    MinimalCoro get_return_object() {
      return {.h_ = std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void                unhandled_exception() {}
  };

  std::coroutine_handle<promise_type> h_;
};

void before();
void after();

MinimalCoro func() {
  before();
  co_await std::suspend_always{};
  after();
}
```

<div class="notes" id="org9abbdf6">
<p>
Cold Start - suspends initially
Suspends at the co_await, which does nothing except yield
Suspends after after()
</p>

</div>

[Compiler Explorer Link](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:'%23include+%3Ccoroutine%3E%0A%0Astruct+MinimalCoro+%7B%0A++struct+promise_type+%7B%0A++++MinimalCoro+get_return_object()+%7B%0A++++++return+%7B%0A++++++++.h_+%3D+std::coroutine_handle%3Cpromise_type%3E::from_promise(*this)%0A++++++%7D%3B%0A++++%7D%0A++++std::suspend_always+initial_suspend()+noexcept+%7B+return+%7B%7D%3B+%7D%0A++++std::suspend_always+final_suspend()+noexcept+%7B+return+%7B%7D%3B+%7D%0A++++void+unhandled_exception()+%7B%7D%0A++%7D%3B%0A%0A++std::coroutine_handle%3Cpromise_type%3E+h_%3B%0A%7D%3B%0A%0Avoid+before()%3B%0Avoid+after()%3B%0A%0AMinimalCoro+func()+%0A%7B%0A++++before()%3B%0A++++co_await+std::suspend_always%7B%7D%3B%0A++++after()%3B%0A%7D%0A%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:33.333333333333336,l:'4',m:100,n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:g111,filters:(b:'0',binary:'1',commentOnly:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:cppcoro,ver:trunk)),options:'--std%3Dc%2B%2B20+-O3',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'x86-64+gcc+11.1+(Editor+%231,+Compiler+%231)+C%2B%2B',t:'0')),header:(),k:46.61565626839317,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compiler:1,editor:1,fontScale:14,fontUsePx:'0',wrap:'1'),l:'5',n:'0',o:'%231+with+x86-64+gcc+11.1',t:'0')),k:20.051010398273487,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)


# A bit of theory


## UML State Diagrams

Describes a "finite automaton"

Standardized as part of the Unified Modeling Lanaguage back in the last century.


### Simple Activation

![img](simple1.png)

The begin and end states correspond with object construction and destruction.


### Substate

![img](substate1.png)

The blinking state has substates on and off.

enter/ and exit/ are actions that must occur entering and leaving a state.


### Orthogonal Regions

![img](keyboard.png)

Keyboard has the orthogonal states of NumLock and CapsLock.


## UML based on Harel State Charts

A generalization of state machine diagrams more usable for human beings.

Allows for grouping states with the same parameters together as the substate chart.

Allows for history, returning to a state with the substate active when the superstate left.

A full formal model.


## Model, not necessarily Code

Modeling a system with a state machine isn't always how it is coded. The transitions, guards, and states may be easier to express in a different paradigm.

But it is possible.


# The core coroutine transform is to a state machine


## The Transform

C++ 20 coroutines are resumable functions. A coroutine is transformed into

-   a handle to the frame holding the stack variables.
-   an indicator of where to resume.
-   an instance comprising this particular execution.


## State is maintained in the coroutine frame

The coroutine frame is equivalent to the member variables of an object.


## `co_await` points are the states

The coroutine is waiting for input.


## Resumptions are transitions firing

When a transition fires the coroutine can decide how to proceed to the next state.


# State machines are more than regexps

Regular expressions are often, even usually, implemented as state machines.

The state machine is built automatically.

Very large state machines need tools to manage them.

Most state machines are small.


# Most state machines are simple


## Golden Path, Error Path, Failure Path

-   Things go well
-   Things go badly in expected ways
    -   Bad input
    -   File Not Found
-   Things go badly in unexpected ways
    -   2 + 2 == 5


## Rule of 5 to 9 and resorting to state machine tools

We can genererally hold 7 plus or minus 2 things in our heads.

More than that and the structure in our head collapses.

5 to 9 states can be maintained by hand.

100s or 1000s can not.


## Generality might mean \`goto\`

If states can be reached from any other state, the code might need a `goto`.

Forward or backward.

It will not need to leave the scope of the machine / coroutine.


## Suspension and Decision

Guarded transitions are just if tests after a suspension point.


# No std library solutions

We don't have coroutine types in the standard.

We hope to have some for C++23.


## Handcrafting types not wrong

The machinery is available for a reason.

These are tools for library writers.

Not Standard Library writers.


## Influences standardization

We want general and proven tools in the library.

We get those by seeing what actually works.


## Will continue to work

If the standard adopts some std::coro\_state\_machine it will not break yours.

If the standard never adopts some std::coro\_state\_machine it will not break yours.


# Code: simple multistep async operations

Not actual production code, but I have real code like this.


## Begin Example

```c++
class CreateUser {
    CreateUser(std::string id);
    // ....
};
```


## Lookup user or create

```c++
Result CreateUser::findUser() {
    db::getUser(id, [](std::unique_ptr<User> user) {
        userCallback(user)
    });
    return CONTINUE;
}
void CreateUser::userCallback(std::unique_ptr<User> user) {
    user_ = std::move(user);
    resume_();
}
```


## Validate request with "compliance"

```c++
Result CreateUser::okToCreate() {
    compliance::checkOK(
        user_, [](bool isOK) { complianceCallback(isOK) });
    return CONTINUE;
}
void CreateUser::complianceCallback(bool isOk) {
    isOK_ = isOK;
    resume_();
}
```


## Broadcast Operation

```c++
Result CreateUser::broadcastNewUser() {
    if (isOK_) {
        queueBroadcast(*user_);
    }
    return CONTINUE;
}
```


## Return status for request

```c++
Result CreateUser::endTransaction() { return DONE; }
```


## CreateUser

```c++
class CreateUser {
    Result CreateUser::findUser();
    Result CreateUser::okToCreate();
    Result CreateUser::broadcastNewUser();
    Result CreateUser::endTransaction();

    void CreateUser::userCallback(std::unique_ptr<User> user);
    void CreateUser::complianceCallback(bool isOk);
// ....
    }
};
```


## Natural non-async code is the inverse coroutine transform

If this were all syncronous it would just be a sequence of calls.


## while not done

Externally this is driven checking if the object said it was done, and if not, scheduling the next operation.


# Async Callbacks and Threads


## C Callback signature

```c++
void (*callback)(void* context, void* response, void* error);
void install(callback cb, void* context);

```

Typical generic C-ish call back interface.

-   You give the framework the context to give back to you,
-   it gives you the response you were waiting for
-   alternatively or additionally it tells you about any errors


## C++  Callback

Often a type-erased callable, like std::function<>.

Binding `this` and other parameters.


## Context: this pointer or coroutine frame &#x2013; Yes

For a C++ mechanism, the `this` pointer is often the context, and you cast back to the known type.


## Whose thread is this anyway - rescheduling

Callback driven frameworks never call you on the right thread.

Often doing complex work could stall the network, or worse deadlock on other callbacks.


## Converting a callback to an Awaitable

```c++
void api_with_callback(std::string p, std::function<void(int result)> callback);

auto api_with_callback_awaitable(const std::string& parameter) {
    struct awaiter : {
        std::string parameter_;
        int         result_;

        awaiter(const std::string& parameter) : parameter_(parameter) {}

        bool await_ready() { return false; } // suspend always

        void await_suspend(std::coroutine_handle<> handle) {
            api_with_callback(parameter_, [this, handle](int result) {
                result_ = result;
                handle.resume();
            });
        }
        int await_resume() { return result_; }
    };
    return awaiter(parameter);
}
```


## Rescheduling on the threadpool

Not really magic. Provide an awaitable that resumes the handle inside the threadpool.

```c++
// FOR EXPOSITION ONLY
void thread_pool::await_suspend(coroutine_handle<> handle) {
    schedule(job([]() { handle.resume() }));
}
```


# Coroutines are NOT async


## Coroutines are deterministic

Transfer of control from the coroutine is deterministic.

It is either outward to the owner or to a particular coroutine.

Resumption of a coroutine is direct.


## Suspension is not async

Nothing happens to a suspended coroutine.

There are no threads.


## Transfer of control is sync

Suspension hands control off on the same thread.

Resumption happens on the same thread as the resumer.


## Async is external to the coroutine

Async can be built with coroutines, but it's external to the coroutine mechanism itself.

Sync can be built from async. The other way around is far more difficult.


# Code: This looks like what you expect

```c++
task<Expected<unique_ptr<User>, bool>>
createUser(std::string id) {
    unique_ptr<User> user = co_await db::getUser(id);
    co_await threadpool_;
    bool isOK = co_await compliance::checkOK(user);
    co_await threadpool_;
    if (isOK) {
        queueBroadcast(*user);
    }
    co_return {std::move(user), isOK};
}
```

-   Logic is clearer
-   Writing new async state machines easier


# Thank You
