TARGET_ONE = k
TARGET_TWO = test

$(TARGET_ONE) : clean app/main.cpp app/y.tab.c app/lex.yy.c
	c++ app/main.cpp app/y.tab.c -o $@

app/y.tab.c : app/lex.yy.c
	bison -d -v app/temp.y -o $@

app/lex.yy.c : app/temp.l
	lex -o $@ $<

$(TARGET_TWO) : test_cpp/test.cpp
	c++ $< -o $@
	./test


clean :
	rm -f $(TARGET_TWO)
	rm -f $(TARGET_ONE)
	rm -f app/lex.yy.c
	rm -f app/y.tab.*
	rm -f app/y.output