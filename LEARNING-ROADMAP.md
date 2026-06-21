# Data Structures From Scratch in C — A Test-First ROADMAP

This roadmap builds **every data structure in *Open Data Structures* (Pat Morin) from
scratch in C**, with **no dependencies beyond the C standard library and a tiny test
harness you write yourself**. The book ([`ods-python.pdf`](ods-python.pdf)) presents each
structure in pseudocode and proves its running time; your job is to turn each one into
working, leak-free C — and to *prove* your implementation is correct against the same
properties the book proves.

The point is not just to "get the structure compiling." It is to **stop treating
`std::map`, `qsort`, and a language's built-in containers as black boxes you don't
understand**, and instead re-derive the machinery yourself — array growth, pointer
surgery, hashing, tree rotations, heap order, the `O(n log n)` sorting barrier — proving
each piece against something you *can* check: the book's interface contracts, its
complexity bounds, and a brute-force reference implementation you trust because it's too
simple to be wrong.

It is written as a **test-driven (TDD) curriculum**. For every step you:

1. **Red** — write the test the step specifies, against a function/struct that does not
   exist yet, and build it so you watch it **fail for the right reason** (won't compile /
   wrong answer / leaks).
2. **Green** — write the smallest implementation that makes that test pass.
3. **(Refactor)** — clean it up while the test stays green.

You advance only when **all** of these are true: the test is *faithful* (it genuinely
exercises the behaviour — not a tautology you bent to make pass), it is **green**, **and
Valgrind reports zero leaks and zero invalid accesses**. In C, a green test over a leaking
or out-of-bounds implementation is not a pass — it is a bug you haven't noticed yet.

---

## How we test: property oracles, anchored by the book

Data structures are a friendly domain for TDD because almost every correctness claim is
either a tiny hand-traceable case, a stated interface contract, or an invariant the book
proves. Every test below uses one of these oracle types.

| Oracle | What it proves | Example |
|---|---|---|
| **Closed-form / hand-traced** | The result is right on a tiny case you traced by hand | `push 1,2,3; pop ⇒ 3`; `get(2)` on `[a,b,c,d] ⇒ c`; `add(1,'x')` on `[a,b,c] ⇒ [a,x,b,c]` |
| **Invariant / property** | A law the structure must *always* satisfy | `size` grows by 1 on `add`; in-order BST traversal is sorted; heap: `parent ≤ child`; `SSet.find(x)` = smallest `y ≥ x`; black-height equal on all root-to-leaf paths |
| **Model / differential** | Matches a trusted brute-force reference under random ops | run N random `add/remove/find` against **both** your USet and a dead-simple array-backed set; assert identical answers at every step |
| **Amortized / complexity** | The running-time bound actually holds | n `ArrayStack` appends do `O(n)` *total* element copies (count them); `find` in a skiplist of `2^k` items touches `O(k)` nodes |
| **Memory safety (Valgrind)** | No leaks / no invalid reads or writes — *the* C-specific oracle | `valgrind --leak-check=full --error-exitcode=1 ./build/test_x` reports `0 errors`, `all heap blocks were freed` |

**Why the differential (model) oracle is your workhorse.** For anything with a rich
interface (List, USet, SSet, heaps), the cheapest way to be *sure* is to write a
**brute-force reference** that is obviously correct (e.g. a USet backed by a plain
unsorted array with linear scan) and then hammer both it and your fast structure with the
**same random operation sequence**, asserting they agree after every operation. A single
1000-operation differential test finds bugs that fifty hand-written cases miss. You write
the reference once (Step 0.4) and reuse it for every USet/SSet/List in the book.

---

## The faithfulness check (read this twice)

A test that asserts nothing, asserts something always-true, or that you edited to stop
failing, is **not** a pass — it is a silent hole. Before you trust a green bar, **sabotage
your own implementation** (flip a `<` to `<=`, drop a `+1`, forget a `free`, skip a pointer
update) and confirm the test goes red. Every step below names the specific sabotage to try.
In C there is a second faithfulness check: **does Valgrind actually bite?** Deliberately
leak one node and confirm `--leak-check=full` reports it. A leak checker you never saw fail
is one you can't trust.

---

## How to read each step

Every step has the same shape. Read it top to bottom: the prose orients you first, the
contract then pins down the exact behaviour, and finally the test / implementation / gate
turn it green. The single most important part is **"What we're building"** — read it before
the contract, because the contract only makes sense once you know what the structure is
*for*.

