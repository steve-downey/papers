<div class="ABSTRACT">
Abstract: Humans are necessary for all useful systems, but nonetheless make terrible System components. It is important to keep in mind when designing, constructing and maintaining systems that the System will attempt to use people in ways that may range from ineffective to unpleasant or hostile, or there may be unplanned and unintended consequences for the people using or interacting with the System. Understanding that Systems are chaotic, emergent and fractal is key for building Systems that humans can both live with and within. As engineers we have an ethical duty to consider how the systems we build will use the humans using our systems.

</div>

System (noun): A set of things working together as parts of a mechanism or an interconnecting network; a complex whole. In particular, a set of things working together as parts of a mechanism or an interconnecting network.

Systems aren't new, but automation and advances in computation have made building increasingly larger systems increasingly feasible. Outsourcing the adminsitration of systems to computers can make systems cheaper, more reliable and more efficient. All of those can occasionally be problems instead of benefits, as we will see.

Two initial points as background for the discussion to follow:

# We make systems for things we are not good at

We build systems to compensate for things humans don't do well enough, fast enough or reliably enough. We rarely create systems for things we naturally do well. We don't generally create systems to help us breath. We frequently create systems to help us organize ourselves. Because the system's design goal is to help humans do things they are not good at, humans often find systems uncomfortable, particularly when they have no choice about adopting or using the system.


# The System has its own goals

Systems will exhibit behaviors and goals other than the ones they were designed for. A usual primary goal is the system's continued existence and expansion, because without that, systems are replaced or consumed by other systems that do have that goal. Budget systems, nominally put in place to make sure company resources are spent in line with company goals, often end up not doing that at all. Instead, they reflect the status of the people or groups involved; capital allocations are given to high prestige groups rather than to groups that could make better use of funding. This is often taken for granted as part of the political system of an organization, even if everyone agrees that the process *should* run as stated. Some will even claim that the system is actually doing what its stated purpose is, when it manifestly is not. This is generally true.


# Parkinson's first law: Systems grow

C. Northcote Parkinson observed <sup><a id="fnr.1" class="footref" href="#fn.1">1</a></sup>that the staffing of the British Navy continued to grow after World War II, even though the actual number of ships declined. While he attributed much of it to human nature in a bureaucracy, it was later observed to be true for almost all systems. Existing systems will be given wider scopes, additional functions, and generally be added to, or they will be consumed and replaced by other systems that have those system goals. James Werner Zawinksi (aka jwz) coined the *Law of Software Envelopment*: "Every program attempts to expand until it can read mail. Those programs which cannot so expand are replaced by ones which can." <sup><a id="fnr.2" class="footref" href="#fn.2">2</a></sup>


# Gall's observation: Systems Fail, always and constantly

Complex systems are usually in a failure mode. Working systems will compensate for that. When something fails visibly, it is often the result of 5 or 6 overlapping failures. What is not often recognized is that, at any given time, 2 or 3 parts of the system have failed, and the overall system has worked around the problem. When investigating a bug, it is not uncommon to find code that quite literally never worked. Fixing that code then causes failures, where the rest of the system was working around the defect. Human users of a system take into account the peculiarities of a system and will avoid problems, sometimes long after the problem is gone, and they will resist changes to their patterns even when they may be an overall improvement. <sup><a id="fnr.3" class="footref" href="#fn.3">3</a></sup>


# System Design: Systems run better when designed to run downhill

Pulling generally works better than pushing. Take into account how humans behave. If you are trying to effect change with a system, you want the forces your system produces to move people in ways they will continue to move. Pushing uphill often leads to rolling back and being in a worse position that when the system was put in place. Pushing developers to write tests doesn't work as well as changing to a test-driven developement system, where programmers are rewarded quickly by succeeding at the game of making tests pass. However, it's often some work to move from one locally stable point - a local minima - to a new, and better, stable point. You have to get over a small hill before getting to the downhill part.


# Efficient Systems are Dangerous, Loose systems last longer and work better

Efficient systems, with little slack, will fail much more catestrophically than loose systems do. Calendaring software often efficiently books meetings and rooms with no slack. This results in back-to-back-to-back meetings in far apart locations. As a result, enough participants are late that all of the efficiency is lost. The system actually works against itself. Sometimes there are "solutions" proposed involving punishing the participants in various ways, leading to further efficiency problems. Postel's Law, "Be conservative in what you do, be liberal in what you accept from others" (RFC 793)<sup><a id="fnr.4" class="footref" href="#fn.4">4</a></sup> is a maxim about making sure there is looseness in a system. The coupling between components is forgiving of error and strives to not stress weaker links in the system.


# Programming is a Human activity. Account for the Programmers

Since systems always continue growing, they are never complete. A class of humans often ignored in the system are the programmers who continue to fix, maintain, debug, and extend the system. The needs of programmers are even ignored by other programmers. We do things to ourselves that we would never accept on behalf of users of our systems. System deployment instructions will be on a web page with 16 steps, 3 of which no longer apply, and one of which may be destructive, but insiders will nonetheless say the process is simple. Being somewhat proud of the number of people affected by an outage, or the frequency of overnight problems, and using that as a proxy for the importance of the sysytem, and neglecting to actually fix the underlying issues causing the outages. Development builds that require more than one- or two-word commands. Tests that are never run, don't work, and may never have worked beyond the first developer's desk. Extreme difficulty in bringing up a full system in an observable and debuggable state so that production problems can be reproduced. Code in the system that no one understands anymore and lives in fear of disturbing.

The entire development process is a system that lives within the larger system being provided. It mush be recognized that problems in the development system are problems for the system as a whole, and it is not selfish for programmers to insist that their system is important.

<div class="ABSTRACT">
Audience Takeaways: The audience will have a better understanding of how Systems tend to grow and behave outside of our control or design. With greater awareness, they will now be in a position to counteract those tendencies whenever possible and necessary. Don't take the System for granted and be prepared to work to change it. Try to avoid what Dan Luu called the "normalization of deviance"<sup><a id="fnr.5" class="footref" href="#fn.5">5</a></sup>, accepting signals that there is a problem with the system as normal, even, or especially, if you know why the deviation from normal was done. Maintain awareness of the people you are affecting with your systems.

</div>

## Footnotes

<sup><a id="fn.1" class="footnum" href="#fnr.1">1</a></sup> Parkinson, Cyril Northcote (19 November 1955), ["Parkinson's Law"](http://www.economist.com/businessfinance/management/displaystory.cfm?story_id=14116121), The Economist.

<sup><a id="fn.2" class="footnum" href="#fnr.2">2</a></sup> [Law of Software Envelopment](https://www.jwz.org/hacks)

<sup><a id="fn.3" class="footnum" href="#fnr.3">3</a></sup> Gall, John. The Systems Bible: The Beginner's Guide to Systems Large and Small (Third Edition of SYSTEMANTICS), General Systemantics Press/Liberty, 2003. ISBN 0-9618251-7-0.

<sup><a id="fn.4" class="footnum" href="#fnr.4">4</a></sup> [TRANSMISSION CONTROL PROTOCOL](https://tools.ietf.org/html/rfc793)

<sup><a id="fn.5" class="footnum" href="#fnr.5">5</a></sup> [Dan Luu, Normalization of Deviance](https://danluu.com/wat/)
