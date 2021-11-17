# prerequisite-checker
Contents of this document:

1. How to use prerequisite-checker
2. Who can use prerequisite-checker and what rules are there for its use.
3. Who can contribute to prerequisite-checker
4. License, for your convenience
5. Current limitations of prerequisite-checker

## How to use prerequisite-checker

Prerequisite-checker takes arguments on the commandline. If prerequisite-checker
receives no arguments, it assumes that you do not know what commands exist and 
outputs the same information as `--help`

### Commands understood by prerequisite-checker:

+ `--help`: outputs information on how to use the program and then exits. If 
prerequisite-checker is run without arguments, it behaves the same as if passed
with `--help`
+ `--version`: outputs version information on prerequisite-checker and may run
other commands. Internally, `--version` is called when `--help` is called.
+ `--license`: outputs the license present in the source code and in the License
section of this document.
+ `--filename`: treats the next argument (if it exists) as the manifest filename
and begins interpreting data. Prerequisite-checker then also checks all plans to
see if each course in the plan's requisites are met. Because prerequisite checker
outputs what courses it compares against, it waits for user input after each plan
finishes.
+ `--detail`: treats the next argument as the manifest filename and the next 
arguments as information on how to give details on courses. If the filename is 
followed by the word `all`, then all information on all known courses are shown.
If followed by a list of course references (ex, CS 101 or MATH 123), it outputs
information on only the course references given.

### How to format the data files in prerequisite-checker
Prerequisite-checker uses a simple and proprietary data format. There are five 
different, near identical, types of data in this format. With the exception of 
one format, all are nearly identical. The formats are as follows:

- Manifests
- Courses
- Requisites
- Semesters
- Plans


#### The format of Manifests
Prerequisite-checker conly uses one manifest at a time. Thus, manifests are the 
only data type without a begin and an end keyword. Both are implicit in the file.

Manifests list the paths of where to find all of the courses, requisites, semesters
and plans. An example manifest is located in `./data/manifest.txt`. There are four
keywords that describe the manifest. Prerequisite-checker will *only check files 
listed in the manifest* and will *only check for the specified data type in that file*

The syntax for all keywords is `keyword path` where keyword is the keyword (duh)
and (path) is the path to the file, these paths can be either relative (ex, ./data/filename.txt)
or absolute (ex, C:\\Users\\Username\\Documents\\OfficialUniversityEnglishCourses.txt).
Prerequisite-checker relies on you, the user, to ensure that these files are valid,
the progam will not care if you give it, say, an image, folder, disk, or a file 
that does not exist and will start trying to interpret this file as a data file. 

- `courses`: tells prerequisite-checker that the path specified contains courses. 
- `semesters`: tells prerequisite-checker that the path specified contains semesters.
- `requisites`: tells prerequisite-checker that the path specified contains requisites
- `plans`: tells prerequisite-checker that the path specified contians plans.

Multiple files can contain the same data, i.e.,
```
courses ./courseFile1.dat
courses ./courseFile2.txt
```
works completely fine, however, you cannot mix data in the same file. In other 
words, you cannot define a semester in a courses file. Prerequisite-checker will
not see the semester since it does not begin with the `course` keyword.

#### Important things to know about the other data types