> **📖 Read first** — the exact book section(s) and page range to read *before* writing any
> code for this step. Do not skip it: the book's prose, figures, and pseudocode are the
> source of truth your test asserts against. The complete step-by-step list is in
> [§Prerequisite reading](#prerequisite-reading-read-the-book-before-each-step) just below;
> each phase header also repeats its chapter up front.
>
> **Goal** — the one-sentence outcome.
>
> **What we're building (read this first)** — a plain-English orientation: what the
> structure *is*, the job it does, and a walk through each moving part in sentences —
> *before* any formal contract. This is the part that makes the step understandable.
>
> **Interface & invariants** — the exact operations, their `O(·)` targets, and the
> properties your test must assert, so you pin down the *true* behaviour rather than a guess.
>
> **Red — write the test** — the test file, the interface it pins down, the oracle type,
> the assertions, and a **worked example** (tiny concrete inputs and the value you expect
> back). End of Red = a test that **fails for the right reason** (usually the function
> doesn't exist, so it won't compile).
>
> **Green — make it pass** — the implementation to write, plus the one subtlety most likely
> to bite (often a memory-ownership or off-by-one issue) and the **§Wiring** recipe.
>
> **Why it matters** — the transferable engineering / CS skill.
>
> **Gate** — the command that must pass (build + the test + **Valgrind clean**), plus the
> **faithfulness sabotage**: a change you make to your own code to confirm the test goes red.
>
> **Commit** — a [Conventional Commits](https://www.conventionalcommits.org/) message. One
> green gate = one commit.

> **🧰 New to writing this in C?** A companion guide, [`doc/c-toolkit.md`](doc/c-toolkit.md),
> is the *language* half of this roadmap. It covers (§0) the C++→C translation table — useful
> since your `Lecture_*` code is C++; (§1) the five idioms every structure reuses (opaque
> structs, `_new`/`_free`, `bool`+out-param for "not found", `compare`, the function-pointer
> "vtable"); (§2) the new C techniques each phase introduces; and (§3) the **signature shapes**
> (`typedef` + prototypes) for every structure. Each step below carries a short **🔧 C toolkit**
> note + **Signatures (shapes)** block that links into it. The shapes give you the *interface*
> to write the Red test against; the struct fields and bodies stay yours to derive.

**Conventions for this codebase (fix them once, use everywhere):**

- **C11**, compiled with `-std=c11 -Wall -Wextra -Werror -g`. Warnings are errors.
- **Naming:** struct types `snake_case_t` (e.g. `array_stack_t`); functions prefixed by
  their module: `array_stack_new`, `array_stack_add`, `array_stack_get`,
  `array_stack_free`. Booleans return `bool` (`<stdbool.h>`).
- **One element type, generalized later.** Define `typedef int T;` (the book's generic `T`)
  in [`include/ods.h`](include/ods.h). Every container stores `T`. Sentinel "not found" for
  `T` is a returned `bool found` out-parameter, *never* a magic value. (Phase optional-G
  generalizes `T` to `void*`.) This keeps early steps about the *structure*, not about
  generics plumbing.
- **Comparisons go through one function:** `int compare(T a, T b)` returning `<0 / 0 / >0`
  (the book's `compare(x,y)`). Every SSet and sort calls it; never inline `a < b`.
- **Memory ownership is explicit and documented.** Every `*_new()` has a matching
  `*_free()` that releases *everything the structure owns*. A function that takes ownership
  of a pointer says so in its header comment; a function that borrows says so too. The
  structure owns its backing arrays/nodes; it does **not** own the `T` values (they're
  by-value `int`s for now).
- **No globbing, manual wiring.** Every new `.c` and every new test is registered in CMake
  by hand (see §Wiring). Forgetting it is the `undefined reference` / "my test never ran"
  class of error.
- **Indexing convention:** lists are `0`-indexed, `x_0 … x_{n−1}`, exactly as the book's
  `List` interface (§1.2.2).
- **Tolerances:** there are none — data structures are *exact*. Equality is `==`. The only
  "fuzzy" assertions are complexity counts (within a constant factor) and randomized
  *expected* depths (assert an upper bound that holds with overwhelming probability, e.g.
  skiplist height `≤ 3·log₂ n` for `n ≥ 16`).

---

## Prerequisite reading (read the book before each step)

**The rule: read the listed book section(s) *before* you write the test for a step.** This
roadmap tells you *what* to build and *how* to test it; the book ([`ods-python.pdf`](ods-python.pdf))
explains *why it works* and gives the pseudocode and figures your code mirrors. Reading first
turns each step from "decode the roadmap" into "translate pseudocode I already understand into
C." Page numbers are the **book's own page numbers** (the ones printed at the bottom of each
page), which run a few pages behind the PDF viewer's page count because of the front matter.

**Read these two things once, up front, before Phase 0:** §1.1 *The Need for Efficiency*
(pp. 2–3) so you know *why* any of this matters, and §1.5 *Correctness, Time Complexity, and
Space Complexity* (pp. 19–21) so the `O(·)` targets in every step make sense.

**Two "just-in-time" math refreshers** — read each only when its phase arrives, not all now:
- Before **Phase 1** (amortized array growth): §1.3.3 *Asymptotic Notation* (pp. 12–14).
- Before **Phase 3** (skiplists — your first randomized structure): §1.3.1 *Exponentials and
  Logarithms* (pp. 10–11) and §1.3.4 *Randomization and Probability* (pp. 15–17). Re-use this
  same reading before Phases 6 (treaps), 9 (MeldableHeap), and 10 (randomized quicksort).
- Before **Phase 10** (the sorting lower bound): §1.3.2 *Factorials* (pp. 11–12).

| Step | Read first (book section → pages) |
|---|---|
| **0.1** Green pipeline | §1.5 Correctness/Time/Space (19–21); §1.6 Code Samples (21–23) |
| **0.2** Assertion harness | §1.6 Code Samples & conventions (21–23) |
| **0.3** Conventions + interface headers | §1.2 **Interfaces** in full — Queue/Stack/Deque (5–6), List (6–7), USet (8), SSet (8–9); §1.4 Model of Computation (18–19) |
| **0.4** Reference + differential harness | §1.2.3 USet (8) and §1.2.4 SSet (8–9) — esp. the USet-`find` vs SSet-`find` distinction |
| **Phase 1** | Chapter 2 intro (31) + §1.3.3 Asymptotic Notation (12–14) |
| **1.1** ArrayStack | §2.1 incl. 2.1.1 Basics, 2.1.2 Growing/Shrinking, 2.1.3 Summary (32–37) |
| **1.2** FastArrayStack | §2.2 (37–38) |
| **1.3** ArrayQueue | §2.3 incl. Summary (38–42) |
| **1.4** ArrayDeque | §2.4 incl. Summary (42–44) |
| **1.5** DualArrayDeque | §2.5 incl. 2.5.1 Balancing, 2.5.2 Summary (44–50) |
| **1.6** RootishArrayStack | §2.6 incl. Analysis, Space Usage, Summary (50–56) |
| **Phase 2** | Chapter 3 intro (61) |
| **2.1** SLList | §3.1 incl. Queue Operations, Summary (61–64) |
| **2.2** DLList | §3.2 incl. Adding/Removing, Summary (64–68) |
| **2.3** SEList | §3.3 all subsections incl. Amortized Analysis of Spreading/Gathering (68–77) |
| **Phase 3** | §4.1 The Basic Structure (83–85) + §1.3.1 & §1.3.4 (10–11, 15–17) |
| **3.1** SkiplistSSet | §4.2 incl. Summary (85–88) **and** §4.4 Analysis of Skiplists (93–97) |
| **3.2** SkiplistList | §4.3 incl. Summary (89–93) |
| **Phase 4** | Chapter 5 intro (101) |
| **4.1** ChainedHashTable | §5.1 incl. 5.1.1 Multiplicative Hashing, Summary (101–108) |
| **4.2** LinearHashTable | §5.2 incl. Analysis, Summary, 5.2.3 Tabulation Hashing (108–117) |
| **4.3** Hash codes | §5.3 — primitive (117), compound (117–119), arrays/strings (119–122) |
| **Phase 5** | Chapter 6 intro (127) |
| **5.1** BinaryTree | §6.1 incl. 6.1.1 Recursive Algorithms, 6.1.2 Traversing (129–133) |
| **5.2** BinarySearchTree | §6.2 incl. Searching, Addition, Removal, Summary (133–139) |
| **6.1** Random BST | §7.1 incl. Proof of Lemma 7.1, Summary (145–150) |
| **6.2** Treap | §7.2 incl. Summary (151–158) |
| **7.1** ScapegoatTree | §8.1 incl. Analysis of Correctness/Running-Time, Summary (166–172) |
| **8.1** 2-4 trees | §9.1 incl. Adding a Leaf, Removing a Leaf (178–182) |
| **8.2** RedBlackTree | §9.2 incl. 9.2.1–9.2.4 (Left-Leaning, Addition, Removal) + §9.3 Summary (182–198) |
| **9.1** BinaryHeap | §10.1 incl. Summary (203–209) |
| **9.2** MeldableHeap | §10.2 incl. 10.2.1 Analysis of merge, Summary (209–214) |
| **10.1** Comparison sorts | §11.1 — Merge-Sort (218), Quicksort (222), Heap-sort (225) |
| **10.2** Lower bound | §11.1.4 A Lower-Bound for Comparison-Based Sorting (227–231) + §1.3.2 Factorials (11–12) |
| **10.3** Counting/radix sort | §11.2 — Counting Sort (231), Radix-Sort (233–235) |
| **11.1** Graph representations | §12.1 AdjacencyMatrix (241–244), §12.2 AdjacencyLists (244–247) |
| **11.2** BFS / DFS | §12.3 incl. 12.3.1 Breadth-First, 12.3.2 Depth-First (247–252) |
| **12.1** BinaryTrie | §13.1 A digital search tree (258–264) |
| **12.2** XFastTrie | §13.2 Searching in Doubly-Logarithmic Time (264–267) |
| **12.3** YFastTrie | §13.3 A Doubly-Logarithmic Time SSet (267–272) |
| **13.1** BlockStore | §14.1 The Block Store (277) |
| **13.2** B-Tree | §14.2 incl. Searching, Addition, Removal, Amortized Analysis (277–296) |
| **14.1–14.2** Capstone | re-read §1.2 Interfaces (4–9) and §1.5 (19–21) — no new chapters |
| **Phase G** *(optional)* | §1.2.3 USet (8) — the Pair / dictionary paragraph |

> **Tip:** each chapter ends with a *"Discussion and Exercises"* section. It's optional for
> the build, but a few exercises after a phase are the fastest way to confirm you actually
> understood the structure rather than just transcribed it.

---

## The interfaces (the contracts every structure implements)

The book is organized around a handful of abstract interfaces (§1.2). Pin these down once,
in headers, before implementing anything — they are the contracts your tests assert and the
reason many different structures are interchangeable.

| Interface | Operations | Meaning |
|---|---|---|
| **Queue** | `add(x)`, `remove()` | add an element; remove the *next* one (FIFO unless stated) |
| **Stack** | `push(x)`, `pop()` | LIFO: `pop` removes the most recently pushed |
| **Deque** | `add_first(x)`, `remove_first()`, `add_last(x)`, `remove_last()` | a sequence with two ends |
| **List** | `size()`, `get(i)`, `set(i,x)`, `add(i,x)`, `remove(i)` | indexed sequence `x_0…x_{n−1}`; subsumes Stack/Queue/Deque |
| **USet** | `size()`, `add(x)`, `remove(x)`, `find(x)` | unordered set of unique elements; `find(x)` returns the element *equal* to `x`, or "not found" |
| **SSet** | `size()`, `add(x)`, `remove(x)`, `find(x)` | sorted set; `find(x)` is **successor search**: returns the smallest `y ≥ x` |

The crucial, often-missed distinction (§1.2.4): a **USet**'s `find(x)` only finds an *equal*
element; an **SSet**'s `find(x)` returns the *smallest element ≥ x* even when `x` itself
isn't present. This single difference is why SSets cost `O(log n)` and USets can be `O(1)`.

---

## Wiring new code into the build (you will do this at almost every step)

CMake here does **not** auto-discover files. Each library lists its sources explicitly, and
each test is registered one by one. The two failure modes this prevents are *"my new test
never runs"* and *"undefined reference to `array_stack_new`"*.

**Top-level testing must stay on.** `enable_testing()` lives in the root
[`CMakeLists.txt`](CMakeLists.txt); without it `ctest` prints *"No tests were found"* even
though the test binaries built fine.

**A — Register a new test executable.** Each test is a standalone C program returning `0` on
success, nonzero on failure. In the relevant `tests/<module>/CMakeLists.txt`:
```cmake
add_executable(array_stack_test array_stack_test.c)
target_link_libraries(array_stack_test ODS_LISTS ODS_TESTUTIL)
add_test(NAME array_stack_test COMMAND array_stack_test)
```

**B — Add a source file to a library** (e.g. `array_stack.c` to the `lists` lib): append the
`.c` to that library's source list in `src/<module>/CMakeLists.txt`. A file not listed there
simply isn't compiled in → `undefined reference` at link time.

**C — Stand up a whole new module library** (done once per phase):
1. Create `src/<module>/CMakeLists.txt` defining a `STATIC` library
   (`add_library(ODS_LISTS STATIC array_stack.c …)`), with
   `target_include_directories(ODS_LISTS PUBLIC ${CMAKE_SOURCE_DIR}/include)`.
2. Add `add_subdirectory(<module>)` to [`src/CMakeLists.txt`](src/CMakeLists.txt).
3. Create `tests/<module>/CMakeLists.txt` (one `add_executable`/`add_test` per test, per
   **A**) and add `add_subdirectory(<module>)` to [`tests/CMakeLists.txt`](tests/CMakeLists.txt).

**D — Run everything under Valgrind in CI/locally.** Register a second test that wraps the
binary in Valgrind so leaks fail the suite:
```cmake
add_test(NAME array_stack_test_memcheck
         COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:array_stack_test>)
```
(Or run `ctest -T memcheck` if you configure CTest's MemCheck. Either way, **a leak must be
able to turn the bar red.**)

**After any wiring change**, re-run from the project root and confirm your new test name
actually appears in the listing — *don't* trust a `100% passed` that only ran the old tests:
```
cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

---

## Phase 0 — Foundations: the harness that makes Red/Green possible

> **📖 Read first (whole phase):** **Chapter 1**, focusing on §1.2 *Interfaces* (pp. 4–9),
> §1.4 *The Model of Computation* (pp. 18–19), §1.5 *Correctness, Time & Space* (pp. 19–21),
> and §1.6 *Code Samples* (pp. 21–23). Skim §1.3 *Mathematical Background* now; you'll
> return to its subsections just-in-time in later phases. (Per-step pages in §Prerequisite reading.)

This phase writes *no data structure*. It makes the build/test/Valgrind loop trustworthy
and records the conventions everything else assumes.

### Step 0.1 — Green pipeline + one-command test run

**Goal:** `cmake --build` succeeds and `ctest` runs a trivial smoke test green, so every
later Red/Green is measured against a known-good baseline.

**What we're building (read this first).** Before any structure, you need one command that
builds everything and runs every test, coming back **green**. TDD only works if "all green"
is a state you can trust and return to. Write a one-line `smoke_test.c` (`assert(1+1==2)`),
wire it, and watch `ctest` find and pass it.

**Interface & invariants:** none — pure plumbing.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 0](doc/c-toolkit.md)): `main` returns `int`,
> `0` = success; CTest reads that exit code. No allocation, no headers beyond the test's own.

**Red — write the test:** `tests/support/smoke_test.c` with `int main(void){ return 0; }`.
Build and run `ctest`. **The worked example of "red":** if `ctest` prints
`No tests were found!!!`, that's your red — add `enable_testing()` to the **root**
`CMakeLists.txt`.

**Green — make it pass:** add `enable_testing()` and the subdirectory wiring (§Wiring C),
confirm `ctest` lists and passes `smoke_test`.

**Why it matters:** A runner that finds zero tests but exits `0` is worse than a failing
one — it lies. Prove the harness is honest before trusting it.

**Gate:** `ctest` names and passes `smoke_test`. **Faithfulness:** make `smoke_test.c`
`return 1;` → `ctest` goes red → revert.

**Commit:** `chore(build): green ctest pipeline with smoke test`

### Step 0.2 — The assertion harness (built test-first)

**Goal:** A tiny header of test macros every later test calls, so failures report *where*
and *what*, and a failing test returns nonzero to `ctest`.

**What we're building (read this first).** You're in pure C, so there's no GoogleTest. You
need three things from a test: a check that prints file/line and the offending values on
failure, a way to run several checks and report a count, and a nonzero exit code so `ctest`
notices. Write a ~40-line `test_util.h`: `CHECK(cond)`, `CHECK_INT_EQ(a,b)`,
`CHECK_TRUE/CHECK_FALSE`, and a `TEST_MAIN`-style runner that tracks failures and returns
their count. **Meta-test it**: a comparison tool you don't trust makes every later green a
lie.

**Interface & invariants:** `CHECK_INT_EQ(a,b)` passes iff `a==b` and on failure prints
`expr`, both values, and `__FILE__:__LINE__`; the process exit code equals the number of
failed checks (so `0` = all passed).

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 0](doc/c-toolkit.md)): macros with `__FILE__`/
> `__LINE__` and the stringizing operator `#x` to print *what* failed and *where*; wrap
> multi-statement macros in `do { … } while (0)`; track a `fails` counter and return it from
> `main`. Needs `<stdio.h>` (`fprintf`/`stderr`).
>
> **Signatures (shapes):**
> ```c
> /* include/test_util.h — header-only */
> #define CHECK(cond)          /* prints file:line + #cond on failure, fails++ */
> #define CHECK_INT_EQ(a, b)   /* prints both values + exprs on mismatch */
> #define CHECK_TRUE(x)
> #define CHECK_FALSE(x)
> /* + a runner that exposes `fails` and returns it from main */
> ```

**Red — write the test:** `tests/support/test_util_test.c`: a self-test that runs one
passing and one *expected-failing* check through a captured counter and asserts the counter
saw exactly one failure. Build → red (`test_util.h` doesn't exist).

**Green — make it pass:** write `include/test_util.h` (header-only macros). **Wire** per
§Wiring A as `ODS_TESTUTIL` (an `INTERFACE` library exposing the header is cleanest).

**Why it matters:** Your oracles are only as trustworthy as the tool that applies them;
meta-testing the harness first means every later "green" means something.

**Gate:** the harness self-test passes. **Faithfulness:** make `CHECK_INT_EQ` always pass →
the self-test's failure half goes red.

**Commit:** `test(support): add C assertion harness (meta-tested)`

### Step 0.3 — Conventions record + the interface headers

**Goal:** Write the conventions down once, and declare the abstract interfaces as C headers,
so Phases 1–13 are "fill in the functions," not "re-decide the API mid-stream."

**What we're building (read this first).** This step produces *documents and headers*, not
algorithms. A short `doc/conventions.md` fixes: `typedef int T;`, the `compare` contract,
the naming scheme, and the memory-ownership rule (every `_new` has a `_free`; the structure
owns its nodes/arrays, not the `T` values). Then declare the interface "shapes" you'll
implement repeatedly — what a USet's four functions look like in C, what an SSet's `find`
returns (`bool` found + out-param) — so every implementation matches one signature and your
differential harness (next step) can drive any of them.

**Interface & invariants:** record the six interfaces from "The interfaces" table above as C
function-signature templates in `include/ods.h`. Pin the "not found" convention:
`bool uset_find(USet*, T x, T *out)` returns `false` and leaves `*out` untouched if absent.

> **🔧 C toolkit** (see [c-toolkit.md §1](doc/c-toolkit.md)): the opaque-struct pattern
> (`typedef struct foo foo_t;` in the header, fields in the `.c`); `assert` for precondition
> bugs vs. `bool`+out-param for "not found"; the `compare` contract; `#pragma once`; the
> headers `<stddef.h> <stdbool.h> <stdint.h> <assert.h>`.
>
> **Signatures (shapes)** — the interface templates to record in `include/ods.h`:
> ```c
> typedef int T;
> int  compare(T a, T b);                               /* <0 / 0 / >0  (book §1.2.4) */
> /* USet: find returns the equal element; SSet: find returns smallest y >= x */
> bool uset_find(const void *self, T x, T *out);        /* false ⇒ absent, *out untouched */
> bool sset_find(const void *self, T x, T *out);        /* successor search */
> ```

**Checkpoint (not a unit test):** `doc/conventions.md` exists; `include/ods.h` declares `T`,
`compare`, and the interface signature templates, and compiles when included.

**Why it matters:** This is real API design. One good decision here turns the rest of the
book into a sequence of small, unambiguous wins.

**Gate:** headers compile; conventions doc is concrete enough to paste signatures from.

**Commit:** `docs: record conventions + declare ODS interfaces`

### Step 0.4 — The brute-force reference + differential harness

**Goal:** A dead-simple, obviously-correct USet/SSet/List reference, plus a randomized
op-sequence runner — the oracle you'll point at every fast structure in the book.

**What we're building (read this first).** This is the single most valuable test asset in
the whole project. You write the *slow but obviously correct* versions: a USet that is a
plain unsorted array with linear-scan `find`/`remove`; an SSet that is a sorted array with
binary-search successor `find`; a List that is a plain growable array. Each is too simple to
be wrong. Then you write a **differential driver**: given two implementations of the same
interface and a seed, it performs the *same* random sequence of operations on both and
asserts they return identical results after *every* operation, printing the operation that
first diverged. From now on, testing a new USet is one function call against this harness.

**Interface & invariants:** the reference must satisfy the interface contracts exactly
(USet: unique elements, `find` returns equal element; SSet: `find` is successor search;
List: indexed, `add(i,x)` shifts right). The driver asserts *operation-by-operation*
equality of return values and of `size()`.

> **🔧 C toolkit** (see [c-toolkit.md §1.5 + §3 Phase 0](doc/c-toolkit.md)): the
> function-pointer "vtable" — a `struct` of function pointers per interface, one instance per
> implementation, with `self` typed `void *` and cast inside each thunk. This is how the driver
> drives any implementation generically (C's stand-in for C++ virtual dispatch).
>
> **Signatures (shapes):**
> ```c
> typedef struct { void *(*create)(void); void (*destroy)(void *);
>                  size_t (*size)(void *); /* …one fn ptr per List/USet/SSet op… */ } list_vtable_t;
> /* references: ref_uset_*, ref_sset_* (successor find), ref_list_* — see c-toolkit §3 */
> void diff_run_list(const list_vtable_t *a, const list_vtable_t *b, unsigned seed, int ops);
> ```

**Red — write the test:** `tests/support/reference_test.c`: hand-traced checks that the
reference USet rejects duplicate `add`, that the reference SSet's `find(x)` returns the
smallest `y ≥ x` on a worked example (`{2,4,6}`, `find(3)==4`, `find(7)==not found`), and a
**self-differential**: drive the reference against *itself* with a fixed seed and confirm
zero divergences (proving the driver reports agreement correctly). Build → red.

**Green — make it pass:** write `tests/support/ref_uset.{c,h}`, `ref_sset.{c,h}`,
`ref_list.{c,h}`, and `diff_driver.{c,h}` (a function taking two vtable-like structs of
function pointers + a seed + an op count).

**Why it matters:** A trustworthy brute-force oracle plus a fuzzing driver is how
professionals test data structures (model-based / differential testing). Build it once,
catch bugs forever.

**Gate:** reference hand-traces pass; self-differential reports zero divergence.
**Faithfulness:** make the driver compare only `size()` (not return values) → introduce a
deliberately wrong SSet `find` and confirm the *weakened* driver misses it, then restore the
full comparison and confirm it catches it. This proves the driver actually discriminates.

**Commit:** `test(support): add brute-force references + differential driver`

---

## Phase 1 — Array-based lists (Chapter 2)

> **📖 Read first (whole phase):** **Chapter 2** intro (p. 31), and — if you haven't —
> §1.3.3 *Asymptotic Notation* (pp. 12–14), since this phase is where amortized `O(·)`
> analysis first bites. Then read each section just before its step (see §Prerequisite reading).

Backing arrays + amortized resizing. The easiest wins, and the foundation everything stacks
on. Every structure here implements `List` (or Stack/Queue/Deque); test each one *both* by
hand and through the Phase-0 differential driver against `ref_list`.

### Step 1.1 — `ArrayStack`: fast stack operations on an array (§2.1)

**Goal:** `get`/`set` in `O(1)`, `add(i,x)`/`remove(i)` in `O(n−i)`, with a backing array
that grows and shrinks so the *amortized* cost is `O(1)` per operation.

**What we're building (read this first).** An `ArrayStack` is the List interface backed by a
plain array `a` and a count `n`, where `n ≤ a.length`. `get(i)`/`set(i,x)` are direct array
access. `add(i,x)` makes room by shifting `a[i..n−1]` right one slot then writes `a[i]=x`;
`remove(i)` shifts left. The clever part is **resizing**: when the array is full you allocate
a new one of size `2n`, copy over, and free the old; when it's less than a third full you
shrink to size `2n`. Doubling (not adding a constant) is what makes a long run of `add`s cost
`O(n)` *total*, i.e. `O(1)` amortized — the headline result of the chapter.

**Interface & invariants:** `n ≤ a.length` always; after a resize `a.length == 2n`; `add`/
`remove` at the end are `O(1)` amortized; in the middle `O(n−i)`. `get(i)`/`set(i,x)` are
`O(1)`. Out-of-range `get/set/add/remove` is a checked precondition.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 1](doc/c-toolkit.md)): `malloc`/`realloc`/`free`
> for the backing array; **`memmove`** (not `memcpy`) for the overlapping shift; growth
> `max(1, 2n)`, shrink when `len ≥ 3n` (book §2.1.2); `size_t` for `n`/`len`/`i`; `assert(i<n)`
> for out-of-range. (You wrote this algorithm in C++ as `AmmortizedDynamicArray` — same logic,
> manual memory.)
>
> **Signatures (shapes):**
> ```c
> typedef struct array_stack array_stack_t;   /* fields you choose: e.g. T *a; size_t len, n; */
> array_stack_t *array_stack_new(void);
> void           array_stack_free(array_stack_t *);
> size_t         array_stack_size(const array_stack_t *);
> T              array_stack_get(const array_stack_t *, size_t i);   /* assert i < n */
> void           array_stack_set(array_stack_t *, size_t i, T x);
> void           array_stack_add(array_stack_t *, size_t i, T x);    /* 0 <= i <= n */
> T              array_stack_remove(array_stack_t *, size_t i);
> ```

**Red — write the test:** `tests/lists/array_stack_test.c`:
- `test_get_set_add_remove` *(closed-form)*: **worked example** — start empty; `add(0,'a')`,
  `add(1,'b')`, `add(2,'c')` → `get(0..2)==a,b,c`, `size()==3`; `add(1,'x')` → sequence is
  `[a,x,b,c]`; `set(2,'y')` → `[a,x,y,c]`; `remove(0)` → `[x,y,c]`, `size()==3`.
- `test_amortized_copies` *(complexity)*: instrument the resize to count element copies;
  perform `n = 10000` `add`s at the end and assert total copies `< 4n` (proving doubling,
  not per-op reallocation).
- `test_differential` *(model)*: drive `ArrayStack` against `ref_list` for 5000 random ops.
- Build → red.

**Green — make it pass:** write `src/lists/array_stack.{c}` + header. The subtlety:
`array_stack_free` must free the backing array; `add`/`remove` shift with `memmove` (regions
overlap — `memcpy` is UB here). Wire per §Wiring A/B/C/D.

**Why it matters:** Amortized analysis via doubling is *the* foundational idea behind every
growable array (C++ `vector`, Java `ArrayList`, Python `list`). You just built it.

**Gate:** all three tests pass + Valgrind clean. **Faithfulness:** grow by `n+1` instead of
`2n` → `test_amortized_copies` goes red (copies become `Θ(n²)`). Also: skip the `free` in
`remove`'s resize → Valgrind reports the leak.

**Commit:** `feat(lists): add ArrayStack (amortized O(1) grow/shrink)`

### Step 1.2 — `FastArrayStack`: bulk-copy optimization (§2.2)

**Goal:** The same `ArrayStack` with element shifts done via `memmove` block copies.

**What we're building (read this first).** Functionally identical to `ArrayStack`, but the
shift loops in `add`/`remove`/`resize` become single `memmove`/`memcpy` calls. The book's
point is that the asymptotics don't change but the constant does — a worthwhile, honest
optimization. (If you already used `memmove` in 1.1, this step is a short note proving the
two behave identically.)

**Interface & invariants:** identical contract to `ArrayStack`; bytewise-identical results.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 1](doc/c-toolkit.md)): the book itself (§2.2)
> names `memcpy(d,s,n)` and `memmove(d,s,n)` here. Bulk-copy whole blocks instead of element
> loops; sizes are **bytes** (`count * sizeof(T)`). Same prototypes as `ArrayStack` with a
> `fast_array_stack_` prefix.

**Red — write the test:** add to `array_stack_test.c` a `test_fast_matches_plain`
*(differential)*: drive `FastArrayStack` and `ArrayStack` against each other for 5000 ops;
zero divergence. Build → red.

**Green — make it pass:** implement (or alias) `fast_array_stack` using block copies.

**Why it matters:** Knowing *which* optimizations change the constant vs. the exponent — and
proving the optimized version still matches — is core engineering judgement.

**Gate:** differential passes + Valgrind clean. **Faithfulness:** use `memcpy` on the
overlapping shift region → a sanitizer/Valgrind or the differential flags the corruption.

**Commit:** `feat(lists): add FastArrayStack (memmove shifts)`

### Step 1.3 — `ArrayQueue`: an array-based FIFO queue (§2.3)

**Goal:** FIFO `add`/`remove` in `O(1)` amortized using a circular ("infinite array") index.

**What we're building (read this first).** A naive array queue degrades because removing from
the front shifts everything. The fix is to *not move anything*: keep a head index `j` and a
count `n`, and store element `i` at `a[(j + i) mod a.length]`. `add` writes at the logical
tail and increments `n`; `remove` returns `a[j]`, advances `j` (mod length), decrements `n`.
Resize the same doubling way, re-laying-out elements starting at index `0`. Modular
arithmetic turns the array into a ring.

**Interface & invariants:** element `i` lives at `a[(j+i) mod a.length]`; `0 ≤ n ≤ a.length`;
`add`/`remove` are `O(1)` amortized; FIFO order preserved across a resize.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 1](doc/c-toolkit.md)): modular arithmetic on a
> circular buffer — element `i` at `a[(j+i) % len]`, `add` writes `a[(j+n) % len]`, `remove`
> does `j = (j+1) % len` (book §2.3). Keep a head index `j` and count `n`; on resize, re-lay-out
> in logical order and reset `j = 0`.
>
> **Signatures (shapes):**
> ```c
> typedef struct array_queue array_queue_t;   /* fields: e.g. T *a; size_t len, j, n; */
> array_queue_t *array_queue_new(void);
> void           array_queue_free(array_queue_t *);
> size_t         array_queue_size(const array_queue_t *);
> void           array_queue_add(array_queue_t *, T x);   /* enqueue at tail */
> T              array_queue_remove(array_queue_t *);     /* dequeue head; assert non-empty */
> ```

