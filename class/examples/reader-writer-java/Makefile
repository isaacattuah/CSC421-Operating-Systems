#
# reader-writer in java
# author: bjr
# last-update:
#	1 oct 2020: created
#


ReaderWriter: ReaderWriter.class
	java $@

Writer.class: Writer.java RingBuffer.class
	javac $< 
	
Reader.class: Reader.java RingBuffer.class
	javac $<

RingBuffer.class: RingBuffer.java
	javac $<
	
ReaderWriter.class: ReaderWriter.java Reader.class Writer.class RingBuffer.class
	javac $<

clean:
	-rm *.class