1. A reference is two word followed by one another. These are whitespace-delimited,
In other words, "CS 101" is a reference (including or excluding the quotes) and 
"Requires Calculus" is also a reference. Courses must all have unique References,
Semesters must all have unique References, and Requisites must all have unique
References. If two different data types have the same reference (ex, a requisite
and a course both labeled EN 100), prerequisite-checker can tell the difference
between the two. *However, this behavior is subject to change.*
2. All keywords are, and always will be, lowercase.
3. Comments are lines that begin with a hashtag (#). The data interpreting routine
immediately continues to the next line if there is a hashtag.
4. A flag is a lone word which prerequisite-checker does not recognize as a keyword.
Flags may be used in the future to, for example, verify that a plan attains all 
general education credits or completes courses that must be in a specific degree.
For now, Flags can be used by the end user to mark information about a course. Flags
must be one word and only one flag can be specified per line. (i.e., Natural Science
must be listed as NaturalScience, otherwise prerequisite-checker will only see 
"Natural")

#### The format of Courses
A course contains the following information:

**REQUIRED**
- `course`: all courses must begin with this keyword. If a course does not begin
with this keyword, prerequisite-checker will not recognize it as a course.
- `ref`: Followed by a reference, tells prerequisite-checker how this course is 
referred by in a requisite or in a plan. All courses must have a unique reference
otherwise prerequisite-checker will only process the first one it sees.
- `endcourse`: all courses must end with this keyword. If a course does not have 
this keyword, prerequisite checker will continue attempting to interpret the file
until it reaches the end of the file or finds `endcourse`.

**OPTIONAL**
- `name`: the more human readable name of this course. Ex, `EN 101` might have the
name `Introductory English`. The course file has no limit on the amount of name 
keywords specified. If more than one is specified, the names are appended onto one
another with a space in-between. The newline at the end of any given name is removed.
- `desc`: marks a description of what the course does. Similar to name, multiple 
desc keywords may appear per course and the contents specified are appended in an
identical manner (internally, desc and name use the same code, but change different
variables.)
- `hours`: indicates the number of hours a course provides. Courses can provide 
a fractional number of hours. This number must in decimal format (i.e., 0.5 or 
.5 instead of 1/2, but I think this part might be intuitive; hence the 
parentheses :) )
- `reqs`: a reference to a requisite data. All requisites (with an exception 
detailed under the semester information) must be met for a course to be allowed.
- Any number of flags. 

#### The format of Semesters
A semester contains the following information:

**REQUIRED**
- `semester`: marks the beginning of a semester, like with `course`, if a semester
does not begin with `semester`, prerequisite-checker does not see it.
- `ref`: Followed by a reference, marks the reference that a semester uses. For 
example, a Semester might be referred to as `AP Credit` or `First-Year Fall`
- `endsemester`: marks the end of a semester, like with `endcourse`, if a semester
does not end with `endsemester`, prerequisite-checker will continue reading the 
file until it either reaches the end of the file or finds the `endsemester` of 
another semester.

**OPTIONAL**
- `unchecked`: If a semester has this keyword, none of its courses requisites are
checked. This is intended for incoming credit. For example, at the University of 
Alabama, credit from the AP Macroeconomics exam tests out of a class that requires
taking an introductory *Microeconomics* class. Unchecked allows prerequisite-checker
to handle a student having credit for the Principles of Macroeconomics while not
having credit for the Principles of Macroeconomics class (and similar conditions
such as AP Calulus BC placing credit for both Calculus I and II in the same semester,
these courses cannot be taken concurrently, therefore checking those requisites
would always fail).

#### The format of Requisites
Separating requisites from the courses they provide information for allows for 
some indirection in requisites (for example, any class that requires the same 
course taken previously can refer to the same requisite group.)



Requisites are the only data type to contain a keyword with optional arguments. 
This keyword is below:

- `req pre|con [pre|con] reference [pre|con [pre|con] reference] ...`: specifies
a course reference that when taken prior to and / or concurrently with the following
course reference, satisfies *the entire requisite*. For example, `req pre MATH 123 pre con MATH 
124` matches the entire group of requisites if MATH 123 or MATH 124 appears in any semester 
previous to someone taking a class requiring the requisites. In this example, MATH 124
may also appear in the same semester as any course requiring this group. The modifiers
`pre` and `con` may appear in any order. These modifiers must appear before each 
reference, otherwise, prerequisite-checker will not know where the reference is.

All other keywords are required.

- `reqs`: indicates the beginning of a group of requisites.
- `ref`: specifies the reference that this requisite group uses.
- `endreqs`: specifies the end of a group of requisites.

**IMPORTANT**
If *any* course in a group of requisites is found in a plan, the entire group is
matched. That is, if a class requires MATH 123 and SP 456, they should be specified
in different requisite groups. For example:

```
# correct (note the pedantic comment): any course that lists both
# reqs Requires MATH_123 and reqs Requires SP_456 will require that
# both classes have been taken in a previous semester.
reqs
    ref Requires MATH_123
    req pre MATH 123
endreqs
reqs
    ref Requires SP_456
    req pre SP_456
endreqs

# INCORRECT: either course will immediately satisfy the requisite group, meaning
# that prerequisite-checker will approve of a plan that erroneously does not 
# include both courses
reqs
    ref ExampleCourse Requisites
    req pre MATH 123
    req pre SP 456
endreqs
```
Also note that if `con` is specified, the requisite will only match if the course
appears in the same semester.