**Red — write the test:** `tests/lists/array_queue_test.c`:
- `test_fifo_order` *(closed-form)*: `add 1,2,3`; `remove ⇒ 1`; `add 4`; `remove ⇒ 2,3,4`.
- `test_wraparound` *(invariant)*: interleave adds/removes so `j` wraps past `a.length`, then
  drain; order is still FIFO (this is the bug-prone path).
- `test_differential` *(model)*: against a FIFO reference for 5000 ops.
- Build → red.

**Green — make it pass:** `src/lists/array_queue.c`. Subtlety: on resize, copy logical
order (`for i: new[i] = a[(j+i)%len]`) and reset `j=0`.

**Why it matters:** The circular buffer is everywhere — ring buffers in audio, networking,
producer/consumer queues. The `mod` trick is the whole idea.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** drop the `mod` on `add` (write at
`a[j+n]`) → `test_wraparound` goes red (out-of-bounds / wrong order).

**Commit:** `feat(lists): add ArrayQueue (circular buffer FIFO)`

### Step 1.4 — `ArrayDeque`: fast operations at both ends (§2.4)

**Goal:** A List where `add(i,x)`/`remove(i)` cost `O(min(i, n−i))` — fast at *both* ends.

**What we're building (read this first).** Combine the circular indexing of `ArrayQueue`
with two-sided shifting. `get/set` use `a[(j+i) mod len]`. For `add(i,x)` you shift whichever
side is *smaller*: if `i` is in the front half, shift the front elements left (moving `j`);
otherwise shift the back elements right. This makes both `add_first` and `add_last` `O(1)`
amortized, so `ArrayDeque` implements the full Deque interface efficiently.

**Interface & invariants:** `add(i,x)`/`remove(i)` are `O(min(i, n−i))` amortized; the four
Deque ops are `O(1)` amortized; circular-index invariant as in 1.3.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 1](doc/c-toolkit.md)): circular index as in
> `ArrayQueue`, plus "shift the smaller side" — for `add(i,x)`, if `i < n/2` shift the front
> (and move `j` back, mod `len`), else shift the back. Get the front-side `j` update and its
> `mod` right. Full List shape + the four Deque convenience ops.
>
> **Signatures (shapes):**
> ```c
> typedef struct array_deque array_deque_t;   /* List shape: new/free/size/get/set/add/remove */
> void array_deque_add_first(array_deque_t *, T x);
> void array_deque_add_last (array_deque_t *, T x);
> T    array_deque_remove_first(array_deque_t *);
> T    array_deque_remove_last (array_deque_t *);
> ```

**Red — write the test:** `tests/lists/array_deque_test.c`:
- `test_both_ends` *(closed-form)*: `add_first` thrice and `add_last` thrice from empty,
  then read out the full sequence and check order.
- `test_min_side_shift` *(complexity)*: instrument shift count; `add(1, …)` on a large deque
  shifts `≈ 1` element (front side), `add(n−1, …)` shifts `≈ 1` (back side).
- `test_differential` against `ref_list` for 5000 ops including `add(i)`/`remove(i)` at
  random `i`.
- Build → red.

**Green — make it pass:** `src/lists/array_deque.c`. Subtlety: the "shift the smaller side"
branch and its `mod` arithmetic — get the front-side `j` update right.

**Why it matters:** This is the structure a real `deque` is built on; "shift the cheaper
side" is a clean cost-minimization idea.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** always shift the back side → the
front-half differential ops or the shift-count test go red.

**Commit:** `feat(lists): add ArrayDeque (two-sided circular shifts)`

### Step 1.5 — `DualArrayDeque`: a deque from two stacks (§2.5)

**Goal:** Build a Deque out of two `ArrayStack`s (front + back) kept balanced.

**What we're building (read this first).** Instead of one circular array, use two stacks
back-to-back: a `front` stack (storing the first half *reversed*) and a `back` stack (the
second half). Index `i < front.size` lives in `front` at `front.size − 1 − i`; otherwise in
`back` at `i − front.size`. After any `add`/`remove`, if the two stacks get too lopsided
(one more than `3×` the other and both nonempty), **rebalance** by redistributing elements
evenly. This earns the same bounds as `ArrayDeque` by composing the structure you already
built — a lesson in building big structures from small ones.

**Interface & invariants:** `front.size` and `back.size` never differ by more than a `3×`
factor after a balance; `add(i,x)`/`remove(i)` are `O(min(i, n−i))` amortized; index mapping
as above.

> **🔧 C toolkit** (see [c-toolkit.md §1.1 + §2 Phase 1](doc/c-toolkit.md)): **composition** —
> embed two `array_stack_t` (front + back) as fields and call their functions; no new memory
> tricks. The work is the index→(stack, slot) mapping and the `3×` rebalance. `_free` must free
> both inner stacks. Full List shape with a `dual_array_deque_` prefix.

**Red — write the test:** `tests/lists/dual_array_deque_test.c`:
- `test_index_mapping` *(closed-form)*: build `[a,b,c,d,e]`, assert each `get(i)` maps to the
  right stack/slot.
- `test_rebalances` *(invariant)*: after a pathological sequence, assert the `3×` balance
  invariant holds and contents are correct.
- `test_differential` against `ref_list`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/lists/dual_array_deque.c` reusing `ArrayStack`. Subtlety: the
balance condition and the rebuild that splits elements evenly.

**Why it matters:** Composition — a correct, efficient structure assembled from two simpler
ones, with an invariant restored after each op. That pattern recurs in trees and heaps.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** never rebalance → the balance
invariant test fails and amortized cost degrades.

**Commit:** `feat(lists): add DualArrayDeque (two balanced stacks)`

### Step 1.6 — `RootishArrayStack`: space-efficient array stack (§2.6)

**Goal:** A List that wastes only `O(√n)` space (vs. `O(n)` for the doublers).

**What we're building (read this first).** The doubling stacks can waste up to `n` unused
slots. `RootishArrayStack` instead stores elements in a sequence of *blocks*, where block `b`
has size `b+1` (sizes `1, 2, 3, …`). To find element `i`, you compute which block it's in via
a closed-form formula (`b = ⌈(−3 + √(9 + 8i)) / 2⌉`) and its offset within that block. Adding
appends a new block when needed; you only ever have `O(√n)` blocks and at most one empty
block, so wasted space is `O(√n)`. This is a genuinely different space/time trade-off.

**Interface & invariants:** number of blocks is `O(√n)`; total wasted space `O(√n)`;
`get/set` are `O(1)`; `add(i,x)`/`remove(i)` are `O(n−i)`; the index→`(block, offset)`
formula is exact.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 1](doc/c-toolkit.md)): an array of block pointers
> (`T **blocks`) — each block `b` is a `malloc`'d array of size `b+1`; `_free` frees every block
> then the block array. The index formula uses `sqrt` from `<math.h>` (**link `-lm`**); cast
> float→int carefully and check against the hand table for small `i` before trusting it. Full
> List shape, `rootish_array_stack_` prefix.

**Red — write the test:** `tests/lists/rootish_array_stack_test.c`:
- `test_index_formula` *(closed-form)*: for `i = 0,1,2,3,4,5` assert the computed
  `(block, offset)` equals the hand-derived `(0,0),(1,0),(1,1),(2,0),(2,1),(2,2)`.
- `test_space_is_sqrt` *(invariant)*: after `n = 10000` adds, the block count is `O(√n)`
  (assert `≤ 2·√n + 2`).
- `test_differential` against `ref_list`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/lists/rootish_array_stack.c`. Subtlety: integer/float care in
the `√` formula — verify it against the hand table for small `i` before trusting it.

**Why it matters:** A concrete `O(√n)`-space data structure shows that "growable array" isn't
one fixed trade-off; the algebraic index formula is a neat bridge from math to layout.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** off-by-one the block formula (use
`floor` instead of `ceil`, or drop the `−3`) → `test_index_formula` goes red.

**Commit:** `feat(lists): add RootishArrayStack (O(√n) wasted space)`

---

## Phase 2 — Linked lists (Chapter 3)

> **📖 Read first (whole phase):** **Chapter 3** intro (p. 61). Read §3.1/§3.2/§3.3 each
> just before its step (see §Prerequisite reading).

Pointer-based sequences. This is where C bites hardest: every `add`/`remove` is pointer
surgery, and every node must be `free`d exactly once. Valgrind is your closest friend here.

### Step 2.1 — `SLList`: a singly-linked list (§3.1)

**Goal:** A singly-linked list supporting Stack ops (`push`/`pop` at the head) and Queue ops
(`add` at tail, `remove` at head) in `O(1)`.

**What we're building (read this first).** Each node holds a value and a `next` pointer; the
list keeps `head` and `tail` pointers and a count `n`. `push` prepends a node; `pop` removes
the head. `add` (enqueue) appends at `tail`; `remove` (dequeue) removes the head. The whole
point: with head+tail pointers, both a stack and a FIFO queue are `O(1)` and use exactly one
node of memory per element — no wasted array slots.

**Interface & invariants:** `head`/`tail`/`n` stay consistent (empty ⇒ both `NULL`, `n==0`;
`tail->next == NULL`); `push/pop/add/remove` are `O(1)`; popping/dequeuing the last element
resets `tail` to `NULL`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 2](doc/c-toolkit.md)): your first heap nodes —
> `struct node { T x; struct node *next; };` (self-referential via the `struct` tag). Pointer
> surgery for `push`/`pop`/`add`/`remove`; watch the empty-list edge cases (reset `tail = NULL`
> on the last `remove`). `_free` walks node→node, saving `next` *before* `free`-ing each node.
>
> **Signatures (shapes):**
> ```c
> typedef struct sllist sllist_t;   /* fields: node *head, *tail; size_t n; */
> sllist_t *sllist_new(void);
> void      sllist_free(sllist_t *);
> size_t    sllist_size(const sllist_t *);
> void      sllist_push(sllist_t *, T x);   T sllist_pop(sllist_t *);     /* stack: head */
> void      sllist_add(sllist_t *, T x);    T sllist_remove(sllist_t *);  /* queue: tail/head */
> ```

**Red — write the test:** `tests/linked/sllist_test.c`:
- `test_stack` *(closed-form)*: `push 1,2,3`; `pop ⇒ 3,2,1`; empty.
- `test_queue` *(closed-form)*: `add 1,2,3`; `remove ⇒ 1,2,3`; empty.
- `test_empties_correctly` *(invariant)*: drain to empty, then `push` again — `head`/`tail`
  both update (the classic last-node bug).
- `test_differential` (queue mode) against the FIFO reference.
- Build → red.

**Green — make it pass:** `src/linked/sllist.c`. Subtlety: when `remove` empties the list,
set `tail = NULL`; `sllist_free` walks and frees every node.

**Why it matters:** This is your first pointer-surgery structure and your first real
memory-ownership discipline in C. Get the empty-list edge cases right.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** forget to update `tail` on the
last-element `remove` → a subsequent `add` corrupts the list and the differential fails; and
omit `sllist_free`'s node walk → Valgrind reports `n` leaked blocks.

**Commit:** `feat(linked): add SLList (singly-linked stack + queue)`

### Step 2.2 — `DLList`: a doubly-linked list with a dummy node (§3.2)

**Goal:** A doubly-linked List with `O(1 + min(i, n−i))` `get/set/add/remove(i)`.

**What we're building (read this first).** Each node has `prev` and `next`. The elegant trick
is a single **dummy** sentinel node whose `next` is the first element and whose `prev` is the
last — the list is circular through the dummy, which *eliminates all the NULL edge cases*
(insert/delete never special-case the ends). `get_node(i)` walks from whichever end is
closer. `add_before(w, x)` splices a new node before `w` in `O(1)`; `remove(node)` unsplices.

**Interface & invariants:** the dummy is never returned to the user and never holds a value;
`get_node(i)` walks `min(i, n−i)` steps; `add`/`remove` are `O(1)` *given the node* and
`O(1 + min(i, n−i))` given an index; `prev/next` stay mutually consistent.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 2](doc/c-toolkit.md)): a `prev`+`next` node and a
> single **dummy/sentinel** node allocated in `_new` — make the list circular through it so
> `add`/`remove` never special-case the ends (no `NULL` checks). When you `remove`, update
> *both* `node->prev->next` and `node->next->prev`. `_free` walks from `dummy->next`, frees all
> real nodes, then the dummy. Full List shape, `dllist_` prefix.

**Red — write the test:** `tests/linked/dllist_test.c`:
- `test_indexed_ops` *(closed-form)*: build `[a,b,c]`; `add(1,'x') ⇒ [a,x,b,c]`;
  `remove(2) ⇒ [a,x,c]`; `get(2)==c`.
- `test_walks_shorter_side` *(complexity)*: instrument `get_node` step count; `get(n−1)` walks
  `≈ 1` step (from the tail via the dummy), not `n`.
