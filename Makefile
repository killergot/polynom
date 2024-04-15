TARGET_ONE = k

$(TARGET_ONE) : app/main.c app/y.tab.c app/lex.yy.c
	c++ app/main.c app/y.tab.c -o $(TARGET_ONE)

app/y.tab.c : app/lex.yy.c
	bison -d app/temp.y -o app/y.tab.c

app/lex.yy.c : app/temp.l
	lex -o app/lex.yy.c app/temp.l