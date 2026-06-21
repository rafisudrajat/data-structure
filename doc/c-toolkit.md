# The C Toolkit — the language half of the roadmap

[`LEARNING-ROADMAP.md`](../LEARNING-ROADMAP.md) tells you *what* to build and *how to test* it.
It deliberately stays in the book's pseudocode and never says "use `memmove` here" or "this
struct needs a `size_t n` field." This document is the other half: **the concrete C techniques
and the type/function *shapes* each step needs**, so a gap in your C — not a gap in your
understanding of the structure — never blocks you.

**Grounded in the book.** The shapes and idioms here are a faithful C translation of Pat Morin's
*Open Data Structures* ([`../ods-python.pdf`](../ods-python.pdf)). The field names match the
book's pseudocode: the backing array is `a`, the element count is `n`, the capacity is
`length(a)` (which we hold in a `len` field), and the queue head index is `j`. Where the book
itself talks about C — e.g. §2.2 explicitly names `memcpy(d,s,n)` and `memmove(d,s,n)` — this
doc just points you at the right one. **Always read the book section the roadmap names for a
step before writing code; it is the source of truth, and these shapes only spare you the "how do
I spell this in C?" friction.**

How to read it, alongside the roadmap:

- **§0 You're coming from C++** — the most useful section for you specifically. Names every C++
  feature you used in `Lecture_*` and what you write *instead* in C.
- **§1 Project-wide idioms** — the five patterns every structure reuses (opaque structs,
  `_new`/`_free`, the `bool found` out-param, `compare`, the function-pointer "vtable"). Learn
  these once.
- **§2 C concepts by phase** — for each phase, the *new* C techniques it introduces, with a tiny
  example. Read a phase's entry before its first step.
- **§3 Signature-shape appendix** — for every structure, the prototype shapes (`typedef` +
  function declarations). **Shapes, not bodies:** the fields inside the struct and the function
  bodies are yours to derive — that's the point of the TDD roadmap. The shapes just remove the
  "I don't know how to *write* this in C" friction.

---

## §0 — You're coming from C++ (read this first)

Your `Lecture_*` code is C++: `template`, `class`, `new`/`delete`, `namespace`, `std::cout`,
`throw`. This roadmap is **pure C11**. C removes the conveniences you leaned on; almost every
"how do I do X in C?" you'll hit is in this table.

| In C++ (what you did) | In C (what you do instead) | First bites at |
|---|---|---|
| `template<typename ElementType>` | one `typedef int T;` now; `void *` + a `compare` callback in Phase G | every container |
| `class Foo { ... };` with methods | `struct foo` (opaque) + free functions `foo_add(foo_t *self, …)` | every structure |
| `obj.method(x)` | `foo_add(obj, x)` — the object is the **explicit first parameter** | everywhere |
| constructor `Foo()` | `foo_t *foo_new(void)` returning a heap pointer | §1.2 |
| destructor `~Foo()` (automatic) | `void foo_free(foo_t *)` you **must call yourself** | §1.2, Valgrind |
| `new T[n]` / `delete[]` | `malloc(n * sizeof(T))` / `free(p)` | Phase 1 |
| growing: `new`, copy, `delete[]` (your `resize`) | `realloc`, or `malloc`+`memcpy`+`free` | Phase 1 |
| `throw "Index..."` (your getters) | `assert(i < n)` for caller bugs; `bool`/out-param for "not found" | §1.3 |
| references `T&` | pointers `T *` (and `const T *` for read-only) | everywhere |
| `std::optional` / exceptions for "absent" | `bool foo_find(const foo_t *, T x, T *out)` — return `false`, leave `*out` alone | §1.3 |
| `std::cout << x` | `printf("%d", x)` (tests / debugging only) | tests |
| `nullptr` | `NULL` | linked structures |
| virtual functions / overriding | a **struct of function pointers** you fill in by hand (a manual vtable) | §1.5, Phase 0.4 |
| `std::vector`, `std::map`, `qsort` | you are *building* these — don't reach for libc containers | the whole point |
| RAII (cleanup automatic) | **manual** cleanup; one `free` per `malloc`, checked by Valgrind | every phase |
| `int` for sizes/indices | `size_t` for sizes/indices (unsigned; matches `sizeof`/`malloc`) | Phase 1 on |

