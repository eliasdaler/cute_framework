[](../header.md ':include')

<br>

"Key-value", or kv, is for text-based serialization to/from an in-memory buffer. The design of the kv api is supposed to allow for *mostly* (but not completely) the same code to be used for both to and from buffer. Here you can view the full [kv API reference](https://randygaul.github.io/cute_framework/#/api_reference?id=serialization).

## kv Format

The kv format is designed based on the [JSON file format](https://en.wikipedia.org/wiki/JSON). The main focus is on key-value based tables and arrays. Here is a quick example of some kv data.

```
size = 10,
name = "Clarence",
data = {
	x = 5,
	z = 15.0,
},
blob = "SGVsbG8gdGhlcmUgOik=",
codes = [3] {
	7, -3, 20,
}
```

kv supports a few types of data listed here.

- integers
- floats
- strings
- [base64](https://en.wikipedia.org/wiki/Base64) encoded blobs, as strings
- arrays
- objects

## kv Instances

An instance of a [`CF_KeyValue`](https://randygaul.github.io/cute_framework/#/serialization/cf_keyvalue) represents a serialization context capabale of reading or writing. Here is an example of how to make a kv instance for reading, with [`cf_kv_read`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_read).

```cpp
CF_KeyValue* kv = cf_kv_read(data, size, NULL);
```

Similarly a kv instance can be made for writing with [`cf_kv_write`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_write).

```cpp
CF_KeyValue* kv = cf_kv_write();
```

## Read Mode

Read mode is setup by the [`cf_kv_read`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_read) function, like so.

```cpp
CF_Result result;
CF_KeyValue* kv = cf_kv_read(kv, buffer, size, &result);
if (cf_is_error(result)) {
	// The data in `buffer` is not well formed, handle this error here...
}

// kv is now ready to be read from.
```

The flow of reading from a kv is to search for a key, then query the value. Each step happens with the corresponding [`cf_kv_key`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_key) and `cf_kv_val_***` functions (such as [`cf_kv_val_float`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_float) or [`cf_kv_val_int32`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_int32)). [`cf_kv_key`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_key) will search for a certain key, and if found the following `cf_kv_val_***` function will fetch the associated value.

Say we are dealing with the following kv text in a buffer.

```cpp
const char* text =
	"a = 10,"
	"b = 12,"
;

void* buffer = (void*)text;
size_t len = strlen(text);


CF_Result result;
CF_KeyValue* kv = cf_kv_read(kv, buffer, len, &result);
if (cf_is_error(result)) {
	// The data in `buffer` is not well formed, handle this error here...
}

// kv is now ready to be read from.
```

To read the value `10` from the key `a` followed by the value `12` from key `b` we can do the following.

```cpp
int value;

cf_kv_key(kv, "a", NULL);
cf_kv_val_int32(kv, &value);
printf("a was %d\n", value);

cf_kv_key(kv, "b", NULL);
cf_kv_val_int32(kv, &value);
printf("b was %d\n", value);
```

Which would print the following.

```
a was 10
b was 12
```

?> In C++ you may use operator overloading and simply call `kv_val`, as opposed to `cf_kv_val_int32`. This can be a huge convenience when reading/writing a variety of different kinds of values.

## Write Mode

You may write values into a kv instace by calling [`cf_kv_write`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_write) like so:

```cpp
CF_KeyValue* kv = cf_kv_write();
```

As in the previous section values can be written to the buffer the same way they were read, by calling `cf_kv_key` followed by `cf_kv_val`.

```cpp
int a = 10;
int b = 12;

cf_kv_key(kv, "a", NULL);
cf_kv_val_int32(kv, &a);

cf_kv_key(kv, "b", NULL);
cf_kv_val_int32(kv, &b);
```

The resulting buffer will be filled with the following text.

```
a = 10,
b = 12,
```

?> In C++ you may use operator overloading and simply call `kv_val`, as opposed to `cf_kv_val_int32`. This can be a huge convenience when reading/writing a variety of different kinds of values.

## Objects

In kv objects are values wrapped in curly braces. In the following example there is an object called `data`.

```
size = 10,
name = "Clarence",
data = {
	x = 5,
	z = 15.0,
},
blob = "SGVsbG8gdGhlcmUgOik=",
codes = [3] {
	7, -3, 20,
}
```

Objects act similar to a scope in the C programming language. Querying for the key `data` should be paired up with a matching [`cf_kv_object_begin`](v) call, like so (after parsing the above example text with [`cf_kv_read`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_read)).

```
cf_kv_key(kv, "data", NULL);
cf_kv_object_begin(kv);
```

Now any calls to [`cf_kv_key`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_key) will search within the `data` object. Once finished pulling values out of the `data` object such as `x` and `z` the function [`cf_kv_object_end`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_object_end) can be used to exit the `data` object and return to the previous scope.

```cpp
cf_kv_object_end(kv);
```

Please note that objects can be arbitrarily nested, like so.

```
a = {
	b = 10,
	c = {
		d = "stuff",
		e = 100.0,
	},
	f = {
		x = 0,
		y = 0,
	},
}
```

## Arrays

Arrays in kv must be entered and exited explicitly by calling [`cf_kv_array_begin`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_array_begin) and [`cf_kv_array_end`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_array_end). Once [`cf_kv_array_begin`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_array_begin) is called a consecutive series of `cf_kv_val_***` calls can be made, one for each element of the array.

Here is an example of reading from an array.

```
data = [5] {
	5, 3, 2, -7, 12,
},
```

```cpp
cf_kv_key(kv, "data", NULL);
int count = 0;
cf_kv_array_begin(kv, &count);

for (int i = 0; i < count; ++i) {
	int val = 0;
	cf_kv_val_int32(kv, &val);
	printf("%d\n", val);
}
```

Which prints the following output.

```
5
3
2
-7
12
```

Please note that arrays can be arbitrarily nested, like so.

```
data = [2] {
	[3] {
		3, 5, 2,
	},
	[7] {
		10, -2, 0, 102, 3, 4, 1,
	},
}
```

## Strings

Strings in kv are dealt with by the [`cf_kv_val_string`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_string) string function, just like the other `cf_kv_val_***` functions.

```cpp
const char* string = "Hello.";
size_t size = strlen(string);

cf_kv_key(kv, "data", NULL);
cf_kv_val_string(kv, &string, &size);
```

## Base64 Blobs

Blobs in kv are for storing binary data. The data is encoded in [base64 format](https://en.wikipedia.org/wiki/Base64). The purpose of using base64 format is to ensure the binary data is safe to copy + paste manually and transmit via text without bugs.

Blobs are serialized as a string. If the contents of the string form a balid base64 blob then [`cf_kv_val_blob`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_blob) can be used instead of [`cf_kv_val_string`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_string). [`cf_kv_val_blob`](https://randygaul.github.io/cute_framework/#/serialization/cf_kv_val_blob) performs base64 encoding and decoding internally, depending on if the kv is in read or write mode.

For example say we have this blob in a kv formatted buffer.

```
blob = "SGVsbG8gdGhlcmUgOik=",
```

It can be decoded and printed like so.

```cpp
size_t blob_len = 0;
cf_kv_key(kv, "blob", NULL);
cf_kv_val_blob(kv, buffer, size, &blob_len);
printf("%.*s\n", buffer, blob_len);
```

Which prints the following output.

```
Hello there :)
```

## Type Conversions

When parsing kv stores all integers in 64-bit format internally. Similarly all floats are stored internally as doubles. Whenever `cf_kv_val` is called the requested type of the `val` parameter will be typecasted internally when dealing with integers and floats.

For example if we have this kv string.

```
val = 100.5,
```

The value can be pulled out of the string in a few ways, all completely valid with different internal typecasts.

```cpp
uint64_t u64;
float f;
double d;
char c;

// Here we demonstrate using the C++ API where `cf_kv_val` is overloaded.
// In C you must call `cf_kv_val_uint64`, `cf_kv_val_float`, etc.
kv_key(kv, "val"); kv_val(kv, &u64); // u64 is now 100
kv_key(kv, "val"); kv_val(kv, &f);   // f is now 100.5f
kv_key(kv, "val"); kv_val(kv, &d);   // d is now 100.5
kv_key(kv, "val"); kv_val(kv, &c);   // c is now 100
```

## Full Program Example

Here is a quick example of a full program you can copy + paste and compile. It simply reads some integers from a kv string.

```cpp
#include <cute.h>

int main(int argc, char** argv)
{
	const char* string =
		"a = 10,\n"
		"b = 13,\n"
	;
	size_t len = strlen(string);
	
	CF_KeyValue* kv = cf_kv_read((void*)string, len, NULL);
	
	int val;
	cf_kv_key(kv, "a", NULL); cf_kv_val_int32(kv, &val); printf("a was %d\n", val);
	cf_kv_key(kv, "b", NULL); cf_kv_val_int32(kv, &val); printf("b was %d\n", val);
	
	cf_kv_destroy(kv);
	
	return 0;
}
```

Which prints the following.

```
a was 10
b was 13
```
