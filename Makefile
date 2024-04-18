TARGET_ONE = k

$(TARGET_ONE) : clean app/main.c app/y.tab.c app/lex.yy.c
	c++ app/main.c app/y.tab.c -o $(TARGET_ONE)

app/y.tab.c : app/lex.yy.c
	bison -d -v app/temp.y -o app/y.tab.c

app/lex.yy.c : app/temp.l
	lex -o app/lex.yy.c app/temp.l

clean :
	rm -f k
	rm -f app/lex.yy.c
	rm -f app/y.tab.*
	rm -f app/y.output