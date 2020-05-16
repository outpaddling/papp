PAPP
====

PAPP (Portable Assembly PreProcessor) preprocesses assembly language code, much as cpp does for C and C++.

Papp parses assembly language programs line-by-line, processing macro definitions, macro references, and includes. Output is sent to the standard output.

Papp can be used with any assembler, but was specifically designed to enhance the programming experience with the SPIM MIPS simulator, and follows a syntax taken from MIPS assembly language documentation.

Macro definitions take the following general form:

    .macro name($arg1[,$arg2...])
    Macro text
    .end_macro

Argument names must begin with a $, and should use standard variable naming conventions that do not conflict with any tokens in the assembly language being preprocessed. References to arguments within the macro text are replaced by the text from the macro invocation.

MIPS assembly language (MAL) example:

    .macro push($x)
    sw      $x, ($sp)
    addi    $sp, $sp, -4
    .end_macro

    .macro swap($r1,$r2,$temp)
    move    $temp, $r1
    move    $r1, $r2
    move    $r2, $temp
    .end_macro

Macros are invoked using parentheses surrounding the argument:

    lw      $t0, x
    push($t0)
    swap($a0, $a1, $t1)

Files can be included using the .include directive. This is especially useful for storing commonly used predefined constants and macros in a separate file for inclusion in other programs. For example, if the macro examples above are stored in a file called mymacros.mal, then the following could be used to import them to another program (in the same directory):

    .include("mymacros.mal")