- `test_differential` against `ref_list`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/linked/dllist.c`. Subtlety: allocate and link the dummy in
`_new`; `_free` walks from `dummy->next` and frees every real node, then the dummy.

**Why it matters:** The sentinel-node technique removes a whole category of NULL-pointer
bugs; "walk from the nearer end" is a clean cost halving. Doubly-linked lists underpin LRU
caches, deques, and adjacency lists.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in `remove`, update only
`node->prev->next` and forget `node->next->prev` → the differential's reverse traversal /
later removes corrupt; Valgrind may flag use-after-free if you also free wrongly.

**Commit:** `feat(linked): add DLList (doubly-linked, dummy node)`

### Step 2.3 — `SEList`: a space-efficient linked list (§3.3)

**Goal:** A linked list of *small arrays* (blocks) that gets linked-list `add`/`remove` speed
with array-like space efficiency, via `spread`/`gather`.

**What we're building (read this first).** A pure linked list wastes memory on `prev`/`next`
pointers — two pointers per single element. `SEList` amortizes that overhead by storing a
**bounded array (a `BDeque` of size `b` to `b+1`) inside each node**, so the pointer overhead
is paid once per `b` elements, not per element. `add`/`remove` work within a block when
possible; when a block overflows or underflows, you `spread` (push a surplus element into the
next blocks) or `gather` (pull from neighbours / merge), keeping every block near-full.
Amortized analysis shows these rebalances are cheap. It's the array/linked-list hybrid.

**Interface & invariants:** every block (except possibly the last) holds between `b−1` and
`b+1` elements; `get/set/add/remove(i)` are `O(b + min(i,n−i)/b)`-ish; space overhead is
`O(n/b)` pointers. `spread`/`gather` restore the block-size invariant.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 2](doc/c-toolkit.md)): **nested structures** — a
> circular doubly-linked list of nodes, each node holding a small bounded array/deque (`BDeque`)
> of `b`..`b+1` elements. `_free` frees each node's inner array *and* the node. Lean on the
> differential driver; `spread`/`gather` bookkeeping is the hardest part of the chapter.
>
> **Signatures (shapes):**
> ```c
> typedef struct selist selist_t;
> selist_t *selist_new(size_t b);   /* block size fixed here */
> /* free/size/get/set/add/remove as the List shape */
> ```

**Red — write the test:** `tests/linked/selist_test.c`:
- `test_block_invariant` *(invariant)*: after many adds/removes with `b=4`, assert every
  non-final block has size in `[b−1, b+1]`.
- `test_get_set` *(closed-form)*: build a known sequence spanning ≥3 blocks; spot-check
  `get(i)` across block boundaries.
- `test_differential` against `ref_list`, 5000 ops, `b=4` (small `b` stresses spread/gather).
- Build → red.

**Green — make it pass:** `src/linked/selist.c` (a circular doubly-linked list of `BDeque`
blocks). Subtlety: `spread`/`gather` and the "add a block / remove a block" bookkeeping —
this is the hardest list in the chapter; lean on the differential driver.

**Why it matters:** Real systems (e.g. some rope/text-buffer and database row-store
implementations) use exactly this "linked list of arrays" idea to balance cache-friendliness
against cheap insertion.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** skip `gather` on underflow → the
block-invariant test fails and space blows up; leak a removed block's array → Valgrind bites.

**Commit:** `feat(linked): add SEList (block list with spread/gather)`

---

## Phase 3 — Skiplists (Chapter 4)

> **📖 Read first (whole phase):** §4.1 *The Basic Structure* (pp. 83–85), and the math you
> now need for *expected*-time analysis: §1.3.1 *Exponentials and Logarithms* (pp. 10–11) and
> §1.3.4 *Randomization and Probability* (pp. 15–17). Then §4.2/§4.3 per step.

Your first *randomized* and first *expected `O(log n)`* structures. Tests shift toward
invariants and probabilistic bounds; seed the RNG so failures reproduce.

### Step 3.1 — `SkiplistSSet`: an efficient SSet (§4.2)

**Goal:** An SSet (`find` = successor search, `add`, `remove`) in `O(log n)` *expected* time
via a tower-of-linked-lists.

**What we're building (read this first).** A skiplist is a stack of linked lists. The bottom
list holds every element in sorted order; each higher list is a random "express lane" sample
of the one below. A node's *height* is chosen by coin flips (height `k` with probability
`2^{−k−1}`). To `find(x)` you start top-left and move right while the next value is `< x`,
dropping down a level when you can't; the search path has expected length `O(log n)`. `add`
finds the insertion point, picks a random height, and splices the node into all levels up to
its height; `remove` unsplices it from every level.

**Interface & invariants:** the bottom list is sorted and complete; a node present at level
`ℓ` is present at all levels `< ℓ`; expected height and expected search-path length are
`O(log n)`; `find(x)` returns the smallest `y ≥ x`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 3](doc/c-toolkit.md)): seeded `srand`/`rand` for
> reproducible coin-flip heights (store the seed). **Variable-height nodes** — a tower of
> `next` pointers per node, best done with a *flexible array member*:
> `struct node { T x; int height; struct node *next[]; };` allocated
> `malloc(sizeof *nd + height * sizeof nd->next[0])`. Free each node **once** along the bottom
> list (it's pointed at from many levels — per-level free = double-free). A reusable predecessor
> "finger" array drives `add`/`remove`.
>
> **Signatures (shapes):**
> ```c
> typedef struct skiplist_sset skiplist_sset_t;
> skiplist_sset_t *skiplist_sset_new(unsigned seed);
> void   skiplist_sset_free(skiplist_sset_t *);   size_t skiplist_sset_size(const skiplist_sset_t *);
> bool   skiplist_sset_add(skiplist_sset_t *, T x);
> bool   skiplist_sset_remove(skiplist_sset_t *, T x, T *out);
> bool   skiplist_sset_find(const skiplist_sset_t *, T x, T *out);   /* smallest y >= x */
> ```

**Red — write the test:** `tests/skiplist/skiplist_sset_test.c`:
- `test_successor_find` *(closed-form)*: insert `{2,4,6}`; `find(3)==4`, `find(4)==4`,
  `find(7)==not found`, `find(1)==2`.
- `test_sorted_invariant` *(invariant)*: after random inserts, walking the bottom list yields
  strictly increasing values.
- `test_height_bound` *(probabilistic)*: with a fixed seed and `n=4096`, assert the tower
  height `≤ 3·log₂ n` (holds w.h.p.; document the seed).
- `test_differential` against `ref_sset`, 5000 ops (the decisive one).
- Build → red.

**Green — make it pass:** `src/skiplist/skiplist_sset.c`. Use a reusable "finger" array `stack`
of predecessors during `add`/`remove`. Subtlety: free every node *once* even though it appears
in multiple levels (free along the bottom list); a sentinel head tower simplifies edges.

**Why it matters:** Skiplists give balanced-tree performance with far simpler code, and they
introduce *randomization as a design tool* and *expected-time* analysis — themes for the rest
of the book. (They're used in Redis sorted sets, LevelDB memtables, etc.)

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in `find`, move right while `next < x`
*or equal* (i.e. `<=`) → `test_successor_find`'s `find(4)==4` goes red. Also: free a node per
level → Valgrind reports double-free.

**Commit:** `feat(skiplist): add SkiplistSSet (expected O(log n) SSet)`

### Step 3.2 — `SkiplistList`: a random-access list via skiplist (§4.3)

**Goal:** A List with `get(i)`/`set(i)`/`add(i,x)`/`remove(i)` in `O(log n)` expected time.

**What we're building (read this first).** Same skiplist skeleton, but instead of comparing
values you navigate by *position*. Each forward pointer is labeled with the **number of
bottom-level steps it skips** (`length[]`). To reach index `i`, you descend the towers adding
up lengths until you've covered `i` steps. `add(i,x)`/`remove(i)` adjust the lengths of the
pointers that span the change. This gives a list with `O(log n)` insertion *anywhere* — better
than an array's `O(n)` — at the cost of `O(log n)` `get`.

**Interface & invariants:** the sum of `length`s along any level equals `n`; `get(i)` returns
`x_i`; lengths stay consistent after `add`/`remove`; expected `O(log n)` per op.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 3](doc/c-toolkit.md)): same tower-of-pointers
> node as 3.1, but each forward pointer carries a `length` (a parallel `size_t length[]` array,
> or a `{node *next; size_t len;}` per level). Navigate by summing lengths; on splice, bump the
> length of *every* spanning pointer. Seeded RNG as in 3.1. Full List shape, `skiplist_list_`
> prefix.

**Red — write the test:** `tests/skiplist/skiplist_list_test.c`:
- `test_indexed_ops` *(closed-form)*: build `[a,b,c]`; `add(1,'x') ⇒ [a,x,b,c]`;
  `get(2)==b`; `remove(0) ⇒ [x,b,c]`.
- `test_length_invariant` *(invariant)*: after random ops, the level-0 length sum equals `n`.
- `test_differential` against `ref_list`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/skiplist/skiplist_list.c`. Subtlety: when splicing at height
`k`, update `length` on *every* affected pointer at every level, including the ones that now
span the new node without pointing at it (their length `+= 1`).

**Why it matters:** Shows the same skeleton serves order-by-value *and* order-by-position —
and gives `O(log n)` arbitrary insertion, which arrays can't.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** on `add`, increment lengths only for
pointers at the new node's own height (skip the spanning ones above) → `test_length_invariant`
and the differential `get` go red.

**Commit:** `feat(skiplist): add SkiplistList (O(log n) indexed list)`

---

## Phase 4 — Hash tables (Chapter 5)

> **📖 Read first (whole phase):** **Chapter 5** intro (p. 101). Then §5.1/§5.2/§5.3 per step
> (see §Prerequisite reading).

Expected `O(1)` USet via hashing. The new idea is *random hashing* with a provable expected
bound, and the C-specific care of integer width (use `uint32_t`/`uint64_t`, mind overflow).

### Step 4.1 — `ChainedHashTable`: hashing with chaining (§5.1)

**Goal:** A USet with expected `O(1)` `add`/`remove`/`find` using buckets of linked lists and
**multiplicative hashing**.

**What we're building (read this first).** Keep an array of `2^d` buckets, each a list. To
place key `x`, compute a bucket index by *multiplicative hashing*: pick a random odd `w`-bit
constant `z`, and use `hash(x) = (z·x mod 2^w) div 2^{w−d}` — i.e. multiply, keep the top `d`
bits. `add` appends to its bucket (after checking absence), `find` scans its bucket, `remove`
unsplices. Keep the load factor ≈ 1 by resizing (double `d`) when `n` exceeds the table size.
With a random `z`, expected bucket length is `O(1)`, so all three ops are expected `O(1)`.

**Interface & invariants:** table size is a power of two `2^d`; `n ≤ 2^d` (resize otherwise);
elements are unique (USet contract); `find(x)` returns the equal element or "not found".

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 4](doc/c-toolkit.md)): `uint32_t`/`uint64_t`
> (`<stdint.h>`) and **deliberate unsigned overflow** — multiplicative hash `(z*x) >> (w-d)`,
> with `z` odd; do the multiply in `uint64_t` when `w==32`. (Unsigned wrap is defined; signed
> overflow is UB — never hash signed.) Table size is `2^d`; buckets are `sllist`/`dllist` reuse
> or a small node list. `_free` frees every bucket node and the bucket array.
>
> **Signatures (shapes):**
> ```c
> typedef struct chained_hash chained_hash_t;
> chained_hash_t *chained_hash_new(unsigned seed);   /* seed picks odd z */
> void   chained_hash_free(chained_hash_t *);   size_t chained_hash_size(const chained_hash_t *);
> bool   chained_hash_add(chained_hash_t *, T x);          /* false if present (USet) */
> bool   chained_hash_remove(chained_hash_t *, T x, T *out);
> bool   chained_hash_find(const chained_hash_t *, T x, T *out);   /* equal element */
> ```

**Red — write the test:** `tests/hash/chained_hash_test.c`:
- `test_uset_contract` *(closed-form)*: `add(5)` returns true; `add(5)` again returns false;
  `find(5)` found; `remove(5)` returns it; `find(5)` not found.
- `test_hash_uses_high_bits` *(invariant)*: for `d` bits, `hash(x)` is always in `[0, 2^d)`;
  spot-check the formula on hand values.
- `test_grows` *(invariant)*: insert `> 2^d` elements; assert a resize happened and all
  elements still `find`-able.
- `test_differential` against `ref_uset`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/hash/chained_hash.c`. Subtlety: do the multiply in `uint64_t`
(or rely on `uint32_t` wraparound deliberately) and document `w`; `z` must be odd; free every
bucket node and the bucket array in `_free`.

**Why it matters:** This is *the* expected-`O(1)` set. Multiplicative hashing with a random
constant is the bridge from "hashing feels fast" to "hashing is provably expected-`O(1)`."

**Gate:** tests pass + Valgrind clean. **Faithfulness:** make `z` even (or `z=1`) → adversarial
keys collide and `test_differential` may still pass but `test_hash_uses_high_bits`/a collision
stress shows degeneracy; better: take the *low* bits (`mod 2^d`) instead of high bits → the
formula test goes red.

**Commit:** `feat(hash): add ChainedHashTable (multiplicative hashing)`

### Step 4.2 — `LinearHashTable`: open addressing with linear probing (§5.2)

**Goal:** A USet storing elements directly in the array (no per-element nodes), resolving
collisions by **linear probing**, with `del` tombstones.

**What we're building (read this first).** Instead of buckets-of-lists, store elements right
in the table. To `add(x)`, hash to a slot and walk forward (wrapping) until you find an empty
or `del` slot. `find(x)` walks forward from the hash until it finds `x` (success) or a truly
empty slot (failure — note it must *not* stop at a `del`). `remove(x)` marks the slot `del`
(a tombstone) rather than emptying it, so probe chains stay intact. Keep the table at most
half full (counting `del`s) by resizing. No pointers, cache-friendly, but tombstones need
care.

**Interface & invariants:** three slot states — empty (`null`), occupied, deleted (`del`);
`q = (occupied + del) ≤ length/2` triggers resize; `find` stops only at `null`, never at
`del`; resize rebuilds and drops tombstones.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 4](doc/c-toolkit.md)): elements stored *in* the
> array (no nodes) with **three slot states** — empty / occupied / `del`. Use a parallel state
> array (`enum { EMPTY, FULL, DEL }`) since any `T` value could be a real key. `find` walks
> forward (`% len`), skipping `del`, stopping only at empty. Resize when `occupied+del ≥ len/2`
> and drop tombstones. Same USet shape, `linear_hash_` prefix.

**Red — write the test:** `tests/hash/linear_hash_test.c`:
- `test_probe_skips_tombstones` *(closed-form/invariant — the crux)*: force a collision chain,
  `remove` a middle element, then `find` an element *after* it in the chain → still found.
- `test_uset_contract` as in 4.1.
- `test_resize_drops_tombstones` *(invariant)*: after many add/remove cycles, a resize reduces
  `del` count to zero and all live elements remain findable.
- `test_differential` against `ref_uset`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/hash/linear_hash.c`. Subtlety: `find` must continue past `del`;
`add` may reuse the first `del` it sees but must still check the whole chain for an existing
equal element first.

**Why it matters:** Open addressing is how most high-performance hash tables actually work
(cache locality). The tombstone subtlety is a famous source of real-world bugs.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** make `find` stop at the first `del`
→ `test_probe_skips_tombstones` goes red.

**Commit:** `feat(hash): add LinearHashTable (linear probing + tombstones)`

### Step 4.3 — Hash codes for compound data, arrays, and strings (§5.3)

**Goal:** Turn structured data (pairs, fixed tuples, arrays/strings) into a single `w`-bit
hash code so it can live in the tables above.

**What we're building (read this first).** Hashing `int`s was easy; real keys are strings and
records. The book gives two recipes: combine a *fixed* number of fields by treating them as
coordinates and taking a random linear combination; hash a *variable-length* array/string by
treating it as the coefficients of a polynomial and evaluating it modulo a prime (e.g.
`2^{w}−1`) at a random point — a rolling, collision-resistant code. You implement
`hash_string`/`hash_array` and feed them into `ChainedHashTable` keyed by strings.

**Interface & invariants:** equal inputs → equal codes (a function); the polynomial hash is
computed mod a prime with a random base; different-length inputs don't trivially collide.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 4](doc/c-toolkit.md)): polynomial hashing —
> `sum(c[i] * base^i) mod prime` in `uint64_t`, reducing mod the prime *inside* the loop to
> avoid overflow; pick the prime (near `2^w`) and a random base once and document them. Position
> weighting is load-bearing (sum-only collides anagrams).
>
> **Signatures (shapes):**
> ```c
> uint32_t hash_int(uint32_t x, uint32_t z, int d);          /* multiplicative: high d bits */
> uint64_t hash_string(const char *s, uint64_t base);        /* polynomial mod prime */
> uint64_t hash_array(const T *a, size_t n, uint64_t base);
> ```

**Red — write the test:** `tests/hash/hash_codes_test.c`:
- `test_determinism` *(invariant)*: same string hashes to the same code twice.
- `test_low_collision` *(statistical)*: hash a dictionary of N distinct strings into `2^d`
  buckets and assert the max bucket load is small (e.g. `< 4·N/2^d` for a fixed seed) — a
  sanity check, not a proof.
- `test_string_uset` *(model)*: a string-keyed `ChainedHashTable` differential-tested against a
  reference string set.
- Build → red.

**Green — make it pass:** `src/hash/hash_codes.c`. Subtlety: polynomial evaluation mod a prime
in `uint64_t` to avoid overflow; pick the prime and base once and document them.

**Why it matters:** Without good hash codes, the fancy tables collapse to `O(n)`. Polynomial
string hashing is the same idea behind Rabin–Karp and rolling hashes.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** drop the position weighting (sum the
characters) → anagrams collide and `test_low_collision`/the differential degrade visibly.

**Commit:** `feat(hash): add string/array/compound hash codes`

---

## Phase 5 — Binary trees (Chapter 6)

