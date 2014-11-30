all:
	mkdir -p bin/include
	mkdir -p bin/lib
	g++ -g -I dependency/simple_log/include/ -I dependency/simple_server/include -I dependency/json-cpp/include/ -I src src/mq_receiver.cpp dependency/simple_flow/lib/libflowserver.a dependency/simple_server/lib/libsimpleserver.a dependency/simple_log/lib/libsimplelog.a dependency/json-cpp/lib/libjson_libmt.a -o bin/mq_receiver
	g++ -g -I dependency/simple_flow/include/ \
	-I dependency/simple_log/include/ \
	-I dependency/simple_server/include \
	-I dependency/json-cpp/include/ \
	-I src src/curl_utils.cpp \
	src/mq_sender.cpp \
	dependency/simple_flow/lib/libflowserver.a \
	dependency/simple_server/lib/libsimpleserver.a \
	dependency/simple_log/lib/libsimplelog.a \
	dependency/json-cpp/lib/libjson_libmt.a \
	-lcurl \
	-o bin/mq_sender
	rm -rf bin/*.o

test: mq_consumer_test

mq_consumer_test:
	g++ -g -I dependency/simple_flow/include/ \
	-I dependency/simple_log/include/ \
	-I dependency/simple_server/include \
	-I dependency/json-cpp/include/ \
	-I src src/curl_utils.cpp \
	test/mq_consumer_test.cpp \
	dependency/simple_flow/lib/libflowserver.a \
	dependency/simple_server/lib/libsimpleserver.a \
	dependency/simple_log/lib/libsimplelog.a \
	dependency/json-cpp/lib/libjson_libmt.a \
	-lcurl \
	-o bin/mq_consumer_test