# Introduction

DARMBI is a Dynamic ARMv7 Binary Instrumentation framework. In other words,
using DARMBI one can instrument the ARMv7 instruction set in order to analyze
native ARMv7 application realtime. Example usages of DARMBI include, but are
not limited to, *taint analysis*, *runtime analysis*, *hot function analysis*,
*self-modifying code tracking*, *obfuscated code analysis*, etc.

This document describes the various design decisions and implementation
details that have been made. It can be used as implementation reference,
reference for optimization tricks, etc.

## Dynamic Binary Instrumentation Framework

Dynamic Binary Instrumentation frameworks, from now on referred to as DBI
framework, allow one to instrument the execution flow of a process. Rather
than relying on a custom emulator, DBI frameworks execute the original
instructions with only those instructions modified that allow the DBI to keep
track of the code path.

As the original instructions are executed in the original process, with
modifications as the DBI engine sees fit, it is possible to, in theory,
execute any normal application as if it was executed in the traditional way
(i.e., using exec(3).)

## Basic Block Instrumentation

DBI frameworks rely on rewriting Basic Blocks. A basic block is a sequence of
instructions which ends with a (conditional) branch. By rewriting each and
every basic block that is executed during execution, the DBI framework is able
to analyze and modify every instruction. For example, as we will see later,
internally the DBI framework rewrites the last instruction of the basic block
in order to branch to internal routines, which in turn handle the execution of
the next basic block (i.e., the basic block that would normally have been
executed right after the branch instruction.) This way, the DBI framework is
able to instrument any new basic blocks that appear during runtime.

# DARMBI Internals

This section describes the way DARMBI instruments basic blocks, how it keeps
track of instrumented blocks, what instrumented blocks look like internally,
how entire basic blocks and instructions can be instrumented by the user, etc.

## DARMBI Initialization

DARMBI begins by initializing itself. This includes setting up any resources
that it will need later, such as locks, internal memory, etc. After having
initialized itself, DARMBI continues by instrumenting the first basic block,
and executing it. At this point, the real application will start executing
under control of our DBI.

## Basic Block Instrumentation

Design details in this section are those that are required internally by
DARMBI, they do not reflect or offer any sort of customization of any kind.

When instrumenting a basic block, we start by obtaining the length of the
basic block. That is, we count the amount of instructions up until the branch
instruction (which indicates the end of the basic block.)

We then allocate enough memory in our own executable buffer to hold all the
instructions (and possibly some more) from the basic block and copy the
original instructions into our buffer. We patch the first instruction of the
original basic block with a branch to our instrumented basic block. This is a
cheap way to make sure that any callbacks to the original basic block are
redirected to our instrumented basic block.

Finally, after having initialized most of the instrumented basic block, we
have to fix the final (conditional) branching instruction. Given that ARMv7
features various ways to branch to another address, our DBI framework has to
support all of these ways. The following section has been dedicated to
explaining the various branching techniques and how we handle them.

## Branching Instrumented
