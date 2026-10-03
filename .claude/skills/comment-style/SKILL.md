---
name: comment-style
description: How to comment code in this repository. Use when writing or editing any comment in Source/, including doc comments on reconstructed classes and notes next to hardcoded addresses. Covers what earns a comment (non-obvious engine behaviour, why an address or layout is what it is, bugs in the original) and what does not (narrating refactors, migration phases, or the task that produced the change).
---

# Comments in GF2ASI

A comment earns its place by telling a reader something about **the system** that
the code cannot tell them itself. Nothing else does.

## Write comments about

- **Non-obvious engine behaviour.** Why a flag has to be set before a call, why
  an order matters, what a magic value means, which thread something runs on.
  `// Runs on the PRESENTATION thread - the SIM thread may be rebuilding the
  context.`
- **Where a fact came from, when it is not derivable.** A hardcoded address, a
  struct offset, a hash constant. Say what it is and how it was established:
  `// Displ_SetGamma: p[0] is the exponent the ramp is raised to.`
- **Bugs and oddities in the original game.** These are valuable and easy to
  lose. `// The original clamps after the divide, so a zero radius gives inf
  rather than the intended 0.`
- **Why reconstructed code deviates from the original**, when it does. A
  signature that had to change, a type that could not be expressed.
- **Interfaces.** What a function expects, what it guarantees, what nullptr or a
  zero return means.

## Do not write comments about

- **The work that produced the change.** No migration phases, no "moved out
  of X", no "split from Y", no "used to live in Z", no reference to a plan, a
  step number, or a conversation. The reader does not have that context and does
  not need it. `git log` and `git blame` already hold it.
- **What the code plainly says.** `// increment the counter` above `++Count`.
- **Justifying a design to whoever asked for it.** If a design needs defending,
  the commit message is the place, not the source.
- **Apologies, uncertainty theatre, or TODOs without content.** `// TODO: I
  don't even know if this works` says nothing actionable; either say what is
  unverified and how to verify it, or leave the code to speak.

## The test

Read the comment as someone opening the file in two years with no knowledge of
how it got there. If it only makes sense to someone who watched it being
written, delete it.

## Examples

Bad - narrates the refactor:

```cpp
// Moved out of simmanager.cpp, which held nothing else: the detour calls
// Mod::DispatchPlatformAgnosticUnlockEvent, so the whole file was
// modding-layer code living under Source/SDK.
void Mod::SDKHooks::ApplyScoreKeeperHooks()
```

Good - says what the thing does:

```cpp
// Routes unlock events to the Scripthook so achievements fire on PC, where the
// platform-specific path is a stub.
void Mod::SDKHooks::ApplyScoreKeeperHooks()
```

Bad - explains a decision to the person who asked about it:

```cpp
// Not an RWS message, because SendMsg is fire-and-forget to N handlers with no
// return value, so substitution would need an in/out payload and two mods would
// race.
```

Good - explains the contract:

```cpp
// Returns nullptr when no resolver is installed or when it declines the packet,
// in which case the original is kept. Called once per packet during sim-group
// load, after pointer recovery and before the group is committed to a list.
```

Bad - restates the code:

```cpp
// Zero-initialised, so every call is a no-op until the sinks are installed.
Sinks g_Sinks;
```

Good - says the thing that is not obvious:

```cpp
// Installed once during start-up and never changed, so unsynchronised reads
// from the sim and presentation threads are safe.
Sinks g_Sinks;
```
