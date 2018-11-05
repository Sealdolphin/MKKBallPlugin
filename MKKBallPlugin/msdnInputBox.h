#pragma once

/*
This is a library for Input like windows in Microsoft.
With these windows you can ask the user for input.
*/

class InputBox {
public:
	static int getStringInput(const char* title, const char* message, int space, char* result, int result_size);
	static int getIntegerInput(const char* title, const char* message, int space, int* result);

};