> **📖 Read first (whole phase):** **Chapter 6** intro (pp. 127–129). Then §6.1/§6.2 per step.

The shape behind half the rest of the book. First the bare tree and traversals, then an
(unbalanced) binary search tree.

### Step 5.1 — `BinaryTree`: the basic node, size/height/traversal (§6.1)

**Goal:** A binary-tree node (`left`, `right`, `parent`) plus recursive `size`/`height`/
`depth` and an in-order traversal — including the **pointer-based traversal with no stack**.

**What we're building (read this first).** Before search trees, build the raw scaffolding: a
node with two children and a parent pointer. `size`/`height` are simple recursions. The neat
part is **traversing without recursion or an explicit stack**, using only the parent pointers:
you move to the leftmost unvisited node, and when you can't descend, you climb via `parent`,
turning right where appropriate. It's the algorithm that makes parent pointers earn their
keep.

**Interface & invariants:** `size(u)=1+size(left)+size(right)`; `height(nil)=−1`,
`height(u)=1+max(height(left),height(right))`; in-order traversal visits the
left-subtree, node, right-subtree; the parent-pointer traversal visits the same sequence.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 5](doc/c-toolkit.md)): a **transparent** node
> struct (in the header) with `left`/`right`/`parent`; recursion over `node *` (`height(NULL)`
> `== -1`); the stackless traversal tracks "where I came from" to choose left/up/right; `_free`
> must be **post-order** (children before parent).
>
> **Signatures (shapes):**
> ```c
> typedef struct bt_node { struct bt_node *left, *right, *parent; T x; } bt_node_t;
> size_t bt_size(const bt_node_t *u);
> int    bt_height(const bt_node_t *u);                       /* -1 for NULL */
> void   bt_traverse_inorder(const bt_node_t *root, void (*visit)(T));
> void   bt_free(bt_node_t *root);                            /* post-order */
> ```

**Red — write the test:** `tests/trees/binary_tree_test.c`:
- `test_size_height` *(closed-form)*: build a small fixed tree by hand; assert `size` and
  `height` equal the values you counted.
- `test_traversal_matches_recursive` *(invariant)*: the stackless parent-pointer in-order
  traversal yields the same sequence as a plain recursive in-order traversal.
- Build → red.

**Green — make it pass:** `src/trees/binary_tree.c`. Subtlety: the climb logic in the
stackless traversal (track where you came from); `_free` must post-order free (children
before parent) to avoid use-after-free.

**Why it matters:** Every tree later is this node plus an ordering rule. The stackless
traversal is a classic interview/embedded technique (constant extra space).

**Gate:** tests pass + Valgrind clean. **Faithfulness:** set `height(nil)=0` → `test_size_height`
goes red on any nonempty tree; free a node before its children → Valgrind use-after-free.

**Commit:** `feat(trees): add BinaryTree (size/height + stackless traversal)`

### Step 5.2 — `BinarySearchTree`: an unbalanced BST (§6.2)

**Goal:** An SSet via the BST ordering rule — `find` (successor), `add`, `remove` — in `O(h)`
where `h` is the (here unbounded) height.

**What we're building (read this first).** Impose the search rule: everything in a node's left
subtree is `<` it, everything right is `>`. `find(x)` walks down comparing, tracking the last
node `≥ x` to return as the successor. `add(x)` walks to the leaf-position and links a new
node. `remove` has three cases: a leaf or one-child node *splices* out (`O(1)` relink); a
two-child node *swaps with its in-order successor* (the leftmost node of its right subtree),
then removes that. This is the prototype every balanced tree later refines.

**Interface & invariants:** in-order traversal is strictly sorted (the BST invariant); `find(x)`
returns the smallest `y ≥ x`; after `remove`, the BST invariant still holds; all ops are `O(h)`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 5 + §3](doc/c-toolkit.md)): reuse the `bt_node_t`;
> `find` walks down via `compare`, tracking the last node `≥ x`. The three-case `remove` (leaf /
> one child / two children → copy in-order successor's value up, then splice it) is all about
> keeping `parent`/`left`/`right` consistent. This SSet shape is reused by treap/scapegoat/RB.
>
> **Signatures (shapes):**
> ```c
> typedef struct bst bst_t;
> bst_t *bst_new(void);   void bst_free(bst_t *);   size_t bst_size(const bst_t *);
> bool   bst_add(bst_t *, T x);                      /* false if present */
> bool   bst_remove(bst_t *, T x, T *out);
> bool   bst_find(const bst_t *, T x, T *out);       /* successor */
> ```

**Red — write the test:** `tests/trees/bst_test.c`:
- `test_bst_invariant` *(invariant — the anchor)*: after random `add`s, an in-order traversal
  is strictly increasing.
- `test_successor_find` *(closed-form)*: insert `{2,4,6}`; `find(3)==4`, `find(7)==not found`.
- `test_remove_two_child` *(closed-form)*: build a tree where the root has two children; remove
  the root; assert the in-order sequence is the original minus that element and still sorted.
- `test_differential` against `ref_sset`, 5000 ops (random keys keep `h` reasonable).
- Build → red.

**Green — make it pass:** `src/trees/bst.c`. Subtlety: the two-child `remove` (find successor,
copy its value up, splice the successor) and keeping `parent` pointers consistent through every
relink.

**Why it matters:** The BST `add`/`find`/`remove` trio (especially the two-child remove) is
the template for treaps, scapegoat, and red-black trees. Master it once here.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in two-child `remove`, swap with the
*predecessor's parent* by mistake (or use the right subtree's root instead of its leftmost) →
`test_bst_invariant` or `test_remove_two_child` goes red.

**Commit:** `feat(trees): add BinarySearchTree (SSet, O(h))`

---

## Phase 6 — Random binary search trees (Chapter 7)

> **📖 Read first (whole phase):** **Chapter 7** intro (p. 145); re-skim §1.3.4 *Randomization
> and Probability* (pp. 15–17) if treaps' expected-depth argument feels shaky. Then §7.1/§7.2
> per step.

The first *balanced* SSets: balance comes for free from randomness.

### Step 6.1 — Random BST: depth in random insertion order (§7.1)

**Goal:** Demonstrate (and test) that inserting `n` keys in *random order* yields expected
depth `O(log n)` (`≈ 2 ln n`).

**What we're building (read this first).** No new structure — you reuse the BST from Step 5.2.
The insight you're *proving experimentally* is that a plain BST is only slow on adversarial
(e.g. sorted) input; on a random permutation, the expected search-path length is `≈ 2 ln n`.
This step turns the book's probabilistic theorem into a measured, reproducible test and sets
up *why* the next structures randomize.

**Interface & invariants:** for `n` keys inserted in uniformly random order, expected node
depth is `2 ln n − O(1)`; worst case is still `O(n)` (sorted input).

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 6](doc/c-toolkit.md)): no new structure — reuse
> the Step 5.2 BST. You need a seeded **Fisher–Yates shuffle** to build a random permutation of
> `1..n` and a small average-depth helper that recurses summing `depth · 1` per node. Document
> the seed.

**Red — write the test:** `tests/trees/random_bst_test.c`:
- `test_sorted_input_is_deep` *(closed-form)*: insert `1..n` in order; height is `n−1` (proving
  the failure mode).
- `test_random_input_is_shallow` *(probabilistic)*: with a fixed seed, insert a random
  permutation of `1..4096`; assert average depth `< 4·ln n` (comfortably above `2 ln n`).
- Build → red.

**Green — make it pass:** no new production code — these validate the Step 5.2 BST. Add a small
average-depth helper.

**Why it matters:** It motivates the entire chapter: if random order is good, *force* random
order. That's exactly what treaps do.

**Gate:** both tests pass. **Faithfulness:** seed the "random" permutation to the identity →
`test_random_input_is_shallow` goes red, proving it measures real randomness.

**Commit:** `test(trees): measure random vs sorted BST depth`

### Step 6.2 — `Treap`: a randomized balanced SSet (§7.2)

**Goal:** An SSet that is always expected `O(log n)` regardless of insertion order, by giving
each node a random priority and maintaining heap order on priorities.

**What we're building (read this first).** A treap is a BST on the *keys* and simultaneously a
heap on independent *random priorities*. Because the priorities are random, the tree's shape is
exactly that of a random-order BST — expected `O(log n)` — no matter what order keys arrive.
`add` does a normal BST insert, then *rotates the node up* while its priority violates heap
order. `remove` rotates the node *down* (toward the child with smaller priority) until it's a
leaf, then splices it. Rotations are the key new primitive: a local relink that preserves the
BST order while changing depths.

**Interface & invariants:** BST order on keys *and* min-heap order on priorities both hold at
all times; rotations preserve in-order sequence; all ops are expected `O(log n)`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phases 6–8 + §3](doc/c-toolkit.md)): add an `int
> priority` (from seeded `rand`) to the node; the new primitive is **rotations** —
> `rotate_left`/`rotate_right` each fix *three* parent links plus the tree's root pointer when
> the root rotates. Write them once here; red-black reuses them. Same SSet shape as BST,
> `treap_` prefix; `treap_new(unsigned seed)`.

**Red — write the test:** `tests/trees/treap_test.c`:
- `test_rotation_preserves_inorder` *(invariant)*: a left rotation followed by a right rotation
  returns the identical in-order sequence (and structure).
- `test_heap_and_bst_invariants` *(invariant — the anchor)*: after random ops, in-order keys are
  sorted *and* every node's priority `≤` its children's.
- `test_sorted_input_stays_shallow` *(probabilistic)*: insert `1..4096` *in sorted order*;
  assert height `< 4·log₂ n` (the whole payoff — sorted input no longer kills it).
- `test_differential` against `ref_sset`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/trees/treap.c` reusing BST search; add `rotate_left`/`rotate_right`.
Subtlety: rotations must fix *three* parent links (node, child, and the moved grandchild) plus
the root pointer when the root rotates.

**Why it matters:** Rotations + "randomness forces balance" is one of the most elegant ideas in
the book, and rotations are the engine of the next two chapters' trees.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in `add`, bubble up while priority is
*greater* (wrong direction) → `test_heap_and_bst_invariants` goes red; botch a parent link in
`rotate_left` → `test_rotation_preserves_inorder` or Valgrind catches it.

**Commit:** `feat(trees): add Treap (rotations, randomized balance)`

---

## Phase 7 — Scapegoat trees (Chapter 8)

> **📖 Read first (whole phase):** **Chapter 8** intro (pp. 165–166), then §8.1 in full
> (pp. 166–172) before Step 7.1.

Balance by *occasional rebuilding* instead of rotations — a different philosophy, amortized.

### Step 7.1 — `ScapegoatTree`: partial rebuilding (§8.1)

**Goal:** A worst-case-`O(log n)`-height SSet whose `add` is amortized `O(log n)` by rebuilding
an unbalanced subtree from scratch when the tree gets too tall.

**What we're building (read this first).** Keep a counter `q` (the max size since the last full
rebuild) and allow the height to grow only up to `log_{3/2} q`. Normal `add` is a plain BST
insert; if the new node's depth exceeds that bound, you walk back up to find the **scapegoat** —
the deepest ancestor whose subtree is too unbalanced (a child holds more than `2/3` of it) — and
**rebuild that entire subtree into a perfectly balanced one** in linear time. Removals just
decrement `n`; when `n` drops far enough below `q`, rebuild the whole tree. Amortized, the
rebuild cost spreads to `O(log n)` per operation. No rotations, no per-node extra data.

**Interface & invariants:** height `≤ log_{3/2} q` after every `add`; a node is a scapegoat iff
`size(child) > (2/3)·size(node)`; rebuild produces a subtree of minimal height; ops amortized
`O(log n)`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phases 6–8](doc/c-toolkit.md)): no rotations — keep a
> counter `q`; recompute **subtree sizes** while walking up to find the scapegoat; rebuild by
> *flattening the subtree into an array* (in-order) then *building a balanced tree from the
> middle outward*. `log_{3/2} q` needs `<math.h>` (`-lm`). Same SSet shape, `scapegoat_` prefix.

**Red — write the test:** `tests/trees/scapegoat_test.c`:
- `test_height_bound` *(invariant — the anchor)*: after inserting `1..4096` *in sorted order*,
  assert height `≤ log_{3/2}(n) + 1`.
- `test_rebuild_is_balanced` *(closed-form)*: force a rebuild of a known unbalanced subtree;
  assert the result has minimal height and identical in-order sequence.
- `test_differential` against `ref_sset`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/trees/scapegoat.c` reusing BST search. Subtlety: the
"flatten subtree to a sorted array, then build a balanced tree from the middle outward"
rebuild, and computing subtree sizes while walking up to find the scapegoat.

**Why it matters:** A completely different route to balance — *amortized rebuilding* — and a
clean example of "do expensive work rarely, pay for it gradually." The same idea reappears in
B-tree and external-memory amortization.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** loosen the rebuild trigger (allow
`size(child) > 0.9·size(node)`) → `test_height_bound` goes red on sorted input; leak the old
subtree nodes during rebuild → Valgrind reports it.

**Commit:** `feat(trees): add ScapegoatTree (partial rebuilding)`

---

## Phase 8 — Red-black trees (Chapter 9)

> **📖 Read first (whole phase):** **Chapter 9** intro (p. 177) and **all of §9.1** *2-4 Trees*
> (pp. 178–182) before *anything* — the red-black tree only makes sense as a simulation of the
> 2-4 tree. Then §9.2 (+ §9.3 Summary) before Step 8.2. This is the longest read in the book;
> budget for it.

Worst-case `O(log n)` *per operation* (not amortized) — the most intricate structure in the
book. Build the conceptual 2-4 tree first, then its red-black simulation.

### Step 8.1 — 2-4 trees: the conceptual model (§9.1)

**Goal:** A tree where every leaf is at the same depth and every internal node has 2–4 children,
with `add`/`remove` via split / merge / borrow — the model the red-black tree simulates.

**What we're building (read this first).** Before the bit-twiddling of red-black trees, build
the clean idea they encode. A 2-4 tree keeps *perfect* height balance by letting nodes vary
their *width* (2 to 4 children). Adding always happens at a leaf; if a node overflows to 5
children it **splits** and pushes one child up (possibly cascading to the root, which is how
height grows — always at the top, keeping leaves level). Removing may underflow a node to 1
child, fixed by **borrowing** from a sibling or **merging** with one. Because every leaf stays
at the same depth and depth is `O(log n)`, every op is `O(log n)`.

**Interface & invariants:** all leaves at equal depth; every internal node has 2–4 children;
height is `O(log n)`; `add`/`remove` preserve both invariants.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phases 6–8](doc/c-toolkit.md)): a node with a small
> fixed array of up to 4 child pointers + up to 3 keys (e.g. `node *child[5]; T key[4]; int
> nchild;` with one slot of slack for the overflow-before-split). Split cascades upward (and
> grows the root); underflow triggers borrow-or-merge. SSet shape, `two_four_` prefix.

**Red — write the test:** `tests/trees/two_four_test.c`:
- `test_all_leaves_same_depth` *(invariant — the anchor)*: after random `add`s, every leaf is at
  identical depth.
- `test_degree_bounds` *(invariant)*: every internal node has between 2 and 4 children
  throughout.
- `test_differential` (SSet ops) against `ref_sset`, 5000 ops.
- Build → red.

**Green — make it pass:** `src/trees/two_four.c`. Subtlety: split cascading up to and including a
root split (which increments height), and the borrow-vs-merge decision on underflow.

**Why it matters:** Understanding 2-4 trees makes red-black trees *legible* instead of magic;
they're also the direct ancestor of B-trees (Chapter 14).

**Gate:** tests pass + Valgrind clean. **Faithfulness:** on overflow, split into uneven halves
(e.g. 1 + 4) → `test_degree_bounds` goes red.

**Commit:** `feat(trees): add 2-4 tree (split/merge/borrow)`

### Step 8.2 — `RedBlackTree`: a simulated 2-4 tree (§9.2)

**Goal:** A binary tree that simulates a 2-4 tree using one red/black bit per node, achieving
worst-case `O(log n)` `add`/`remove`/`find`.

**What we're building (read this first).** A red-black tree encodes each 2-4 node as a little
cluster of binary nodes colored red/black. The colors enforce two rules that together bound the
height to `2 log n`: **no red node has a red child**, and **every root-to-leaf path crosses the
same number of black nodes** (equal *black-height*). `add` inserts a red node then restores the
rules by recoloring and rotating up the tree; `remove` is the famously fiddly part, fixing a
"double-black" deficit by borrowing/merging colors. (Implementing the *left-leaning* red-black
variant, as the book does, roughly halves the number of cases.)

