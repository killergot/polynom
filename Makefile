TARGET_ONE = k
TARGET_TWO = test

$(TARGET_ONE) : clean app/main.cpp app/y.tab.c app/lex.yy.c
	c++ app/main.cpp app/y.tab.c -o $(TARGET_ONE)

app/y.tab.c : app/lex.yy.c
	bison -d -v app/temp.y -o app/y.tab.c

app/lex.yy.c : app/temp.l
	lex -o app/lex.yy.c app/temp.l

$(TARGET_TWO) : test_cpp/map.cpp
	c++ test_cpp/map.cpp -o $(TARGET_TWO)
	./test


clean :
	rm -f $(TARGET_TWO)
	rm -f $(TARGET_ONE)
	rm -f app/lex.yy.c
	rm -f app/y.tab.*
	rm -f app/y.output