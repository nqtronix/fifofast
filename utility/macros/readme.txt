


MACRO NAMING CONVENTIONS:

Although macros make many complicated things easier they can also easily generate problems by
incorrect use, which are very hard to debug. Therfore naming conventions are important to
distinguish various functions.

Example:		Description:

__NAME__		Compiler defined macros. Might be used in rare cases to create compiler macro
				depended macros such as __TIME_UNIX__
NAME			Literals.
NAME()			Preprocessor macro. Performs various operations with the preprocessor, typically
				# or ##, but can be more complex as well. Their result can be used to create further
				macros.
_NAME()			Hidden preprocessor macro.
_name()			Function-like macro. Unlike preprocessor macros these macros are intended to be used
				like any other C function. The _ differentiates it from a normal function and hints
				to possible subtle problems in use.
_name(id)		Any normally written parameter of a function-like macro can be any C expression of
				the correct type (such as uint8_t, uint16_t, ...)
_name(_id)		Any parameter starting with _ is taken literally and thus has to be known at compile
				time. See description of macro in question for allowed inputs.
type_t _tmp		All local variables of a function like macro are marked with _ to prevent conflicts
				substituted C names for the parameter. DO NOT pass any C identifier starting with _
_return			Is used within compound statements as a label for the return value.


From https://gcc.gnu.org/onlinedocs/gcc/Typeof.html
The reason for using names that start with underscores for the local variables is to avoid
conflicts with variable names that occur within the expressions that are substituted for a and b.
Eventually we hope to design a new form of declaration syntax that allows you to declare variables
whose scopes start only after their initializers; this will be a more reliable way to prevent such
conflicts.