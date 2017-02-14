main: source.java _struct/struct.java _algo/algo.java _inout/inout.java
	javac source.java _struct/struct.java _algo/algo.java _inout/inout.java

jar: source.class
	jar -cfm source.jar manifest source.class _struct/struct.class _algo/algo.class _inout/inout.class

source.class: source.java _struct/struct.java _algo/algo.java _inout/inout.java
	javac source.java _struct/struct.java _algo/algo.java _inout/inout.java

clean:
	rm -rv *.class _struct/*.class _algo/*.class _inout/*.class
        