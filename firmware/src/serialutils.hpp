#include <Arduino.h>

namespace SerialUtils {
static int readInt(int timeout = 0);
static String readString(bool hideInput = false);
}