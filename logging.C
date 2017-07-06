#include <logging.h>
#include <string.h>

FILE * Logger::logger = NULL;

void Logger::LogEvent(const char *event) {
	if (logger == NULL) { //first time here
		logger = fopen("logger", "w");
	}
	fprintf(logger, "%s\n", event);
}

void Logger::Finalize() {
	if (logger != NULL) {
		fclose(logger);
	}
}

DataFlowException::DataFlowException(const char *type, const char *error) {
	sprintf(msg, "Throwing exception: (%s): %s\n", type, error); //store it in 'msg' data member
	char loggermsg[128];
	sprintf(loggermsg, "Throwing exception: (%s): %s", type, error); //dont want end of line character in msg pass to LogEvent
	Logger::LogEvent(loggermsg); 
}