**The two habits that hurt at first, and the fix:**

1. **Memory is manual; Valgrind is the destructor you lost.** In C++ the destructor freed
   everything automatically. In C *you* free it, exactly once. Every `*_new` allocates; its
   matching `*_free` must release *everything the structure owns* (the backing array **and**,
   for linked structures, every node). Missed `free` = leak; double `free`/use-after-free =
   corruption. Valgrind (`valgrind --leak-check=full --error-exitcode=1 ./test`) catches all
   three — it is your replacement for `~Foo()`.

2. **No polymorphism keyword.** Your differential driver (Step 0.4) must drive an `ArrayStack`
   and a `DLList` through *the same code*. In C++ you'd use a virtual base class; in C you make a
   `struct` of **function pointers** and fill one in per implementation — see §1.5. This is the
   one genuinely new idea; everything else is "the same thing, more typing."

> **Note on your `AmmortizedDynamicArray`:** you already built the resize/doubling logic in C++.
> Step 1.1 is that same algorithm in C — `new ElementType[]`→`malloc`, `delete[]`→`free`, your
> `copyArrayContainer...` loops→`memmove`. The book's field names are `a` (array), `n` (count),
> `length(a)` (capacity); it grows to `max(1, 2n)` and shrinks when `length(a) ≥ 3n`.

---

## §1 — Project-wide idioms (learn these five, reuse them everywhere)

### 1.1 The opaque struct (your "class")

Put the *type name* in the header, hide the *fields* in the `.c` — the C version of `private`.

```c
/* array_stack.h */
typedef struct array_stack array_stack_t;   /* opaque: fields not visible here */

/* array_stack.c */
struct array_stack { T *a; size_t len; size_t n; };  /* the real definition (a, len, n: book's names) */
```

