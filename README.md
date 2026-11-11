# I broke into my own program: Stack Buffer Overflows

**Shlok Bhutani** · COMP 211 · Checkoff 3
**Topic:** Buffer Overflows and Memory Vulnerabilities → Stack Buffer Overflows

Can one extra letter break a password check? In C, yes. I wrote a small login program with a classic bug, broke into it without the password, watched the compiler stop a bigger attack, and then fixed it.

## Why I chose this topic

- **It's hacking.** I wanted to actually see how attackers break programs, not just hear that C is "unsafe."
- **It builds on COMP 211.** It only needs the stack, arrays, strings and ASCII, which we've already covered.
- **It still happens.** In February 2026, a stack buffer overflow (CVE-2026-2329) let anyone on the network take full control of Grandstream office desk phones, with no password, and even listen in on calls.

## What a stack buffer overflow is

A **buffer** is an array, a fixed-size box in memory. An **overflow** is writing past the end of it. When the array is a local variable, it lives on the **stack**, right next to the function's other variables. C never checks whether data fits, so the extra bytes land on whatever comes next.

## What I found

| I typed | `authorized` | Result |
|---|---|---|
| `secret` | 1 | Access granted (correct) |
| `hello` | 0 | Access denied (correct) |
| `AAAAAAAA` (8) | 0 | Access denied |
| `AAAAAAAAA` (9) | **65** | **Access granted, without the password** |
| `AAAAAAAAAAAA` (12) | **1094795585** | **Access granted** |
| 60 A's | — | Crash: `*** stack smashing detected ***` |

1. **One extra letter is enough.** The 9th `A` spills out of `password[8]` into `authorized`.
2. **`authorized` becomes 65,** the ASCII code for `A`. Anything that isn't 0 counts as true in C, so the check passes.
3. **8 letters don't work** because C adds an invisible `\0` at the end of the string, and that zero lands on `authorized`, which was already 0.
4. **12 letters give 1094795585.** That's `0x41414141`: all four bytes of the int are now `A`.
5. **60 letters crash the program.** The compiler places a secret **stack canary** before the return address and checks it before the function returns. The overflow smashed it, so the program stopped itself instead of being hijacked.
6. **The fix is one line.** `fgets(buf, sizeof(buf), stdin)` respects the size, and every long input gets denied.

## Real-world example: Grandstream office phones (2026)

- **Jan 6, 2026:** a Rapid7 researcher reports the bug to Grandstream.
- **Early Feb:** Grandstream ships a fix (firmware 1.0.7.81).
- **Feb 18:** the bug goes public as CVE-2026-2329, rated 9.3/10.
- **The bug:** the phone's web API stored incoming data on the stack without checking its length, the same mistake as `login.c`.
- **The impact:** with no password, an attacker could run code as root and intercept calls.

## Run it yourself

You need `gcc` or `clang`.

```bash
cd code
gcc login.c -o login
./login
```

Try typing `secret`, then 8 A's, then 9 A's.

Run every test at once:

```bash
sh code/run_tests.sh
```

On macOS the 60-letter crash may just say `Abort trap: 6` instead of `stack smashing detected`.

**Why a struct?** Compilers are allowed to rearrange separate local variables, and modern ones often put arrays after other variables on purpose so an overflow can't reach them. Putting `password` and `authorized` in a struct guarantees the order, so the demo behaves the same everywhere.

## Repository contents

```
code/
  login.c           vulnerable program (uses scanf("%s") with no limit)
  login_fixed.c     same program, fixed with fgets
  run_tests.sh      builds both and runs every input above
slides/
  slides.pdf        presentation slides
  speaker-notes.md  what I say on each slide
```

## How I explored it

I started with the basics: what a buffer is, where local variables live, and why C doesn't check array bounds. Then I wrote `login.c` and tried different inputs to find the exact point where the check breaks. The 60-letter crash surprised me, and looking into it is how I learned about stack canaries. Finally, I read the original security reports on the Grandstream bug to connect the demo to a real attack.

## Resources

- Rapid7, *CVE-2026-2329: Critical Unauthenticated Stack Buffer Overflow in Grandstream GXP1600 VoIP Phones*: https://www.rapid7.com/blog/post/ve-cve-2026-2329-critical-unauthenticated-stack-buffer-overflow-in-grandstream-gxp1600-voip-phones-fixed
- The Hacker News, *Grandstream GXP1600 VoIP Phones Exposed*: https://thehackernews.com/2026/02/grandstream-gxp1600-voip-phones-exposed.html
- SecurityWeek, *Critical Grandstream Phone Vulnerability Exposes Calls to Interception*: https://www.securityweek.com/critical-grandstream-phone-vulnerability-exposes-calls-to-interception/
- OWASP, *Buffer Overflow*: https://owasp.org/www-community/vulnerabilities/Buffer_Overflow
- GCC docs, `-fstack-protector`: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
- Microsoft Security Response Center (2019): about 70% of the security bugs Microsoft fixes are memory-safety issues
- `man 3 fgets`, `man 3 scanf`