**Interface & invariants:** root is black; no red node has a red child; all root-to-NULL paths
have equal black-height; therefore height `≤ 2·log₂(n+1)`; every op worst-case `O(log n)`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phases 6–8](doc/c-toolkit.md)): add a one-byte color to
> the node — `typedef enum { RED, BLACK } color_t;` (or `bool is_red`). Reuse the treap's
> `rotate_left`/`rotate_right`. Build the `add` fixups first, then `remove` case-by-case, with
> the invariant checks running after *every* op to localize the failing case. SSet shape,
> `red_black_` prefix.

**Red — write the test:** `tests/trees/red_black_test.c`:
- `test_no_red_red` *(invariant)*: after every op in a random sequence, no red node has a red
  child.
- `test_equal_black_height` *(invariant — the anchor)*: every root-to-leaf path has the same
  black-height.
- `test_height_bound` *(invariant)*: after `1..8192` inserted in sorted order, height
  `≤ 2·log₂(n+1)`.
- `test_differential` against `ref_sset`, 10000 ops (insert *and* delete heavy — removal is
  where bugs hide).
- Build → red.

**Green — make it pass:** `src/trees/red_black.c` reusing the rotation primitives from the treap.
Subtlety: removal's rebalancing cases — build them incrementally and let the differential +
invariant checks (run after *every* op) localize the failing case.