When the *algorithm* needs to touch the fields directly (a tree `node`'s `left`/`right`, a
linked `node`'s `next`), define that struct in the header instead. Rule of thumb: nodes are
transparent, top-level containers are opaque.

> **`struct foo` vs `foo_t`.** C makes you write `struct foo` unless you `typedef`. The
> convention `typedef struct foo foo_t;` lets you write `foo_t` like a C++ type name. A struct
> can refer to itself by its `struct` tag before the typedef completes — needed for nodes:
> `struct node { T x; struct node *next; };`.

### 1.2 `_new` / `_free` — the ownership pair (your constructor/destructor)

```c
foo_t *foo_new(void);     /* malloc the struct (+ initial buffers); return it (NULL on OOM) */
void   foo_free(foo_t *); /* free everything it owns, then the struct; must be NULL-safe */
```

- **One `_free` per `_new`** — calling it is *your* job (no RAII).
- `foo_free(NULL)` is a safe no-op (mirrors `free(NULL)`).
- Linked/tree `_free` walks and frees **every node** before the struct.
- The structure owns its *buffers and nodes*, **not** the `T` values (they're `int`s by value).
  Phase G adds an optional `free_elem` callback for when `T` becomes `void *`.

### 1.3 Reporting results: `assert` for bugs, `bool`+out-param for "not found"

Two different situations — don't conflate them:

- **Precondition violation = a bug in the caller** (e.g. `get(i)` with `i >= n`). Use
  `assert(i < n);` (`<assert.h>`). This replaces your `throw "Index..."`: it documents the
  contract and aborts loudly in debug builds.
- **A legitimate "absent" answer** (`find(x)` when `x` isn't there). C has no `std::optional`
  and no exceptions, and the roadmap forbids magic sentinel values. Return `bool` + write through
  an **out pointer**:

```c
/* true ⇒ found, *out written. false ⇒ absent, *out untouched. */
bool uset_find(const uset_t *s, T x, T *out);
```

The book's `find` returns "`y` or `nil`"; the `bool`+out-param is the faithful C encoding of
that (and it's *every* `find` in the book — USet and SSet differ only in *which* `y`).

### 1.4 `compare` — one comparison function, never inline `<`

The book (§1.2.4) compares with `compare(x,y)`:

```c
/* the book's contract, verbatim:
     compare(x,y)  < 0  if x < y
                   > 0  if x > y
                   = 0  if x = y                                  */
int compare(T a, T b);
```

Every SSet and every sort calls it. Centralizing it means changing `T` (Phase G) or sort order
touches one function — and its signature matches libc `qsort`'s callback, so your sorts and
`qsort` agree in `test_matches_qsort`.

### 1.5 The function-pointer "vtable" (your `virtual`, by hand)

To test many implementations of one interface with one driver (Step 0.4 / Phase 14), describe
the interface as a struct of function pointers and make one instance per implementation:

```c
typedef struct {
    void *(*create)(void);
    void  (*destroy)(void *self);
    void  (*add)(void *self, size_t i, T x);
    T     (*get)(void *self, size_t i);
    /* ...one field per List operation... */
} list_vtable_t;

/* one table per implementation; the driver takes a list_vtable_t * + a seed */
static const list_vtable_t array_stack_vt = {
    .create  = (void *(*)(void))            array_stack_new,
    .destroy = (void  (*)(void *))          array_stack_free,
    .add     = (void  (*)(void *, size_t, T)) array_stack_add,
    /* ... */
};
```

`self` is `void *` because the driver is generic over the concrete type; each function casts it
back. This *is* dynamic dispatch — you're writing the table the C++ compiler would have
generated. Designated initializers (`.field = value`) keep the tables readable; the casts are
needed because each real function takes `array_stack_t *`, not `void *`.

### 1.6 Small but constant gotchas

- **`size_t` for sizes/indices** (unsigned; matches `sizeof`/`malloc`). Beware:
  `for (size_t i = n - 1; i >= 0; i--)` is an **infinite loop**. Count down with
  `for (size_t i = n; i-- > 0; )`.
- **`const`-correctness:** read-only ops take `const foo_t *` (`get`, `size`, `find`).
- **Check `malloc`/`realloc` for `NULL`** before use (at minimum `assert` it).
- **`realloc` may move the block** — always assign back: `p = realloc(p, newsize);`.
- **`memmove` vs `memcpy`:** overlapping regions ⇒ `memmove`; non-overlapping ⇒ either. Sizes
  are **bytes**: `count * sizeof(T)`. (The book points this out in §2.2.)
- **Headers:** `#pragma once` atop every `.h`. Include what you use: `<stddef.h>` (`size_t`,
  `NULL`), `<stdbool.h>` (`bool`), `<stdint.h>` (`uint32_t`/`uint64_t`), `<string.h>`
  (`memmove`/`memcpy`), `<assert.h>`, `<stdlib.h>` (`malloc`/`free`/`rand`), `<math.h>`
  (`sqrt`/`log`, link with `-lm`).

---

## §2 — C concepts by phase

Read a phase's entry *before* its first step. Each lists only the **new** C techniques that
phase introduces; everything in §1 is assumed throughout.

### Phase 0 — Foundations (harness, conventions, references)
- **Macros with `__FILE__`/`__LINE__` and stringizing (`#x`).** Your `CHECK` macros print
  *which* assertion failed and *where*:
  ```c
  #define CHECK_INT_EQ(a, b) do { if ((a) != (b)) { \
      fprintf(stderr, "%s:%d: CHECK_INT_EQ(%s, %s): %ld != %ld\n", \
              __FILE__, __LINE__, #a, #b, (long)(a), (long)(b)); fails++; } } while (0)
  ```
  Wrap multi-statement macros in `do { ... } while (0)` so they act like one statement.
- **Process exit codes:** `main` returns `int`; `0` = success (CTest reads it). Return the
  failure count so a green bar is honest.
- **Header-only / `INTERFACE` CMake library** for `test_util.h` (no `.c` to compile).
- **Function pointers** (§1.5) — the differential driver is built on them.

### Phase 1 — Array-based lists (Chapter 2)
- **`malloc`/`realloc`/`free`** for the backing array. Growth = `b ← new_array(max(1, 2n))`,
  copy `n` elements, free old, `a ← b` (book §2.1.2).
- **`memmove` vs `memcpy`** — `add(i,x)`/`remove(i)` shift *overlapping* regions ⇒ `memmove`.
  `memcpy` on overlap is UB (the deliberate trap in Step 1.2).
- **`size_t` capacity arithmetic** with `n`, `len`, invariant `n ≤ len`. Shrink when `len ≥ 3n`.
- **Modular arithmetic for circular buffers** (`ArrayQueue`/`ArrayDeque`): element `i` lives at
  `a[(j + i) % len]`; `add` writes `a[(j + n) % len]`; `remove` does `j = (j + 1) % len`.
- **Integer/float formula care** (`RootishArrayStack`): block index via `sqrt` (`<math.h>`,
  `-lm`); verify against a hand table — truncation bugs hide here.

### Phase 2 — Linked lists (Chapter 3)
- **Self-referential structs + pointer surgery.**
  `struct node { T x; struct node *next, *prev; };` — `add`/`remove` are a few reassignments;
  order matters (relink before you orphan).
- **`NULL` checks and empty-list edge cases** — the classic "forgot to update `tail`" bug.
- **The dummy/sentinel node** (`DLList`): a non-data node makes the list circular and erases all
  the `NULL` special-cases. Allocate in `_new`, never expose, free last.
- **Freeing a linked structure:** save `next` *before* `free`-ing the current node.
- **Nested structures** (`SEList`): a node that itself holds a small bounded array/deque.

### Phase 3 — Skiplists (Chapter 4)
- **Reproducible randomness:** `srand(seed)`/`rand()` (seed it and document it, per the roadmap).
  A node's height = number of leading coin-flips.
- **Variable-height nodes / arrays of pointers.** A node has a *tower* of `next` pointers.
  Either `struct node **next` (a malloc'd pointer array) or a **flexible array member**:
  `struct node { T x; int height; struct node *next[]; };` allocated with
  `malloc(sizeof(struct node) + height * sizeof(struct node *))`. Free each node **once** (walk
  the bottom list), though it appears at many levels.
- **A reusable "finger" array** of predecessors collected during search, reused by `add`/`remove`.

### Phase 4 — Hash tables (Chapter 5)
- **Fixed-width ints + deliberate overflow.** `uint32_t`/`uint64_t` (`<stdint.h>`).
  Multiplicative hash `(z·x) mod 2^w` is just unsigned multiplication *letting it wrap* —
  **unsigned overflow is defined; signed overflow is UB, so never hash with signed ints.** Do
  the multiply in `uint64_t` when `w == 32` to keep the high bits, then shift.
- **Shifts and masks:** "top `d` bits" = `(z * x) >> (w - d)`; "mod `2^d`" = `& ((1u << d) - 1)`.
- **Three-state slots** (`LinearHashTable`): empty / occupied / `del`. `find` skips `del`, stops
  only at empty; resize drops tombstones.
- **Polynomial string hashing** (§5.3): `sum(c[i] · base^i) mod prime` in `uint64_t`; reduce mod
  prime *inside* the loop to avoid overflow; pick prime + random base once and document them.

### Phase 5 — Binary trees (Chapter 6)
- **Recursion over `node *`** for `size`/`height` (`height(NULL) == -1`).
- **Parent pointers** drive the stackless traversal — track where you came from to go
  left / up / right.
- **Post-order `free`** (children before parent).
- **Three-case `remove`** (leaf / one child / two children → swap with in-order successor),
  keeping `parent`/`left`/`right` consistent through each relink.

### Phases 6–8 — Balanced trees (Chapters 7–9)
- **Rotations** (`rotate_left`/`rotate_right`): a local relink fixing *three* parent links plus
  the tree's root pointer when the root rotates. Write once, reuse in treap and red-black.
- **`enum`/one-byte flags** for colors: `typedef enum { RED, BLACK } color_t;` (or `bool is_red`).
- **Random priorities** (treap): one `int` per node from `rand()`; min-heap order on it.
- **Subtree-size bookkeeping** (scapegoat): recompute sizes walking up to find the scapegoat;
  "flatten to array → rebuild balanced from the middle" for the rebuild.
- **`double`/`log`** (`<math.h>`, `-lm`) for the `log_{3/2} q` height bound.

### Phase 9 — Heaps (Chapter 10)
- **Implicit tree in an array** (`BinaryHeap`): no pointers — parent of `i` is `(i-1)/2`,
  children `2i+1`/`2i+2`. Bubble-up/down are swaps; reuse Phase 1's doubling array.
- **Recursive pointer merge** (`MeldableHeap`): `merge` reassigns parent pointers; random child
  choice via the seeded RNG; post-order `free`.

### Phase 10 — Sorting (Chapter 11)
- **A `qsort`-style comparator** `int (*)(const void *, const void *)` so `test_matches_qsort`
  compares against libc using the same logic as your `compare`.
- **A temp buffer owned once** (merge sort): allocate before recursion, free after.
- **In-place partition** (quicksort): the index dance; random pivot via the seeded RNG.
- **Bit extraction** (radix): digit `k` of width `d` = `(x >> (k*d)) & ((1u << d) - 1)`.
  Counting sort's prefix-sum offsets + **stable** placement (iterate source backward).

### Phase 11 — Graphs (Chapter 12)
- **A flat 2D boolean grid** (adjacency matrix): one `bool *m` of size `n*n`, indexed
  `m[i*n + j]` (beats `bool **` — one allocation, cache-friendly).
- **Array of adjacency lists:** reuse your `DLList`/`SLList`, or an array of small dynamic arrays.
- **Reusing your own containers:** BFS uses your `ArrayQueue`, DFS your `ArrayStack` (or
  recursion); a `bool *visited` array marks vertices — mark *on enqueue* in BFS.

### Phase 12 — Integer structures (Chapter 13)
- **An integer as a bit string:** walk MSB→LSB with `(x >> (w - 1 - level)) & 1u`. Keys are
  fixed `w`-bit (`uint32_t` with a chosen `w`).
- **Composing earlier structures:** XFastTrie = trie + your hash table per level; YFastTrie =
  XFastTrie + your treaps. The C work is keeping several invariants consistent per op, plus the
  leaf doubly-linked list (mirrors `DLList`).

### Phase 13 — External memory (Chapter 14)
- **Block buffers + (de)serialization:** a block is a fixed-size byte buffer; `memcpy` keys and
  child *indices* in/out. Nodes are referenced by **block index**, not pointer.
- **A free-list of reusable indices** and a monotone **access counter** — incremented on every
  read/write; the counter *is* the cost oracle.

### Phase G — Generic containers (`void *`)
- **`void *` elements + callbacks:** `T` becomes `void *`; the user supplies
  `int (*compare)(const void *, const void *)` and optional `void (*free_elem)(void *)`.
- **A `pair_t { void *key; void *value; }`** to build a map on a USet, exactly as the book's
  §1.2.3 dictionary paragraph describes.
- **Ownership care:** the container may now own the values via `free_elem` — a leak there is a
  Valgrind target.

---

## §3 — Signature-shape appendix (per structure)

Prototype **shapes** only — the opaque type and the function declarations. You design the fields
and write the bodies. `T` is `typedef int T;` (Phase G → `void *`). All `_free` are NULL-safe and
release everything owned; read-only ops take `const`. Names follow the roadmap's
`module_operation` convention so the differential driver and CMake wiring line up.

### Phase 0 — harness & references

```c
/* test_util.h — header-only */
#define CHECK(cond)          /* ... */
#define CHECK_INT_EQ(a, b)   /* ... */
#define CHECK_TRUE(x)        /* ... */
#define CHECK_FALSE(x)       /* ... */
/* + a runner that tracks a failure count and returns it from main */

/* ods.h — the contracts every structure fills in */
typedef int T;
int compare(T a, T b);                              /* <0 / 0 / >0 */

/* reference impls (Step 0.4) — obviously-correct, slow */
typedef struct ref_uset ref_uset_t;
ref_uset_t *ref_uset_new(void);
void        ref_uset_free(ref_uset_t *);
size_t      ref_uset_size(const ref_uset_t *);
bool        ref_uset_add(ref_uset_t *, T x);             /* false if already present */
bool        ref_uset_remove(ref_uset_t *, T x, T *out);  /* false if absent */
bool        ref_uset_find(const ref_uset_t *, T x, T *out); /* equal element */

typedef struct ref_sset ref_sset_t;
/* same shape as ref_uset, except find is successor search: */
bool        ref_sset_find(const ref_sset_t *, T x, T *out); /* smallest y >= x */

typedef struct ref_list ref_list_t;
ref_list_t *ref_list_new(void);
void        ref_list_free(ref_list_t *);
size_t      ref_list_size(const ref_list_t *);
T           ref_list_get(const ref_list_t *, size_t i);
void        ref_list_set(ref_list_t *, size_t i, T x);
void        ref_list_add(ref_list_t *, size_t i, T x);   /* shifts right */
T           ref_list_remove(ref_list_t *, size_t i);     /* shifts left, returns removed */

/* differential driver: same seeded op sequence on two impls of one interface */
void diff_run_list(const list_vtable_t *a, const list_vtable_t *b, unsigned seed, int ops);
/* analogous diff_run_uset / diff_run_sset; vtable structs per §1.5 */
```

### Phase 1 — array lists (the List interface)

Every List below shares this shape (swap the module prefix):

```c
typedef struct array_stack array_stack_t;
array_stack_t *array_stack_new(void);
void           array_stack_free(array_stack_t *);
size_t         array_stack_size(const array_stack_t *);
T              array_stack_get(const array_stack_t *, size_t i);   /* assert i < n */
void           array_stack_set(array_stack_t *, size_t i, T x);    /* assert i < n */
void           array_stack_add(array_stack_t *, size_t i, T x);    /* 0 <= i <= n */
T              array_stack_remove(array_stack_t *, size_t i);      /* returns removed */
```

Same shape, different prefix: `fast_array_stack`, `array_deque`, `dual_array_deque`,
`rootish_array_stack`. Plus the Queue/Deque convenience ops:

```c
/* ArrayQueue — FIFO */
void array_queue_add(array_queue_t *, T x);     /* enqueue at tail */
T    array_queue_remove(array_queue_t *);       /* dequeue from head; assert non-empty */

/* ArrayDeque — both ends (in addition to the List ops) */
void array_deque_add_first(array_deque_t *, T x);
void array_deque_add_last (array_deque_t *, T x);
T    array_deque_remove_first(array_deque_t *);
T    array_deque_remove_last (array_deque_t *);
```

### Phase 2 — linked lists

```c
/* SLList — singly linked: Stack (push/pop at head) + Queue (add tail / remove head) */
typedef struct sllist sllist_t;
sllist_t *sllist_new(void);
void      sllist_free(sllist_t *);
size_t    sllist_size(const sllist_t *);
void      sllist_push(sllist_t *, T x);   /* prepend */
T         sllist_pop(sllist_t *);         /* remove head; assert non-empty */
void      sllist_add(sllist_t *, T x);    /* enqueue at tail */
T         sllist_remove(sllist_t *);      /* dequeue from head */

/* DLList — doubly linked w/ dummy node; full List interface (Phase 1 List shape) */
typedef struct dllist dllist_t;
/* dllist_new/free/size/get/set/add/remove */

/* SEList — block list; full List interface; block size b fixed at _new */
typedef struct selist selist_t;
selist_t *selist_new(size_t b);
/* free/size/get/set/add/remove */
```

### Phase 3 — skiplists

```c
/* SkiplistSSet — SSet (successor find) */
typedef struct skiplist_sset skiplist_sset_t;
skiplist_sset_t *skiplist_sset_new(unsigned seed);   /* seed the height RNG */
void             skiplist_sset_free(skiplist_sset_t *);
size_t           skiplist_sset_size(const skiplist_sset_t *);
bool             skiplist_sset_add(skiplist_sset_t *, T x);             /* false if present */
bool             skiplist_sset_remove(skiplist_sset_t *, T x, T *out);
bool             skiplist_sset_find(const skiplist_sset_t *, T x, T *out); /* smallest y >= x */

/* SkiplistList — List by position (Phase 1 List shape), seeded at _new */
```

### Phase 4 — hash tables (USet interface)

```c
/* ChainedHashTable / LinearHashTable — USet */
typedef struct chained_hash chained_hash_t;
chained_hash_t *chained_hash_new(unsigned seed);   /* seed picks the odd multiplier z */
void            chained_hash_free(chained_hash_t *);
size_t          chained_hash_size(const chained_hash_t *);
bool            chained_hash_add(chained_hash_t *, T x);              /* false if present */
bool            chained_hash_remove(chained_hash_t *, T x, T *out);
bool            chained_hash_find(const chained_hash_t *, T x, T *out); /* equal element */

/* hash codes (§5.3) — feed these into the tables for non-int keys */
uint32_t hash_int(uint32_t x, uint32_t z, int d);    /* multiplicative: high d bits */
uint64_t hash_string(const char *s, uint64_t base);  /* polynomial mod prime */
uint64_t hash_array(const T *a, size_t n, uint64_t base);
```

### Phases 5–8 — binary & balanced trees (SSet)

Node structs here are usually **transparent** (in the header) so algorithms touch
`left`/`right`/`parent` directly:

```c
/* BinaryTree (Step 5.1) */
typedef struct bt_node { struct bt_node *left, *right, *parent; T x; } bt_node_t;
size_t bt_size(const bt_node_t *u);
int    bt_height(const bt_node_t *u);                 /* height(NULL) == -1 */
void   bt_traverse_inorder(const bt_node_t *root, void (*visit)(T));
void   bt_free(bt_node_t *root);                      /* post-order */

/* BST / Treap / ScapegoatTree / RedBlackTree — all SSet, same shape: */
typedef struct bst bst_t;
bst_t *bst_new(void);                                 /* treap takes an RNG seed */
void   bst_free(bst_t *);
size_t bst_size(const bst_t *);
bool   bst_add(bst_t *, T x);                         /* false if present */
bool   bst_remove(bst_t *, T x, T *out);
bool   bst_find(const bst_t *, T x, T *out);          /* successor */

/* rotation primitive shared by treap & red-black (takes the tree, so the root can change) */
void rotate_left (bst_t *, bt_node_t *u);
void rotate_right(bst_t *, bt_node_t *u);
```

### Phase 9 — heaps (priority queue)

```c
typedef struct binary_heap binary_heap_t;
binary_heap_t *binary_heap_new(void);                  /* meldable_heap_new(unsigned seed) */
void           binary_heap_free(binary_heap_t *);
size_t         binary_heap_size(const binary_heap_t *);
void           binary_heap_add(binary_heap_t *, T x);
bool           binary_heap_find_min(const binary_heap_t *, T *out);   /* false if empty */
bool           binary_heap_remove_min(binary_heap_t *, T *out);       /* false if empty */
/* MeldableHeap additionally exposes its core op:
   meldable_heap_t *meldable_heap_merge(meldable_heap_t *a, meldable_heap_t *b); */
```

### Phase 10 — sorting (functions over arrays, not types)

```c
void merge_sort(T *a, size_t n);
void quick_sort(T *a, size_t n, unsigned seed);   /* random pivot */
void heap_sort (T *a, size_t n);
void counting_sort(T *a, size_t n, T k);          /* values in [0, k) */
void radix_sort(uint32_t *a, size_t n);           /* w-bit ints, d-bit digits */

double log2_factorial(size_t n);                  /* the Omega(n log n) bound, Step 10.2 */
```

### Phase 11 — graphs

```c
typedef struct graph graph_t;                     /* one interface, two backends */
graph_t *adjacency_matrix_new(size_t n);
graph_t *adjacency_list_new(size_t n);
void     graph_free(graph_t *);
void     graph_add_edge(graph_t *, size_t i, size_t j);
void     graph_remove_edge(graph_t *, size_t i, size_t j);
bool     graph_has_edge(const graph_t *, size_t i, size_t j);
/* out_edges/in_edges fill a caller buffer or return a small list */

void bfs(const graph_t *, size_t source, int  *dist);  /* dist[v] = hops, or -1 if unreachable */
void dfs(const graph_t *, size_t source, bool *seen);
```

### Phase 12 — integer SSets

```c
/* BinaryTrie / XFastTrie / YFastTrie — SSet over w-bit ints */
typedef struct binary_trie binary_trie_t;
binary_trie_t *binary_trie_new(int w);
void           binary_trie_free(binary_trie_t *);
bool           binary_trie_add(binary_trie_t *, uint32_t x);
bool           binary_trie_remove(binary_trie_t *, uint32_t x);
bool           binary_trie_find(const binary_trie_t *, uint32_t x, uint32_t *out); /* successor */
/* x_fast_trie_*, y_fast_trie_* share this shape (y_fast takes an RNG seed for its treaps) */
```

### Phase 13 — external memory

```c
typedef struct block_store block_store_t;
block_store_t *block_store_new(size_t block_size);
void           block_store_free(block_store_t *);
size_t         block_store_place(block_store_t *, const void *buf);  /* returns new index */
void           block_store_read (block_store_t *, size_t i, void *buf);       /* counted */
void           block_store_write(block_store_t *, size_t i, const void *buf); /* counted */
void           block_store_free_block(block_store_t *, size_t i);
size_t         block_store_access_count(const block_store_t *);     /* the cost oracle */

/* BTree — SSet over a BlockStore */
typedef struct btree btree_t;
btree_t *btree_new(block_store_t *, int B);
/* free/size/add/remove/find as the SSet shape */
```

### Phase G — generics

```c
typedef int  (*cmp_fn)(const void *a, const void *b);
typedef void (*free_fn)(void *elem);

typedef struct dllist_g dllist_g_t;
dllist_g_t *dllist_g_new(cmp_fn cmp, free_fn free_elem /* may be NULL */);
/* ...same List shape but elements are void * ... */

typedef struct { void *key; void *value; } pair_t;   /* build a map on a void* USet */
```

---

## Quick reference: the C-specific faithfulness traps the roadmap names

These are the "sabotage" steps where the *bug is a C bug*, not an algorithm bug. Knowing the
mechanism makes the sabotage meaningful:

| Step | C mechanism | The bug it proves |
|---|---|---|
| 1.1 / 1.2 | `memcpy` on overlapping shift | UB / corruption — must be `memmove` |
| 1.1 | skip `free` on shrink | leak (Valgrind) |
| 1.3 | drop `% len` on circular index | out-of-bounds write |
| 2.1 | forget `tail = NULL` on last remove | dangling tail, later corruption |
| 2.1 / 5.1 | omit the node-walk in `_free` | leak / use-after-free |
| 3.1 | free a node *per level* | double-free |
| 4.1 | low bits / even `z` / signed overflow in hash | degenerate distribution / UB |
| 5.1 | free parent before children | use-after-free |
| 6.2 / 9.2 | botch a parent link in rotate/merge | corruption (Valgrind / invariant) |
| G | leak in the `free_elem` path | leak (Valgrind) |

`valgrind --leak-check=full --error-exitcode=1 ./test` is the oracle for every row — it's the
destructor you no longer get for free.
