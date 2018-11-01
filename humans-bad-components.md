<div class="ABSTRACT">
Abstract: Humans are necessary for all useful systems, but nonetheless make terrible system components. It is important in designing, constructing, and maintaining systems to keep in mind that the System will attempt to use people, and that this may not be desireable. Understanding that Systems are chaotic, emergent, and fractal, is key for building Systems that humans can live with and within.

</div>

<div class="ABSTRACT">
Audience Takeaways: The audience will have a better understanding of how Systems tend to grow and behave outside of our control or design, and with greater awareness, be in a position to counter-act those tendencies when possible and necessary.

</div>


# Table of Contents

1.  [We make systems for things we are not good at](#org6666826)
2.  [The System has its own goals](#org703b1c1)
3.  [Parkinson's first law: Systems grow](#org88eb747)
4.  [Gall's observation: Systems Fail, always and constantly](#org15629d1)
5.  [System Design: Systems run better when designed to run downhill](#org34ec00b)
6.  [Efficient Systems are Dangerous, Loose systems last longer and work better](#orgdccde85)
7.  [Programming is a Human activity. Account for the Programmers](#orgabb9f26)

Humans are necessary for all useful systems, but nonetheless make terrible system components. It is important in designing, constructing, and maintaining systems to keep in mind that the System will attempt to use people, and that this may not be desireable. Understanding that Systems are chaotic, emergent, and fractal, is key for building Systems that humans can live with and within.


<a id="org6666826"></a>

# We make systems for things we are not good at

We build systems to compensate for things human being don't do well enough; either not fast enough or not reliably enough. We rarely create systems for things we naturally do well. We don't generally create systems to help us breath. We frequently create systems to help us organize ourselves.


<a id="org703b1c1"></a>

# The System has its own goals

Systems will exhibit behaviors and goals other than the ones they were designed for. A primary goal is usually the systems continued existence and expansion, because systems without that are replaced or consumed by systems that do have that as a goal. Budget systems, nominally put in place to make sure company resources are spent in line with company goals, often end up not doing that at all, and instead reflect status.


<a id="org88eb747"></a>

# Parkinson's first law: Systems grow

C.Northcote Parkinson observed that the staffing of the British Navy continued to grow after WWII even though the actual number of ships declined. Although he attributed much of it to human nature in a bureaucracy, it has later been observer to be true for almost all systems. Existing systems will be given wider scopes, additions functions, and generally be added to, or they will be consumed and replaced by other systems.


<a id="org15629d1"></a>

# Gall's observation: Systems Fail, always and constantly

Complex systems are usually in a failure mode. Working systems will compensate for that. When something fails visibly, it is often the result of 5 or 6 overlapping failures. What is not often recognized is that at any given time 2 or 3 parts of the system have failed, and the overall system has worked around the problem. When investigating a bug, it is not uncommon to find code that quite literally never worked. Fixing that code then causes failures, where the rest of the system was working around the defect. Human users of a system take into account the peculiarities of a system and will avoid problems, sometimes long after the problem is gone, and they will resist changes to their patterns even when they may be an overall improvement.


<a id="org34ec00b"></a>

# System Design: Systems run better when designed to run downhill

Pulling generally works better than pushing. Take into account how human beings behave. If you are trying to effect change with a system, you want the forces your system produces to move people in ways they will continue to move. Pushing up hill often leads to rolling back and being in a worse position that when the system was put in place. Pushing developers to write tests works less well than changing to a test-driven developement system, where programmers are rewarded quickly by succeeding at the game of making tests pass.


<a id="orgdccde85"></a>

# Efficient Systems are Dangerous, Loose systems last longer and work better

Efficient systems, with little slack, will fail much more catestrophically than loose systems do. Calendaring software often efficiently books meetings and rooms with no slack. This results in back-to-back-to-back meetings in widely separated locations leading to enough participants being late that all of the efficiency is lost. The system actually works against itself. Sometimes there are "solutions" proposed involving punishing the participants in various ways, leading to further efficiency problems. Postel's Law, "Be conservative in what you do, be liberal in what you accept from others." (RFC 793) is a maxim about making sure there is loosness in a system. The coupling between components is forgiving of error, and strives to not stress weaker links in the system.


<a id="orgabb9f26"></a>

# Programming is a Human activity. Account for the Programmers

Systems, since they continue to grow, are never complete. A class of humans often ignored in the system are the programmers continuing to fix, maintain, debug, and extend the system. The needs of programmers are even ignored by programmers. We do things to ourselves that we would never accept on behalf of users of our systems.
