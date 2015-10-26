-------How to Compile Project-----
	_From Windows_
1.Open command prompt.
2.Open source code's folder.
3.write "g++ -o myCompiler.exe Main.cpp CompileChecker.cpp" , hit enter .
4.myCompiler.exe is our myLanguage compiler.(Already compiled version exists in the directory)

------How to run myLanguage compiler----

1.Put myCode.my into myCompiler.exe's directory.(myCode can be changed to other names, input doesn't have to be name myCode.my)
2.Program runs with arguments, from console line write "myCompiler.exe myCode.my"
3.Program will give myCode.asm file which have assembly codes inside.

------Extra Features-------
1. It can find syntax errors with token numbers(string by string tokens) and tell some words that are expected depends on structure.
	-it can tell if you need to add "end" in some token number place if you used begin before.
	Ex: begin stmt1 ; stmt2 anotherstmt, it will say that you need to add "end" after stmt2.
	+Same also works for "then","do","=", ")"

------ShortComings---------

* If given argument file isn't available, program gives error.
     Ex: if you want to compile add.my but accidentally write addr.my, program gives error.
* My program can't read argument from directory.
     Ex: if you want to compile add.my in Mycodes/ directory and write Mycodes/add.my , it will give error. You have to copy add.my to executable's folder. 

-----Any Assumption------

* There has to be a space between every token.
     Ex: print x ; works but print x; won't.
* Executable and x.my has to be in same directory.
* You shouldn't use a = b  without declaring b before.
	Ex: a = b + 3 shouldn't be used if you haven't declared b before.(either by read b, or b=3) 
* Code works from recursive Statement, if there are extra words after first statement they won't be read.
	Ex: begin a = 5 ; print a end asd  ---> problem won't read asd after the end and gives an output .asm code that prints 5.
-----External Library-----

*No external libraries used.