**Why it matters:** Red-black trees are the workhorse ordered map in real libraries (C++
`std::map`, the Linux kernel's `rbtree`, Java `TreeMap`). Implementing one — especially deletion
— is a rite of passage.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** skip the recolor in one `add` fixup
case → `test_no_red_red` goes red within a few inserts; mishandle a delete case → the
black-height or differential test pinpoints it.

**Commit:** `feat(trees): add RedBlackTree (worst-case O(log n))`

---

## Phase 9 — Heaps (Chapter 10)

> **📖 Read first (whole phase):** **Chapter 10** intro (p. 203). Then §10.1/§10.2 per step;
> §1.3.4 (pp. 15–17) again underpins MeldableHeap's expected-time analysis.

Priority queues: fast access to the minimum. One implicit-array heap, one randomized meldable
heap.

### Step 9.1 — `BinaryHeap`: an implicit binary tree in an array (§10.1)

**Goal:** A priority queue (`add`, `find_min`, `remove_min`) in `O(log n)`, stored in a flat
array with no pointers.

**What we're building (read this first).** A binary heap is a *complete* binary tree packed into
an array: the node at index `i` has parent `⌊(i−1)/2⌋` and children `2i+1`, `2i+2` — so the tree
structure is implicit in the indices, zero pointer overhead. The **heap property** is that each
node is `≤` its children, so the minimum is always at index 0. `add` puts the new element at the
end and **bubbles it up** while it's smaller than its parent; `remove_min` moves the last element
to the root and **bubbles it down** to the smaller child. Both are `O(log n)` because the tree is
balanced by construction.

**Interface & invariants:** for all `i>0`, `a[parent(i)] ≤ a[i]` (heap property); the tree is
complete (array has no gaps); `find_min` is `a[0]` in `O(1)`; `add`/`remove_min` are `O(log n)`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 9 + §3](doc/c-toolkit.md)): **no pointers** — an
> implicit tree in a doubling array (reuse Phase 1's growth); parent of `i` is `(i-1)/2`,
> children `2i+1`/`2i+2`. Bubble-up/down are swaps; bubble-down picks the *smaller* child and
> stops at leaves.
>
> **Signatures (shapes):**
> ```c
> typedef struct binary_heap binary_heap_t;
> binary_heap_t *binary_heap_new(void);   void binary_heap_free(binary_heap_t *);
> size_t binary_heap_size(const binary_heap_t *);
> void   binary_heap_add(binary_heap_t *, T x);
> bool   binary_heap_find_min(const binary_heap_t *, T *out);    /* false if empty */
> bool   binary_heap_remove_min(binary_heap_t *, T *out);
> ```

**Red — write the test:** `tests/heaps/binary_heap_test.c`:
- `test_heap_property` *(invariant — the anchor)*: after random `add`/`remove_min`, every
  parent `≤` its children.
- `test_sorted_extraction` *(closed-form/model)*: add a known multiset, then `remove_min`
  repeatedly; the extracted sequence is sorted ascending (and equals the input sorted) — this is
  heapsort.
- `test_index_arithmetic` *(closed-form)*: assert `parent(1)==0`, `parent(2)==0`,
  `left(0)==1`, `right(0)==2`.
- Build → red.

**Green — make it pass:** `src/heaps/binary_heap.c` (reuse `ArrayStack`-style growth for the
backing array). Subtlety: bubble-down must compare against the *smaller* child and stop
correctly at leaves.

**Why it matters:** The implicit-array heap is the standard priority queue (Dijkstra, event
simulation, `heapq`, `std::priority_queue`) and the in-place engine of heapsort (Phase 10).

**Gate:** tests pass + Valgrind clean. **Faithfulness:** bubble down toward the *larger* child →
`test_heap_property`/`test_sorted_extraction` go red.

**Commit:** `feat(heaps): add BinaryHeap (implicit array, O(log n))`

### Step 9.2 — `MeldableHeap`: a randomized meldable heap (§10.2)

**Goal:** A pointer-based heap whose core operation is `merge(h1,h2)` in `O(log n)` *expected*,
from which `add` and `remove_min` fall out trivially.

**What we're building (read this first).** Where a binary heap is rigid, a meldable heap is
flexible: every operation reduces to **merging two heaps**. `merge` compares the two roots, keeps
the smaller as the new root, and then recursively merges its *randomly chosen* child with the
other heap. The coin flip at each step keeps the expected path length `O(log n)` without any
balancing bookkeeping. `add(x)` = merge with a singleton; `remove_min` = merge the root's two
children. Beautifully short code.

**Interface & invariants:** min-heap property holds; `merge` is expected `O(log n)`; `add` and
`remove_min` are expected `O(log n)`; merging is order-independent in *result* (same multiset).

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 9](doc/c-toolkit.md)): pointer-based nodes; the
> core is a **recursive `merge`** that keeps the smaller root and recurses into a *randomly
> chosen* child (seeded RNG), reassigning parent pointers. `add` = merge a singleton;
> `remove_min` = merge the root's two children; post-order `_free`. `meldable_heap_new(unsigned
> seed)`; exposes `meldable_heap_merge(a, b)`.

**Red — write the test:** `tests/heaps/meldable_heap_test.c`:
- `test_heap_property` *(invariant)*: after random ops, parent `≤` children everywhere.
- `test_sorted_extraction` *(model)*: repeated `remove_min` yields sorted output.
- `test_merge_combines_multisets` *(invariant)*: merging two heaps then draining yields the
  union (with multiplicity) of their elements, sorted.
- `test_differential` against a reference min-priority-queue, 5000 ops.
- Build → red.

**Green — make it pass:** `src/heaps/meldable_heap.c`. Subtlety: `merge` is recursive and reassigns
parent pointers; the random child choice needs a seeded RNG; `_free` post-order frees all nodes.

**Why it matters:** "Everything is a merge" is an elegant design, and meldable/randomized heaps
show up in functional data structures and certain graph algorithms.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** always recurse into the *left* child
(remove the randomness) → adversarial insertion order makes paths `O(n)`; a height stress test
or timing shows the degradation; also keeping the *larger* root breaks `test_heap_property`.

**Commit:** `feat(heaps): add MeldableHeap (randomized merge)`

---

## Phase 10 — Sorting algorithms (Chapter 11)

> **📖 Read first (whole phase):** **Chapter 11** intro (p. 217). Read §1.3.2 *Factorials*
> (pp. 11–12) before Step 10.2's `log₂(n!)` lower bound. Then §11.1/§11.2 per step.

Algorithms over arrays rather than container types. Test by the *sortedness + permutation*
invariant and against `qsort` as a model, plus a complexity argument for the lower bound.

### Step 10.1 — Comparison sorts: merge / quick / heap (§11.1)

**Goal:** Implement merge sort, randomized quicksort, and heap sort, each `O(n log n)`, sharing
one `compare` and one sortedness oracle.

**What we're building (read this first).** Three classic `O(n log n)` sorts, each illustrating a
paradigm: **merge sort** (divide, sort halves, merge — stable, `O(n)` extra space); **quicksort**
(partition around a *random* pivot, recurse — in-place, expected `O(n log n)`, randomization
defuses the sorted-input worst case); **heap sort** (build a heap, then repeatedly extract the
min — in-place, worst-case `O(n log n)`, reusing Phase 9's bubble-down). You write all three
against the same `compare` and validate them identically.

**Interface & invariants:** output is sorted (`compare(a[i],a[i+1]) ≤ 0`) **and** a permutation
of the input (same multiset); merge sort is stable; quicksort uses a random pivot.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 10 + §3](doc/c-toolkit.md)): functions over `T *`,
> not types. Merge sort owns one temp buffer (allocate before recursion, free after); quicksort
> partitions in place around a seeded random pivot; heap sort reuses Phase 9's bubble-down. For
> `test_matches_qsort`, write a `qsort`-style `int (*)(const void *, const void *)` that calls
> your `compare`.
>
> **Signatures (shapes):**
> ```c
> void merge_sort(T *a, size_t n);
> void quick_sort(T *a, size_t n, unsigned seed);   /* random pivot */
> void heap_sort (T *a, size_t n);
> ```

**Red — write the test:** `tests/sorting/comparison_sort_test.c`, parameterized over all three:
- `test_sorts_and_permutes` *(invariant — the anchor)*: for many random arrays (incl. empty,
  one element, all-equal, already-sorted, reverse-sorted), output is sorted and a permutation of
  the input.
- `test_matches_qsort` *(model)*: each sort agrees element-for-element with libc `qsort` using
  the same comparator.
- `test_quicksort_sorted_input_ok` *(probabilistic)*: quicksort on `1..100000` already sorted
  completes without `O(n²)` blowup (bounded comparison count, fixed seed).
- Build → red.

**Green — make it pass:** `src/sorting/{merge_sort,quick_sort,heap_sort}.c`. Subtlety: merge's
temp buffer ownership (allocate once, free once); quicksort partition correctness (the classic
off-by-one); heap sort reusing the bubble-down.

**Why it matters:** These three are the canon. The "sorted ∧ permutation" pair is the *complete*
correctness spec for any sort — many buggy sorts produce sorted-but-not-permuted output.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** make merge drop the last element of a
run → `test_sorts_and_permutes`'s permutation half goes red (sorted half might still pass —
that's why you assert *both*).

**Commit:** `feat(sorting): add merge, quick, and heap sort`

### Step 10.2 — The `Ω(n log n)` comparison lower bound (§11.1.4)

**Goal:** Make the decision-tree lower bound *concrete*: any comparison sort needs `≥ log₂(n!)`
comparisons in the worst case.

**What we're building (read this first).** This step is a small experiment + an argument, not a
data structure. The proof: a comparison sort is a binary decision tree whose leaves are the `n!`
possible orderings, so its height (worst-case comparisons) is `≥ log₂(n!) ≈ n log₂ n − 1.44n`.
You make it tangible by counting your sorts' comparisons and confirming they sit at/above the
bound, and that no comparison sort beats it.

**Interface & invariants:** for `n` distinct elements, worst-case comparisons `≥ ⌈log₂(n!)⌉`;
`log₂(n!) = Θ(n log n)` (Stirling).

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 10](doc/c-toolkit.md)): compute `log2(n!)` as a
> *sum of logs* (`<math.h>`, `-lm`) — never `n!` directly (it overflows fast). Instrument a sort
> by routing its comparisons through a counter (a `static` counter or a counting comparator).
>
> **Signatures (shapes):**
> ```c
> double log2_factorial(size_t n);   /* sum_{i=1..n} log2(i) */
> ```

**Red — write the test:** `tests/sorting/lower_bound_test.c`:
- `test_comparison_count_above_bound` *(complexity)*: instrument merge sort's comparator; sort
  random size-`n` arrays and assert comparisons `≥ ⌈log₂(n!)⌉` is *not violated as a floor for a
  correct sort's worst case* — concretely, assert `merge`'s count is `≤ c·n log n` and a small
  brute-force "sort by trying all permutations" needs `≥ log₂(n!)` comparisons to distinguish
  orderings.
- `test_stirling_growth` *(closed-form)*: `⌈log₂(n!)⌉` for `n = 4` is `⌈log₂ 24⌉ = 5`; for
  `n = 8` is `⌈log₂ 40320⌉ = 16`.
- Build → red.

**Green — make it pass:** `src/sorting/log_factorial.c` computing `log₂(n!)` (sum of logs) and a
small comparison-counter harness.

**Why it matters:** Knowing *why* `O(n log n)` is optimal for comparison sorts — and that
counting/radix sort beat it only by *not comparing* — is foundational algorithmic literacy.

**Gate:** tests pass. **Faithfulness:** compute `log₂(n!)` as `n·log₂ n` exactly (no `−1.44n`
correction is needed, but) — instead, off-by-one the factorial product (start at 2 vs 1) →
`test_stirling_growth` goes red on `n=4`.

**Commit:** `test(sorting): pin the Ω(n log n) comparison lower bound`

### Step 10.3 — Counting sort and radix sort (§11.2)

**Goal:** Sort *integers* in `o(n log n)` by not comparing: counting sort in `O(n + k)`, radix
sort in `O(d(n + 2^d))` ≈ `O(n)` for word-size integers.

**What we're building (read this first).** When keys are small integers you can beat the
comparison bound. **Counting sort** tallies how many of each value `0..k−1` occur, turns the
tallies into starting offsets (prefix sums), then places each element directly — `O(n + k)`,
stable. **Radix sort** applies counting sort to one *digit* (block of `d` bits) at a time, least
significant first; with `w`-bit integers in `w/d` passes it sorts in `O((w/d)(n + 2^d))`, linear
for fixed word size. The lesson: complexity bounds depend on the *operations you're allowed*.

**Interface & invariants:** counting sort is stable and `O(n + k)`; radix sort over LSD passes
is correct *because* each pass is stable; both output a sorted permutation of the input.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 10](doc/c-toolkit.md)): counting sort uses a tally
> array + **prefix sums** as offsets, placing elements **stably** (iterate the source *backward*).
> Radix sort extracts digit `k` of width `d` with `(x >> (k*d)) & ((1u << d) - 1)` and applies a
> stable counting pass per digit, LSD first.
>
> **Signatures (shapes):**
> ```c
> void counting_sort(T *a, size_t n, T k);     /* values in [0, k) */
> void radix_sort(uint32_t *a, size_t n);      /* w-bit ints, d-bit digits */
> ```

**Red — write the test:** `tests/sorting/integer_sort_test.c`:
- `test_counting_sort` *(invariant)*: random arrays with values in `[0,k)` come out sorted and
  permuted; stability holds (tag equal keys and check order preserved).
- `test_radix_sort` *(model)*: random 32-bit ints agree with `qsort`.
- `test_radix_needs_stable_passes` *(invariant)*: replace the per-digit pass with an *unstable*
  sort and confirm radix sort breaks — proving stability is load-bearing.
- Build → red.

**Green — make it pass:** `src/sorting/{counting_sort,radix_sort}.c`. Subtlety: the prefix-sum
offset computation and stable placement; digit extraction via shifts/masks.

**Why it matters:** Linear-time integer sorting (and *why* it doesn't violate the lower bound) is
a beautiful boundary case, and radix sort is genuinely used for large fixed-width key sets.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** make the counting-sort placement
overwrite rather than respect prefix offsets (or iterate the placement loop forward instead of
backward) → stability breaks and `test_radix_needs_stable_passes` goes red.

**Commit:** `feat(sorting): add counting sort + radix sort`

---

## Phase 11 — Graphs (Chapter 12)

> **📖 Read first (whole phase):** **Chapter 12** intro (pp. 239–241). Then §12.1–§12.2 before
> Step 11.1 and §12.3 before Step 11.2.

Two representations, two traversals. Test by reachability/distance invariants on hand-drawn
graphs and against a brute-force reference.

### Step 11.1 — `AdjacencyMatrix` and `AdjacencyList` (§12.1–12.2)

**Goal:** Represent a directed graph two ways and compare their trade-offs concretely.

**What we're building (read this first).** A graph is vertices plus directed edges. The **adjacency
matrix** is an `n×n` boolean grid: `m[i][j]` is "is there an edge `i→j`?" — `O(1)` edge queries,
but `O(n²)` space and `O(n)` to list a vertex's neighbours. The **adjacency list** stores, per
vertex, a list of its out-neighbours — `O(n+m)` space and `O(deg)` to list neighbours, but
`O(deg)` edge queries. You build both behind one graph interface (`add_edge`, `remove_edge`,
`has_edge`, `out_edges`, `in_edges`) so the traversals in 11.2 work on either.

**Interface & invariants:** `has_edge(i,j)` reflects exactly the edges added/removed; matrix uses
`O(n²)` space with `O(1)` `has_edge`; list uses `O(n+m)` space with `O(deg)` `has_edge`; both
report identical edge sets.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 11 + §3](doc/c-toolkit.md)): matrix = one **flat**
> `bool *m` of size `n*n`, indexed `m[i*n + j]` (one allocation, cache-friendly). List = an array
> of adjacency lists (reuse your `dllist`/`sllist`). Put both behind one `graph_t` interface so
> the traversals work on either.
>
> **Signatures (shapes):**
> ```c
> typedef struct graph graph_t;
> graph_t *adjacency_matrix_new(size_t n);   graph_t *adjacency_list_new(size_t n);
> void graph_free(graph_t *);
> void graph_add_edge(graph_t *, size_t i, size_t j);   void graph_remove_edge(graph_t *, size_t i, size_t j);
> bool graph_has_edge(const graph_t *, size_t i, size_t j);
> ```

**Red — write the test:** `tests/graphs/graph_repr_test.c`:
- `test_edge_queries` *(closed-form)*: build a small fixed digraph; assert `has_edge` for present
  and absent edges; `out_edges(v)` returns exactly the right neighbour set.
- `test_two_reprs_agree` *(differential)*: apply the same random edge add/remove sequence to both
  representations; `has_edge` and neighbour sets always agree.
- Build → red.

**Green — make it pass:** `src/graphs/{adjacency_matrix,adjacency_list}.c` behind a common header.
Subtlety: `remove_edge` from the list form (unsplice the neighbour); matrix `in_edges` scans a
column.

**Why it matters:** Choosing the representation by graph density (sparse → list, dense → matrix)
is a real engineering decision you can now reason about from first principles.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in the list form, `add_edge` without
checking for duplicates, then `remove_edge` only the first copy → `test_two_reprs_agree` goes red.

**Commit:** `feat(graphs): add adjacency matrix + list representations`

### Step 11.2 — Breadth-first and depth-first search (§12.3)

**Goal:** BFS (shortest-path layers from a source) and DFS (recursive and stack-based), each
`O(n + m)`.

**What we're building (read this first).** Two ways to explore a graph. **BFS** uses a FIFO queue
(your Phase-1 `ArrayQueue`!) to visit vertices in increasing distance from the source, which
yields *unweighted shortest-path distances* and a BFS tree. **DFS** dives as deep as possible
before backtracking, using recursion or an explicit stack; it underlies cycle detection,
topological sort, and connectivity. Both visit every reachable vertex once and every edge once →
`O(n + m)`.

**Interface & invariants:** BFS visits vertices in nondecreasing distance and computes the
correct shortest-path distance (hop count) to each reachable vertex; DFS visits exactly the set
of reachable vertices; both mark each vertex once.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 11](doc/c-toolkit.md)): BFS reuses your
> `array_queue` + a `bool *visited` (mark *on enqueue*) + an `int *dist` (init `-1`). DFS uses
> recursion or your `array_stack` + `bool *seen`. Caller owns the output arrays.
>
> **Signatures (shapes):**
> ```c
> void bfs(const graph_t *, size_t source, int  *dist);   /* dist[v] = hops, -1 if unreachable */
> void dfs(const graph_t *, size_t source, bool *seen);
> ```

**Red — write the test:** `tests/graphs/traversal_test.c`:
- `test_bfs_distances` *(closed-form — the anchor)*: on a hand-drawn graph with known hop
  distances, BFS returns those exact distances; unreachable vertices are `∞`.
- `test_dfs_reaches_component` *(invariant)*: DFS from a source visits exactly its reachable set;
  the recursive and stack-based versions visit the same set.
- `test_against_reference_reachability` *(model)*: random graphs — BFS/DFS reachable sets equal a
  brute-force transitive-closure reference.
- Build → red.

**Green — make it pass:** `src/graphs/{bfs,dfs}.c` reusing `ArrayQueue` (BFS) and either recursion
or `ArrayStack` (DFS). Subtlety: mark a vertex *when enqueued* in BFS (not when dequeued) to avoid
double-enqueue.

**Why it matters:** BFS and DFS are the foundation of essentially all graph algorithms; reusing
your own queue/stack here is the payoff for Phase 1.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** in BFS, mark vertices when *dequeued*
instead of when enqueued → distances may still look right but a stress graph double-visits;
better: skip the visited check entirely → infinite loop / wrong distances and the test bites.

**Commit:** `feat(graphs): add BFS + DFS traversals`

---

## Phase 12 — Data structures for integers (Chapter 13)

> **📖 Read first (whole phase):** **Chapter 13** intro (pp. 257–258). The three structures
> build on each other, so read §13.1 → §13.2 → §13.3 strictly in order, each before its step.

SSets that beat `O(log n)` by exploiting that keys are `w`-bit integers — down to `O(log w)`
(doubly-logarithmic). Conceptually the hardest chapter; lean hard on the differential oracle.

### Step 12.1 — `BinaryTrie`: a digital search tree (§13.1)

**Goal:** An SSet over `w`-bit integers in `O(w)` time using a trie on the bits, with a linked
list of leaves for fast successor queries.

**What we're building (read this first).** Treat each integer as a string of `w` bits and store
it as a root-to-leaf path in a binary trie (left = 0, right = 1). Every stored value is a leaf at
depth `w`. To answer `find(x)` (successor) quickly, you thread all leaves into a sorted
doubly-linked list and add *jump pointers* so that when a search falls off the trie you can hop
straight to the successor leaf. All ops are `O(w)` — independent of `n`.

**Interface & invariants:** each value is a depth-`w` leaf; leaves form a sorted linked list;
`find(x)` returns the smallest stored `y ≥ x` in `O(w)`; `add`/`remove` maintain the jump
pointers.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 12 + §3](doc/c-toolkit.md)): treat keys as `w`-bit
> `uint32_t`; walk bits MSB→LSB with `(x >> (w-1-level)) & 1u`. Nodes have `child[2]`; values are
> depth-`w` leaves threaded into a sorted doubly-linked list (mirrors your `DLList`) with jump
> pointers for fast successor.
>
> **Signatures (shapes):**
> ```c
> typedef struct binary_trie binary_trie_t;
> binary_trie_t *binary_trie_new(int w);   void binary_trie_free(binary_trie_t *);
> bool binary_trie_add(binary_trie_t *, uint32_t x);   bool binary_trie_remove(binary_trie_t *, uint32_t x);
> bool binary_trie_find(const binary_trie_t *, uint32_t x, uint32_t *out);   /* successor */
> ```

**Red — write the test:** `tests/integers/binary_trie_test.c`:
- `test_successor_find` *(closed-form)*: insert `{2,4,6}` (as `w`-bit ints); `find(3)==4`,
  `find(7)==not found`, `find(5)==6`.
- `test_leaf_list_sorted` *(invariant)*: the leaf linked list is in increasing order after random
  ops.
- `test_differential` against `ref_sset`, 5000 ops with `w`-bit keys.
- Build → red.

**Green — make it pass:** `src/integers/binary_trie.c`. Subtlety: maintaining jump pointers on
`add`/`remove` is the fiddly part; the leaf-list splice mirrors your `DLList` work.

**Why it matters:** Tries trade comparison-based `O(log n)` for digit-based `O(w)` — the
foundation of IP routing tables, autocomplete, and the doubly-log structures next.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** drop the jump-pointer update on `add`
→ `find` returns the wrong successor and `test_successor_find` goes red.

**Commit:** `feat(integers): add BinaryTrie (O(w) integer SSet)`

### Step 12.2 — `XFastTrie`: searching in `O(log w)` (§13.2)

**Goal:** Speed `find` up from `O(w)` to `O(log w)` by binary-searching the trie's *levels* with
hash tables.

**What we're building (read this first).** The `BinaryTrie` walks all `w` levels top-down. The
`XFastTrie` instead keeps, for each level, a **hash table of the prefixes present at that level**,
then **binary-searches over the levels** to find the longest matching prefix of `x` in `O(log w)`
hash lookups — from which the successor leaf is one hop away. The cost is space: `O(n·w)`. It's a
gorgeous combination of two structures you already built (trie + hash table).

**Interface & invariants:** level `ℓ`'s hash table contains exactly the length-`ℓ` prefixes of
stored values; `find` is `O(log w)` expected; space `O(n·w)`; answers identical to `BinaryTrie`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 12](doc/c-toolkit.md)): **composition** — reuse
> your `chained_hash` (or `linear_hash`), one per level `0..w`, keyed by the length-`ℓ` prefix.
> Binary-search the levels for the longest matching prefix (`O(log w)` lookups). The work is
> keeping all `w` prefix tables consistent on every `add`/`remove`. Same SSet shape,
> `x_fast_trie_` prefix.

**Red — write the test:** `tests/integers/x_fast_trie_test.c`:
- `test_matches_binary_trie` *(differential — the anchor)*: same random op sequence on `XFastTrie`
  and `BinaryTrie`; identical `find` answers throughout.
- `test_prefix_tables` *(invariant)*: after inserts, each level's hash table holds exactly the
  expected prefixes for a small worked example.
- Build → red.

**Green — make it pass:** `src/integers/x_fast_trie.c` reusing your `ChainedHashTable` (or
`LinearHashTable`) per level. Subtlety: keeping all `w` prefix tables consistent on every
`add`/`remove`.

**Why it matters:** "Binary-search the levels instead of walking them" is a clever meta-trick,
and it shows your hash table and trie composing into something neither could do alone.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** binary-search levels with an off-by-one
midpoint (or forget to register a prefix) → `test_matches_binary_trie` diverges.

**Commit:** `feat(integers): add XFastTrie (O(log w) find)`

### Step 12.3 — `YFastTrie`: `O(log w)` with `O(n)` space (§13.3)

**Goal:** Keep the `XFastTrie`'s `O(log w)` query but cut space to `O(n)` by storing only ~`n/w`
representatives in the trie and bucketing the rest in small treaps.

**What we're building (read this first).** The `XFastTrie` is fast but space-hungry. The
`YFastTrie` fixes that: pick about `n/w` of the elements as **representatives**, store *only*
those in an `XFastTrie` (so its space is `O((n/w)·w) = O(n)`), and hang the other elements off
each representative in a small **treap** of size `O(w)`. A `find(x)` does one `O(log w)` XFastTrie
lookup to land in the right bucket, then an `O(log w)` treap search. Expected `O(log w)` time,
`O(n)` space — the best of both. It composes the previous two phases' work directly.

**Interface & invariants:** the XFastTrie holds `Θ(n/w)` representatives; each treap holds
`O(w)` elements; `find` is `O(log w)` expected; total space `O(n)`; answers identical to the
plain `BinaryTrie`.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 12](doc/c-toolkit.md)): **composition** — an
> `x_fast_trie` of ~`n/w` representatives, each owning a `treap` of `O(w)` elements. A `find`
> does one XFastTrie lookup then one treap search. Hardest part: re-choosing representatives
> (split/merge treaps) as buckets cross the size threshold. `y_fast_trie_new(int w, unsigned
> seed)`.

**Red — write the test:** `tests/integers/y_fast_trie_test.c`:
- `test_matches_binary_trie` *(differential — the anchor)*: identical `find` answers vs.
  `BinaryTrie` across 5000 random ops.
- `test_space_is_linear` *(invariant)*: after `n` inserts, the XFastTrie's element count is
  `O(n/w)` and no treap exceeds `O(w)` (assert a constant-factor bound).
- Build → red.

**Green — make it pass:** `src/integers/y_fast_trie.c` reusing `XFastTrie` + `Treap`. Subtlety:
re-choosing representatives as elements cross the bucket-size threshold (split/merge treaps), the
trickiest bookkeeping in the chapter.

**Why it matters:** A doubly-logarithmic SSet in *linear* space — and a capstone in *composing*
your own structures (trie + hash + treap) into one. Few courses ever build this; you will.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** never re-balance representatives (let a
treap grow past `O(w)`) → `test_space_is_linear` fails and queries slow down.

**Commit:** `feat(integers): add YFastTrie (O(log w) time, O(n) space)`

---

## Phase 13 — External memory searching (Chapter 14)

> **📖 Read first (whole phase):** **Chapter 14** intro (pp. 275–277). Then §14.1 before
> Step 13.1 and all of §14.2 before Step 13.2. Re-skim §9.1 *2-4 Trees* (pp. 178–182) — the
> B-tree is a width-`B` generalization of it.

When data doesn't fit in RAM, the cost model changes: you count *block transfers*, not
instructions. Build a block store, then a B-tree.

### Step 13.1 — `BlockStore`: the external-memory model (§14.1)

**Goal:** A block abstraction that makes "disk reads/writes" countable, so the B-tree's cost can
be measured in block transfers.

**What we're building (read this first).** External-memory algorithms are analyzed by how many
fixed-size **blocks** they move between fast memory and slow storage, because a disk/SSD transfer
dwarfs a CPU op. You build a `BlockStore` that allocates, reads, writes, and frees numbered
blocks (backed by RAM for testing, but counting every access). This counter *is* your cost oracle
for the B-tree: it lets you assert "this search did `O(log_B n)` block reads."

**Interface & invariants:** `read_block`/`write_block`/`place_block`/`free_block` by index; a
monotone counter of total block accesses; freed indices are reusable.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 13 + §3](doc/c-toolkit.md)): blocks are fixed-size
> byte buffers referenced by **index**, not pointer; `memcpy` data in/out. Keep a free-list of
> reusable indices and a monotone **access counter** incremented on every read/write — it *is*
> the B-tree's cost oracle.
>
> **Signatures (shapes):**
> ```c
> typedef struct block_store block_store_t;
> block_store_t *block_store_new(size_t block_size);   void block_store_free(block_store_t *);
> size_t block_store_place(block_store_t *, const void *buf);
> void   block_store_read (block_store_t *, size_t i, void *buf);          /* counted */
> void   block_store_write(block_store_t *, size_t i, const void *buf);    /* counted */
> void   block_store_free_block(block_store_t *, size_t i);
> size_t block_store_access_count(const block_store_t *);
> ```

**Red — write the test:** `tests/external/block_store_test.c`:
- `test_read_after_write` *(closed-form)*: write a block, read it back, contents match.
- `test_access_counter` *(complexity)*: a sequence of N reads/writes increments the counter by
  exactly N.
- Build → red.

**Green — make it pass:** `src/external/block_store.c`. Subtlety: free-list management for reused
indices; clear ownership of block buffers.

**Why it matters:** Different cost model, same rigor. The block counter turns an abstract
"I/O-efficient" claim into a number you can assert — exactly the discipline real database/
filesystem engineers use.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** forget to increment the counter on
`read_block` → `test_access_counter` goes red.

**Commit:** `feat(external): add BlockStore (counted block I/O)`

### Step 13.2 — `BTree`: a B-tree of order `B` (§14.2)

**Goal:** An SSet whose every node is one block of up to `B` keys, giving `O(log_B n)` block
transfers per `add`/`remove`/`find` — the structure behind real databases and filesystems.

**What we're building (read this first).** A B-tree is a generalized 2-4 tree (Phase 8) tuned for
block storage: each node holds up to `B−1` keys and `B` children and lives in *one block*, so a
search touches only `O(log_B n)` *blocks* even for billions of keys (e.g. `B=100` → a billion
keys in ~5 block reads). `add` inserts into a leaf and **splits** full nodes on the way down/up;
`remove` **merges or borrows** to fix underfull nodes. It's the 2-4 tree's split/merge logic
widened to `B`, with every node access routed through (and counted by) the `BlockStore`.

**Interface & invariants:** all leaves at equal depth; every node (except root) has between
`⌈B/2⌉` and `B` children; height is `O(log_B n)`; each op does `O(log_B n)` block transfers
(assert via the counter); in-order traversal is sorted.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 13](doc/c-toolkit.md)): a width-`B` generalization
> of your 2-4 tree (Phase 8) — node = one block holding up to `B-1` keys + `B` child *indices*,
> read/written through the `BlockStore` so every node touch is counted. Split/merge/borrow at
> width `B`. `btree_new(block_store_t *, int B)`; SSet shape, `btree_` prefix.

**Red — write the test:** `tests/external/btree_test.c`:
- `test_sorted_and_balanced` *(invariant — the anchor)*: after random ops, in-order keys are
  sorted and all leaves are at equal depth.
- `test_node_degree_bounds` *(invariant)*: every non-root node has between `⌈B/2⌉` and `B`
  children.
- `test_block_transfers_logarithmic` *(complexity — the point)*: a `find` in a B-tree of `n`
  keys does `O(log_B n)` block reads (assert against the `BlockStore` counter for several `n`).
- `test_differential` against `ref_sset`, 10000 ops.
- Build → red.

**Green — make it pass:** `src/external/btree.c` over `BlockStore`. Subtlety: split/merge/borrow
at width `B` (generalize your 2-4 tree), and routing *every* node touch through `BlockStore` so
the transfer count is honest.

**Why it matters:** B-trees run essentially every relational database index and many filesystems.
Building one — and *measuring* its block-transfer cost — connects the whole book to how data is
stored at scale.

**Gate:** tests pass + Valgrind clean. **Faithfulness:** allow nodes to underflow below `⌈B/2⌉`
(skip merge/borrow) → `test_node_degree_bounds` and the logarithmic-transfer test go red.

**Commit:** `feat(external): add B-Tree (O(log_B n) block transfers)`

---

## Phase 14 — Capstone validation (the real correctness proof)

> **📖 Read first (whole phase):** no new chapters — re-read §1.2 *Interfaces* (pp. 4–9) and
> §1.5 *Correctness, Time & Space* (pp. 19–21) with fresh eyes; you'll see why "interchangeable
> implementations of one interface" was the point all along.

Unit tests proved each structure matches its contract in isolation. This phase proves the
*whole library* is interchangeable, leak-free, and meets its bounds — the system-level verdict.

### Step 14.1 — One differential harness over *every* USet / SSet / List

**Goal:** Run *all* your implementations of each interface through the same massive randomized
op sequence and confirm they're indistinguishable from the reference and from each other.

**What we're building (read this first).** You wrote a differential driver in Step 0.4 and pointed
it at each structure as you built it. Now make it a single *parameterized* suite: register every
List (`ArrayStack`, `DLList`, `SkiplistList`, `SEList`, …), every USet (`ChainedHashTable`,
`LinearHashTable`), and every SSet (`SkiplistSSet`, `Treap`, `ScapegoatTree`, `RedBlackTree`,
`BinaryTrie`, `XFastTrie`, `YFastTrie`, `BTree`) behind its interface vtable, and drive *all of
them plus the reference* with the same 100k-operation seeded sequence, asserting they agree at
every step. One harness, total confidence.

**Interface & invariants:** for a fixed seed, every implementation of an interface returns
identical results to the reference on every operation; all are Valgrind-clean over the long run.

> **🔧 C toolkit** (see [c-toolkit.md §1.5](doc/c-toolkit.md)): this is the **vtable** pattern at
> scale — one `list_vtable_t`/`uset_vtable_t`/`sset_vtable_t` instance per implementation, all in
> an array the driver loops over. The casts from `array_stack_t *` to `void *` live in each
> table; the driver never knows the concrete type.

**Red — write the test:** `tests/capstone/all_implementations_test.c`: loop over the registered
vtables, run the long differential against `ref_*`, fail on the first divergence (printing the
op index, the operation, and which implementation diverged). Build → red until every structure is
registered.

**Green — make it pass:** register each structure's vtable; fix whatever the harness exposes
(late-surfacing bugs love deletion-heavy long runs).

**Why it matters:** This is the honest, system-level proof that your structures are truly
interchangeable realizations of the book's interfaces — the entire premise of Chapter 1.

**Gate:** the full matrix passes under a long seeded run + Valgrind clean. **Faithfulness:** swap
in a deliberately broken structure (e.g. an SSet whose `find` returns the predecessor) → the
harness pinpoints it at the first diverging op.

**Commit:** `test(capstone): differential-test all implementations together`

### Step 14.2 — Complexity benchmarks: confirm the bounds empirically

**Goal:** Measure operation cost vs. `n` for each structure and confirm the curve matches the
book's `O(·)`.

**What we're building (read this first).** A correctness suite proves *what*; this proves *how
fast*. For each structure, time (or count key operations on) a growing `n` and check the growth
matches theory: array list `add`-at-end amortizes to `O(1)`; balanced SSets scale as `O(log n)`;
hash tables stay flat (`O(1)`); the integer tries flatten further (`O(log w)`); the B-tree's
block-transfer count grows as `O(log_B n)`. You don't need nanosecond precision — you need the
*shape*: doubling `n` should multiply a `log n` op's cost by roughly a constant increment, not by
2.

**Interface & invariants:** measured cost vs. `n` matches the asymptotic class (assert ratio
bounds across doublings, with a fixed seed and enough repetitions to be stable).

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase 0](doc/c-toolkit.md)): prefer **counting key
> operations** (comparisons, element copies, probe steps, block accesses) over wall-clock —
> deterministic and machine-independent. A `static` counter or a counting comparator, reset per
> measurement; assert ratios across `n`-doublings, not absolute times.

**Red — write the test:** `tests/capstone/complexity_bench_test.c`:
- `test_array_append_amortized_o1` *(complexity)*: total element-copies for `n` appends is `< c·n`.
- `test_sset_logarithmic` *(complexity)*: comparisons-per-`find` grows like `log₂ n` (assert the
  per-op count roughly *adds a constant* each time `n` doubles, not doubles).
- `test_hash_constant` *(complexity)*: average probe length stays bounded as `n` grows.
- Build → red.

**Green — make it pass:** a small benchmarking harness with operation counters (prefer counting
key operations over wall-clock time — it's deterministic and machine-independent).

**Why it matters:** Closing the loop between *proved* complexity and *observed* behaviour is what
separates "I implemented it" from "I understand it." A structure that's correct but secretly
`O(n)` per op is still wrong for its purpose.

**Gate:** the growth-shape assertions hold. **Faithfulness:** point the `O(log n)` benchmark at
the *unbalanced* BST fed sorted input → it shows `O(n)` and the test goes red, proving the
benchmark distinguishes balanced from unbalanced.

**Commit:** `test(capstone): empirical complexity benchmarks`

---

## Phase G (optional) — Generic containers via `void *`

> **📖 Read first:** re-read the dictionary/map paragraph in §1.2.3 *USet* (p. 8) — the
> `Pair (key, value)` idea is exactly what you're enabling here.

Once everything works for `typedef int T`, generalize one structure (e.g. `DLList` and
`SkiplistSSet`) to store `void *` elements with a user-supplied `compare` and an optional
`free_elem` callback, so the containers can hold arbitrary types. Test it by instantiating with a
`Pair` (key→value) to build a real dictionary/map, exactly as the book describes USet-as-map in
§1.2.3. Faithfulness: a leak in the `free_elem` path must be caught by Valgrind.

> **🔧 C toolkit** (see [c-toolkit.md §2 Phase G + §3](doc/c-toolkit.md)): `T` becomes `void *`;
> the user supplies `int (*compare)(const void *, const void *)` and an optional
> `void (*free_elem)(void *)` stored at `_new`. The container now *may* own the pointed-to
> values — `_free` calls `free_elem` on each (if non-NULL). Build a map with
> `typedef struct { void *key; void *value; } pair_t;`.
>
> **Signatures (shapes):**
> ```c
> typedef int  (*cmp_fn)(const void *a, const void *b);
> typedef void (*free_fn)(void *elem);
> dllist_g_t *dllist_g_new(cmp_fn cmp, free_fn free_elem /* may be NULL */);
> ```

**Commit:** `feat(core): generalize containers to void* elements`

---

## Key contracts & bounds (one place to look them up)

| Structure (chapter) | `find` / search | `add` | `remove` | Space | Notes |
|---|---|---|---|---|---|
| **ArrayStack** (2) | `get/set` O(1) | O(n−i) amrt | O(n−i) amrt | O(n) | doubling array |
| **ArrayQueue / ArrayDeque** (2) | get O(1) | O(1)/O(min(i,n−i)) amrt | same | O(n) | circular index `a[(j+i)%len]` |
| **RootishArrayStack** (2) | get O(1) | O(n−i) | O(n−i) | **O(√n)** waste | block `b` size `b+1` |
| **SLList / DLList** (3) | O(1+min(i,n−i)) | O(1) given node | O(1) given node | O(n) | DLList uses a dummy node |
| **SEList** (3) | O(b+…) | amrt | amrt | O(n) + O(n/b) ptrs | list of size-`b` arrays |
| **SkiplistSSet / List** (4) | O(log n) exp | O(log n) exp | O(log n) exp | O(n) exp | random tower heights |
| **ChainedHashTable** (5) | O(1) exp | O(1) exp | O(1) exp | O(n) | `hash=(z·x mod 2^w) div 2^{w−d}` |
| **LinearHashTable** (5) | O(1) exp | O(1) exp | O(1) exp | O(n) | open addressing + `del` tombstones |
| **BinarySearchTree** (6) | O(h) | O(h) | O(h) | O(n) | unbalanced; `h` up to `n` |
| **Treap** (7) | O(log n) exp | O(log n) exp | O(log n) exp | O(n) | BST on key + heap on random priority |
| **ScapegoatTree** (8) | O(log n) | O(log n) amrt | O(log n) amrt | O(n) | rebuild when `height > log_{3/2} q` |
| **RedBlackTree** (9) | O(log n) | O(log n) | O(log n) | O(n) | worst-case; no red-red, equal black-height |
| **BinaryHeap** (10) | min O(1) | O(log n) | O(log n) | O(n) | implicit array; `parent=⌊(i−1)/2⌋` |
| **MeldableHeap** (10) | min O(1) | O(log n) exp | O(log n) exp | O(n) | everything via random `merge` |
| **BinaryTrie** (13) | O(w) | O(w) | O(w) | O(n·w) | bits + sorted leaf list + jumps |
| **XFastTrie** (13) | **O(log w)** exp | O(w) exp | O(w) exp | O(n·w) | per-level prefix hash tables |
| **YFastTrie** (13) | **O(log w)** exp | O(log w) exp | O(log w) exp | **O(n)** | reps in XFastTrie + size-`w` treaps |
| **BTree** (14) | O(log_B n) xfers | O(log_B n) | O(log_B n) | O(n) | node = one block; like a width-`B` 2-4 tree |

| Sort (chapter 11) | Time | Space | Stable? | How it's tested |
|---|---|---|---|---|
| Merge sort | O(n log n) | O(n) | yes | sorted ∧ permutation; vs `qsort` |
| Quicksort (random pivot) | O(n log n) exp | O(log n) | no | sorted ∧ permutation; sorted-input safe |
| Heap sort | O(n log n) | O(1) | no | sorted ∧ permutation; reuses BinaryHeap |
| Comparison lower bound | `≥ ⌈log₂(n!)⌉ = Θ(n log n)` | — | — | decision-tree / comparison count |
| Counting sort | O(n + k) | O(n + k) | yes | stable placement via prefix sums |
| Radix sort | O((w/d)(n + 2^d)) | O(n + 2^d) | yes (per pass) | breaks if a pass is unstable |

| Concept | Value / formula |
|---|---|
| **`compare(x,y)`** | `<0` if `x<y`, `0` if `=`, `>0` if `x>y` |
| **USet `find`** | returns element *equal* to `x` (or not-found) |
| **SSet `find`** | returns smallest `y ≥ x` (successor search) |
| **Amortized doubling** | `n` end-adds cost `O(n)` total ⇒ `O(1)` each |
| **`log₂(n!)`** | `n=4 ⇒ 5`, `n=8 ⇒ 16`; `≈ n log₂ n − 1.44n` (Stirling) |
| **Multiplicative hash** | random *odd* `z`; keep the high `d` bits |

---

## Interface → C implementation map

| Interface | Implementations you build |
|---|---|
| **List** | `ArrayStack`, `FastArrayStack`, `ArrayDeque`, `DualArrayDeque`, `RootishArrayStack`, `DLList`, `SEList`, `SkiplistList` |
| **Stack / Queue / Deque** | `ArrayStack` (stack), `ArrayQueue` (FIFO), `ArrayDeque`/`SLList` |
| **USet** | `ChainedHashTable`, `LinearHashTable` |
| **SSet** | `SkiplistSSet`, `Treap`, `ScapegoatTree`, `RedBlackTree`, `BinaryTrie`, `XFastTrie`, `YFastTrie`, `BTree` |
| **Priority queue** | `BinaryHeap`, `MeldableHeap` |
| **Algorithms** | merge/quick/heap/counting/radix sort; BFS; DFS |
| **Test oracles** | `ref_uset`, `ref_sset`, `ref_list` + the differential driver (Step 0.4) |

---

## Suggested directory structure

Tests are first-class: each `*_test.c` is written **before** the code it checks.

```
data-structure/
├── include/
│   ├── ods.h                      # typedef T; compare; interface signature templates (0.3)
│   └── test_util.h                # CHECK_* macros, test runner (0.2)
├── src/
│   ├── CMakeLists.txt             # add_subdirectory per module
│   ├── lists/                     # Chapter 2  (ODS_LISTS)
│   ├── linked/                    # Chapter 3  (ODS_LINKED)
│   ├── skiplist/                  # Chapter 4
│   ├── hash/                      # Chapter 5
│   ├── trees/                     # Chapters 6–9 (binary_tree, bst, treap, scapegoat, two_four, red_black)
│   ├── heaps/                     # Chapter 10
│   ├── sorting/                   # Chapter 11
│   ├── graphs/                    # Chapter 12
│   ├── integers/                  # Chapter 13
│   └── external/                  # Chapter 14 (block_store, btree)
├── tests/
│   ├── CMakeLists.txt             # add_subdirectory per module + support + capstone
│   ├── support/                   # smoke, test_util self-test, ref_* + diff_driver (0.1–0.4)
│   ├── lists/ linked/ … external/ # one *_test.c per structure (mirrors src/)
│   └── capstone/                  # all_implementations_test.c, complexity_bench_test.c (Phase 14)
├── doc/
│   └── conventions.md             # 0.3
├── CMakeLists.txt                 # enable_testing(); add_subdirectory(src); add_subdirectory(tests)
├── ods-python.pdf                 # the reference book
└── LEARNING-ROADMAP.md            # this file
```

*(This sits alongside the existing `Lecture_1/2/3` directories; you can grow it incrementally
phase by phase without touching them.)*

---

## Dependencies

**Effectively none.** C11 + the C standard library, built with **CMake ≥ 3.16** and run via
**CTest**. The only "tool" is **Valgrind** for the memory-safety gate (`apt install valgrind`).
No external libraries, no test framework — you write the ~40-line assertion harness yourself in
Step 0.2. That's the whole point: every line of every data structure is code *you* wrote and
*you* tested.

---

## Milestone checklist (tape this above your desk)

Each box flips only when its step's test was written first, seen to fail, is now green —
**Valgrind-clean** — *and you've sabotaged the code to confirm the test can fail.*

- [ ] **Phase 0** — honest `ctest`; meta-tested assertion harness; conventions + interface headers; **brute-force references + differential driver**
- [ ] **Phase 1** (Ch 2) — ArrayStack **amortized doubling**; circular ArrayQueue/Deque; DualArrayDeque balance; RootishArrayStack **O(√n) space**
- [ ] **Phase 2** (Ch 3) — SLList empty-edge cases; DLList dummy node; SEList spread/gather block invariant
- [ ] **Phase 3** (Ch 4) — SkiplistSSet successor `find` + height bound; SkiplistList length invariant
- [ ] **Phase 4** (Ch 5) — ChainedHashTable multiplicative hashing; LinearHashTable **tombstone probing**; string hash codes
- [ ] **Phase 5** (Ch 6) — BinaryTree stackless traversal; BST **in-order-sorted invariant** + two-child remove
- [ ] **Phase 6** (Ch 7) — random vs sorted BST depth; Treap **rotations** + heap/BST invariants
- [ ] **Phase 7** (Ch 8) — ScapegoatTree **height ≤ log_{3/2} q** via rebuilding
- [ ] **Phase 8** (Ch 9) — 2-4 tree leaves-level invariant; RedBlackTree **no red-red + equal black-height**
- [ ] **Phase 9** (Ch 10) — BinaryHeap implicit-array heap property; MeldableHeap random merge
- [ ] **Phase 10** (Ch 11) — merge/quick/heap **sorted ∧ permutation**; **Ω(n log n)** bound; counting/radix (stability load-bearing)
- [ ] **Phase 11** (Ch 12) — matrix vs list agree; **BFS distances** + DFS reachability
- [ ] **Phase 12** (Ch 13) — BinaryTrie O(w); XFastTrie **O(log w)**; YFastTrie **O(log w) in O(n) space**
- [ ] **Phase 13** (Ch 14) — BlockStore counter; B-Tree **O(log_B n) block transfers**
- [ ] **Phase 14** — one differential harness over **all** implementations; empirical complexity benchmarks
- [ ] **Phase G** *(optional)* — generic `void *` containers; USet-as-map (Pair)
