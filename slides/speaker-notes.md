# Speaker notes

What I say on each slide. About 6–7 minutes including the live demo.

## 1. I broke into my own program.

Hey everyone, I’m Shlok. My topic is stack buffer overflows, under buffer overflows and memory vulnerabilities. In the next few minutes I’m going to break into a program without knowing its password. It’s my own program, so nobody’s getting arrested.

## 2. Can one extra letter break a password check?

Here’s the question that got me interested: can one extra letter break a password check? I chose this for three reasons. It’s basically hacking, and I wanted to actually see it, not just hear that C is unsafe. It only uses things we’ve already learned in 211: the stack, arrays, strings and ASCII. And it’s not an old problem. This year the same kind of bug let researchers take over office phones. Here’s the plan: what it is, a live break-in, the real attack, and how to stop it.

## 3. What’s a buffer overflow?

So what is a buffer overflow? It’s when you put more data into a box than it can hold, and the extra spills onto whatever is next to it. Three words to know. A buffer is just an array, a box of fixed size. Overflow means writing past the end of it. And the stack is where a function keeps its local variables, which we’ve seen in class. A stack buffer overflow is when that array lives on the stack.

## 4. Variables sit right next to each other

My program has two variables: a password box that holds 8 characters, and a number called authorized. If authorized is 0, you’re locked out. Anything else, you’re in. Both live on the stack, where each function call keeps its local variables packed close together. So these two sit right next to each other in memory. Remember that.

## 5. C never checks if it fits

Here’s the catch. In Python, if you write past the end of a list, you get an IndexError. C doesn’t check. It just writes into whatever memory comes next. C skips the check because checking every access costs time. It trusts you to get the size right.

## 6. The program

Here’s my program, simplified. The two variables are in a struct, which guarantees they sit side by side. It asks for a password, and scanf reads whatever you type, with no limit. If it matches "secret", authorized becomes 1, and if authorized isn’t 0 you get in. So the only way in is the password. Right? Let me show you.

## 7. What happens with one more letter?

[Run the demo. Type secret: granted. Type 8 A’s: denied.] Quick question for you. Eight A’s got denied. What happens if I type nine? Just one more letter. [Wait for guesses.] Let’s see. [Type 9 A’s.] Access granted.

## 8. The 9th letter spilled over

So what happened? The first 8 A’s filled the password box. The 9th had nowhere to go, so it landed in authorized. A as a number is 65, its ASCII code. 65 isn’t 0, and in C anything that isn’t 0 is true. So I got in. I never set authorized to 1. The overflow did it for me.

## 9. What I actually got

Here’s everything I tried. Why do 8 A’s fail? C adds an invisible zero at the end of every string, and that zero lands on authorized, which was already 0. 12 A’s fill all four bytes of authorized, so it becomes this huge number. But 60 A’s didn’t get me in. The program crashed. That surprised me, so I looked into it.

## 10. The compiler fought back

The compiler was protecting me. It puts a secret value on the stack called a canary, after the canaries miners took into coal mines. Before the function returns, it checks whether the canary changed. With 60 A’s I smashed it, so the program stopped on purpose. Behind the canary is the return address, which tells the program where to go next. If an attacker overwrites that, they decide what code runs. That’s exactly what happened in a real case this year.

## 11. Hacking an office desk phone

Here’s the real case. In January, a researcher at the security company Rapid7 found a bug in a popular line of office desk phones made by Grandstream. The phone’s web API took incoming data and stored it on the stack without checking how long it was. Same mistake as my demo. But here it went much further than flipping a variable. With no password, someone on the network could run any code they wanted as root, and even listen in on phone calls. It was rated 9.3 out of 10. Grandstream released a fix in early February, and the details went public on February 18.

## 12. How to actually fix it

How do you fix it? One: tell C how big the box is. fgets takes the size and ignores anything extra. I re-ran my 9 A’s on the fixed version and got denied. Two: keep the compiler protections on. You saw them save me. Three: use a language that checks for you, like Python, Java or Rust. That’s part of why Linux started accepting Rust. And for things like those phones, the fix is just installing the update.

## 13. C trusts you completely. That’s its power and its danger.

So, my takeaway. C trusts you completely. That’s why it’s fast, and that’s why it’s dangerous. What surprised me most was how small the mistake is: one missing size limit, one extra letter. And it’s common. Microsoft says about 70 percent of the security bugs they fix are memory bugs like this. One thing I still want to learn is how attackers get around canaries. So next time you write a C array, think about what happens if someone types one more character than you planned for.

## 14. Questions?

That’s it, thanks! Everything’s on my GitHub if you want to try breaking it yourself. Happy to take questions.