Note that requisites will *only match the courses listed in them* In other words,
if we have:

```
# note: these declarations are in separate files :)

#note, explained in course
course
    ref ENGR 101
    name General Engineering
    # NOTE: hours number is satirical humor
    hours 15
    # NOTE: also satirical humor
    desc Hell
    reqs Some Precalculus
endcourse

course
    ref MATH 100
    name Precalculus
    hours 4
endcourse

course
    ref MATH 101
    name Calculus
    hours 4
    reqs Some Precalculus
endcourse

#note, explained in semester
semester
    ref Incoming Credit
    unchecked
endsemester

semester
    ref First-Year Fall
endsemester

reqs
    ref Some Precalculus
    req pre MATH 100
endreqs

#note, explained in plan
plan
    ref Example Plan
    semester Incoming Credit MATH 101
    # Fails: no credit for MATH 100
    semester First-Year Fall ENGR 101
endplan
```
Fails intentionally even though the credit for MATH 101 implies credit for MATH 100.
Prerequisite-checker chooses not to make this inference because many admissions 
counselors can force credit for a specific course or override the prerequisites. 
In other words, prerequisite-checker does not assume credit for classes not taken;
which makes sense. A workaround here would be to have a second requisites that 
the calculus class also satisfies when taken concurrently.

Requisites do not require all of their references to be valid. If a reference to 
a course is invalid, that reference is ignored.

#### The format of Plans
Plans arrange semesters and courses and give them a name. When prerequisite-checker
runs, checks if all courses have all of their requisite groups met if the semester
should be checked. If the plan passes this test, prerequisite-checker indicates so
by outputting `[name of plan] passes.` If the plan fails this test, prerequisite-checker
indicates so by outputtiing `[name of plan] fails: [course with unmet requisites] is 
missing [reference to these requisites]`. Prerequisite checker currently outputs 
which two courses it is comparing against to the terminal. If a plan has no courses,
it passes by default.

**REQUIRED**
- `plan`: marks the beginning of a plan
- `ref`: both the reference to the plan and the name of the plan.
- `endplan`: marks the end of a plan

**OPTIONAL -- BUT YOU SHOULD INCLUDE THEM**
- `semester`: indicates that a specific semester is used. Semesters are inferred
to occurr in the order listed. For a group of semesters to have their prerequisites
processed differently, they must have different references. As a result, multiple
lines of the same semester can be specified; giving control over how long a line 
is (useful if a student has many hours of incoming credits). After the reference
to a semester, this line is followed by any number of course references.


## License

MIT License

Copyright (c) 2021 Joshua Buchanan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Current Limitations of prerequisite-checker

1. Currently, prerequisite-checker has no method of specifying a group of courses
that must be taken (an "and" relationship) where any group being matched will allow
the course to be taken. This example occurrs in a real course at the University of 
Alabama: ECE 492. ECE 492 requires ECE 333 taken concurrently or previously, then 
(ECE 409 and ECE 409) or (ECE 451 and ECE 452) or (ECE 453 and ECE 454) or (ECE 
475 and ECE 476) or (ECE 480 and ECE 481) or (ECE 486 and ECE 487). While in this
case prerequisite-checker handles this information just fine since all such courses
that require each other must be taken concurrently (meaning that ECE 486 implies 
credit for ECE 486). However, if this implication did not exist, prerequisite-checker
would not have a method to correctly allow taking this course.
2. Prerequisite-checker cannot currently verify that a degree plan is physically
possible (i.e., there is nothing preventing a plan from requiring every class that
a student could *possibly* take. Even if the hours spent in the classroom would 
imply a student in multiple places at once) or that a plan actually allows the 
student to obtain the degree. 
3. Prerequisite-checker cannot atuomatically obtain its own data. While it should
be possible to, in theory obtain this information through HTTP requiests and or 
a unviersity API. Not only do I not have expertise in this area of computing, but
each university would likely have its own API.